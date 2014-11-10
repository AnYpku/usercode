#ifndef TPhotonCuts_h
#define TPhotonCuts_h

#include "TCut.h"
 //ROOT

class TPhotonCuts
{
  public:
    TPhotonCuts();
    virtual ~TPhotonCuts();


    bool PassedKinematics(float phoPt, float phoEta, bool& ifPassedPt, bool& ifPassedEta);

     //see https://twiki.cern.ch/twiki/bin/view/CMS/CutBasedPhotonID2012
     //for reference

    //the following three functions determine effective areas depending on |eta|
    //the values are taken from
    //https://twiki.cern.ch/twiki/bin/view/CMS/CutBasedPhotonID2012

    //the following function calculated corrected particle flow isolation
    float IsoCorr(float iso, float rho, float EA);
    bool IsBarrel(float phoEta);
    bool IsEndcap(float phoEta);


    float GetPhoChIsoCorr(float phoChIso, float rho2012, float eta);
    float GetPhoNeuIsoCorr(float phoNeuIso, float rho2012, float eta);
    float GetPhoPhoIsoCorr(float phoPhoIso, float rho2012, float eta);

    float GetPhoEcalIsoDR04Corr(float iso, float rho2011, float eta);
    float GetPhoHcalIsoDR04Corr(float iso, float rho2011, float eta);
    float GetPhoTrkIsoHollowDR04Corr(float iso, float rho2011, float eta);

    float GetPhoSigmaIEtaIEtaCutB(int year, int wp);
    float GetPhoSigmaIEtaIEtaCutE(int year, int wp);
    float GetPhoSigmaIEtaIEtaCutLeftB();
    float GetPhoSigmaIEtaIEtaCutLeftE();
    float GetOneIsolationCutB(int year, int wp, int isoType, float phoEt);
    float GetOneIsolationCutE(int year, int wp, int isoType, float phoEt);
    float GetPhoSCRChIsoCorrCut();


    TCut RangeBarrel();
    TCut RangeEndcap();
    TCut RangeSigmaIEtaIEta(int year, int wp);
    TCut SidebandSigmaIEtaIEta();
    TCut RangeOneIsolation(int year, int wp, int isoType);
    TCut RangeHoverE(int year);
    TCut RangePhoEleVeto();
    TCut RangePhoHasPixelSeed();
    TCut RangePhoton(int year, int wp, 
           bool doSigmaIEtaIEtaCut=1, bool doChOrTrkIsoCut=1, 
           bool doNeuOrHcalIsoCut=1, bool doPhoOrEcalIsoCut=1, 
           bool doHoverECut=1, bool doElectronVetoCut=1);

    TCut RangeGenFakePhoton();
    TCut RangeGenTruePhoton();


    const static int _nWP=3;
    enum {WP_LOOSE, WP_MEDIUM, WP_TIGHT};
    enum {ISO_CHorTRK, ISO_NEUorHCAL, ISO_PHOorECAL};

  private:

    const static float _phoPtCut = 15.0;
      //WP - working point
      //nWP - total number of working points
      //0 - loose  (90% for barrel, 85% for endcap) 
      //1 - medium (80% - barrel, 75% - endcap) 
      //2 - tight  (70% - barrel, 65% - endcap)
      //see https://twiki.cern.ch/twiki/bin/view/CMS/CutBasedPhotonID2012
      //for reference
    float EffAreaCharged(float eta);
    float EffAreaNeutral(float eta);
    float EffAreaPhotons(float eta);
};

//for some reason, I'm not allowed to initialize these arrays inside {}
//so, I introduce them outside:
//(and some of them are arrays because there are 3 working points)

//  const static float phoHoverE12BarrelCut_[TPhotonCuts::_nWP]={0.05,0.05,0.05};
//  const static float phoHoverE12EndcapCut_[TPhotonCuts::_nWP]={0.05,0.05,0.05};
    const static float _phoHoverECut=0.05;//same for 2011 and 2012
    const static float _phoSigmaIEtaIEtaBarrelCut2012[TPhotonCuts::_nWP]={0.012,0.011,0.011};
    const static float _phoSigmaIEtaIEtaEndcapCut2012[TPhotonCuts::_nWP]={0.034,0.033,0.031};
    const static float _phoSigmaIEtaIEtaBarrelCut2011=0.011;
    const static float _phoSigmaIEtaIEtaEndcapCut2011=0.030;
    const static float _phoSigmaIEtaIEtaBarrelCutLeft=0.005;
    const static float _phoSigmaIEtaIEtaEndcapCutLeft=0.019;
      //left phoSigmaIEtaIEta cuts are not recommended by POG
      //and are introduced for DD template bkg estimation
      //they cut less than 0.5% of events (in Signal MC)
    const static float _phoSigmaIEtaIEtaBarrelSideband=0.012;//0.011
    const static float _phoSigmaIEtaIEtaEndcapSideband=0.034;//0.033

    const static float _phoIsoBarrelCut2012[3][2*TPhotonCuts::_nWP]=
                           {{2.6,0.000,1.5,0.000,0.7,0.000}, //changed
                            {3.5,0.040,1.0,0.040,0.4,0.040}, //neutral
                            {1.3,0.005,0.7,0.005,0.5,0.005}};//photon
    const static float _phoIsoEndcapCut2012[3][2*TPhotonCuts::_nWP]=
                           {{2.3,0.000,1.2,0.000,0.5,0.000}, //changed
                            {2.9,0.040,1.5,0.040,1.5,0.040}, //neutral
                          {10000,0.000,1.0,0.005,1.0,0.005}};//photon

    const static float _phoIsoCut2011[3][2]=
                           {{2.0,0.0010}, //trk (changed)
                            {2.2,0.0025}, //Hcal (neutral)
                            {4.2,0.0060}};//Ecal (photon)
    //the cuts for PFNeuIso and PFPhoIso have forms of
    //IsoCut = A+phoPt*B; 
    //10000 means there is no cut for PFPhoIso 
    //for loose working point in endcap

    const static float _phoSCRChIsoCorrCut = 4.0;
      // this cut is not from POG gamma ID
      // after all cuts except phoSigmaIEtaIEta and phoPFChIsoCorr,
      // condition "phoEt>15 && phoPFChIsoCorr<1.5 && phoSCRChIsoCorr>4"
      // contains less than 1% of events from 
      // condition "phoEt>15 && phoPFChIsoCorr<1.5" (in Signal MC)
    const static float _phoSCRChIsoCorrSideband = 4.0;

   

#endif //TPhotonCuts_h
