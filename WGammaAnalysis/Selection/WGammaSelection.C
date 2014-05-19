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

  _INPUT = new TAllInputSamples(channel, configFile);

  _channel=channel;
  _isNoPuReweight=isNoPuReweight;
  _isDebugMode=isDebugMode;
  _photonCorrector = new zgamma::PhosphorCorrectionFunctor(_config.GetPhosphorConstantFileName());
    //field of this class, WGammaSelection

  _sampleMode=sampleMode;
  if (_sampleMode==NOTSPECIFIED){
    std::cout<<"ERROR in WGammaSelection::WGammaSelection: _sampleMode is NONSPECIFIED"<<std::endl;
    return;
  }
  else if (_sampleMode>NOTSPECIFIED){
    std::cout<<"ERROR in WGammaSelection::WGammaSelection: wrong _sampleMode "<<std::endl;
    return;    
  }
  for (int i=0; i<_INPUT->nSources_; i++)
    {
      _doAnalizeSample.push_back(1);      
    } 
}

WGammaSelection::WGammaSelection(int channel, string analyzedSampleNames, string configFile, bool isNoPuReweight, bool isDebugMode)
{

  _INPUT = new TAllInputSamples(channel, configFile);

  _channel=channel;
  _isNoPuReweight=isNoPuReweight;
  _isDebugMode=isDebugMode;
  _photonCorrector = new zgamma::PhosphorCorrectionFunctor(_config.GetPhosphorConstantFileName());
    //field of this class, WGammaSelection

  _sampleMode=NOTSPECIFIED;
  stringstream ss(analyzedSampleNames);
  vector <string> names;
  string name;
  while (ss >> name) 
    names.push_back(name);
  int nNames = names.size();

  std::cout<<"1 - will do, 0 - will not do"<<std::endl;
  for (int i=0; i<_INPUT->nSources_; i++)
    {
      _doAnalizeSample.push_back(0);  
      for (int j=0; j<nNames; j++)
        {
          if (names[j]==_INPUT->allInputs_[i].sourceName_)
            _doAnalizeSample.back()=1; 
        }
      std::cout<<_INPUT->allInputs_[i].sourceName_<<": "<<_doAnalizeSample[i]<<std::endl;
    }  
  if ((int)_doAnalizeSample.size()!=_INPUT->nSources_)
    std::cout<<"ERROR in WGammaSelection::WGammaSelection: wrong _doAnalizeSample.size()"<<std::endl;

}

WGammaSelection::~WGammaSelection()
{
   _eventTree.fChain = 0;
     //field of TEventTree 
   delete _photonCorrector;
}

