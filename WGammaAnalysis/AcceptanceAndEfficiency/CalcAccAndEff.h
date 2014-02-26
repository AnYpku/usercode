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

class CalcAccAndEff: public TEventTree
  {
     public:
       CalcAccAndEff (int channel, string configfile="../Configuration/config.txt", bool isNoPuReweight=0, bool isDebugMode=0);

       virtual ~CalcAccAndEff();
       void    LoopOverInputFiles();
       void    LoopOverTreeEvents();
       bool    AcceptancePassed(bool** accLeptonPhotonPassed);    
       bool    EfficiencyPassed(bool** effLeptonPhotonPassed, float* WMt, float** lePhoDeltaR, float weightPU);
       void    PlotAndSaveOutput();
       bool    CheckMaxNumbersInTree();
//       void    PrintErrorMessageMaxNumberOf(int particle);

     private:

       TAllInputSamples* INPUT_;

//       enum {MUON_, ELECTRON_, PHOTON_, JET_, LOWPTJET_, MC_};
       int channel_;

       bool isDebugMode_;
       bool isNoPuReweight_;

       float lumiWeight_;
       float debugModeWeight_;
       float totalWeight_;

       int nLe_;

       float nAccTotal_;
       float nAccPassed_;
       float nEffTotal_;
       float nEffPassed_;
       float nAccTotalErr_;
       float nAccPassedErr_;
       float nEffTotalErr_;
       float nEffPassedErr_;

       float acc_;
       float eff_;
       float accErr_;
       float effErr_;

       vector <float> vecPhoPtLimits_;

       vector <float> vecnAccTotal_;
       vector <float> vecnAccPassed_;
       vector <float> vecnEffTotal_;
       vector <float> vecnEffPassed_;
       vector <float> vecnAccPassedErr_;
       vector <float> vecnAccTotalErr_;
       vector <float> vecnEffPassedErr_;
       vector <float> vecnEffTotalErr_;

       vector <float> vecacc_;
       vector <float> veceff_;
       vector <float> vecaccErr_;
       vector <float> veceffErr_;

       TFullCuts fullCuts_;
       TMathTools math_;
       TPhotonCuts photonEmpty_;
       TMuonCuts muonEmpty_;
 //      TElectronCuts electronEmpty_;

       const static int debugModeNEntries_=100000;

       zgamma::PhosphorCorrectionFunctor* photonCorrector_;
       TPuReweight* puWeight_;
       TConfiguration config_;
  };

#endif 
  //#ifndef 
