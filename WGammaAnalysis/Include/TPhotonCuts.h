#ifndef TPhotonCuts_h
#define TPhotonCuts_h

#include "TCut.h"
 //ROOT

class TPhotonCuts
{
  public:
    TPhotonCuts();
    TPhotonCuts(    int phoEleVeto_ipho,
                    float phoEt_ipho,
                    float phoEta_ipho,
                    float phoSCEt_ipho,
                    float phoSCEta_ipho,
                    float phoHoverE12_ipho,
                    float phoSigmaIEtaIEta_ipho,
                    float phoChIso_ipho,//phoPFChIso or phoSCRChIso
                    float phoNeuIso_ipho,
                    float phoPhoIso_ipho,
                    float rho2012);
    virtual ~TPhotonCuts();

    bool Passed(int wp, bool doSigmaIEtaIEtaCut=0, bool doPhoChIsocut=0);
    bool PassedExceptKinematics(int wp, bool doSigmaIEtaIEtaCut=0, bool doPhoChIsocut=0);
    bool PassedKinematics(float phoPt, float phoEta);
    bool PhoKinematics();
    bool SimpleCutBasedPhotonID2012(int wp, bool doSigmaIEtaIEtaCut=0, bool doPhoChIsoCut=0); 
      //see https://twiki.cern.ch/twiki/bin/view/CMS/CutBasedPhotonID2012
      //for reference

    //the following three functions determine effective areas depending on |eta|
    //the values are taken from
    //https://twiki.cern.ch/twiki/bin/view/CMS/CutBasedPhotonID2012
    bool CutSigmaIEtaIEta(int wp, float eta, float sigmaIEtaIEta);
    bool CutSigmaIEtaIEta(int wp);
    bool CutPhoChIso(int wp, float phoChIso, float rho2012, float eta);
    bool CutPhoChIso(int wp);

    float EffAreaCharged(float eta);
    float EffAreaNeutral(float eta);
    float EffAreaPhotons(float eta);
    float EffAreaCharged();
    float EffAreaNeutral();
    float EffAreaPhotons();
    //the following function calculated corrected particle flow isolation
    float IsoCorr(float iso, float rho, float EA);
    bool IsBarrel(float phoEta);
    bool IsEndcap(float phoEta);
    bool IsBarrel();
    bool IsEndcap();

    float GetPhoChIsoCorr(float phoChIso, float rho2012, float eta);
    float GetPhoNeuIsoCorr(float phoNeuIso, float rho2012, float eta);
    float GetPhoPhoIsoCorr(float phoPhoIso, float rho2012, float eta);
    float GetPhoSigmaIEtaIEtaCutB(int wp);
    float GetPhoSigmaIEtaIEtaCutE(int wp);
    float GetPhoSigmaIEtaIEtaCutLeftB();
    float GetPhoSigmaIEtaIEtaCutLeftE();
    int   GetWP();

    TCut RangeBarrel();
    TCut RangeEndcap();
    TCut RangeSigmaIEtaIEta(int wp);
    TCut SidebandSigmaIEtaIEta();
    TCut RangePhoChIso(TString strIso, int wp);
    TCut SidebandPhoChIso(TString strIso);
    TCut RangePhoNeuIso(TString strIso, int wp);
    TCut RangePhoPhoIso(TString strIso, int wp);
    TCut RangeHoverE12();
    TCut RangePhoEleVeto();
    TCut RangePhoton(int wp, TString isoBase, 
           bool doSigmaIEtaIEtaCut=1, bool doChIsoCut=1, 
           bool doNeuIsoCut=1, bool doPhoIsoCut=1, 
           bool doHoverE12Cut=1, bool doElectronVetoCut=1);

    TCut RangeGenFakePhoton();
    TCut RangeGenTruePhoton();


    const static int nWP_=3;
    enum {WP_LOOSE, WP_MEDIUM, WP_TIGHT};


