#include "CalcAccAndEff.h" 
  //this class
#include "../Include/TMathTools.h"
#include "../Include/TMetTools.h" 
#include "../Include/TMuonCuts.h" 
#include "../Include/TElectronCuts.h" 
#include "../Include/TPhotonCuts.h" 
#include "../Include/TFullCuts.h" 
#include "../Include/TPuReweight.h" 
#include "../Configuration/TConfiguration.h" 
#include "../Configuration/TInputSample.h"
#include "../Configuration/TAllInputSamples.h"
  //this package
#include "../Include/PhosphorCorrectorFunctor.hh"
  //taken from http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/CPena/src/PHOSPHOR_Corr_v2/
  //currently in this package
#include "TMath.h" 
#include "TH1F.h" 
#include "TVectorF.h" 
#include "TCanvas.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TLegend.h"
  // ROOT class
#include <iostream> 
#include <string>
#include <sstream>  
  //standard C++ class

CalcAccAndEff::CalcAccAndEff(int channel, string configFile, bool isPuReweight, bool isDebugMode)
{

  INPUT_ = new TAllInputSamples(channel, configFile);

  channel_=channel;
  isDebugMode_=isDebugMode;
  isPuReweight_=isPuReweight;
  photonCorrector_ = new zgamma::PhosphorCorrectionFunctor((config_.GetPhosphorConstantFileName()) );
    //field of this class

  nEvents_=0;
  nAccPassed_=0;
  nEventsInAcc_=0;
  nEffPassed_=0;
  nEventsErr_=0;
  nAccPassedErr_=0;
  nEventsInAccErr_=0;
  nEffPassedErr_=0;

  acc_=-1;
  eff_=-1;
  accErr_=-1;
  effErr_=-1;

  for (int i=0; i<config_.GetNPhoPtBins(); i++){
    vecnEvents_.push_back(0);
    vecnAccPassed_.push_back(0);
    vecnEventsInAcc_.push_back(0);
    vecnEffPassed_.push_back(0);
    vecnEventsErr_.push_back(0);
    vecnAccPassedErr_.push_back(0);
    vecnEventsInAccErr_.push_back(0);
    vecnEffPassedErr_.push_back(0);

    vecacc_.push_back(-1);
    veceff_.push_back(-1);
    vecaccErr_.push_back(-1);
    veceffErr_.push_back(-1);
  }

  vecPhoPtLimits_ = config_.GetPhoPtBinsLimits();
}

CalcAccAndEff::~CalcAccAndEff()
{
   fChain = 0;
     //field of TEventTree 
   delete photonCorrector_;
}

