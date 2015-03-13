#ifndef Selection_h
#define Selection_h

#include "../Include/TEventTree.h" 
#include "TSelectedEventsTree.h"
#include "../Include/TMuonCuts.h" 
#include "../Include/TElectronCuts.h" 
#include "../Include/TFullCuts.h" 
#include "../Include/TPhotonCuts.h" 
#include "../Include/TPuReweight.h" 
#include "../Configuration/TAllInputSamples.h"
  //this package
//#include "../Include/PhosphorCorrectorFunctor.hh"
#include "../PHOSPHOR_CORRECTION/PhosphorCorrectorFunctor.hh"
  //taken from http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/CPena/src/PHOSPHOR_Corr_v2/
  //currently in this package
#include "TTree.h" 
#include "TMath.h" 
  //ROOT package

class Selection
  {
     //TEventTree - class for input tree
     //TSelectedEventsTree - class for output tree and output root file
     public:
       Selection ();
       Selection (TConfiguration::AnalysisParameters &anPars);
//       Selection(int channel, int vgamma, string analyzedSampleNames, string configFile="../Configuration/config.txt", bool isNoPuReweight=0, bool isDebugMode=0);
       virtual ~Selection();
       void    LoopOverInputFiles();
       void    LoopOverTreeEvents();
       bool    CheckMaxNumbersInTree();
       void    PrintErrorMessageMaxNumberOf(int particle);
       void    ExtraSelection(TConfiguration::AnalysisParameters &anPars);
       void    ExtraSelectionOne(TAllInputSamples &INPUT, int iSource, TConfiguration& config, TFullCuts &fullCut, int year, int channel, int vgamma, int wp, int blind);

       enum {DATA, SIGMC, BKGMC, MC, NOBKG, ALL, NOTSPECIFIED};//sampleMode
       TString StrSampleMode(int sampleMode);


     private:

       TAllInputSamples* _INPUT;
       TEventTree _eventTree;

       TSelectedEventsTree _selEvTree;
       TTree* _outTree;
       TFile* _fileOut;
       TRandom _rnd;

       TConfiguration _config;
       TTree* _tr;
       TTree* _trReduced;
       TTree* _tr1;
       TTree* _tr2;
       TTree* _tr3;
       TTree* _tr4;

       enum {_MUON, _ELECTRON, _PHOTON, _JET, _MC};

       int _channel;//_config.ELECTRON, _config.MUON
       //int _blind;//_config.UNBLIND, _config.BLIND_PRESCALE, _config.BLIND_DECRACC
       int _sample; //_config.DATA, _config.SIGMC, _config.BKGMC
       int _vgamma;
//       int _blind;
       bool _isVJets;

       bool _isDebugMode;
       bool _isNoPuReweight;
       int _sampleMode;
       vector <bool> _doAnalizeSample;

       
       float _lumiData;
       float _lumiWeight;
       float _totalWeight;
       int _nLe;
       int _inputFileN;

       const static int _debugModeNEntries=10000;
    
       TFullCuts::PassedLevels _passed;

       zgamma::PhosphorCorrectionFunctor* _photonCorrector;
       TPuReweight* _puWeight;
       TString _selectedTreeFileName;

       void PickHardestPhotonInEvent(TTree* trToBeWritten, TTree* trReduced, int blind);
  };

#endif //#ifndef Selection_h