  private:
    int phoEleVeto_ipho_; //conversion safe lectron veto
    float phoEt_ipho_; //super cluster photon et
    float phoEta_ipho_; //super cluster photon eta
    float phoSCEt_ipho_; //super cluster detector et
    float phoSCEta_ipho_; //super cluster detector eta
    float phoHoverE12_ipho_; //single tower H/E
    float phoSigmaIEtaIEta_ipho_; //shower shape variable
    float phoChIso_ipho_; //particle flow charged isolation, PF or SCR
    float phoNeuIso_ipho_;//particle flow neutral isolation, PF or SCR
    float phoPhoIso_ipho_;//particle flow photon isolation, PF or SCR
    float rho2012_;

    //the following cuts depend on photon Pt
    //(which will be phoSCEt_ here)
    //these cut-arrays are filled in the constructor 
    float phoNeuIsoBarrelCut_[3];
    float phoNeuIsoEndcapCut_[3];
    float phoPhoIsoBarrelCut_[3];
    float phoPhoIsoEndcapCut_[3];

    const static float phoPtCut_ = 10.0;
    const static int WP_=WP_MEDIUM; 
      //WP - working point
      //nWP - total number of working points
      //0 - loose  (90% for barrel, 85% for endcap) 
      //1 - medium (80% - barrel, 75% - endcap) 
      //2 - tight  (70% - barrel, 65% - endcap)
      //see https://twiki.cern.ch/twiki/bin/view/CMS/CutBasedPhotonID2012
      //for reference
};

//for some reason, I'm not allowed to initialize these arrays inside {}
//so, I introduce them outside:
//(and they are arrays because there are 3 working points)

//    const static float phoHoverE12BarrelCut_[TPhotonCuts::nWP_]={0.05,0.05,0.05};
//    const static float phoHoverE12EndcapCut_[TPhotonCuts::nWP_]={0.05,0.05,0.05};
    const static float _phoHoverE12Cut=0.05;
    const static float phoSigmaIEtaIEtaBarrelCut_[TPhotonCuts::nWP_]={0.012,0.011,0.011};
    const static float phoSigmaIEtaIEtaEndcapCut_[TPhotonCuts::nWP_]={0.034,0.033,0.031};
    const static float phoSigmaIEtaIEtaBarrelCutLeft_=0.005;
    const static float phoSigmaIEtaIEtaEndcapCutLeft_=0.019;
      //left phoSigmaIEtaIEta cuts are not recommended by POG
      //and are introduced for DD template bkg estimation
    const static float phoChIsoBarrelCut_[TPhotonCuts::nWP_]={2.6,1.5,0.7};
    const static float phoChIsoEndcapCut_[TPhotonCuts::nWP_]={2.3,1.2,0.5};

    const static float phoSigmaIEtaIEtaBarrelSideband_=0.012;//0.011
    const static float phoSigmaIEtaIEtaEndcapSideband_=0.034;//0.033
    const static float phoChIsoBarrelSideband_=2.6; //;
    const static float phoChIsoEndcapSideband_=2.3; //;

    //the cuts for PFNeuIso and PFPhoIso have forms of
    //IsoCut = A+phoPt*B; 
    //they are calculated in the constructor of the class
    const static float phoNeuIsoBarrelCutA_[TPhotonCuts::nWP_]={3.5,1.0,0.4};
    const static float phoNeuIsoEndcapCutA_[TPhotonCuts::nWP_]={2.9,1.5,1.5};
    const static float phoPhoIsoBarrelCutA_[TPhotonCuts::nWP_]={1.3,0.7,0.5};
    const static float phoPhoIsoEndcapCutA_[TPhotonCuts::nWP_]={10000,1.0,1.0};
      //10000 means there is no cut for PFPhoIso 
      //for loose working point in endcap
    const static float phoNeuIsoBarrelCutB_[TPhotonCuts::nWP_]={0.04,0.04,0.04};
    const static float phoNeuIsoEndcapCutB_[TPhotonCuts::nWP_]={0.04,0.04,0.04};
    const static float phoPhoIsoBarrelCutB_[TPhotonCuts::nWP_]={0.005,0.005,0.005};
    const static float phoPhoIsoEndcapCutB_[TPhotonCuts::nWP_]={0,    0.005,0.005};

#endif //TPhotonCuts_h
