#ifndef TMuonCuts_h
#define TMuonCuts_h

#include "../Selection/TEventTree.h" //class of this package

class TMuonCuts
  {
     public:
       TMuonCuts ();
       TMuonCuts ( int imu,
               float muPt_imu,
               float* muPt,
               float muEta_imu,
               float* muEta,
               int nMu,
               int muNumberOfValidPixelHits_imu,
               int muNumberOfValidTrkHits_imu,
               int muNumberOfValidTrkLayers_imu_,
               int muNumberOfValidMuonHits_imu_,
               int muStations_imu,
               int HLT_HLTIndex_18,
               int HLT_HLTIndex_19,
               int muTrg_imu_0,
               int muTrg_imu_1,
               float muChi2NDF_imu_,
               float muD0_imu,
               float muDZ_imu,
               float muPFIsoR04_CH_imu,
               float muPFIsoR04_NH_imu,
               float muPFIsoR04_Pho_imu,
               float muPFIsoR04_PU_imu);

       virtual ~TMuonCuts();
       bool MoreMuonsVeto();
         //veto on having another muon
       bool Passed();
         //if muon passed the whole selection requirement 
         //(except veto on having another muon)
       bool MuTriggerMatch();
       bool MuKinematics(float muPt, float muEta);
       bool MuIsolation();
        //mu Isolation as recommended here:
        //https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideMuonId#Muon_Isolation
       bool MuId ();
        //tight muon ID as recommended here:
        //https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideMuonId#Tight_Muon

     private:
       int imu_;
       int nMu_;
       int muNumberOfValidPixelHits_imu_;
       int muNumberOfValidTrkHits_imu_;
       int muNumberOfValidTrkLayers_imu_;
       int muNumberOfValidMuonHits_imu_;
       int muStations_imu_;
       int HLT_HLTIndex_18_;
       int muTrg_imu_0_;
       int HLT_HLTIndex_19_;
       int muTrg_imu_1_;
       float muChi2NDF_imu_;
       float muPt_imu_;
       float muEta_imu_;
       float muD0_imu_;
       float muDZ_imu_;
       float muPFIsoR04_CH_imu_;
       float muPFIsoR04_NH_imu_;
       float muPFIsoR04_Pho_imu_;
       float muPFIsoR04_PU_imu_;
       float muPt_[TEventTree::kMaxnMu];
       float muEta_[TEventTree::kMaxnMu];
       
 

       const static float muPtCut_=35.;
       const static float extraMuPtCut_=10.;
       const static float muEtaCut_=2.1;
       const static float extraMuEtaCut_=2.4;
       const static float impactParameterCut_=0.2;//it is 2 mm
       const static float longitudinalDistanceCut_=0.5;//it is 5 mm
       const static float muChi2NDFCut_=10;
       const static int muStationsCut_=1;
       const static int muNOfValidTrkHitsCut_ = 10;
       const static int muNOfValidPixelHitsCut_=0;
       const static int muNOfValidTrkLayersCut_=5;
       const static int muNOfValidMuonHitsCut_=0;
       const static float isoTightCut_=0.12;
       const static float isoLooseCut_=0.20;
       
  };

#endif //#ifndef TMuonCuts_h
