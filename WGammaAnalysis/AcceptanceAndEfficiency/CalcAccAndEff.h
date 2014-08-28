#ifndef CalcAccAndEff_h
#define CalcAccAndEff_h

#include "../Configuration/TConfiguration.h"
#include "../Configuration/TAllInputSamples.h"
#include "../Include/TEventTree.h"
#include "../Include/TPuReweight.h"
#include "../Include/TMathTools.h"
#include "../Include/TPhotonCuts.h"
#include "../Include/TMuonCuts.h"
#include "../Include/TElectronCuts.h"
#include "../Include/TFullCuts.h"
  //this package
#include "../Include/PhosphorCorrectorFunctor.hh"
  //taken from http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/CPena/src/PHOSPHOR_Corr_v2/
#include "TTree.h" 
#include "TMath.h" 
  //ROOT package

class CalcAccAndEff
  {
     public:
       CalcAccAndEff (int year, int channel, int vgamma, int phoWP, string configfile="../Configuration/config.txt", bool isNoPuReweight=0, bool isDebugMode=0);

       virtual ~CalcAccAndEff();
       void    LoopOverInputFiles();

     private:

       void    LoopOverTreeEvents();
       bool    AcceptancePassed(bool** accLeptonPhotonPassed);    
       bool    EfficiencyPassed(bool** effLeptonPhotonPassed, float** lePhoDeltaR, float weightPU);
       void    ComputeAcceptance();
       void    ComputeEfficiency();
       void    ComputeAccTimesEff();
       void    PrintAccAndEffSummary();
       void    PlotAndSaveOutput();
       bool    CheckMaxNumbersInTree();

       TFile* _fSelectedVeryPreliminary;
       int _year;

       TAllInputSamples* _INPUT;
       TEventTree _eventTree;

       int _channel;
       int _vgamma;
       int _phoWP;

       bool _isDebugMode;
       bool _isNoPuReweight;

       float _lumiWeight;
       float _debugModeWeight;
       float _totalWeight;

       TFullCuts::PassedLevels _passed;

       int _nLe;
       float* _phoPtLimits;

       //Acceptance:
       float  _nAccTotEvents;
       float  _nAccTotEventsErr;
       float  _nAccTotPassed;
       float  _nAccTotPassedErr;
       float* _nAcc1DEvents;
       float* _nAcc1DEventsErr;
       float* _nAcc1DPassed;
       float* _nAcc1DPassedErr;

       float  _accTot;
       float  _accTotErr;
       float* _acc1D;
       float* _acc1DErr;

       //Efficiency:
       float  _nEffTotEvents;
       float  _nEffTotEventsErr;
       float  _nEffTotPrePassed;
       float  _nEffTotPrePassedErr;
       float  _nEffTotPrePassed2;
       float  _nEffTotPrePassed2Err;
       float  _nEffTotPassed;
       float  _nEffTotPassedErr;
       float* _nEff1DEvents;
       float* _nEff1DEventsErr;
       float* _nEff1DPrePassed;
       float* _nEff1DPrePassedErr;
       float* _nEff1DPrePassed2;
       float* _nEff1DPrePassed2Err;
       float* _nEff1DPassed;
       float* _nEff1DPassedErr;
       float  _effTot;
       float  _effTotErr;
       float* _eff1D;
       float* _eff1DErr;

       //Acc times Eff
       float  _accXeffTot;
       float  _accXeffTotErr;
       float* _accXeff1D;
       float* _accXeff1DErr;

       TFullCuts _fullCuts;
       TMathTools _math;
       TPhotonCuts _photonEmpty;
       TMuonCuts _muonEmpty;
//     TElectronCuts electronEmpty_;

       const static int _debugModeNEntries=100000;

       zgamma::PhosphorCorrectionFunctor* _photonCorrector;
       TPuReweight* _puWeight;
       TConfiguration _config;
  };

#endif 
  //#ifndef 
