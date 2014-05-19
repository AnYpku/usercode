#ifndef WGammaSelection_h
#define WGammaSelection_h

#include "../Include/TEventTree.h" 
#include "TSelectedEventsTree.h"
#include "../Include/TMuonCuts.h" 
#include "../Include/TElectronCuts.h" 
#include "../Include/TFullCuts.h" 
#include "../Include/TPhotonCuts.h" 
#include "../Include/TPuReweight.h" 
#include "../Configuration/TAllInputSamples.h"
  //this package
#include "../Include/PhosphorCorrectorFunctor.hh"
  //taken from http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/CPena/src/PHOSPHOR_Corr_v2/
  //currently in this package
#include "TTree.h" 
#include "TMath.h" 
  //ROOT package

class WGammaSelection
  {
     //TEventTree - class for input tree
     //TSelectedEventsTree - class for output tree and output root file
     public:
       WGammaSelection (int channel, int sampleMode = ALL, string configfile="../Configuration/config.txt", bool isNoPuReweight=0, bool isDebugMode=0);
       WGammaSelection(int channel, string analyzedSampleNames, string configFile="../Configuration/config.txt", bool isNoPuReweight=0, bool isDebugMode=0);
       virtual ~WGammaSelection();
       void    LoopOverInputFiles();
       void    LoopOverTreeEvents();
       bool    CheckMaxNumbersInTree();
       void    PrintErrorMessageMaxNumberOf(int particle);

       enum {DATA, SIGMC, BKGMC, MC, NOBKG, ALL, NOTSPECIFIED};

     private:

       TAllInputSamples* _INPUT;
       TEventTree _eventTree;
       TSelectedEventsTree _selEvTree;
       TConfiguration _config;

       enum {_MUON, _ELECTRON, _PHOTON, _JET, _MC};

       int _channel;//_config.ELECTRON, _config.MUON
       int _sample; //_config.DATA, _config.SIGMC, _config.BKGMC

       bool _isDebugMode;
       bool _isNoPuReweight;
       int _sampleMode;
       vector <bool> _doAnalizeSample;

       
       float _lumiData;
       float _lumiWeight;
       float _totalWeight;
       int _nLe;
       int _inputFileN;

       const static int _debugModeNEntries=1000000;
    
       TFullCuts::PassedLevels _passed;

       zgamma::PhosphorCorrectionFunctor* _photonCorrector;
       TPuReweight* _puWeight;
       TString _selectedTreeFileName;
  };

#endif //#ifndef WGammaSelection_h
