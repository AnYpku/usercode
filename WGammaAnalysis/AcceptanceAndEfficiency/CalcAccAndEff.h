#ifndef CalcAccAndEff_h
#define CalcAccAndEff_h

#include "../Configuration/TConfiguration.h"
#include "../Configuration/TAllInputSamples.h"
#include "../Include/TEventTree.h"
#include "../Include/TPuReweight.h"
  //this package
#include "../Include/PhosphorCorrectorFunctor.hh"
  //taken from http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/CPena/src/PHOSPHOR_Corr_v2/
#include "TTree.h" 
#include "TMath.h" 
  //ROOT package

class CalcAccAndEff: public TEventTree
  {
     public:
       CalcAccAndEff (int channel, string configfile="../Configuration/config.txt", bool isReseasedCutsMode=0, bool isPuReweight=1, bool isDebugMode=0);

       virtual ~CalcAccAndEff();
       void    LoopOverInputFiles();
       void    LoopOverTreeEvents();
//       bool    CheckMaxNumbersInTree();
//       void    PrintErrorMessageMaxNumberOf(int particle);

     private:

       TAllInputSamples* INPUT_;

//       enum {MUON_, ELECTRON_, PHOTON_, JET_, LOWPTJET_, MC_};
       int channel_;

       bool isDebugMode_;
       bool isReleasedCutsMode_;
       bool isPuReweight_;

       float lumiWeight_;
       float debugModeWeight_;
       float totalWeight_;

       int nLe_;

       float nEvents_;
       float nAccPassed_;
       float nEventsInAcc_;
       float nEffPassed_;
       float nEventsErr_;
       float nAccPassedErr_;
       float nEventsInAccErr_;
       float nEffPassedErr_;

       vector <float> vecPhoPtLimits_;
       vector <float> vecnEvents_;
       vector <float> vecnAccPassed_;
       vector <float> vecnEventsInAcc_;
       vector <float> vecnEffPassed_;
       vector <float> vecnEventsErr_;
       vector <float> vecnAccPassedErr_;
       vector <float> vecnEventsInAccErr_;
       vector <float> vecnEffPassedErr_;


       const static int debugModeNEntries_=100000;

       zgamma::PhosphorCorrectionFunctor* photonCorrector_;
       TPuReweight* puWeight_;
       TConfiguration config_;
  };

#endif 
  //#ifndef 
