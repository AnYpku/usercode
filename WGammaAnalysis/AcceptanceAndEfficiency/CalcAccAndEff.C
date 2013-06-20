#include "CalcAccAndEff.h" 
  //this class
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
  // ROOT class
#include <iostream> 
#include <string>
#include <sstream>  
  //standard C++ class

CalcAccAndEff::CalcAccAndEff(int channel, string configFile, bool isReleasedCutsMode, bool isPuReweight, bool isDebugMode)
{

  INPUT_ = new TAllInputSamples(channel, configFile);

  channel_=channel;
  isReleasedCutsMode_=isReleasedCutsMode;
  isDebugMode_=isDebugMode;
  isPuReweight_=isPuReweight;
  photonCorrector_ = new zgamma::PhosphorCorrectionFunctor((config_.GetPhosphorConstantFileName()).c_str());
    //field of this class

  nEvents_=0;
  nAccPassed_=0;
  nEventsInAcc_=0;
  nEffPassed_=0;
  nEventsErr_=0;
  nAccPassedErr_=0;
  nEventsInAccErr_=0;
  nEffPassedErr_=0;
  for (int i=0; i<config_.GetNPhoPtBins(); i++){
    vecnEvents_.push_back(0);
    vecnAccPassed_.push_back(0);
    vecnEventsInAcc_.push_back(0);
    vecnEffPassed_.push_back(0);
    vecnEventsErr_.push_back(0);
    vecnAccPassedErr_.push_back(0);
    vecnEventsInAccErr_.push_back(0);
    vecnEffPassedErr_.push_back(0);
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
           TFile f((INPUT_->allInputs_[iSource].fileNames_[inputFileN]).c_str());
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

     std::cout<<"Summary:"<<std::endl;
     std::cout<<"nEvents="<<nEvents_<<"+-"<<nEventsErr_;
     if (nEvents_==0) continue;
     std::cout<<", nAccPassed="<<nAccPassed_<<"+-"<<nAccPassedErr_<<", acc="<<(double)nAccPassed_/nEvents_<<std::endl;
     std::cout<<"nEventsInAcc="<<nEventsInAcc_<<"+-"<<nEventsInAccErr_;
     if (nEventsInAcc_==0) continue;
     std::cout<<", nEffPassed="<<nEffPassed_<<"+-"<<nEffPassedErr_<<", eff="<<(double)nEffPassed_/nEventsInAcc_<<std::endl;
     std::cout<<std::endl;

     std::cout<<std::endl;
     for (int i=0; i<config_.GetNPhoPtBins(); i++){
       std::cout<<std::endl;
       std::cout<<"pho Pt: "<<vecPhoPtLimits_[i]<<" - "<<vecPhoPtLimits_[i+1]<<std::endl;
       std::cout<<" -- nEvents="<<vecnEvents_[i]<<"+-"<<vecnEventsErr_[i];
       if (vecnEvents_[i]==0) continue;
       std::cout<<", nAccPassed="<<vecnAccPassed_[i]<<"+-"<<vecnAccPassedErr_[i]<<", acc="<<(double)vecnAccPassed_[i]/vecnEvents_[i]<<std::endl;
       std::cout<<" -- nEventsInAcc="<<vecnEventsInAcc_[i]<<"+-"<<vecnEventsInAccErr_[i];
       if (vecnEventsInAcc_[i]==0) continue;
       std::cout<<", nEffPassed="<<vecnEffPassed_[i]<<"+-"<<vecnEffPassedErr_[i]<<", eff="<<(double)vecnEffPassed_[i]/vecnEventsInAcc_[i]<<std::endl;
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
   if (channel_==TInputSample::MUON) nLeptonMax=kMaxnMu;
   else if (channel_==TInputSample::ELECTRON) nLeptonMax=kMaxnEle;
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

   bool** accLeptonPhotonPassed = new bool*[nLeptonMax];
   for (int ile=0; ile<nLeptonMax; ile++)
     accLeptonPhotonPassed[ile]=new bool[kMaxnPho];

//   CheckMaxNumbersInTree();
  
   TFullCuts fullCuts;
   TMuonCuts muonEmpty;
//   TElectronCuts electronEmpty;
   TPhotonCuts photonEmpty;

   for (Long64_t entry=0; entry<nentries; entry++) 
   //loop over events in the tree
     {
        GetEntryNeededBranchesOnly(entry);

        if (!treeLeaf.isData) GetEntryMCSpecific(entry);
          //method of TEventTree class

        float weightPU;
        if (isPuReweight_)
          weightPU = puWeight_->GetPuWeightMc(treeLeaf.puTrue[1]);
        else weightPU=1;

        if (channel_==TInputSample::MUON) nLe_=treeLeaf.nMu;
        else if (channel_==TInputSample::ELECTRON) nLe_=treeLeaf.nEle;
        else
          {
             std::cout<<"Error detected in  CalcAccAndEff::LoopOverTreeEvents: channel must be either MUON or ELECTRON."<<std::cout;
             return;
          }
        
        // apply acceptance cuts
        bool accPassed=0;
        int bin=-1;

        for (int ile=0; ile<nLe_; ile++)
          for (int ipho=0; ipho<treeLeaf.nPho; ipho++){
            int iphoMC=-999;
            int ileMC=-999;
            for (int imc=0; imc<treeLeaf.nMC; imc++){
              if (treeLeaf.mcIndex[imc]==treeLeaf.phoGenIndex[ipho]) 
                iphoMC=imc;
              if (channel_==TInputSample::MUON && treeLeaf.mcIndex[imc]==treeLeaf.muGenIndex[ile])   
                ileMC=imc;
            }//end of loop over iMC

            nEvents_+=lumiWeight_;
            nEventsErr_+=lumiWeight_*lumiWeight_;
            bin = config_.FindPhoPtBinByPhoPt(treeLeaf.mcEt[iphoMC]);
            if (bin<vecnEvents_.size() && bin!=-1){
              vecnEvents_[bin]+=lumiWeight_;
              vecnEventsErr_[bin]+=lumiWeight_*lumiWeight_;
            }
            else if (bin==-1);
            else {
              std::cout<<"ERROR detected in CalcAccAndEff::LoopOverTreeEvents(): pho pt bin found = "<<bin<<", but size of vector = "<<vecnEvents_.size()<<std::endl;
              return;
            }

            if (iphoMC>=0 && photonEmpty.PhoKinematics(treeLeaf.mcEt[iphoMC],treeLeaf.mcEta[iphoMC])){

              if ((ileMC>=0 && channel_==TInputSample::MUON && 
                   muonEmpty.MuKinematics(treeLeaf.mcPt[ileMC], 
                                         treeLeaf.mcEta[ileMC])) ||
                   (ileMC>=0 && channel_==TInputSample::ELECTRON)) {

                  accPassed=1;
                  accLeptonPhotonPassed[ile][ipho]=1;
                  nAccPassed_+=lumiWeight_;
                  nAccPassedErr_+=lumiWeight_*lumiWeight_;
                  nEventsInAcc_+=lumiWeight_*weightPU;
                  nEventsInAccErr_+=(lumiWeight_*weightPU)*(lumiWeight_*weightPU);
                  bin = config_.FindPhoPtBinByPhoPt(treeLeaf.mcEt[iphoMC]);
                  if (bin<vecnAccPassed_.size() && bin!=-1){
                    vecnAccPassed_[bin]+=lumiWeight_;
                    vecnAccPassedErr_[bin]+=lumiWeight_*lumiWeight_;
                  }
                  bin = config_.FindPhoPtBinByPhoPt(treeLeaf.phoEt[ipho]);
                  if (bin<vecnEventsInAcc_.size() && bin!=-1){
                    vecnEventsInAcc_[bin]+=lumiWeight_*weightPU;
                    vecnEventsInAccErr_[bin]+=(lumiWeight_*weightPU)*(lumiWeight_*weightPU);
                  }
              }//end of if ((ileMC>=0 && channel_==TInputSample::MUON ...
            } //end of if (iphoMC>=0 && photonEmpty.PhoKinematics())

          }//end of loop over ipho
        //end of loop over ile

        if (!accPassed) continue;

        //apply efficiency cuts
        if (fullCuts.Cut(effLeptonPhotonPassed, treeLeaf,   
                channel_,  isReleasedCutsMode_,
                WMt, lePhoDeltaR, photonCorrector_) == 1){
              //method of this class (WGammaSelection)
          for (int ile=0; ile<nLe_; ile++)
            for (int ipho=0; ipho<treeLeaf.nPho; ipho++)
                if (effLeptonPhotonPassed[ile][ipho] && accLeptonPhotonPassed[ile][ipho]){

                    nEffPassed_+=lumiWeight_*weightPU;
                    nEffPassedErr_+=(lumiWeight_*weightPU)*(lumiWeight_*weightPU);
                    bin = config_.FindPhoPtBinByPhoPt(treeLeaf.phoEt[ipho]);
                    if (bin<vecnEffPassed_.size() && bin!=-1){
                      vecnEffPassed_[bin]+=lumiWeight_*weightPU;
                      vecnEffPassedErr_[bin]+=(lumiWeight_*weightPU)*(lumiWeight_*weightPU);
                    }

                }

        }//end of if (fullCuts.Cut())
        else continue;

     } //end of loop over events in the tree

  //memory release:

  for (int ile=0; ile<nLeptonMax; ile++)
    delete effLeptonPhotonPassed[ile];
  delete[] effLeptonPhotonPassed;

  for (int ile=0; ile<nLeptonMax; ile++)
    delete accLeptonPhotonPassed[ile];
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