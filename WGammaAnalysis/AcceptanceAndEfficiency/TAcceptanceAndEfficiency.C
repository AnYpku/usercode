#include "TAcceptanceAndEfficiency.h" 
  //this class
#include "../Include/TMetTools.h" 
#include "../Include/TMuonCuts.h" 
#include "../Include/TElectronCuts.h" 
#include "../Include/TPhotonCuts.h" 
#include "../Include/TFullCuts.h" 
#include "../Configuration/TConfiguration.h" 
#include "../Configuration/TInputSample.h"
#include "../Configuration/TAllInputSamples.h"
  //this package
#include "../Include/PhosphorCorrectorFunctor.hh"
  //taken from http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/CPena/src/PHOSPHOR_Corr_v2/
  //currently in this package
#include "TMath.h" 
  // ROOT class
#include <iostream> 
#include <string>
#include <sstream>  
  //standard C++ class

TAcceptanceAndEfficiency::TAcceptanceAndEfficiency(int channel, string configFile, bool isReleasedCutsMode, bool isDebugMode)
{

  INPUT_ = new TAllInputSamples(channel, configFile);

  channel_=channel;
  isReleasedCutsMode_=isReleasedCutsMode;
  isDebugMode_=isDebugMode;
  TConfiguration config;
  photonCorrector_ = new zgamma::PhosphorCorrectionFunctor((config.GetPhosphorConstantFileName()).c_str());
    //field of this class
}

TAcceptanceAndEfficiency::~TAcceptanceAndEfficiency()
{
   fChain = 0;
     //field of TEventTree 
   delete photonCorrector_;
}

void TAcceptanceAndEfficiency::LoopOverInputFiles()
{
  TConfiguration config;
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

           TFile f((INPUT_->allInputs_[iSource].fileNames_[inputFileN]).c_str());
           if (f.IsOpen()) 
             std::cout<<std::endl<<"processing file "<<INPUT_->allInputs_[iSource].fileNames_[inputFileN]<<std::endl;
           else
             {
                std::cout<<"ERROR detected in TAcceptanceAndEfficiency::LoopOverInputFiles: file "<<INPUT_->allInputs_[iSource].fileNames_[inputFileN]<<" was not found"<<std::endl;
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
                std::cout<<"ERROR detected in TAcceptanceAndEfficiency::LoopOverInputFiles: tree in the file "<<INPUT_->allInputs_[iSource].fileNames_[inputFileN]<<" does not exist"<<std::endl;
                return;
             }  
 
      
           LoopOverTreeEvents();
             //method of this class (TAcceptanceAndEfficiency)
           fChain=0;
             //field of TEventTree

         }//loop over inputFileN_ ends

      std::cout<<"the output will be saved to "<<std::endl<<std::endl;

    } //loop over iSource ends
}


