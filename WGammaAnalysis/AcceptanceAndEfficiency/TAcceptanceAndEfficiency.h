#ifndef TAcceptanceAndEfficiency_h
#define TAcceptanceAndEfficiency_h

#include "../Configuration/TAllInputSamples.h"
#include "../Include/TEventTree.h"
  //this package
#include "../Include/PhosphorCorrectorFunctor.hh"
  //taken from http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/CPena/src/PHOSPHOR_Corr_v2/
#include "TTree.h" 
#include "TMath.h" 
  //ROOT package

class TAcceptanceAndEfficiency: public TEventTree
  {
     public:
       TAcceptanceAndEfficiency (int channel, string configfile="../Configuration/config.txt", bool isReseasedCutsMode=0, bool isDebugMode=0);

       virtual ~TAcceptanceAndEfficiency();
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
       int nLe_;

       float nEvents_;
       float nSkimPassed_;
       float nAccPassed_;
       float nEffPassed_;


       const static int debugModeNEntries_=100000;

       zgamma::PhosphorCorrectionFunctor* photonCorrector_;
  };

#endif 
  //#ifndef 
