#ifndef WGammaSelection_h
#define WGammaSelection_h

#include "TEventTree.h" 
#include "TSelectedEventsTree.h"
#include "../Include/TMuonCuts.h" 
#include "../Include/TElectronCuts.h" 
#include "../Include/TPhotonCuts.h" 
  //this package
#include "../Include/PhosphorCorrectorFunctor.hh"
  //taken from http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/CPena/src/PHOSPHOR_Corr_v2/
#include "TTree.h" 
#include "TMath.h" 
  //ROOT package

class WGammaSelection : public TEventTree, public TSelectedEventsTree 
  {
     //TEventTree - class for input tree
     //TSelectedEventsTree - class for output tree and output root file
     public:
       WGammaSelection (int channel, int sample, bool isDebugMode=0);
       virtual ~WGammaSelection();
       bool    Cut(bool** goodLeptonPhotonPairs); 
       void    LoopOverInputFiles();
       void    LoopOverTreeEvents();
       bool    LeptonPhotonMatch(int ile, int ipho);
       void    PrintErrorMessageMaxNumberOf(int particle);

     private:
       enum {MUON_, ELECTRON_, PHOTON_};
       int channel_;
       int sample_;
       bool isDebugMode_;
       int nLe_;
       int inputFileN_;
       float WMt_[kMaxnMu+kMaxnEle];
       float lePhoDeltaR_[kMaxnMu+kMaxnEle][kMaxnPho];
         //kMaxnMu,kMaxnEle,kMaxnPho - 
         //constant fields of TEventTree
       const static float WMtCut_ = 0.;//70.;
       const static float lePhoDeltaRCut_ = 0.;//0.7;
       zgamma::PhosphorCorrectionFunctor* photonCorrector_;
  };

#endif //#ifndef WGammaSelection_h