void TAcceptanceAndEfficiency::LoopOverTreeEvents()
{
   if (fChain == 0) return;
   Long64_t nentries = fChain->GetEntries();
   if (isDebugMode_) 
     {
       if (fChain->GetEntries()<debugModeNEntries_) nentries=fChain->GetEntries();
       else nentries=debugModeNEntries_;
     }
   int nAccPassed=0;
   int nEffPassed=0;

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

        if (channel_==TInputSample::MUON) nLe_=treeLeaf.nMu;
        else if (channel_==TInputSample::ELECTRON) nLe_=treeLeaf.nEle;
        else
          {
             std::cout<<"Error detected in  TAcceptanceAndEfficiency::LoopOverTreeEvents: channel must be either MUON or ELECTRON."<<std::cout;
             return;
          }
        
        // apply acceptance cuts
        bool accPassed=0;
        for (int ile=0; ile<nLe_; ile++)
          for (int ipho=0; ipho<treeLeaf.nPho; ipho++)
            if (photonEmpty.PhoKinematics(treeLeaf.phoSCEt[treeLeaf.phoGenIndex[ipho]],
                                          treeLeaf.phoSCEta[treeLeaf.phoGenIndex[ipho]])){

              if (channel_==TInputSample::MUON && 
                  muonEmpty.MuKinematics(treeLeaf.muPt[treeLeaf.muGenIndex[ile]], 
                                         treeLeaf.muEta[treeLeaf.muGenIndex[ile]])) 
                {
                  accPassed=1;
                  accLeptonPhotonPassed[ile][ipho]=1;
                  nAccPassed++;
                }
              else if (channel_==TInputSample::ELECTRON) 
                {
                  accPassed=1;
                  accLeptonPhotonPassed[ile][ipho]=1;
                  nAccPassed++;
                }
            } //if photonEmpty.PhoKinematics()
        if (!accPassed) continue;

        //apply efficiency cuts
        if (fullCuts.Cut(effLeptonPhotonPassed, treeLeaf,   
                channel_,  isReleasedCutsMode_,
                WMt, lePhoDeltaR, photonCorrector_) == 1)
              //method of this class (WGammaSelection)

          for (int ile=0; ile<nLe_; ile++)
            for (int ipho=0; ipho<treeLeaf.nPho; ipho++)
                if (effLeptonPhotonPassed[ile][ipho] && accLeptonPhotonPassed[ile][ipho])
                    nEffPassed+=1;
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

  std::cout<<"Summary:"<<std::endl;
  std::cout<<"nEntries="<<nentries<<std::endl;
  if (nentries==0) return;
  std::cout<<"nAccPassed="<<nAccPassed<<", acc="<<(double)nAccPassed/nentries<<std::endl;
  if (nAccPassed==0) return;
  std::cout<<"nEffPassed="<<nEffPassed<<", eff="<<(double)nEffPassed/nAccPassed<<std::endl;
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

/*
bool WGammaSelection::CheckMaxNumbersInTree()
{
   if ((channel_=TInputSample::MUON) 
       && (fChain->GetMaximum("nMu")>kMaxnMu))
     //kMaxnMu - field of TEventTree
     {
       PrintErrorMessageMaxNumberOf(MUON_);
          //methof of this class (WGammaSelection)
       return 0;
     }
   if (channel_==TInputSample::ELECTRON 
       && fChain->GetMaximum("nEle")>kMaxnEle)
     {
       PrintErrorMessageMaxNumberOf(ELECTRON_);
          //methof of this class (WGammaSelection)
       return 0;
     }
   if (fChain->GetMaximum("nPho")>kMaxnPho)
     {
       PrintErrorMessageMaxNumberOf(PHOTON_);
          //methof of this class (WGammaSelection)
       return 0;
     }
   if (fChain->GetMaximum("nJet")>kMaxnJet)
     {
       PrintErrorMessageMaxNumberOf(JET_);
          //methof of this class (WGammaSelection)
       return 0;
     }
   if (fChain->GetMaximum("nLowPtJet")>kMaxnLowPtJet)
     {
       PrintErrorMessageMaxNumberOf(LOWPTJET_);
          //methof of this class (WGammaSelection)
       return 0;
     }
   if (!treeLeaf.isData && fChain->GetMaximum("nMC")>kMaxnMC)
     {
       PrintErrorMessageMaxNumberOf(MC_);
          //methof of this class (WGammaSelection)
       return 0;
     }
   return 1;
}

void WGammaSelection::PrintErrorMessageMaxNumberOf(int particle)
{
       std::cout<<std::endl;
       std::cout<<"Error detected in WGammaSelection::PrintErrorMessageMaxNumberOf:"<<std::endl;
       if (particle==MUON_)
         std::cout<<"maximum number of muons in the file nMu="<<fChain->GetMaximum("nMu")<<", when kMaxnMu="<<kMaxnMu<<" only"<<std::endl;
       else if (particle==ELECTRON_)
         std::cout<<"maximum number of electrons in the file nEle="<<fChain->GetMaximum("nEle")<<", when kMaxnEle="<<kMaxnEle<<" only"<<std::endl;
       else if (particle==PHOTON_)
         std::cout<<"maximum number of photons in the file nPho="<<fChain->GetMaximum("nPho")<<", when kMaxnPho="<<kMaxnPho<<" only"<<std::endl;
       else if (particle==JET_)
         std::cout<<"maximum number of jets in the file nJet="<<fChain->GetMaximum("nJet")<<", when kMaxnJet="<<kMaxnJet<<" only"<<std::endl;
      else if (particle==LOWPTJET_)
         std::cout<<"maximum number of low Pt jets in the file nJet="<<fChain->GetMaximum("nJet")<<", when kMaxnLowPtJet="<<kMaxnLowPtJet<<" only"<<std::endl;
      else if (particle==MC_)
         std::cout<<"maximum number of mc particles in the file nMC="<<fChain->GetMaximum("nMC")<<", when kMaxnMC="<<kMaxnMC<<" only"<<std::endl;
       std::cout<<"please, go to TEventTree.h to increase this number up to proper value"<<std::endl;
       std::cout<<std::endl;
}
*/
