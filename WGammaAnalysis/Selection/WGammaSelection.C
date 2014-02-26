#include "WGammaSelection.h" 
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
  // ROOT class
#include <iostream> 
#include <string>
#include <sstream>  
  //standard C++ class

WGammaSelection::WGammaSelection(int channel, int sampleMode, string configFile, bool isNoPuReweight, bool isDebugMode)
{

  INPUT_ = new TAllInputSamples(channel, configFile);

  channel_=channel;
  isNoPuReweight_=isNoPuReweight;
  isDebugMode_=isDebugMode;
  photonCorrector_ = new zgamma::PhosphorCorrectionFunctor(config.GetPhosphorConstantFileName());
    //field of this class, WGammaSelection

  sampleMode_=sampleMode;
  if (sampleMode_==NOTSPECIFIED){
    std::cout<<"ERROR in WGammaSelection::WGammaSelection: sampleMode_ is NONSPECIFIED"<<std::endl;
    return;
  }
  else if (sampleMode_>NOTSPECIFIED){
    std::cout<<"ERROR in WGammaSelection::WGammaSelection: wrong sampleMode_ "<<std::endl;
    return;    
  }
  for (int i=0; i<INPUT_->nSources_; i++)
    {
      doAnalizeSample_.push_back(1);      
    } 
}

WGammaSelection::WGammaSelection(int channel, string analyzedSampleNames, string configFile, bool isNoPuReweight, bool isDebugMode, bool isVeryLooseSelectionMode)
{

  INPUT_ = new TAllInputSamples(channel, configFile);

  channel_=channel;
  isNoPuReweight_=isNoPuReweight;
  isDebugMode_=isDebugMode;
  isVeryLooseSelectionMode_=isVeryLooseSelectionMode;
  photonCorrector_ = new zgamma::PhosphorCorrectionFunctor(config.GetPhosphorConstantFileName());
    //field of this class, WGammaSelection

  sampleMode_=NOTSPECIFIED;
  stringstream ss(analyzedSampleNames);
  vector <string> names;
  string name;
  while (ss >> name) 
    names.push_back(name);
  int nNames = names.size();

  std::cout<<"1 - will do, 0 - will not do"<<std::endl;
  for (int i=0; i<INPUT_->nSources_; i++)
    {
      doAnalizeSample_.push_back(0);  
      for (int j=0; j<nNames; j++)
        {
          if (names[j]==INPUT_->allInputs_[i].sourceName_)
            doAnalizeSample_.back()=1; 
        }
      std::cout<<INPUT_->allInputs_[i].sourceName_<<": "<<doAnalizeSample_[i]<<std::endl;
    }  
  if ((int)doAnalizeSample_.size()!=INPUT_->nSources_)
    std::cout<<"ERROR in WGammaSelection::WGammaSelection: wrong doAnalizeSample_.size()"<<std::endl;

  doSigmaIEtaIEtaCut_=1;
}

WGammaSelection::~WGammaSelection()
{
   fChain = 0;
     //field of TEventTree 
   delete photonCorrector_;
}

