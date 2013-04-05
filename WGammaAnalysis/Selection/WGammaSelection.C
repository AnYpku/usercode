#include "WGammaSelection.h" 
  //this class
#include "../Include/TMuonCuts.h" 
#include "../Include/TElectronCuts.h" 
#include "../Include/TPhotonCuts.h" 
#include "../Configuration/TConfiguration.h" 
  //this package
#include "../Include/PhosphorCorrectorFunctor.hh"
  //taken from http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/CPena/src/PHOSPHOR_Corr_v2/
  //currently in this package
#include "TMath.h" 
  // ROOT class
#include <iostream> 
  //standard C++ class

WGammaSelection::WGammaSelection(int channel, int sample, bool isDebugMode)
{
  channel_=channel;
  sample_=sample;
  isDebugMode_=isDebugMode;
  TConfiguration config(channel, sample);
  config.SetSelectedFileName();
  selectedTreeFileName_=config.selectedFileName;
    //name of output root file
    //field of TSelectedEventsTree
  photonCorrector_ = new zgamma::PhosphorCorrectionFunctor(config.GetPhosphorConstantFileName(),true);
    //field of this class, WGammaSelection
}

WGammaSelection::~WGammaSelection()
{
   fChain = 0;
     //field of TEventTree 
   delete photonCorrector_;
}

void WGammaSelection::LoopOverInputFiles()
{
  TConfiguration config(channel_, sample_);
  config.SetInputFileNames();
  TTree* tree;
  SetOutputTreeFile();
    //method of TSelectedEventsTree
  int inputFileNMax = config.nInputFiles;
  if (isDebugMode_) 
    {
      if (config.nInputFiles<1) inputFileNMax=config.nInputFiles;
      else inputFileNMax=1;
    }
  for (inputFileN_=0; inputFileN_< inputFileNMax; inputFileN_++)
    {
      TFile f(config.inputFileNames[inputFileN_]);
      if (f.IsOpen()) 
        std::cout<<"processing data file "<<config.inputFileNames[inputFileN_]<<std::endl;
      else
        {
          std::cout<<"ERROR detected in WGammaSelection::LoopOverInputFiles: file "<<config.inputFileNames[inputFileN_]<<" was not found"<<std::endl;
          return;
        } 
      f.cd("ggNtuplizer");
      tree = (TTree*)gDirectory->Get("EventTree");
      if (tree) 
        Init(tree);
          //field of TEventTree
      else
        {
          std::cout<<"ERROR detected in WGammaSelection::LoopOverInputFiles: tree in the file "<<config.inputFileNames[inputFileN_]<<" does not exist"<<std::endl;
          return;
        }  

       
      
      LoopOverTreeEvents();
        //method of this class (WGammaSelection)
      fChain=0;
        //field of TEventTree

    }
  SaveTreeToFile();
    //method of TSelectedEventsTree
}


