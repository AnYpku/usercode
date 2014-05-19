#ifndef TFullCuts_h
#define TFullCuts_h

#include "TEventTree.h"

#include "TString.h"
#include "TCut.h"
//ROOT

class TFullCuts{
public:
  TFullCuts();
  virtual ~TFullCuts();
  struct PassedLevels{
    long leptonPtPassed;
    long leptonEtaPassed;
    long phoPtPassed;
    long phoBarrelPassed;
    long phoEndcapPassed;
    long dRPassed;
  };
  bool VeryPreliminaryCut(bool** goodLeptonPhotonPairs,
                    TEventTree::InputTreeLeaves inpTreeLeaf,   
                    int channel, float** lePhoDeltaR, PassedLevels& passed);
  float DeltaR(float phi1, float eta1, float phi2, float eta2); 
  float GetWMtCut();
  TCut RangeMoreLeptonsVeto();
  TCut RangeMetRelatedCut();
  TCut RangeForMetCut(int channel, int phoWP, TString phoIsoBase);
  TCut RangeForTemplateMethodCut(int channel, int phoWP, TString phoIsoBase);
  TCut RangeFullCut(int channel, int phoWP, TString phoIsoBase);

private:
  const static float _WMtCut = 50.;//70.;
  const static float _lePhoDeltaRCut = 0.7;//0.7;
};

#endif 
  //#ifndef TFullCuts_h