void WGammaSelection::LoopOverInputFiles()
{
  int nSources = INPUT_->nSources_; 
  //if (isDebugMode_ && nSources>3)
  //  nSources = 3;

  for (int iSource=0; iSource<nSources; iSource++)
    {
       if (!doAnalizeSample_[iSource])
         continue;

       sample_ = INPUT_->allInputs_[iSource].sample_;
       if (sample_==TInputSample::DATA)
           if (sampleMode_==SIGMC || sampleMode_==NOBKG || sampleMode_==MC)
             continue;
       if (sample_==TInputSample::SIGMC)
           if (sampleMode_==DATA || sampleMode_==BKGMC)
             continue;
       if (sample_==TInputSample::BKGMC)
           if (sampleMode_==DATA || sampleMode_==SIGMC || sampleMode_==NOBKG)
           continue;
       selectedTreeFileName_=config.GetSelectedName(config.VERY_PRELIMINARY,channel_,INPUT_->allInputs_[iSource].sample_,INPUT_->allInputs_[iSource].sourceName_,isDebugMode_,isNoPuReweight_,isVeryLooseSelectionMode_);
       TTree* tree;
       int inputFileNMax = INPUT_->allInputs_[iSource].nFiles_;
       //if (isDebugMode_) 
       //  {
       //    if (INPUT_->allInputs_[iSource].nFiles_<1) inputFileNMax=INPUT_->allInputs_[iSource].nFiles_;
       //    else inputFileNMax=1;
       //  }

       TString fileOutName;
       fileOutName = selectedTreeFileName_;
       TFile fileOut(fileOutName,"recreate");
       TTree* outTree = new TTree("selectedEvents","selected Events");
       SetOutputTree(outTree); 

       for (inputFileN_=0; inputFileN_< inputFileNMax; inputFileN_++)
         {

           lumiWeight_=INPUT_->allInputs_[iSource].lumiWeights_[inputFileN_];
           if (!treeLeaf.isData && isDebugMode_){
             if (INPUT_->allInputs_[iSource].isSharedCS_)
               debugModeWeight_=1.0/INPUT_->allInputs_[iSource].csTotal_;
              else
               debugModeWeight_=1.0/INPUT_->allInputs_[iSource].cs_[inputFileN_];
           }

           TFile f(INPUT_->allInputs_[iSource].fileNames_[inputFileN_]);
           if (f.IsOpen()) 
             std::cout<<std::endl<<"processing file "<<INPUT_->allInputs_[iSource].fileNames_[inputFileN_]<<std::endl;
           else
             {
                std::cout<<"ERROR detected in WGammaSelection::LoopOverInputFiles: file "<<INPUT_->allInputs_[iSource].fileNames_[inputFileN_]<<" was not found"<<std::endl;
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
                std::cout<<"ERROR detected in WGammaSelection::LoopOverInputFiles: tree in the file "<<INPUT_->allInputs_[iSource].fileNames_[inputFileN_]<<" does not exist"<<std::endl;
                return;
             }  
 
           puWeight_=new TPuReweight(config.GetPileupDataFileName(),INPUT_->allInputs_[iSource].fileNames_[inputFileN_]);

           
          LoopOverTreeEvents();
             //method of this class (WGammaSelection)
           fChain=0;
             //field of TEventTree

           delete puWeight_;

         }//loop over inputFileN_ ends

      std::cout<<"the output will be saved to "<<fileOut.GetName()<<std::endl<<std::endl;
      fileOut.cd();
      outTree->Write();
      fileOut.Close();
      //delete outTree;
      outTree = 0;


    } //loop over iSource ends
}