void CalcAccAndEff::LoopOverInputFiles()
{
  int nSources = INPUT_->nSources_;
  for (int iSource=0; iSource<nSources; iSource++)
    {
       int sample = INPUT_->allInputs_[iSource].sample_;
       if (sample!=TInputSample::SIGMC) continue;
         // acceptance and efficiency are calculated only through signal MC

       TTree* tree;
       int inputFileNMax = INPUT_->allInputs_[iSource].nFiles_;

       for (int inputFileN=0; inputFileN< inputFileNMax; inputFileN++)
         {
           lumiWeight_=INPUT_->allInputs_[iSource].lumiWeights_[inputFileN];
           TFile f((INPUT_->allInputs_[iSource].fileNames_[inputFileN]) );
           if (f.IsOpen()) 
             std::cout<<std::endl<<"processing file "<<INPUT_->allInputs_[iSource].fileNames_[inputFileN]<<std::endl;
           else
             {
                std::cout<<"ERROR detected in CalcAccAndEff::LoopOverInputFiles: file "<<INPUT_->allInputs_[iSource].fileNames_[inputFileN]<<" was not found"<<std::endl;
                return;
             } 
           f.cd("ggNtuplizer");
           tree = (TTree*)gDirectory->Get("EventTree");

           if (tree) 
             {
                Init(tree);
                //if (sample_!=TInputSample::DATA) SetMCSpecificAddresses();
             }   
           else
             {
                std::cout<<"ERROR detected in CalcAccAndEff::LoopOverInputFiles: tree in the file "<<INPUT_->allInputs_[iSource].fileNames_[inputFileN]<<" does not exist"<<std::endl;
                return;
             }  

           bool isSkimmed = gDirectory->GetListOfKeys()->Contains("hskim");

           if (isSkimmed){
              std::cout<<"ERROR detected in CalcAccAndEff::LoopOverInputFiles: file"<<INPUT_->allInputs_[iSource].fileNames_[inputFileN]<<" is skimmed; skimmed signal MC file can't be used for acceptance and efficiency calculation; please, use the full signal MC file"<<std::endl;
              return;
           }

//           if (!isSkimmed){
//             nEvents_+=tree->GetEntries();
//             nSkimPassed_+=tree->GetEntries();
//           }
//           else if (isSkimmed){
//              TH1F* hist = (TH1F*)gDirectory->Get("hskim");
//              nEvents_ += tree->GetEntries() * hist->GetBinContent(1) / hist->GetBinContent(2);
//              nSkimPassed_+=tree->GetEntries();
//              hist = 0;
//           }

           


           puWeight_=new TPuReweight(config_.GetPileupDataFileName(),INPUT_->allInputs_[iSource].fileNames_[inputFileN]);

           LoopOverTreeEvents();
             //method of this class (CalcAccAndEff)
           fChain=0;
             //field of TEventTree

           delete puWeight_;

         }//loop over inputFileN_ ends

     nEventsErr_=sqrt(nEventsErr_);
     nAccPassedErr_=sqrt(nAccPassedErr_);
     nEventsInAccErr_=sqrt(nEventsInAccErr_);
     nEffPassedErr_=sqrt(nEffPassedErr_);
     for (int i=0; i<config_.GetNPhoPtBins(); i++){
       vecnEventsErr_[i]=sqrt(vecnEventsErr_[i]);
       vecnAccPassedErr_[i]=sqrt(vecnAccPassedErr_[i]);
       vecnEventsInAccErr_[i]=sqrt(vecnEventsInAccErr_[i]);
       vecnEffPassedErr_[i]=sqrt(vecnEffPassedErr_[i]);
     }

     TMathTools mathTools;
     if (nEvents_!=0){ 
       acc_=nAccPassed_/nEvents_;
       accErr_=mathTools.ErrOfTwoIndependent("x1/(x1+x2)",nAccPassed_,nEvents_,nAccPassedErr_,nEventsErr_);
     }
     if (nEventsInAcc_!=0){ 
       eff_=(double)nEffPassed_/nEventsInAcc_;
       effErr_=mathTools.ErrOfTwoIndependent("x1/(x1+x2)",nEffPassed_,nEventsInAcc_,nEffPassedErr_,nEventsInAccErr_);
     }

     for (int i=0; i<config_.GetNPhoPtBins(); i++){
       vecacc_[i]=vecnAccPassed_[i]/vecnEvents_[i];
       vecaccErr_[i]=mathTools.ErrOfTwoIndependent("x1/(x1+x2)",vecnAccPassed_[i],vecnEvents_[i],vecnAccPassedErr_[i],vecnEventsErr_[i]);
       veceff_[i]=vecnEffPassed_[i]/vecnEventsInAcc_[i];
       veceffErr_[i]=mathTools.ErrOfTwoIndependent("x1/(x1+x2)",vecnEffPassed_[i],vecnEventsInAcc_[i],vecnEffPassedErr_[i],vecnEventsInAccErr_[i]);
     }

     PlotAndSaveOutput();

     std::cout<<"Summary:"<<std::endl;
     std::cout<<"nEvents="<<nEvents_<<"+-"<<nEventsErr_;
     std::cout<<", nAccPassed="<<nAccPassed_<<"+-"<<nAccPassedErr_<<", acc="<<acc_<<"+-"<<accErr_<<std::endl;
     std::cout<<"nEventsInAcc="<<nEventsInAcc_<<"+-"<<nEventsInAccErr_;
     std::cout<<", nEffPassed="<<nEffPassed_<<"+-"<<nEffPassedErr_<<", eff="<<eff_<<"+-"<<effErr_<<std::endl;
     std::cout<<std::endl;

     std::cout<<std::endl;
     for (int i=0; i<config_.GetNPhoPtBins(); i++){
       std::cout<<std::endl;
       std::cout<<"pho Pt: "<<vecPhoPtLimits_[i]<<" - "<<vecPhoPtLimits_[i+1]<<std::endl;
       std::cout<<" -- nEvents="<<vecnEvents_[i]<<"+-"<<vecnEventsErr_[i];
       if (vecnEvents_[i]==0) continue;
       std::cout<<", nAccPassed="<<vecnAccPassed_[i]<<"+-"<<vecnAccPassedErr_[i]<<", acc="<<vecacc_[i]<<"+-"<<vecaccErr_[i]<<std::endl;
       std::cout<<" -- nEventsInAcc="<<vecnEventsInAcc_[i]<<"+-"<<vecnEventsInAccErr_[i];
       if (vecnEventsInAcc_[i]==0) continue;
       std::cout<<", nEffPassed="<<vecnEffPassed_[i]<<"+-"<<vecnEffPassedErr_[i]<<", eff="<<veceff_[i]<<"+-"<<veceffErr_[i]<<std::endl;
     }//end of for (int i=0; i<config_.GetNPhoPtBins(); i++)

     std::cout<<std::endl;
     std::cout<<"the output will be saved to "<<std::endl<<std::endl;

    } //loop over iSource ends
}


