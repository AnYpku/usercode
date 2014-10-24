#ifndef TFullCuts_h
#define TFullCuts_h

#include "TString.h"
#include "TCut.h"
//ROOT

#include <vector>

#include "../Configuration/TConfiguration.h"
#include "TEventTree.h"
#include "TMuonCuts.h"
#include "TPhotonCuts.h"

class TFullCuts{
public:
  TFullCuts();
  virtual ~TFullCuts();
  struct PassedLevels{
    long triggerPassed;
    long goodVertexPassed;
    long phoPtPassed;
    long phoEtaPassed;
    long vgvjOverlapPassed;
    long leptonPtPassed;
    long leptonEtaPassed;
    long dRPassed;
    long evAfterKinCuts;

//    long phoBarrelPassed;
//    long phoEndcapPassed;
  };
  struct Candidate{
    int ipho;
    int ilep1;
    int ilep2;
    float dRlep1pho;
    float dRlep2pho;    
  };

  bool VeryPreliminaryCut(TEventTree::InputTreeLeaves& inpTreeLeaf,   
                    int channel, int vgamma, bool isVJets,
                    int& nCands, Candidate* cands,
                    PassedLevels& passed);


  float DeltaR(float phi1, float eta1, float phi2, float eta2); 
  float GetWMtCut(int year);

  TCut RangeMoreLeptonsVeto();
  TCut RangeMetRelatedCut(int year);
  TCut RangePhoEt();
  TCut RangeExtraLeptonPt2011();
  TCut RangeForMetCut(int year, int channel, int vgamma, int phoWP);
  TCut RangeForTemplateMethodCut(int year, int channel, int vgamma, int phoWP);
  TCut RangeFullCut(int year, int channel, int vgamma, int phoWP, bool noPhoPFChIsoCut);

private:
  TConfiguration _config;
  TMuonCuts _emptyMuon;
  TPhotonCuts _emptyPhoton;

  TEventTree::InputTreeLeaves _inpTreeLeaf;
  PassedLevels _passed;
  Candidate* _cands;
  bool* _kinPhoton;
  bool* _kinLepton1;
  bool* _kinLepton2;
  void CheckDRandProceed(int vgamma, bool isVJets, int& icand,
                    bool& hasEventAfterPass, bool& hasOverlap,
                    float dR1, float dR2, int ipho, int ilep1, int ilep2);
  bool IsOverlapVJetsVGamma(int ipho);
  

  const static float _WMtCut2012 = 50.;//for 8 TeV
  const static float _WMtCut2011 = 70.;//from 7 TeV
  const static float _lePhoDeltaRCut = 0.7;//0.7;
};

#endif 
  //#ifndef TFullCuts_h
