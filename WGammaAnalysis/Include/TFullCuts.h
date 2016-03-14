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
    long metFilters;
    long trigger;
    long goodVertex;
    long vgvjOverlap;
    long phoPt;
    long phoEta;
    long photon;
    long leptonPt;
    long leptonEta;
    long leptonId;
    long leptonTriggerMatch;
    long lepton;
    long secondLeptonVeto;
    long leptonInvMass;
    long zMassWindow;
    long dR;
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

   void SetPassedToZeros(PassedLevels& p);
   void Print(PassedLevels& p);

  //float DeltaR(float phi1, float eta1, float phi2, float eta2); 
  float GetWMtCut(int year, int channel);

  TCut RangeMoreLeptonsVeto();
  TCut RangeMetRelatedCut(int year, int channel);
  TCut RangeZmassWindowCut();
  TCut RangePhoEt();
  TCut RangeBlinding(int blind);
  TCut RangeExtraLeptonPt2011();
  TCut RangeDeltaR(int vgamma);
  TCut RangeForTemplateMethodCut(int year, int channel, int vgamma, int blind, int phoWP);
  TCut RangeForEtoGamma(int phoWP, bool doPSVcut);
  TCut RangeFullCut(int year, int channel, int vgamma, int blind, int phoWP);
  TCut RangeFsrCut();
  TCut RangeFsrExcludedCut();

private:

  int FindGoodPhotons(int channel, int vgamma);
  int FindGoodLeptonIndex(int channel, int vgamma, bool isLead);
  bool MLeptonLeptonCut(int channel);
  bool ZMassWindowCut( int ipho, int iele);

  bool TriggerCut(int channel, int vgamma);
  bool TriggerMatch(int channel, int vgamma, int ilep);

  TConfiguration _config;
  TMuonCuts _muon;
  TElectronCuts _electron;
  TPhotonCuts _photon;
  TMathTools _math;

  TEventTree::InputTreeLeaves _leaf;
  PassedLevels _passed;
  Candidate* _cands;
  bool _passedPhoton[TEventTree::kMaxnPho];
  int _ilepLead;
  int _ilepSublead;
//  bool _passedLeptonLead[TEventTree::kMaxnMu+TEventTree::kMaxnEle];
//  bool _passedLeptonSublead[TEventTree::kMaxnMu+TEventTree::kMaxnEle];
  bool CheckDRandProceed(int channel, int vgamma, bool isVJets, int& icand);
  bool IsOverlapVJetsVGamma(int channel);
  
  int _nLep;

  bool _isEvForCheck;


  const static float _WMtCut2011 = 70.;//from 7 TeV
  const static float _lePhoDeltaRCut = 0.7;//0.7;
  const static float _lePhoDeltaRPreCut = 0.4;//0.4;//for FSR selection

//  const static float _ZmassLeft = 70.; 
//  const static float _ZmassRight = 110.;
    // for Z-mass window cut for W_GAMMA in ELECTRON channel
};

  const static float _WMtCut2012[2] = {40.,40.};//MUON, ELECTRON //for 8 TeV

#endif 
  //#ifndef TFullCuts_h