void CalcAccAndEff::LoopOverTreeEvents()
{
   if (fChain == 0) return;
   Long64_t nentries = fChain->GetEntries();
   if (isDebugMode_) 
     {
       if (fChain->GetEntries()<debugModeNEntries_) nentries=fChain->GetEntries();
       else nentries=debugModeNEntries_;
     }

   //goodLeptonPhotonPairs(two-dimentional array of bool-s)
   //memory allocation for some variables: 
   int nLeptonMax;
   if (channel_==TConfiguration::MUON) nLeptonMax=kMaxnMu;
   else if (channel_==TConfiguration::ELECTRON) nLeptonMax=kMaxnEle;
     //kMaxnMu - field of TEventTree
   else
     {
       std::cout<<"Error detected in WGammaSelection::LoopOverTreeEvents: channel must be either MUON or ELECTRON."<<std::cout;
       return;
     }

   float* WMt = new float[nLeptonMax];

   float** lePhoDeltaR = new float*[nLeptonMax];
   for (int ile=0; ile<nLeptonMax; ile++)
     lePhoDeltaR[ile]=new float[kMaxnPho];
   
   bool** effLeptonPhotonPassed = new bool*[nLeptonMax];
   for (int ile=0; ile<nLeptonMax; ile++)
     effLeptonPhotonPassed[ile]=new bool[kMaxnPho];

   bool** accLeptonPhotonPassed = new bool*[kMaxnMC];
   for (int imc=0; imc<kMaxnMC; imc++)
     accLeptonPhotonPassed[imc]=new bool[kMaxnMC];

   //   CheckMaxNumbersInTree();
  
   TFullCuts fullCuts;
   TMuonCuts muonEmpty;
   //   TElectronCuts electronEmpty;
   TPhotonCuts photonEmpty;

   TMathTools math;

   for (Long64_t entry=0; entry<nentries; entry++) 
   //loop over events in the tree
     {
        GetEntryNeededBranchesOnly(entry);
          //method of TEventTree class
        if (!treeLeaf.isData) GetEntryMCSpecific(entry);
          //method of TEventTree class

        float weightPU;
        if (isPuReweight_)
          weightPU = puWeight_->GetPuWeightMc(treeLeaf.puTrue[1]);
        else weightPU=1;

        if (channel_==TConfiguration::MUON) nLe_=treeLeaf.nMu;
        else if (channel_==TConfiguration::ELECTRON) nLe_=treeLeaf.nEle;
        else
          {
             std::cout<<"Error detected in  CalcAccAndEff::LoopOverTreeEvents: channel must be either MUON or ELECTRON."<<std::cout;
             return;
          }
        
        // apply acceptance cuts
        bool accPassed=0;
        int bin=-1;

        bool genPhotons[TEventTree::kMaxnMC];
        bool genLeptons[TEventTree::kMaxnMC];
        for (int iMC=0; iMC<treeLeaf.nMC; iMC++){
          genPhotons[iMC]=0;
          genLeptons[iMC]=0;
          if (treeLeaf.mcPID[iMC]==22)
            genPhotons[iMC]=1;
          if (channel_==TConfiguration::MUON && ((treeLeaf.mcPID[iMC]==13) || (treeLeaf.mcPID[iMC]==-13)))
            genLeptons[iMC]=1;
          if (channel_==TConfiguration::ELECTRON && ((treeLeaf.mcPID[iMC]==11) || (treeLeaf.mcPID[iMC]==-11)))
            genLeptons[iMC]=1;
 
        } // loop over iMC; search for gen level photons and leptons

        for (int iGenLep=0; iGenLep<treeLeaf.nMC; iGenLep++)
          for (int iGenPho=0; iGenPho<treeLeaf.nMC; iGenPho++){
            if (!(genLeptons[iGenLep] && genPhotons[iGenPho])) continue;
            float dR = math.DeltaR(treeLeaf.mcEta[iGenLep],treeLeaf.mcPhi[iGenLep],
                                     treeLeaf.mcEta[iGenPho],treeLeaf.mcPhi[iGenPho]);
            if (dR<=config_.GetLePhoDeltaRMin()) continue;

            /////////////////////////////////////////////////
            // count number of events 
            if (treeLeaf.mcEt[iGenPho]>config_.GetPhoPtMin()){
              nEvents_+=lumiWeight_;
              nEventsErr_+=lumiWeight_*lumiWeight_;
            }
            bin = config_.FindPhoPtBinByPhoPt(treeLeaf.mcEt[iGenPho]);
            if (bin<(int)vecnEvents_.size() && bin!=-1){
              vecnEvents_[bin]+=lumiWeight_;
              vecnEventsErr_[bin]+=lumiWeight_*lumiWeight_;
            }
              
            /////////////////////////////////////////////////
            // count number of events within acceptance

            bool isPhoAcc = photonEmpty.PhoKinematics(treeLeaf.mcEt[iGenPho],treeLeaf.mcEta[iGenPho]) ;
            bool isLepAcc = (channel_==TConfiguration::MUON && 
                   muonEmpty.MuKinematics(treeLeaf.mcPt[iGenLep],treeLeaf.mcEta[iGenLep])) ||
                   (channel_==TConfiguration::ELECTRON);
            if (isPhoAcc && isLepAcc) {
              accPassed=1;
              accLeptonPhotonPassed[iGenLep][iGenPho]=1;
              if (treeLeaf.mcEt[iGenPho]>config_.GetPhoPtMin()){
                nAccPassed_+=lumiWeight_;
                nAccPassedErr_+=lumiWeight_*lumiWeight_;
              }
              bin = config_.FindPhoPtBinByPhoPt(treeLeaf.mcEt[iGenPho]);
              if (bin<(int)vecnAccPassed_.size() && bin!=-1){
                vecnAccPassed_[bin]+=lumiWeight_;
                vecnAccPassedErr_[bin]+=lumiWeight_*lumiWeight_;
              }
            }  //end of if (isPhoAcc && isLepAcc)
          }//end of loops over iGenPho and iGenLep

        if (!accPassed) continue;

        ////////////////////////
        //apply efficiency cuts
        ////////////////////////
        bool effPassed = fullCuts.Cut(effLeptonPhotonPassed, treeLeaf,   
                channel_,  WMt, lePhoDeltaR, photonCorrector_);
        for (int ile=0; ile<nLe_; ile++)
          for (int ipho=0; ipho<treeLeaf.nPho; ipho++){

            int iGenPho=-1;
            int iGenLep=-1;
            for (int imc=0; imc<treeLeaf.nMC; imc++){
              if (treeLeaf.mcIndex[imc]==treeLeaf.phoGenIndex[ipho]) 
                iGenPho=imc;
              if ((channel_==TConfiguration::MUON && treeLeaf.mcIndex[imc]==treeLeaf.muGenIndex[ile]) ||
                  (channel_==TConfiguration::ELECTRON))   
                iGenLep=imc;
            }//end of loop over iMC

            if (iGenPho<0 || iGenLep<0) continue;
            if (!accLeptonPhotonPassed[iGenLep][iGenPho]) continue;
              //check if event is "within acceptance"

            float dR = 0;
            if (channel_==TConfiguration::MUON) 
              dR = math.DeltaR(treeLeaf.muEta[ile],treeLeaf.muPhi[ile],treeLeaf.phoEta[ipho],treeLeaf.phoPhi[ipho]);
            else if (channel_==TConfiguration::ELECTRON)
              dR = math.DeltaR(treeLeaf.eleEta[ile],treeLeaf.elePhi[ile],treeLeaf.phoEta[ipho],treeLeaf.phoPhi[ipho]);
            if (dR<=config_.GetLePhoDeltaRMin()) continue;

            ///////////////////////////////////
            // count number of events "in acceptance"
            // with reconstructed variables bins
            // so, apply PU weight here
            if (treeLeaf.phoEt[ipho]>config_.GetPhoPtMin()){
              nEventsInAcc_+=lumiWeight_*weightPU;
              nEventsInAccErr_+=(lumiWeight_*weightPU)*(lumiWeight_*weightPU);
            }
            bin = config_.FindPhoPtBinByPhoPt(treeLeaf.phoEt[ipho]);
            if (bin<(int)vecnEventsInAcc_.size() && bin!=-1){
              vecnEventsInAcc_[bin]+=lumiWeight_*weightPU;
              vecnEventsInAccErr_[bin]+=(lumiWeight_*weightPU)*(lumiWeight_*weightPU);
            } 
            
            if (!effPassed) continue;
            ////////////////////////////////////
            // count number of events in efficiency
            if (effLeptonPhotonPassed[ile][ipho]){
               if (treeLeaf.phoEt[ipho]>config_.GetPhoPtMin()){
                 nEffPassed_+=lumiWeight_*weightPU;
                 nEffPassedErr_+=(lumiWeight_*weightPU)*(lumiWeight_*weightPU);
               }
               bin = config_.FindPhoPtBinByPhoPt(treeLeaf.phoEt[ipho]);
               if (bin<(int)vecnEffPassed_.size() && bin!=-1){
                 vecnEffPassed_[bin]+=lumiWeight_*weightPU;
                 vecnEffPassedErr_[bin]+=(lumiWeight_*weightPU)*(lumiWeight_*weightPU);
               }
             }//end of if (effLeptonPhotonPassed[ile][ipho])
          }//end of loops over ipho and ile


     } //end of loop over events in the tree

  //memory release:

  for (int ile=0; ile<nLeptonMax; ile++)
    delete effLeptonPhotonPassed[ile];
  delete[] effLeptonPhotonPassed;

  for (int imc=0; imc<kMaxnMC; imc++)
    delete accLeptonPhotonPassed[imc];
  delete[] accLeptonPhotonPassed;

  for (int ile=0; ile<nLeptonMax; ile++)
    delete lePhoDeltaR[ile];
  delete[] lePhoDeltaR;
 
  delete WMt;

//  std::cout<<"nTotal_="<<nTotal_<<std::endl;
//  std::cout<<"nBeforeLeptonLoop_="<<nBeforeLeptonLoop_<<" (IsVtxGood!=-1, nPho >= 0, nLe_ >=0, metFilters[6]==1)"<<std::endl;
//  std::cout<<"nLeptons_="<<nLeptons_<<std::endl;
//  if (nLeptons_!=0)       
//    std::cout<<"nLeptonsPassed_="<<nLeptonsPassed_<<", eff="<<1.0*nLeptonsPassed_/nLeptons_<<std::endl; 
//  std::cout<<"nMoreVetoPassed_="<<nMoreVetoPassed_<<std::endl;
//  std::cout<<"nWMtPassed_="<<nWMtPassed_<<std::endl;
//  std::cout<<"nPhotons_="<<nPhotons_<<std::endl;
//  if (nPhotons_!=0) 
//    std::cout<<"nPhotonsPassed_="<<nPhotonsPassed_<<", eff="<<1.0*nPhotonsPassed_/nPhotons_<<std::endl;
//  std::cout<<"nPhoLepPassed_="<<nPhoLepPassed_<<std::endl; 
//  std::cout<<std::endl;

}

