#ifndef TSelectedEventsTree_h
#define TSelectedEventsTree_h

#include "TTree.h" //ROOT class
#include "TFile.h" //ROOT class
#include "TString.h" //ROOT class

#include "../Include/TEventTree.h" //class of this package
#include "../Include/TPhotonCuts.h" //class of this package
#include "../Include/TMuonCuts.h" //class of this package
#include "../Include/TElectronCuts.h" //class of this package
#include "../Include/TFullCuts.h" //class of this package
#include "../Include/TMathTools.h" //class of this package
#include "../PHOSPHOR_CORRECTION/PhosphorCorrectorFunctor.hh"

class TSelectedEventsTree
{
  public:
    TSelectedEventsTree();
    virtual ~TSelectedEventsTree();

    void SetAsOutputTree(TTree* tree);
    void SetAsInputTree(TTree* tree);
    void SetValues(int channel, int sample, TEventTree::InputTreeLeaves& leaf, TFullCuts::Candidate cand, int inputFileN, float weight, float PUweight, float PU,zgamma::PhosphorCorrectionFunctor* photonCorrector);
    void SetMuonValues(TEventTree::InputTreeLeaves& leaf, TFullCuts::Candidate cand,int ilMax); 
    void SetElectronValues(TEventTree::InputTreeLeaves& leaf, TFullCuts::Candidate cand, int ilMax); 
    void SetPhotonValues(TEventTree::InputTreeLeaves& leaf, TFullCuts::Candidate cand, int channel, int ilMax); 
    void SetPhotonIsoValues(TEventTree::InputTreeLeaves& leaf, TFullCuts::Candidate cand, int sample, int channel); 
    void SetThreeIsolations(TEventTree::InputTreeLeaves& leaf, TFullCuts::Candidate cand, float &chCorr, float &neuCorr, float &phoCorr, float chOrig, float neuOrig, float phoOrig);


  private:
    TConfiguration _config;
    TMathTools _math;
    TPhotonCuts _photon;
    TMuonCuts _muon;
    TElectronCuts _electron;

    int _event;
    int _ilep[2];
    int _iMClep[2];
    float _lepEta[2];
    float _lepPhi[2];
    float _lepSCEta[2];
    float _lepSCPhi[2];
    float _lepPt[2];
    float _lepGenEta[2];
    float _lepGenPhi[2];
    float _lepGenPt[2];
    int _lepGenPID[2];
    int _lepGenParentage[2];
    int _lepGenMomPID[2];
    int _lepGenGMomPID[2];
    bool _lepId2012[2];
    bool _lepId2011[2];
    float _lepIsolation2012[2];
    float _lepIsolation2011[2];
//    int _lepTrg[2];
    int _lepType[2];
   // bool _trgMatchIsoMu24eta2p1[2];//0th bit of muTrg[imu]
   // bool _trgMatchIsoMu24[2];//1th bit of muTrg[imu]
   // bool _trgMatchMu22Mu8[2];//4th(?) bit of muTrg[imu]
   // bool _trgMatchEle27WP80[2];//0th bit of eleTrg[iele]
   // bool _trgMatchEle17Ele8[2];//2nd bit of eleTrg[iele]
    bool _lepTrgMatch[2];
    float _lepSF[2];
    bool _hasMoreLeptons;
//    int _HLT_IsoMu24_eta2p1_;//HLT[HLTIndex[18]]
//    int _HLT_IsoMu24_v;//HLT[HLTIndex[19]]
//    int _HLT_Mu22_Mu8_v;//HLT[HLTIndex[21]]
//    int _HLT_Ele27_WP80_v;//HLT[HLTIndex[17]]
//    int _HLT_Ele17_Ele8_v;//HLT[HLTIndex[22]]
    int _HLT;
    int _ipho;
    int _iMCpho;
    int _phoEleVeto;
    float _phoEta;
    float _phoPhi;
    float _phoEt;
    float _phoEtNoPhosphor;
    float _phoSCEta;
    float _phoSCPhi;
    float _phoSCEt;
    int _phoGenPID;
    int _phoGenParentage;
    int _phoGenMomPID;
    int _phoGenGMomPID;
    float _phoGenEta;
    float _phoGenPhi;
    float _phoGenEt;
    float _phoHoverE;//for 2011 cuts
    float _phoHoverE12;
    float _phoSigmaIEtaIEta;

    float _pho_genEle_dRMin;

    float _phoSF;

    float _phoPFChIsoCorr;
    float _phoPFNeuIsoCorr;
    float _phoPFPhoIsoCorr;

    float _phoSCRChIsoCorr;
    float _phoSCRNeuIsoCorr;
    float _phoSCRPhoIsoCorr;

    float _phoSCRChIso04Corr;
    float _phoSCRNeuIso04Corr;
    float _phoSCRPhoIso04Corr;

    float _phoRandConeChIsoCorr;
    float _phoRandConeNeuIsoCorr;
    float _phoRandConePhoIsoCorr;

    float _phoRandConeChIso04Corr;
    float _phoRandConeNeuIso04Corr;
    float _phoRandConePhoIso04Corr;

    float _phoEcalIsoDR04Corr;//for 2011 cuts
    float _phoHcalIsoDR04Corr;//for 2011 cuts
    float _phoTrkIsoHollowDR04Corr;//for 2011 cuts
    int _phohasPixelSeed;//for 2011 cuts
    float _lepPhoDeltaR[2];
    float _WMt;
    float _Mpholep[2];
    float _Mleplep;//relevant for Z_GAMMA only
    float _Mpholeplep;//relevant for Z_GAMMA only
    float _pfMET;
    float _pfMETPhi;
    float _pfMET_notSmeared;
    float _pfMETPhi_notSmeared;

    float _totEt;
    float _totEtPhi;

