#ifndef TPhotonCuts_h
#define TPhotonCuts_h

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
                    float phoPFChIso_ipho,
                    float phoPFNeuIso_ipho,
                    float phoPFPhoIso_ipho,
                    float rho2012);
    virtual ~TPhotonCuts();

    bool Passed(bool doSigmaIEtaIEtaCut=1);
    bool PassedExceptKinematics(bool doSigmaIEtaIEtaCut=1);
    bool PhoKinematics(float phoPt, float phoEta);
    bool PhoKinematics();
    bool SimpleCutBasedPhotonID2012(bool doSigmaIEtaIEtaCut=1); 
      //see https://twiki.cern.ch/twiki/bin/view/CMS/CutBasedPhotonID2012
      //for reference

    //the following three functions determine effective areas depending on |eta|
    //the values are taken from
    //https://twiki.cern.ch/twiki/bin/view/CMS/CutBasedPhotonID2012
    bool CutSigmaIEtaIEta();

    float EffAreaCharged();
    float EffAreaNeutral();
    float EffAreaPhotons();
    //the following function calculated corrected particle flow isolation
    float PFIsoCorr(float PFIso, float rho, float EA);
    bool IsBarrel(float phoEta);
    bool IsEndcap(float phoEta);
    bool IsBarrel();
    bool IsEndcap();

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
    float phoPFChIso_ipho_; //particle flow charged isolation
    float phoPFNeuIso_ipho_;//particle flow neutral isolation
    float phoPFPhoIso_ipho_;//particle flow photon isolation
    float rho2012_;

    //the following cuts depend on photon Pt
    //(which will be phoSCEt_ here)
    //these cut-arrays are filled in the constructor 
    float phoPFNeuIsoBarrelCut_[3];
    float phoPFNeuIsoEndcapCut_[3];
    float phoPFPhoIsoBarrelCut_[3];
    float phoPFPhoIsoEndcapCut_[3];

    const static float phoPtCut_ = 15.0;
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

    const static float phoHoverE12BarrelCut_[TPhotonCuts::nWP_]={0.05,0.05,0.05};
    const static float phoHoverE12EndcapCut_[TPhotonCuts::nWP_]={0.05,0.05,0.05};
    const static float phoSigmaIEtaIEtaBarrelCut_[TPhotonCuts::nWP_]={0.012,0.011,0.011};
    const static float phoSigmaIEtaIEtaEndcapCut_[TPhotonCuts::nWP_]={0.034,0.033,0.031};
    const static float phoPFChIsoBarrelCut_[TPhotonCuts::nWP_]={2.6,1.5,0.7};
    const static float phoPFChIsoEndcapCut_[TPhotonCuts::nWP_]={2.3,1.2,0.5};

    //the cuts for PFNeuIso and PFPhoIso have forms of
    //IsoCut = A+phoPt*B; 
    //they are calculated in the constructor of the class
    const static float phoPFNeuIsoBarrelCutA_[TPhotonCuts::nWP_]={3.5,1.0,0.4};
    const static float phoPFNeuIsoEndcapCutA_[TPhotonCuts::nWP_]={2.9,1.5,1.5};
    const static float phoPFPhoIsoBarrelCutA_[TPhotonCuts::nWP_]={1.3,0.7,0.5};
    const static float phoPFPhoIsoEndcapCutA_[TPhotonCuts::nWP_]={10000,1.0,1.0};
      //10000 means there is no cut for PFPhoIso 
      //for loose working point in endcap
    const static float phoPFNeuIsoBarrelCutB_[TPhotonCuts::nWP_]={0.04,0.04,0.04};
    const static float phoPFNeuIsoEndcapCutB_[TPhotonCuts::nWP_]={0.04,0.04,0.04};
    const static float phoPFPhoIsoBarrelCutB_[TPhotonCuts::nWP_]={0.005,0.005,0.005};
    const static float phoPFPhoIsoEndcapCutB_[TPhotonCuts::nWP_]={0,    0.005,0.005};

#endif //TPhotonCuts_h