void CalcAccAndEff::PlotAndSaveOutput()
{
  int nBins = config_.GetNPhoPtBins();
  float binLims[nBins+1];
  for (int i=0; i<nBins+1; i++){
    binLims[i]=vecPhoPtLimits_[i];
  }

  TH1D* hAcc1D = new TH1D(config_.GetAcc1DName(),config_.GetAcc1DName(),nBins,binLims);
  TH1D* hEff1D = new TH1D(config_.GetEff1DName(),config_.GetEff1DName(),nBins,binLims);
  TH1D* hAccEff1D = new TH1D(config_.GetAccEff1DName(),config_.GetAccEff1DName(),nBins,binLims);
  TH1D* hAccTot = new TH1D(config_.GetAccTotalName(),config_.GetAccTotalName(),1,15,1000);
  TH1D* hEffTot = new TH1D(config_.GetEffTotalName(),config_.GetEffTotalName(),1,15,1000);
  TH1D* hAccEffTot = new TH1D(config_.GetAccEffTotalName(),config_.GetAccEffTotalName(),1,15,1000);

  for (int i=0; i<config_.GetNPhoPtBins(); i++){
    hAcc1D->SetBinContent(i+1,vecacc_[i]);
    hAcc1D->SetBinError(i+1,vecaccErr_[i]);
    hEff1D->SetBinContent(i+1,veceff_[i]);
    hEff1D->SetBinError(i+1,veceffErr_[i]);
    hAccEff1D->SetBinContent(i+1,vecacc_[i]*veceff_[i]);
    hAccEff1D->SetBinError(i+1,sqrt(vecacc_[i]*vecacc_[i]*veceffErr_[i]*veceffErr_[i]+vecaccErr_[i]*vecaccErr_[i]*veceff_[i]*veceff_[i]));
  }
  hAccTot->SetBinContent(1,acc_);
  hAccTot->SetBinError(1,accErr_);
  hEffTot->SetBinContent(1,eff_);
  hEffTot->SetBinError(1,effErr_);
  hAccEffTot->SetBinContent(1,acc_*eff_);
  hAccEffTot->SetBinError(1,sqrt(acc_*acc_*effErr_*effErr_+eff_*eff_*accErr_*accErr_));

  hAcc1D->SetLineWidth(2);
  hEff1D->SetLineWidth(2);
  hAccEff1D->SetLineWidth(2);
  hAccTot->SetLineWidth(2);
  hEffTot->SetLineWidth(2);
  hAccEffTot->SetLineWidth(2);

  hAcc1D->SetLineColor(2);
  hEff1D->SetLineColor(4);
  hAccEff1D->SetLineColor(1);
  hAccTot->SetLineColor(2);
  hEffTot->SetLineColor(4);
  hAccEffTot->SetLineColor(1);

  TString fName=config_.GetAccEffFileName(channel_) ;
  TFile f(fName,"recreate");
  hAcc1D->Write((config_.GetAcc1DName()) );
  hEff1D->Write((config_.GetEff1DName()) );
  hAccTot->Write((config_.GetAccTotalName()) );
  hEffTot->Write((config_.GetEffTotalName()) );



  //Draw
  TCanvas* cAccEff = new TCanvas("cAccEff","cAccEff");
  

  TLegend* leg = new TLegend(0.2,0.7,0.5,0.85);
  leg->AddEntry(hAcc1D,"acceptance","L");
  leg->AddEntry(hEff1D,"efficiency","L");
  leg->AddEntry(hAccEff1D,"acc x eff","L");

  hAcc1D->SetStats(0);
  hAcc1D->GetYaxis()->SetRangeUser(0.0,1.0);
  hAcc1D->SetTitle("acceptance and efficiency");
  hAcc1D->Draw();
  hEff1D->Draw("same");
  hAccEff1D->Draw("same");
  hAccTot->Draw("same");
  hEffTot->Draw("same");
  hAccEffTot->Draw("same");
  leg->Draw("same");
  //cAccEff.SetLogy();
  cAccEff->SetLogx();
  cAccEff->Write("cAccEff");

}
