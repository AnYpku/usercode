#include "WGammaSelection.h" 
  //this class
#include "../Include/TMetTools.h" 
#include "../Include/TMuonCuts.h" 
#include "../Include/TElectronCuts.h" 
#include "../Include/TPhotonCuts.h" 
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

WGammaSelection::WGammaSelection(int channel, int sampleMode, string configFile, bool isReleasedCutsMode, bool isDebugMode)
{

  INPUT_ = new TAllInputSamples(channel, configFile);

  channel_=channel;
  isReleasedCutsMode_=isReleasedCutsMode;
  isDebugMode_=isDebugMode;
  TConfiguration config;
  photonCorrector_ = new zgamma::PhosphorCorrectionFunctor((config.GetPhosphorConstantFileName()).c_str());
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

WGammaSelection::WGammaSelection(int channel, string analyzedSampleNames, string configFile, bool isReleasedCutsMode, bool isDebugMode)
{

  INPUT_ = new TAllInputSamples(channel, configFile);

  channel_=channel;
  isReleasedCutsMode_=isReleasedCutsMode;
  isDebugMode_=isDebugMode;
  TConfiguration config;
  photonCorrector_ = new zgamma::PhosphorCorrectionFunctor((config.GetPhosphorConstantFileName()).c_str());
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
  if (doAnalizeSample_.size()!=INPUT_->nSources_)
    std::cout<<"ERROR in WGammaSelection::WGammaSelection: wrong doAnalizeSample_.size()"<<std::endl;
}

WGammaSelection::~WGammaSelection()
{
   fChain = 0;
     //field of TEventTree 
   delete photonCorrector_;
}

void WGammaSelection::LoopOverInputFiles()
{
  TConfiguration config;
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

       if (!isDebugMode_)
         selectedTreeFileName_=INPUT_->allInputs_[iSource].fileSelected_;
       else 
         selectedTreeFileName_=INPUT_->allInputs_[iSource].fileSelectedDebug_;
       TTree* tree;
       int inputFileNMax = INPUT_->allInputs_[iSource].nFiles_;
       //if (isDebugMode_) 
       //  {
       //    if (INPUT_->allInputs_[iSource].nFiles_<1) inputFileNMax=INPUT_->allInputs_[iSource].nFiles_;
       //    else inputFileNMax=1;
       //  }
       TFile fileOut(selectedTreeFileName_,"recreate");
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

           TFile f((INPUT_->allInputs_[iSource].fileNames_[inputFileN_]).c_str());
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
 
      
           LoopOverTreeEvents();
             //method of this class (WGammaSelection)
           fChain=0;
             //field of TEventTree

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
   int nPassed=0;

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
  
   for (Long64_t entry=0; entry<nentries; entry++) 
   //loop over events in the tree
     {

        //fChain->GetEntry(entry);

        GetEntryNeededBranchesOnly(entry);

        if (!treeLeaf.isData) GetEntryMCSpecific(entry);
          //method of TEventTree class

        if (channel_==TInputSample::MUON) nLe_=treeLeaf.nMu;
        else if (channel_==TInputSample::ELECTRON) nLe_=treeLeaf.nEle;
        else
          {
             std::cout<<"Error detected in  WGammaSelection::LoopOverTreeEvents: channel must be either MUON or ELECTRON."<<std::cout;
             return;
          }

        if (Cut(goodLeptonPhotonPairs) == 1)
              //method of this class (WGammaSelection)

          for (int ile=0; ile<nLe_; ile++)
            for (int ipho=0; ipho<treeLeaf.nPho; ipho++)
              {
                if (goodLeptonPhotonPairs[ile][ipho])
                  {
                    totalWeight_ = lumiWeight_;
                    nPassed+=1;
                    if (channel_==TInputSample::MUON) 
                       SetValues(treeLeaf.muEta[ile],treeLeaf.muPhi[ile],
                              treeLeaf.muPt[ile], 
                              treeLeaf.phoEta[ipho], 
                              treeLeaf.phoPhi[ipho], treeLeaf.phoEt[ipho],
                              lePhoDeltaR_[ile][ipho],
                              WMt_[ile],
                              treeLeaf.pfMET, treeLeaf.pfMETPhi,
                              treeLeaf.run,
                              inputFileN_,
                              totalWeight_);
                    else if (channel_==TInputSample::ELECTRON) 
                       SetValues(treeLeaf.eleEta[ile],treeLeaf.elePhi[ile],
                              treeLeaf.elePt[ile], 
                              treeLeaf.phoEta[ipho], 
                              treeLeaf.phoPhi[ipho], treeLeaf.phoEt[ipho],
                              lePhoDeltaR_[ile][ipho],
                              WMt_[ile],
                              treeLeaf.pfMET, treeLeaf.pfMETPhi,
                              treeLeaf.run,
                              inputFileN_,
                              totalWeight_);
                       //method of TSelectedEventsTree
                       //variables are fields of TEventTree
                     Fill();
                       //method of TSelectedEventsTree
                  }
              }
        else continue;

     } //end of loop over events in the tree
  //memory release:

  for (int ile=0; ile<nLeptonMax; ile++)
    delete goodLeptonPhotonPairs[ile];
  delete[] goodLeptonPhotonPairs;

  std::cout<<"Summary:"<<std::endl;
  std::cout<<"nEntries="<<nentries<<", nPassed="<<nPassed<<", eff="<<(double)nPassed/nentries<<", nWeightes="<<nPassed*debugModeWeight_;
  if (debugModeWeight_!=0) std::cout<<" or  "<<nPassed/debugModeWeight_;
  std::cout<<std::endl;
  std::cout<<"nTotal_="<<nTotal_<<std::endl;
  std::cout<<"nBeforeLeptonLoop_="<<nBeforeLeptonLoop_<<" (IsVtxGood!=-1, nPho >= 0, nLe_ >=0, metFilters[6]==1)"<<std::endl;
  std::cout<<"nLeptons_="<<nLeptons_<<std::endl;
  if (nLeptons_!=0)       
    std::cout<<"nLeptonsPassed_="<<nLeptonsPassed_<<", eff="<<1.0*nLeptonsPassed_/nLeptons_<<std::endl; 
  std::cout<<"nMoreVetoPassed_="<<nMoreVetoPassed_<<std::endl;
  std::cout<<"nWMtPassed_="<<nWMtPassed_<<std::endl;
  std::cout<<"nPhotons_="<<nPhotons_<<std::endl;
  if (nPhotons_!=0) 
    std::cout<<"nPhotonsPassed_="<<nPhotonsPassed_<<", eff="<<1.0*nPhotonsPassed_/nPhotons_<<std::endl;
  std::cout<<"nPhoLepPassed_="<<nPhoLepPassed_<<std::endl; 
  std::cout<<std::endl;

}

bool WGammaSelection::Cut(bool** goodLeptonPhotonPairs)
{
  // This function is called from LoopOverEvents.
  // returns  1 if entry is accepted.
  // returns 0 otherwise.

   nTotal_++;

   if ((treeLeaf.IsVtxGood)==-1) return 0;
   if (treeLeaf.nPho < 1) return 0; 
   if (nLe_ < 1) return 0;
      //these variables are fields of TEventTree
   if (!treeLeaf.metFilters[6]) return 0;
      //metFilters - is field of TEventTree
      //[6] - ecalLaserCorrFilter
      //necessary to remove spikes in the photon distributions 
      //for Jul13 rereco


   if (!treeLeaf.isData) {
     //MET smearing
     TMetTools met(treeLeaf.event, treeLeaf.pfMET, treeLeaf.pfMETPhi,
              treeLeaf.nLowPtJet, treeLeaf.jetLowPtRawPt, treeLeaf.jetLowPtRawEn,
              treeLeaf.jetLowPtPt, treeLeaf.jetLowPtEta, treeLeaf.jetLowPtPhi,
              treeLeaf.jetLowPtGenJetPt, treeLeaf.jetLowPtGenJetEta, treeLeaf.jetLowPtGenJetPhi,
              treeLeaf.nJet, treeLeaf.jetRawPt, treeLeaf.jetRawEn, treeLeaf.jetPt, treeLeaf.jetEta, treeLeaf.jetPhi,
              treeLeaf.jetGenJetPt, treeLeaf.jetGenJetEta, treeLeaf.jetGenJetPhi);
     met.METSmearCorrection();
     treeLeaf.pfMET = met.GetRecoPfMET();
     treeLeaf.pfMETPhi = met.GetRecoPfMETPhi();
  
   }

   bool goodLeptonPhotonPairsExist=0;
   for (int ipho=0; ipho<treeLeaf.nPho; ipho++)
     for (int ile=0; ile<nLe_; ile++) 
       goodLeptonPhotonPairs[ile][ipho]=0;   

   bool goodLepton[nLe_];
   bool goodLeptonExists=0;
   bool thisLeptonPassed;

   nBeforeLeptonLoop_++;

   for (int ile=0; ile<nLe_; ile++) 
   //start of lepton loop
     { 

        nLeptons_++; 

        thisLeptonPassed=0;        
        if (channel_==TInputSample::MUON)
          {
            TMuonCuts muon(ile,treeLeaf.muPt[ile],treeLeaf.muPt,treeLeaf.muEta[ile],treeLeaf.muEta,
               treeLeaf.nMu,treeLeaf.muNumberOfValidPixelHits[ile],
               treeLeaf.muNumberOfValidTrkHits[ile],treeLeaf.muNumberOfValidTrkLayers[ile],
               treeLeaf.muNumberOfValidMuonHits[ile],
               treeLeaf.muStations[ile],  
               treeLeaf.HLT[treeLeaf.HLTIndex[18]],treeLeaf.HLT[treeLeaf.HLTIndex[19]],
               treeLeaf.muTrg[ile][0],treeLeaf.muTrg[ile][1],
               treeLeaf.muChi2NDF[ile],
               treeLeaf.muD0[ile],treeLeaf.muDz[ile],
               treeLeaf.muPFIsoR04_CH[ile],
               treeLeaf.muPFIsoR04_NH[ile],
               treeLeaf.muPFIsoR04_Pho[ile],
               treeLeaf.muPFIsoR04_PU[ile]);
               //variables which are input for TMuonCuts constructor
               //are fields of TEventTree
             if ( (!isReleasedCutsMode_ && muon.Passed()) || (isReleasedCutsMode_ && muon.PassedExceptKinematics()) ) 
               {

                 nLeptonsPassed_++;

                 if (!muon.MoreMuonsVeto()) return 0; 

                 nMoreVetoPassed_++;

                 WMt_[ile] = sqrt(2*treeLeaf.muPt[ile]*treeLeaf.pfMET*(1-cos(treeLeaf.muPhi[ile]-treeLeaf.pfMETPhi)));
                 if ( (WMt_[ile]>WMtCut_ && !isReleasedCutsMode_) || (isReleasedCutsMode_))
                   {
                     nWMtPassed_++;
                     thisLeptonPassed=1;
                   }    
               } 
          }
        else if (channel_==TInputSample::ELECTRON)
          {
            //TElectronCuts functions are empty now;
            //they are included here for the future
            TElectronCuts electron(treeLeaf.elePt[ile]);
            if (electron.Passed()) 
              {
                if (!electron.MoreElectronsVeto()) return 0;  
                WMt_[ile] = sqrt(2*treeLeaf.elePt[ile]*treeLeaf.pfMET*(1-cos(treeLeaf.elePhi[ile]-treeLeaf.pfMETPhi)));
                if ((WMt_[ile]>WMtCut_ && !isReleasedCutsMode_) || (isReleasedCutsMode_)) thisLeptonPassed=1;        
              } 
          }

        if (thisLeptonPassed)
          {
            if (goodLeptonExists) return 0;  
              //to remove events with more than one leptons in the event
            goodLeptonExists=1;
            goodLepton[ile]=1; 
          } 
        else goodLepton[ile]=0; 
      } //end of lepton loop 

   if (!goodLeptonExists) return 0;
     //skip loop over photons if no good leptons found

   bool goodPhoton[treeLeaf.nPho];
   bool goodPhotonExists=0;
   for (int ipho=0; ipho<treeLeaf.nPho; ipho++) 
   //start of photon loop
     {       
       
        nPhotons_++;

        if (sample_==TInputSample::DATA) 
           treeLeaf.phoEt[ipho] = photonCorrector_->GetCorrEtData(treeLeaf.phoR9[ipho], 2012, treeLeaf.phoEt[ipho], treeLeaf.phoEta[ipho]);
          //Phosphor correction needs to be applied for the photon Et only, 
          //not for SC Et
        else
          {
            int phoMCIndex = -1;
            for (int iMC=0; iMC<treeLeaf.nMC; iMC++){
              if (treeLeaf.mcPID[iMC]==22) phoMCIndex = iMC;
            }
            if (phoMCIndex > -1) 
              treeLeaf.phoEt[ipho] = photonCorrector_->GetCorrEtMC(treeLeaf.phoR9[ipho], 2012, treeLeaf.phoEt[ipho], treeLeaf.phoEta[ipho], treeLeaf.mcE[phoMCIndex]);
          }

                
        TPhotonCuts photon(treeLeaf.phoEleVeto[ipho],
                    treeLeaf.phoEt[ipho],treeLeaf.phoEta[ipho],
                    treeLeaf.phoSCEt[ipho],treeLeaf.phoSCEta[ipho],
                    treeLeaf.phoHoverE12[ipho],
                    treeLeaf.phoSigmaIEtaIEta[ipho],
                    treeLeaf.phoPFChIso[ipho],treeLeaf.phoPFNeuIso[ipho],treeLeaf.phoPFPhoIso[ipho],
                    treeLeaf.rho2012);
          //variables which are input for TPhotonCuts constructor
          //are fields of TEventTree

	if ( (!isReleasedCutsMode_ && photon.Passed()) || (isReleasedCutsMode_ && photon.PassedExceptKinematics()) ) 
          {
     
            nPhotonsPassed_++;

            goodPhotonExists=1;
            goodPhoton[ipho]=1;
          }
        else goodPhoton[ipho]=0;
      } //end of photon loop   

   if (!goodPhotonExists) return 0;
     //skip checking lepton-photon matching if no good photons found

    //check matching between muon and photon
    for (int ile=0; ile<nLe_; ile++) 
      {
        for (int ipho=0; ipho<treeLeaf.nPho; ipho++)
          {
            if (goodPhoton[ipho] && goodLepton[ile] && LeptonPhotonMatch(ile,ipho))
              {

                nPhoLepPassed_++;

                goodLeptonPhotonPairs[ile][ipho]=1;
                goodLeptonPhotonPairsExist=1;  
              }                
           }    
       }     

   return goodLeptonPhotonPairsExist;
}

bool WGammaSelection::LeptonPhotonMatch(int ile, int ipho) 
{ 
  float lePhi=0;
  float leEta=0;
  if (channel_==TInputSample::MUON)
    {lePhi=treeLeaf.muPhi[ile]; leEta=treeLeaf.muEta[ile];}
  else if (channel_==TInputSample::ELECTRON)
    {lePhi=treeLeaf.elePhi[ile]; leEta=treeLeaf.eleEta[ile];}
  float dphi;
  for (dphi=fabs(lePhi-treeLeaf.phoSCPhi[ipho]);
       fabs(dphi)>=2*TMath::Pi(); 
       dphi=dphi-2*TMath::Pi()) ;
  float dR=sqrt((leEta-treeLeaf.phoSCEta[ipho])*(leEta-treeLeaf.phoSCEta[ipho])+dphi*dphi);
  lePhoDeltaR_[ile][ipho]=dR;
  if (!isReleasedCutsMode_ && dR <= lePhoDeltaRCut_) return false;
  return true ; 
}

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