    float _rho2011;//for 2011 cuts
    float _rho2012;
    int _run;
    int _inputFileN;
    float _weight; 
    float _PUweight;
    float _PU;
    int _nMC;
    vector <int>* _mcPID;
    vector <int>* _mcMomPID;
    vector <int>* _mcGMomPID;

    TBranch* _b_event;
    TBranch* _b_ilep[2];
    TBranch* _b_iMClep[2];
    TBranch* _b_lepEta[2];
    TBranch* _b_lepPhi[2];
    TBranch* _b_lepSCEta[2];
    TBranch* _b_lepSCPhi[2];
    TBranch* _b_lepPt[2];
    TBranch* _b_lepGenPID[2];
    TBranch* _b_lepGenParentage[2];
    TBranch* _b_lepGenMomPID[2];
    TBranch* _b_lepGenGMomPID[2];
    TBranch* _b_lepGenPt[2];
    TBranch* _b_lepGenEta[2];
    TBranch* _b_lepGenPhi[2];
    TBranch* _b_lepId2012[2];
    TBranch* _b_lepId2011[2];
    TBranch* _b_lepIsolation2012[2];
    TBranch* _b_lepIsolation2011[2];
//    TBranch* _b_lepTrg[2];
    TBranch* _b_lepType[2];
//    TBranch* _b_trgMatchIsoMu24eta2p1[2];//0th bit of muTrg[imu]
//    TBranch* _b_trgMatchIsoMu24[2];//1th bit of muTrg[imu]
//    TBranch* _b_trgMatchMu22Mu8[2];//5th (?) bit of muTrg[imu]
//    TBranch* _b_trgMatchEle27WP80[2];//0th bit of eleTrg[iele]
//    TBranch* _b_trgMatchEle17Ele8[2];//2nd bit of eleTrg[iele]
    TBranch* _b_lepTrgMatch[2];
    TBranch* _b_lepSF[2];
    TBranch* _b_hasMoreLeptons;
//    TBranch* _b_HLT_IsoMu24_eta2p1_;//HLT[HLTIndex[18]]
//    TBranch* _b_HLT_IsoMu24_v;//HLT[HLTIndex[19]]
//    TBranch* _b_HLT_Mu22_Mu8_v;//HLT[HLTIndex[21]]
//    TBranch* _b_HLT_Ele27_WP80_v;//HLT[HLTIndex[17]]
//    TBranch* _b_HLT_Ele17_Ele8_v;//HLT[HLTIndex[22]]
    TBranch* _b_HLT;
    TBranch* _b_ipho;
    TBranch* _b_iMCpho;
    TBranch* _b_phoEleVeto;
    TBranch* _b_phoEta;
    TBranch* _b_phoPhi;
    TBranch* _b_phoEt;
    TBranch* _b_phoEtNoPhosphor;
    TBranch* _b_phoSCEta;
    TBranch* _b_phoSCPhi;
    TBranch* _b_phoSCEt;
    TBranch* _b_phoGenPID;
    TBranch* _b_phoGenParentage;
    TBranch* _b_phoGenMomPID;
    TBranch* _b_phoGenGMomPID;
    TBranch* _b_phoGenEt;
    TBranch* _b_phoGenEta;
    TBranch* _b_phoGenPhi;
    TBranch* _b_phoHoverE;//for 2011 cuts
    TBranch* _b_phoHoverE12;
    TBranch* _b_phoSigmaIEtaIEta;

    TBranch* _b_phoSF;

    TBranch* _b_pho_genEle_dRMin;

    TBranch* _b_phoPFChIsoCorr;
    TBranch* _b_phoPFNeuIsoCorr;
    TBranch* _b_phoPFPhoIsoCorr;

    TBranch* _b_phoSCRChIsoCorr;
    TBranch* _b_phoSCRNeuIsoCorr;
    TBranch* _b_phoSCRPhoIsoCorr;

    TBranch* _b_phoSCRChIso04Corr;
    TBranch* _b_phoSCRNeuIso04Corr;
    TBranch* _b_phoSCRPhoIso04Corr;

    TBranch* _b_phoRandConeChIsoCorr;
    TBranch* _b_phoRandConeNeuIsoCorr;
    TBranch* _b_phoRandConePhoIsoCorr;

    TBranch* _b_phoRandConeChIso04Corr;
    TBranch* _b_phoRandConeNeuIso04Corr;
    TBranch* _b_phoRandConePhoIso04Corr;

    TBranch* _b_phoEcalIsoDR04Corr;//for 2011 cuts
    TBranch* _b_phoHcalIsoDR04Corr;//for 2011 cuts
    TBranch* _b_phoTrkIsoHollowDR04Corr;//for 2011 cuts
    TBranch* _b_phohasPixelSeed;//for 2011 cuts
    TBranch* _b_lepPhoDeltaR[2];
    TBranch* _b_WMt;
    TBranch* _b_Mpholep[2];
    TBranch* _b_Mleplep;
    TBranch* _b_Mpholeplep;
    TBranch* _b_pfMET;
    TBranch* _b_pfMETPhi;
    TBranch* _b_pfMET_notSmeared;
    TBranch* _b_pfMETPhi_notSmeared;
    TBranch* _b_totEt;
    TBranch* _b_totEtPhi;
    TBranch* _b_rho2011;//for 2011 cuts
    TBranch* _b_rho2012;
    TBranch* _b_run;
    TBranch* _b_inputFileN;
    TBranch* _b_weight; 
    TBranch* _b_PUweight;
    TBranch* _b_PU;
    TBranch* _b_nMC;
    TBranch* _b_mcPID;
    TBranch* _b_mcMomPID;
    TBranch* _b_mcGMomPID;
};

#endif //#ifndef TSelectedEventsTree_h
