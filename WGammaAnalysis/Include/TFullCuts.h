#ifndef TFullCuts_h
#define TFullCuts_h

#include "TEventTree.h"

#include "PhosphorCorrectorFunctor.hh"
  //taken from http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/CPena/src/PHOSPHOR_Corr_v2/
  //currently in this package

class TFullCuts{
public:
  TFullCuts();
  virtual ~TFullCuts();
//  bool Cut(bool** goodLeptonPhotonPairs, 
//                    TEventTree::InputTreeLeaves &inpTreeLeaf,   
//                    int channel, bool isReleasedCutsMode,
//                    float* WMt, float** lePhoDeltaR,
//                    zgamma::PhosphorCorrectionFunctor* photonCorrector);
  bool Cut(bool** goodLeptonPhotonPairs, 
                    TEventTree::InputTreeLeaves &inpTreeLeaf,   
                    int channel, bool isReleasedCutsMode,
                    float* WMt, float** lePhoDeltaR,
                    zgamma::PhosphorCorrectionFunctor* photonCorrector,
                    bool isWjets=0);
  float DeltaR(float phi1, float eta1, float phi2, float eta2); 

private:
  const static float WMtCut_ = 0.;//70.;
  const static float lePhoDeltaRCut_ = 0.7;//0.7;
};

#endif 
  //#ifndef TFullCuts_h
