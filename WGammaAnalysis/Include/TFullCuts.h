#ifndef TFullCuts_h
#define TFullCuts_h

#include "TEventTree.h"

#include "TString.h"
#include "TCut.h"
//ROOT

#include "../Configuration/TConfiguration.h"

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
  float GetWMtCut(int year);
  TCut RangeMoreLeptonsVeto();
  TCut RangeMetRelatedCut(int year);
  TCut RangePhoEt();
  TCut RangeExtraLeptonPt2011();
  TCut RangeForMetCut(int year, int channel, int phoWP);
  TCut RangeForTemplateMethodCut(int year, int channel, int phoWP);
  TCut RangeFullCut(int year, int channel, int phoWP, bool noPhoPFChIsoCut);

private:
  TConfiguration _config;
  const static float _WMtCut2012 = 50.;//for 8 TeV
  const static float _WMtCut2011 = 70.;//from 7 TeV
  const static float _lePhoDeltaRCut = 0.7;//0.7;
};

#endif 
  //#ifndef TFullCuts_h