void WGammaSelection::LoopOverTreeEvents()
{
   if (fChain == 0) return;
   Long64_t nentries = fChain->GetEntries();
   if (isDebugMode_) 
     {
       if (fChain->GetEntries()<100000) nentries=fChain->GetEntries();
       else nentries=100000;
     }
   Long64_t nPassed=0;

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
   bool** goodLeptonPhotonPairs = new bool*[nLeptonMax];
   for (int ile=0; ile<nLeptonMax; ile++)
     goodLeptonPhotonPairs[ile]=new bool[kMaxnPho];

   if (channel_==TConfiguration::MUON 
       && fChain->GetMaximum("nMu")>kMaxnMu)
     //kMaxnMu - field of TEventTree
     {
       PrintErrorMessageMaxNumberOf(MUON_);
          //methof of this class (WGammaSelection)
       return;
     }
   if (channel_==TConfiguration::ELECTRON 
       && fChain->GetMaximum("nEle")>kMaxnEle)
     {
       PrintErrorMessageMaxNumberOf(ELECTRON_);
          //methof of this class (WGammaSelection)
       return;
     }
   if (fChain->GetMaximum("nPho")>kMaxnPho)
     {
       PrintErrorMessageMaxNumberOf(PHOTON_);
          //methof of this class (WGammaSelection)
       return;
     }
  
   for (Long64_t entry=0; entry<nentries; entry++) 
   //loop over events in the tree
     {

        GetEntryNeededBranchesOnly(entry);
          //method of TEventTree class

        if (channel_==TConfiguration::MUON) nLe_=nMu;
        else if (channel_==TConfiguration::ELECTRON) nLe_=nEle;
        else
          {
             std::cout<<"Error detected in  WGammaSelection::LoopOverTreeEvents: channel must be either MUON or ELECTRON."<<std::cout;
             return;
          }

        if (Cut(goodLeptonPhotonPairs) == 1)
              //method of this class (WGammaSelection)

          for (int ile=0; ile<nLe_; ile++)
            for (int ipho=0; ipho<nPho; ipho++)
              {
                nPassed+=goodLeptonPhotonPairs[ile][ipho];
                if (goodLeptonPhotonPairs[ile][ipho])
                  {
                    if (channel_==TConfiguration::MUON) 
                       SetValues(muEta[ile],muPhi[ile],
                              muPt[ile], 
                              phoEta[ipho], 
                              phoPhi[ipho], phoEt[ipho],
                              lePhoDeltaR_[ile][ipho],
                              WMt_[ile],
                              pfMET, pfMETPhi,
                              run,
                              inputFileN_);
                    else if (channel_==TConfiguration::ELECTRON) 
                       SetValues(eleEta[ile],elePhi[ile],
                              elePt[ile], 
                              phoEta[ipho], 
                              phoPhi[ipho], phoEt[ipho],
                              lePhoDeltaR_[ile][ipho],
                              WMt_[ile],
                              pfMET, pfMETPhi,
                              run,
                              inputFileN_);
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
  std::cout<<"nEntries="<<nentries<<", nPassed="<<nPassed<<std::endl;

}

bool WGammaSelection::Cut(bool** goodLeptonPhotonPairs)
{
  // This function is called from LoopOverEvents.
  // returns  1 if entry is accepted.
  // returns 0 otherwise.
   if ((IsVtxGood)==-1) return 0;
   if (nPho < 1) return 0; 
   if (nLe_ < 1) return 0;
      //these variables are fields of TEventTree
   if (!metFilters[6]) return 0;
      //metFilters - is field of TEventTree
      //[6] - ecalLaserCorrFilter
      //necessary to remove spikes in the photon distributions 
      //for Jul13 rereco
   bool goodLeptonPhotonPairsExist=0;
   for (int ipho=0; ipho<nPho; ipho++)
     for (int ile=0; ile<nLe_; ile++) 
       goodLeptonPhotonPairs[ile][ipho]=0;   

   bool goodLepton[nLe_];
   bool goodLeptonExists=0;
   bool thisLeptonPassed;
   for (int ile=0; ile<nLe_; ile++) 
   //start of lepton loop
     { 
        thisLeptonPassed=0;        
        if (channel_==TConfiguration::MUON)
          {
            TMuonCuts muon(ile,muPt[ile],muPt,muEta[ile],muEta,
               nMu,muNumberOfValidPixelHits[ile],
               muNumberOfValidTrkHits[ile],muNumberOfValidTrkLayers[ile],
               muNumberOfValidMuonHits[ile],
               muStations[ile],  
               HLT[HLTIndex[19]],muTrg[ile][1],
               muChi2NDF[ile],
               muD0[ile],muDz[ile],
               muPFIsoR04_CH[ile],
               muPFIsoR04_NH[ile],
               muPFIsoR04_Pho[ile],
               muPFIsoR04_PU[ile]);
               //variables which are input for TMuonCuts constructor
               //are fields of TEventTree
             if (muon.Passed()) 
               {
                 if (!muon.MoreMuonsVeto()) return 0; 
                 WMt_[ile] = sqrt(2*muPt[ile]*pfMET*(1-cos(muPhi[ile]-pfMETPhi)));
                 if (WMt_[ile]>WMtCut_) thisLeptonPassed=1;        
               } 
          }
        else if (channel_==TConfiguration::ELECTRON)
          {
            //TElectronCuts functions are empty now;
            //they are included here for the future
            TElectronCuts electron(elePt[ile]);
            if (electron.Passed()) 
              {
                if (!electron.MoreElectronsVeto()) return 0;  
                WMt_[ile] = sqrt(2*elePt[ile]*pfMET*(1-cos(elePhi[ile]-pfMETPhi)));
                if (WMt_[ile]>WMtCut_) thisLeptonPassed=1;        
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

   bool goodPhoton[nPho];
   bool goodPhotonExists=0;
   for (int ipho=0; ipho<nPho; ipho++) 
   //start of photon loop
     {       
       
        phoEt[ipho] = photonCorrector_->GetCorrEtData(phoR9[ipho], 2012, phoEt[ipho], phoEta[ipho]);
          //Phosphor correction needs to be applied for the photon Et only, 
          //not for SC Et
  
        TPhotonCuts photon(phoEleVeto[ipho],
                    phoEt[ipho],phoEta[ipho],
                    phoSCEt[ipho],phoSCEta[ipho],
                    phoHoverE12[ipho],
                    phoSigmaIEtaIEta[ipho],
                    phoPFChIso[ipho],phoPFNeuIso[ipho],phoPFPhoIso[ipho],
                    rho2012);
          //variables which are input for TPhotonCuts constructor
          //are fields of TEventTree
	if (photon.Passed()) 
          {
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
        for (int ipho=0; ipho<nPho; ipho++)
          {
            if (goodPhoton[ipho] && goodLepton[ile] && LeptonPhotonMatch(ile,ipho))
              {
                goodLeptonPhotonPairs[ile][ipho]=1;
                goodLeptonPhotonPairsExist=1;  
              }                
           }    
       }     

   return goodLeptonPhotonPairsExist;
}

bool WGammaSelection::LeptonPhotonMatch(int ile, int ipho) 
{ 
  float lePhi;
  float leEta;
  if (channel_==TConfiguration::MUON)
    {lePhi=muPhi[ile]; leEta=muEta[ile];}
  else if (channel_==TConfiguration::ELECTRON)
    {lePhi=elePhi[ile]; leEta=eleEta[ile];}
  float dphi;
  for (dphi=fabs(lePhi-phoSCPhi[ipho]);
       fabs(dphi)>=2*TMath::Pi(); 
       dphi=dphi-2*TMath::Pi());
  float dR=sqrt((leEta-phoSCEta[ipho])*(leEta-phoSCEta[ipho])+dphi*dphi);
  lePhoDeltaR_[ile][ipho]=dR;
  if (dR <= lePhoDeltaRCut_) return false;
  return true ; 
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
       std::cout<<"please, go to TEventTree.h to increase this number up to proper value"<<std::endl;
       std::cout<<std::endl;
}