void WGammaSelection::LoopOverTreeEvents()
{
   if (fChain == 0) return;
   Long64_t nentries = fChain->GetEntries();
   if (isDebugMode_) 
     {
       if (fChain->GetEntries()<debugModeNEntries_) nentries=fChain->GetEntries();
       else nentries=debugModeNEntries_;
     }

   float nTotal=0;
   float nPassed=0;

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

   if (isVeryLooseSelectionMode_) nLeptonMax=1;

   float* WMt = new float[nLeptonMax];

   float** lePhoDeltaR = new float*[nLeptonMax];
   for (int ile=0; ile<nLeptonMax; ile++)
     lePhoDeltaR[ile]=new float[kMaxnPho];
   
   bool** goodLeptonPhotonPairs = new bool*[nLeptonMax];
   for (int ile=0; ile<nLeptonMax; ile++)
     goodLeptonPhotonPairs[ile]=new bool[kMaxnPho];

   CheckMaxNumbersInTree();

   nTotal_=0;
   nBeforeLeptonLoop_=0;
   nLeptons_=0;
   nLeptonsPassed_=0; 
   nMoreVetoPassed_=0;
   nWMtPassed_=0;
   nPhotons_=0;
   nPhotonsPassed_=0;
   nPhoLepPassed_=0;


   TPhotonCuts emptyPhoton;
   for (Long64_t entry=0; entry<nentries; entry++) 
   //loop over events in the tree
     {

        GetEntryNeededBranchesOnly(channel_,sample_,entry);
        if (!treeLeaf.isData) GetEntryMCSpecific(entry);
          //method of TEventTree class
        if (channel_==TConfiguration::MUON) nLe_=treeLeaf.nMu;
        else if (channel_==TConfiguration::ELECTRON) nLe_=treeLeaf.nEle;
        else
          {
             std::cout<<"Error detected in  WGammaSelection::LoopOverTreeEvents: channel must be either MUON or ELECTRON."<<std::cout;
             return;
          }
       totalWeight_ = lumiWeight_;
       if (!treeLeaf.isData && !isNoPuReweight_)
         totalWeight_*=puWeight_->GetPuWeightMc(treeLeaf.puTrue->at(1));
       nTotal+=totalWeight_;

       TFullCuts fullCuts;
       if (fullCuts.Cut(goodLeptonPhotonPairs, treeLeaf,   
                channel_,
                WMt, lePhoDeltaR, photonCorrector_, doSigmaIEtaIEtaCut_, doPhoChIsoCut_,isVeryLooseSelectionMode_) == 1)
              //method of this class (WGammaSelection)

          for (int ile=0; ile<nLe_; ile++)
            for (int ipho=0; ipho<treeLeaf.nPho; ipho++)
              {
                if (goodLeptonPhotonPairs[ile][ipho])
                  {

                    nPassed+=totalWeight_;
                    phoGenPID_=-1000;
                    leGenPID_=-1000;
                    phoGenEt_=-1000;
                      //members of TSelectedEvents
                    if (sample_==TInputSample::SIGMC ||
                        sample_==TInputSample::BKGMC)
                      {
                        for (int iMC=0; iMC<treeLeaf.nMC; iMC++){
                          if(treeLeaf.mcIndex->at(iMC)==treeLeaf.phoGenIndex->at(ipho)){
                            phoGenPID_=treeLeaf.mcPID->at(iMC);
                            phoGenEt_=treeLeaf.mcEt->at(iMC);
                          }
                          if (channel_==TConfiguration::MUON && 
                              treeLeaf.mcIndex->at(iMC)==treeLeaf.muGenIndex->at(ile))
                            leGenPID_=treeLeaf.mcPID->at(iMC);
                          if (channel_==TConfiguration::ELECTRON && 
                              treeLeaf.mcIndex->at(iMC)==treeLeaf.eleGenIndex->at(ile))
                            leGenPID_=treeLeaf.mcPID->at(iMC);
                        }
                      }
                    float puWeightVal=1;
                    float puTrueVal=1;
                    if (sample_!=TConfiguration::DATA){
                      puWeightVal=puWeight_->GetPuWeightMc(treeLeaf.puTrue->at(1));
                      puTrueVal=treeLeaf.puTrue->at(1);
                    }
                    if (channel_==TConfiguration::MUON && !isVeryLooseSelectionMode_) 
                       SetValues(treeLeaf.muEta->at(ile),treeLeaf.muPhi->at(ile),
                              treeLeaf.muPt->at(ile), leGenPID_,
                              treeLeaf.phoEta->at(ipho), 
                              treeLeaf.phoPhi->at(ipho), treeLeaf.phoEt->at(ipho),phoGenPID_,phoGenEt_,
                              treeLeaf.phoSigmaIEtaIEta->at(ipho),
                              emptyPhoton.GetPhoPFChIsoCorr(treeLeaf.phoPFChIso->at(ipho),treeLeaf.rho2012,treeLeaf.phoEta->at(ipho)),
                              emptyPhoton.GetPhoPFChIsoCorr(treeLeaf.phoSCRChIso->at(ipho),treeLeaf.rho2012,treeLeaf.phoEta->at(ipho)),
                              lePhoDeltaR[ile][ipho],
                              WMt[ile],
                              treeLeaf.pfMET, treeLeaf.pfMETPhi,
                              treeLeaf.rho2012,
                              treeLeaf.run,
                              inputFileN_,
                              totalWeight_,puWeightVal,puTrueVal);
                    else if (isVeryLooseSelectionMode_)
                       SetValues(0,0,0,0,
                              treeLeaf.phoEta->at(ipho), 
                              treeLeaf.phoPhi->at(ipho), treeLeaf.phoEt->at(ipho),phoGenPID_,phoGenEt_,
                              treeLeaf.phoSigmaIEtaIEta->at(ipho),
                              emptyPhoton.GetPhoPFChIsoCorr(treeLeaf.phoPFChIso->at(ipho),treeLeaf.rho2012,treeLeaf.phoEta->at(ipho)),
                              emptyPhoton.GetPhoPFChIsoCorr(treeLeaf.phoSCRChIso->at(ipho),treeLeaf.rho2012,treeLeaf.phoEta->at(ipho)),
                              lePhoDeltaR[ile][ipho],
                              WMt[ile],
                              treeLeaf.pfMET, treeLeaf.pfMETPhi,
                              treeLeaf.rho2012,
                              treeLeaf.run,
                              inputFileN_,
                              totalWeight_,puWeightVal,puTrueVal);

                    else if (channel_==TConfiguration::ELECTRON); 
//
//                       //method of TSelectedEventsTree
//                       //variables are fields of TEventTree
                     Fill();
                       //method of TSelectedEventsTree

                  }// "if (goodLeptonPhotonPairs[ile][ipho])"

              }// "for (int ipho=0; ipho<treeLeaf.nPho; ipho++)"

        else continue;
     } //end of loop over events in the tree

  //memory release:

  for (int ile=0; ile<nLeptonMax; ile++)
    delete goodLeptonPhotonPairs[ile];
  delete[] goodLeptonPhotonPairs;

  for (int ile=0; ile<nLeptonMax; ile++)
    delete lePhoDeltaR[ile];
  delete[] lePhoDeltaR;
 
  delete WMt;
 
  std::cout<<"Summary:"<<std::endl;
  std::cout<<"nEntries="<<nentries<<", nTotal="<<nTotal<<", nPassed="<<nPassed<<", eff="<<(float)nPassed/nTotal<<std::endl;
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

bool WGammaSelection::CheckMaxNumbersInTree()
{
   if ((channel_=TConfiguration::MUON) 
       && (fChain->GetMaximum("nMu")>kMaxnMu))
     //kMaxnMu - field of TEventTree
     {
       PrintErrorMessageMaxNumberOf(MUON_);
          //methof of this class (WGammaSelection)
       return 0;
     }
   if (channel_==TConfiguration::ELECTRON 
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
//   if (fChain->GetMaximum("nJet")>kMaxnJet)
//     {
//       PrintErrorMessageMaxNumberOf(JET_);
//          //methof of this class (WGammaSelection)
//       return 0;
//     }
//   if (fChain->GetMaximum("nLowPtJet")>kMaxnLowPtJet)
//     {
//       PrintErrorMessageMaxNumberOf(LOWPTJET_);
//          //methof of this class (WGammaSelection)
//       return 0;
//     }
//   if (!treeLeaf.isData && fChain->GetMaximum("nMC")>kMaxnMC)
//     {
//       PrintErrorMessageMaxNumberOf(MC_);
//          //methof of this class (WGammaSelection)
//       return 0;
//     }
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
//       else if (particle==JET_)
//         std::cout<<"maximum number of jets in the file nJet="<<fChain->GetMaximum("nJet")<<", when kMaxnJet="<<kMaxnJet<<" only"<<std::endl;
//      else if (particle==LOWPTJET_)
//         std::cout<<"maximum number of low Pt jets in the file nJet="<<fChain->GetMaximum("nJet")<<", when kMaxnLowPtJet="<<kMaxnLowPtJet<<" only"<<std::endl;
//      else if (particle==MC_)
//         std::cout<<"maximum number of mc particles in the file nMC="<<fChain->GetMaximum("nMC")<<", when kMaxnMC="<<kMaxnMC<<" only"<<std::endl;
       std::cout<<"please, go to TEventTree.h to increase this number up to proper value"<<std::endl;
       std::cout<<std::endl;
}