void WGammaSelection::LoopOverInputFiles()
{
  int nSources = _INPUT->nSources_; 
  //if (_isDebugMode && nSources>3)
  //  nSources = 3;

  for (int iSource=0; iSource<nSources; iSource++)
    {
       if (!_doAnalizeSample[iSource])
         continue;

       _sample = _INPUT->allInputs_[iSource].sample_;
       if (_sample==TConfiguration::DATA){
           _lumiData=_INPUT->allInputs_[iSource].lumiTotal_;
           if (_sampleMode==SIGMC || _sampleMode==NOBKG || _sampleMode==MC)
             continue;
       }
       if (_sample==TConfiguration::SIGMC)
           if (_sampleMode==DATA || _sampleMode==BKGMC)
             continue;
       if (_sample==TConfiguration::BKGMC)
           if (_sampleMode==DATA || _sampleMode==SIGMC || _sampleMode==NOBKG)
           continue;
       _selectedTreeFileName=_config.GetSelectedName(_config.VERY_PRELIMINARY,_channel,_INPUT->allInputs_[iSource].sample_,_INPUT->allInputs_[iSource].sourceName_,_isDebugMode,_isNoPuReweight);
       TTree* tree;
       int inputFileNMax = _INPUT->allInputs_[iSource].nFiles_;
       //if (_isDebugMode) 
       //  {
       //    if (_INPUT->allInputs_[iSource].nFiles_<1) inputFileNMax=_INPUT->allInputs_[iSource].nFiles_;
       //    else inputFileNMax=1;
       //  }

       TString fileOutName;
       fileOutName = _selectedTreeFileName;
       TFile fileOut(fileOutName,"recreate");
       TTree* outTree = new TTree("selectedEvents","selected Events");
       _selEvTree.SetOutputTree(outTree); 

       for (_inputFileN=0; _inputFileN< inputFileNMax; _inputFileN++){

         TFile f(_INPUT->allInputs_[iSource].fileNames_[_inputFileN]);
         if (f.IsOpen()) 
           std::cout<<std::endl<<"processing file "<<_INPUT->allInputs_[iSource].fileNames_[_inputFileN]<<std::endl;
         else{
            std::cout<<"ERROR detected in WGammaSelection::LoopOverInputFiles: file "<<_INPUT->allInputs_[iSource].fileNames_[_inputFileN]<<" was not found"<<std::endl;
            return;
         } 
         f.cd("ggNtuplizer");
         tree = (TTree*)gDirectory->Get("EventTree");
         if (tree){
           _eventTree.Init(tree);
           //if (_sample!=TConfiguration::DATA) SetMCSpecificAddresses();
         }   
         else{
           std::cout<<"ERROR detected in WGammaSelection::LoopOverInputFiles: tree in the file "<<_INPUT->allInputs_[iSource].fileNames_[_inputFileN]<<" does not exist"<<std::endl;
           return;
         }  

         //determine _lumiWeight
         if (_eventTree.treeLeaf.isData)
           _lumiWeight=1;
         else{
           _lumiWeight=_INPUT->allInputs_[iSource].lumiWeights_[_inputFileN];
           if (!_eventTree.treeLeaf.isData && _isDebugMode){
             float cs;
             if (_INPUT->allInputs_[iSource].isSharedCS_)
               cs=_INPUT->allInputs_[iSource].csTotal_;
             else cs=_INPUT->allInputs_[iSource].cs_[_inputFileN];
             TH1F* hEvents = (TH1F*)gDirectory->Get("hEvents");
             double nEvents = (double)hEvents->GetBinContent(1);
             hEvents=0;
             nEvents = nEvents/_INPUT->allInputs_[iSource].factor_[_inputFileN];
             double nDebugEvents=nEvents*_debugModeNEntries/_eventTree.fChain->GetEntries();
             _lumiWeight=cs*_lumiData/nDebugEvents;
           }
         }


         _puWeight=new TPuReweight(_config.GetPileupDataFileName(),_INPUT->allInputs_[iSource].fileNames_[_inputFileN]);         
         LoopOverTreeEvents();
             //method of this class (WGammaSelection)
         _eventTree.fChain=0;
             //field of TEventTree
         delete _puWeight;
       }//loop over _inputFileN ends
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
   if (_eventTree.fChain == 0) return;
   Long64_t nentries = _eventTree.fChain->GetEntries();
   if (_isDebugMode) 
     {
       if (_eventTree.fChain->GetEntries()<_debugModeNEntries) nentries=_eventTree.fChain->GetEntries();
       else nentries=_debugModeNEntries;
     }

   float nTotal=0;
   float nPassed=0;

   _passed.leptonPtPassed=0;
   _passed.leptonEtaPassed=0;
   _passed.phoPtPassed=0;
   _passed.phoBarrelPassed=0;
   _passed.phoEndcapPassed=0;
   _passed.leptonPtPassed=0;
   _passed.dRPassed=0;

   //goodLeptonPhotonPairs(two-dimentional array of bool-s)
   //memory allocation for some variables: 
   int nLeptonMax;
   if (_channel==TConfiguration::MUON) nLeptonMax=_eventTree.kMaxnMu;
   else if (_channel==TConfiguration::ELECTRON) nLeptonMax=_eventTree.kMaxnEle;
     //kMaxnMu - field of TEventTree
   else
     {
       std::cout<<"Error detected in WGammaSelection::LoopOverTreeEvents: channel must be either MUON or ELECTRON."<<std::cout;
       return;
     }

   float* WMt = new float[nLeptonMax];

   float** lePhoDeltaR = new float*[nLeptonMax];
   for (int ile=0; ile<nLeptonMax; ile++)
     lePhoDeltaR[ile]=new float[_eventTree.kMaxnPho];
   
   bool** goodLeptonPhotonPairs = new bool*[nLeptonMax];
   for (int ile=0; ile<nLeptonMax; ile++)
     goodLeptonPhotonPairs[ile]=new bool[_eventTree.kMaxnPho];

   CheckMaxNumbersInTree();


   TPhotonCuts emptyPhoton;
   for (Long64_t entry=0; entry<nentries; entry++) 
   //loop over events in the tree
     {
        _eventTree.GetEntryNeededBranchesOnly(_channel,_sample,entry);
        if (!_eventTree.treeLeaf.isData) _eventTree.GetEntryMCSpecific(entry);
          //method of TEventTree class
        if (_channel==TConfiguration::MUON) _nLe=_eventTree.treeLeaf.nMu;
        else if (_channel==TConfiguration::ELECTRON) _nLe=_eventTree.treeLeaf.nEle;
        else
          {
             std::cout<<"Error detected in  WGammaSelection::LoopOverTreeEvents: channel must be either MUON or ELECTRON."<<std::cout;
             return;
          }
        _totalWeight = _lumiWeight;
        if (!_eventTree.treeLeaf.isData && !_isNoPuReweight)
          _totalWeight*=_puWeight->GetPuWeightMc(_eventTree.treeLeaf.puTrue->at(1));
        nTotal+=_totalWeight;

        TFullCuts fullCuts;
        bool selPassed = fullCuts.VeryPreliminaryCut(goodLeptonPhotonPairs,
          _eventTree.treeLeaf,_channel,lePhoDeltaR,_passed);
        
              //method of this class (WGammaSelection)
        if (!selPassed) continue;
        for (int ile=0; ile<_nLe; ile++){
          for (int ipho=0; ipho<_eventTree.treeLeaf.nPho; ipho++){
            if (!goodLeptonPhotonPairs[ile][ipho]) continue;
            nPassed+=_totalWeight;
            float puWeightVal=1;
            float puTrueVal=1;
            if (_sample!=TConfiguration::DATA){
              puWeightVal=_puWeight->GetPuWeightMc(_eventTree.treeLeaf.puTrue->at(1));
              puTrueVal=_eventTree.treeLeaf.puTrue->at(1);
            }//end of if (_sample!=TConfiguration::DATA)            
            _selEvTree.SetValues(_channel, _sample, 
               _eventTree.treeLeaf,  ipho, ile, 
               lePhoDeltaR[ile][ipho], _inputFileN, 
               _totalWeight, puWeightVal, puTrueVal,
               _photonCorrector);
            _selEvTree.Fill();
         }// "for (int ipho=0; ipho<treeLeaf.nPho; ipho++)"
       }  // "for (int ile=0; ile<_nLe; ile++)"
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
  std::cout<<"leptonPt="<<_passed.leptonPtPassed<<", leptonEta="<<_passed.leptonEtaPassed<<", ptoPt="<<_passed.phoPtPassed<<", phoBarrel="<<_passed.phoBarrelPassed<<", phoEndcap="<<_passed.phoEndcapPassed<<", dR="<<_passed.dRPassed<<std::endl;

}

bool WGammaSelection::CheckMaxNumbersInTree()
{
   if ((_channel=TConfiguration::MUON) 
       && (_eventTree.fChain->GetMaximum("nMu")>_eventTree.kMaxnMu))
     //kMaxnMu - field of TEventTree
     {
       PrintErrorMessageMaxNumberOf(_MUON);
          //methof of this class (WGammaSelection)
       return 0;
     }
   if (_channel==TConfiguration::ELECTRON 
       && _eventTree.fChain->GetMaximum("nEle")>_eventTree.kMaxnEle)
     {
       PrintErrorMessageMaxNumberOf(_ELECTRON);
          //methof of this class (WGammaSelection)
       return 0;
     }
   if (_eventTree.fChain->GetMaximum("nPho")>_eventTree.kMaxnPho)
     {
       PrintErrorMessageMaxNumberOf(_PHOTON);
          //methof of this class (WGammaSelection)
       return 0;
     }
//   if (fChain->GetMaximum("nJet")>kMaxnJet)
//     {
//       PrintErrorMessageMaxNumberOf(_JET);
//          //methof of this class (WGammaSelection)
//       return 0;
//     }
//   if (fChain->GetMaximum("nLowPtJet")>kMaxnLowPtJet)
//     {
//       PrintErrorMessageMaxNumberOf(LOWPT_JET);
//          //methof of this class (WGammaSelection)
//       return 0;
//     }
//   if (!treeLeaf.isData && fChain->GetMaximum("nMC")>kMaxnMC)
//     {
//       PrintErrorMessageMaxNumberOf(_MC);
//          //methof of this class (WGammaSelection)
//       return 0;
//     }
   return 1;
}

void WGammaSelection::PrintErrorMessageMaxNumberOf(int particle)
{
       std::cout<<std::endl;
       std::cout<<"Error detected in WGammaSelection::PrintErrorMessageMaxNumberOf:"<<std::endl;
       if (particle==_MUON)
         std::cout<<"maximum number of muons in the file nMu="<<_eventTree.fChain->GetMaximum("nMu")<<", when kMaxnMu="<<_eventTree.kMaxnMu<<" only"<<std::endl;
       else if (particle==_ELECTRON)
         std::cout<<"maximum number of electrons in the file nEle="<<_eventTree.fChain->GetMaximum("nEle")<<", when kMaxnEle="<<_eventTree.kMaxnEle<<" only"<<std::endl;
       else if (particle==_PHOTON)
         std::cout<<"maximum number of photons in the file nPho="<<_eventTree.fChain->GetMaximum("nPho")<<", when kMaxnPho="<<_eventTree.kMaxnPho<<" only"<<std::endl;
//       else if (particle==_JET)
//         std::cout<<"maximum number of jets in the file nJet="<<fChain->GetMaximum("nJet")<<", when kMaxnJet="<<kMaxnJet<<" only"<<std::endl;
//      else if (particle==LOWPT_JET)
//         std::cout<<"maximum number of low Pt jets in the file nJet="<<fChain->GetMaximum("nJet")<<", when kMaxnLowPtJet="<<kMaxnLowPtJet<<" only"<<std::endl;
//      else if (particle==_MC)
//         std::cout<<"maximum number of mc particles in the file nMC="<<fChain->GetMaximum("nMC")<<", when kMaxnMC="<<kMaxnMC<<" only"<<std::endl;
       std::cout<<"please, go to TEventTree.h to increase this number up to proper value"<<std::endl;
       std::cout<<std::endl;
}

