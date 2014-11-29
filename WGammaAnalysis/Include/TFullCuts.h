#ifndef TFullCuts_h
#define TFullCuts_h

#include "TString.h"
#include "TCut.h"
//ROOT

#include "../PHOSPHOR_CORRECTION/PhosphorCorrectorFunctor.hh"
  //taken from http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/CPena/src/PHOSPHOR_Corr_v2/
  //currently in this package

#include <vector>

#include "../Configuration/TConfiguration.h"
#include "TEventTree.h"
#include "TMuonCuts.h"
#include "TElectronCuts.h"
#include "TPhotonCuts.h"
#include "TMathTools.h"

class TFullCuts{
public:
  TFullCuts();
  virtual ~TFullCuts();
  struct PassedLevels{
    long metFiltersPassed;
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

  bool VeryPreliminaryCut(TEventTree::InputTreeLeaves& leaf,
                    zgamma::PhosphorCorrectionFunctor* photonCorrector,   
                    int channel, int vgamma, bool isVJets,
                    int& nCands, Candidate* cands,
                    PassedLevels& passed);

  bool ZMassWindowCut(TEventTree::InputTreeLeaves& leaf, int ipho, int iele);

  bool TriggerCut(TEventTree::InputTreeLeaves& leaf, int channel, int vgamma);
  bool TriggerMatch(TEventTree::InputTreeLeaves& leaf, int channel, int vgamma, int ilep);


  //float DeltaR(float phi1, float eta1, float phi2, float eta2); 
  float GetWMtCut(int year);

  TCut RangeMoreLeptonsVeto();
  TCut RangeMetRelatedCut(int year);
  TCut RangePhoEt();
  TCut RangeExtraLeptonPt2011();
  TCut RangeForTemplateMethodCut(int year, int channel, int vgamma, int phoWP);
  TCut RangeFullCut(int year, int channel, int vgamma, int phoWP);

private:
  TConfiguration _config;
  TMuonCuts _muon;
  TElectronCuts _electron;
  TPhotonCuts _photon;
  TMathTools _math;

  TEventTree::InputTreeLeaves _leaf;
  PassedLevels _passed;
  Candidate* _cands;
  bool* _kinPhoton;
  bool* _kinLepton1;
  bool* _kinLepton2;
  void CheckDRandProceed(int vgamma, bool isVJets, int& icand,
                    bool& hasEventAfterPass, bool& hasOverlap,
                    float dR1, float dR2, int ipho, int ilep1, int ilep2);
  bool IsOverlapVJetsVGamma(int channel);
  

  const static float _WMtCut2012 = 50.;//for 8 TeV
  const static float _WMtCut2011 = 70.;//from 7 TeV
  const static float _lePhoDeltaRCut = 0.7;//0.7;

  const static float _ZmassLeft = 70.; 
  const static float _ZmassRight = 110.;
    // for Z-mass window cut for W_GAMMA in ELECTRON channel
};

#endif 
  //#ifndef TFullCuts_h
