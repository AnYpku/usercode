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

CalcAccAndEff::CalcAccAndEff(int channel, string configFile, bool isNoPuReweight, bool isDebugMode)
{

  INPUT_ = new TAllInputSamples(channel, configFile);

  channel_=channel;
  isDebugMode_=isDebugMode;
  isNoPuReweight_=isNoPuReweight;
  photonCorrector_ = new zgamma::PhosphorCorrectionFunctor((config_.GetPhosphorConstantFileName()) );
    //field of this class

  nAccTotal_=0;
  nAccPassed_=0;
  nEffTotal_=0;
  nEffPassed_=0;
  nAccTotalErr_=0;
  nAccPassedErr_=0;
  nEffTotalErr_=0;
  nEffPassedErr_=0;

  acc_=-1;
  eff_=-1;
  accErr_=-1;
  effErr_=-1;

  for (int i=0; i<config_.GetNPhoPtBins(); i++){
    vecnAccTotal_.push_back(0);
    vecnAccPassed_.push_back(0);
    vecnEffTotal_.push_back(0);
    vecnEffPassed_.push_back(0);
    vecnAccTotalErr_.push_back(0);
    vecnAccPassedErr_.push_back(0);
    vecnEffTotalErr_.push_back(0);
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

           puWeight_=new TPuReweight(config_.GetPileupDataFileName(),INPUT_->allInputs_[iSource].fileNames_[inputFileN]);

           LoopOverTreeEvents();
             //method of this class (CalcAccAndEff)
           fChain=0;
             //field of TEventTree

           delete puWeight_;

         }//loop over inputFileN_ ends

     nAccTotalErr_=sqrt(nAccTotalErr_);
     nAccPassedErr_=sqrt(nAccPassedErr_);
     nEffTotalErr_=sqrt(nEffTotalErr_);
     nEffPassedErr_=sqrt(nEffPassedErr_);
     for (int i=0; i<config_.GetNPhoPtBins(); i++){
       vecnAccTotalErr_[i]=sqrt(vecnAccTotalErr_[i]);
       vecnAccPassedErr_[i]=sqrt(vecnAccPassedErr_[i]);
       vecnEffTotalErr_[i]=sqrt(vecnEffTotalErr_[i]);
       vecnEffPassedErr_[i]=sqrt(vecnEffPassedErr_[i]);
     }

     if (nAccTotal_!=0){ 
       acc_=nAccPassed_/nAccTotal_;
       accErr_=math_.ErrOfTwoIndependent("x1/(x1+x2)",nAccPassed_,nAccTotal_,nAccPassedErr_,nAccTotalErr_);
     }
     if (nEffTotal_!=0){ 
       eff_=(double)nEffPassed_/nEffTotal_;
       effErr_=math_.ErrOfTwoIndependent("x1/(x1+x2)",nEffPassed_,nEffTotal_,nEffPassedErr_,nEffTotalErr_);
     }

     for (int i=0; i<config_.GetNPhoPtBins(); i++){
       vecacc_[i]=vecnAccPassed_[i]/vecnAccTotal_[i];
       vecaccErr_[i]=math_.ErrOfTwoIndependent("x1/(x1+x2)",vecnAccPassed_[i],vecnAccTotal_[i],vecnAccPassedErr_[i],vecnAccTotalErr_[i]);
       veceff_[i]=vecnEffPassed_[i]/vecnEffTotal_[i];
       veceffErr_[i]=math_.ErrOfTwoIndependent("x1/(x1+x2)",vecnEffPassed_[i],vecnEffTotal_[i],vecnEffPassedErr_[i],vecnEffTotalErr_[i]);
     }

     PlotAndSaveOutput();

     std::cout<<"Summary:"<<std::endl;
     std::cout<<"nAccTotal="<<nAccTotal_<<"+-"<<nAccTotalErr_;
     std::cout<<", nAccPassed="<<nAccPassed_<<"+-"<<nAccPassedErr_<<", acc="<<acc_<<"+-"<<accErr_<<std::endl;
     std::cout<<"nEffTotal="<<nEffTotal_<<"+-"<<nEffTotalErr_;
     std::cout<<", nEffPassed="<<nEffPassed_<<"+-"<<nEffPassedErr_<<", eff="<<eff_<<"+-"<<effErr_<<std::endl;
     std::cout<<std::endl;

     std::cout<<std::endl;
     for (int i=0; i<config_.GetNPhoPtBins(); i++){
       std::cout<<std::endl;
       std::cout<<"pho Pt: "<<vecPhoPtLimits_[i]<<" - "<<vecPhoPtLimits_[i+1]<<std::endl;
       std::cout<<" -- nAccTotal="<<vecnAccTotal_[i]<<"+-"<<vecnAccTotalErr_[i];
       if (vecnAccTotal_[i]==0) std::cout<<std::endl;
       else std::cout<<", nAccPassed="<<vecnAccPassed_[i]<<"+-"<<vecnAccPassedErr_[i]<<", acc="<<vecacc_[i]<<"+-"<<vecaccErr_[i]<<std::endl;

       std::cout<<" -- nEffTotal="<<vecnEffTotal_[i]<<"+-"<<vecnEffTotalErr_[i];
       if (vecnEffTotal_[i]==0) std::cout<<std::endl; 
       else std::cout<<", nEffPassed="<<vecnEffPassed_[i]<<"+-"<<vecnEffPassedErr_[i]<<", eff="<<veceff_[i]<<"+-"<<veceffErr_[i]<<std::endl;
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

   CheckMaxNumbersInTree();
  
   //nentries=20;
   for (Long64_t entry=0; entry<nentries; entry++) 
   //loop over events in the tree
     {

        GetEntryNeededBranchesOnly(channel_,TConfiguration::SIGMC,entry);
          //method of TEventTree class
        if (!treeLeaf.isData) GetEntryMCSpecific(entry);
          //method of TEventTree class

        float weightPU;
        if (isNoPuReweight_)
          weightPU = puWeight_->GetPuWeightMc(treeLeaf.puTrue->at(1));
        else weightPU=1;

        if (channel_==TConfiguration::MUON) nLe_=treeLeaf.nMu;
        else if (channel_==TConfiguration::ELECTRON) nLe_=treeLeaf.nEle;
        else
          {
             std::cout<<"Error detected in  CalcAccAndEff::LoopOverTreeEvents: channel must be either MUON or ELECTRON."<<std::cout;
             return;
          }  

        // apply acceptance cuts

        bool accPassed = AcceptancePassed(accLeptonPhotonPassed);

        //apply efficiency cuts
        bool effPassed = EfficiencyPassed(effLeptonPhotonPassed,WMt,lePhoDeltaR,weightPU);
   
  


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

bool CalcAccAndEff::AcceptancePassed(bool** accLeptonPhotonPassed)
{
   bool accPassed=0;
   int bin=-1;

   bool genPhotons[TEventTree::kMaxnMC];
   bool genLeptons[TEventTree::kMaxnMC];
   for (int iMC=0; iMC<treeLeaf.nMC; iMC++){
     genPhotons[iMC]=0;
     genLeptons[iMC]=0;
     //if (treeLeaf.mcMomPID->at(iMC)==24 || treeLeaf.mcMomPID->at(iMC)==-24){
       if (treeLeaf.mcPID->at(iMC)==22)
         genPhotons[iMC]=1;
       else if (channel_==TConfiguration::MUON && ((treeLeaf.mcPID->at(iMC)==13) || (treeLeaf.mcPID->at(iMC)==-13)))
         genLeptons[iMC]=1;
       else if (channel_==TConfiguration::ELECTRON && ((treeLeaf.mcPID->at(iMC)==11) || (treeLeaf.mcPID->at(iMC)==-11)))
         genLeptons[iMC]=1;
     //}
 
   } // loop over iMC; search for gen level photons and leptons

   for (int iGenLep=0; iGenLep<treeLeaf.nMC; iGenLep++)
     for (int iGenPho=0; iGenPho<treeLeaf.nMC; iGenPho++){
       if (!(genLeptons[iGenLep] && genPhotons[iGenPho])) continue;
         float dR = math_.DeltaR(treeLeaf.mcEta->at(iGenLep),treeLeaf.mcPhi->at(iGenLep),treeLeaf.mcEta->at(iGenPho),treeLeaf.mcPhi->at(iGenPho));
         if (dR<=config_.GetLePhoDeltaRMin()) continue;

//         if (treeLeaf.mcEt->at(iGenPho)>80) 
//           std::cout<<"treeLeaf.mcEt->at(iGenPho)="<<treeLeaf.mcEt->at(iGenPho)<<std::endl;
         /////////////////////////////////////////////////
         // count number of events 
         if (treeLeaf.mcEt->at(iGenPho)>config_.GetPhoPtMin()){
            nAccTotal_+=lumiWeight_;
            nAccTotalErr_+=lumiWeight_*lumiWeight_;
         }
         bin = config_.FindPhoPtBinByPhoPt(treeLeaf.mcEt->at(iGenPho));
         if (bin<(int)vecnAccTotal_.size() && bin!=-1){
           vecnAccTotal_[bin]+=lumiWeight_;
           vecnAccTotalErr_[bin]+=lumiWeight_*lumiWeight_;
         }
         /////////////////////////////////////////////////
         // count number of events within acceptance

         bool isPhoAcc = photonEmpty_.PhoKinematics(treeLeaf.mcE->at(iGenPho),treeLeaf.mcEta->at(iGenPho)) ;
         bool isLepAcc = (channel_==TConfiguration::MUON && 
                   muonEmpty_.MuKinematics(treeLeaf.mcPt->at(iGenLep),treeLeaf.mcEta->at(iGenLep))) ||
                   (channel_==TConfiguration::ELECTRON);
         if (isPhoAcc && isLepAcc) {
           accPassed=1;
           accLeptonPhotonPassed[iGenLep][iGenPho]=1;
           if (treeLeaf.mcEt->at(iGenPho)>config_.GetPhoPtMin()){
             nAccPassed_+=lumiWeight_;
             nAccPassedErr_+=lumiWeight_*lumiWeight_;
           }
           bin = config_.FindPhoPtBinByPhoPt(treeLeaf.mcEt->at(iGenPho));
           if (bin<(int)vecnAccPassed_.size() && bin!=-1){
             vecnAccPassed_[bin]+=lumiWeight_;
             vecnAccPassedErr_[bin]+=lumiWeight_*lumiWeight_;
            }
          }  //end of if (isPhoAcc && isLepAcc)
        }//end of loops over iGenPho and iGenLep
  return accPassed;
}

bool CalcAccAndEff::EfficiencyPassed(bool** effLeptonPhotonPassed, float* WMt, float** lePhoDeltaR,float weightPU)
{
  
  bool effPassed = fullCuts_.Cut(effLeptonPhotonPassed, treeLeaf,   
                channel_,  WMt, lePhoDeltaR, photonCorrector_);
  for (int ile=0; ile<nLe_; ile++)
    for (int ipho=0; ipho<treeLeaf.nPho; ipho++){

      float dR = 0;
      if (channel_==TConfiguration::MUON) 
      dR = math_.DeltaR(treeLeaf.muEta->at(ile),treeLeaf.muPhi->at(ile),treeLeaf.phoEta->at(ipho),treeLeaf.phoPhi->at(ipho));
      else if (channel_==TConfiguration::ELECTRON);
      //dR = math_.DeltaR(treeLeaf.eleEta->at(ile),treeLeaf.elePhi->at(ile),treeLeaf.phoEta->at(ipho),treeLeaf.phoPhi->at(ipho));
      if (dR<=config_.GetLePhoDeltaRMin()) continue;

      /////////////////////////////////////////////////
      // count number of events 
      if (treeLeaf.phoEt->at(ipho)>config_.GetPhoPtMin()){
        nEffTotal_+=lumiWeight_*weightPU;
        nEffTotalErr_+=lumiWeight_*weightPU*lumiWeight_*weightPU;
      }
      int bin = config_.FindPhoPtBinByPhoPt(treeLeaf.phoEt->at(ipho));
      if (bin<(int)vecnEffTotal_.size() && bin!=-1){
        vecnEffTotal_[bin]+=lumiWeight_*weightPU;
        vecnEffTotalErr_[bin]+=lumiWeight_*weightPU*lumiWeight_*weightPU;
      }
      if (!effPassed) continue;
      ////////////////////////////////////
      // count number of events in efficiency
      if (effLeptonPhotonPassed[ile][ipho]){
        if (treeLeaf.phoEt->at(ipho)>config_.GetPhoPtMin()){
          nEffPassed_+=lumiWeight_*weightPU;
          nEffPassedErr_+=(lumiWeight_*weightPU)*(lumiWeight_*weightPU);
        }
        bin = config_.FindPhoPtBinByPhoPt(treeLeaf.phoEt->at(ipho));
        if (bin<(int)vecnEffPassed_.size() && bin!=-1){
          vecnEffPassed_[bin]+=lumiWeight_*weightPU;
          vecnEffPassedErr_[bin]+=(lumiWeight_*weightPU)*(lumiWeight_*weightPU);
         }
       }//end of if (effLeptonPhotonPassed[ile][ipho])
    }//end of loops over ipho and ile
  return effPassed;
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

bool CalcAccAndEff::CheckMaxNumbersInTree()
{
  if (fChain->GetMaximum("nMC")>kMaxnMC){
    std::cout<<"ERROR in CalcAccAndEff::LoopOverTreeEvents: treeLeaf.nMC="<<fChain->GetMaximum("nMC")<<" > kMaxnMC="<<kMaxnMC<<"; go to ../Include/TEventTree.h to increase kMaxnMC"<<std::endl;
    return 0;
  }
  if (fChain->GetMaximum("nPho")>kMaxnPho){
     std::cout<<"ERROR in CalcAccAndEff::LoopOverTreeEvents: treeLeaf.nPho="<<fChain->GetMaximum("nPho")<<" > kMaxnPho="<<kMaxnPho<<"; go to ../Include/TEventTree.h to increase kMaxnPho"<<std::endl;
     return 0;
   }
   if (channel_==TConfiguration::MUON && fChain->GetMaximum("nMu")>kMaxnMu){
     std::cout<<"ERROR in CalcAccAndEff::LoopOverTreeEvents: treeLeaf.nMu="<<fChain->GetMaximum("nMu")<<" > kMaxnMu="<<kMaxnMu<<"; go to ../Include/TEventTree.h to increase kMaxnMu"<<std::endl;
     return 0;
   }
   if (channel_==TConfiguration::ELECTRON && fChain->GetMaximum("nEle")>kMaxnEle){
     std::cout<<"ERROR in CalcAccAndEff::LoopOverTreeEvents: treeLeaf.nEle="<<fChain->GetMaximum("nEle")<<" > kMaxnEle="<<kMaxnEle<<"; go to ../Include/TEventTree.h to increase kMaxnEle"<<std::endl;
     return 0;
   }
  return 1;
}
