#include "TMuonCuts.h" //class of this package
#include "math.h" //general C++ class
#include <iostream> //general C++ class
#include "TMath.h" //ROOT class

TMuonCuts::TMuonCuts()
{
}

TMuonCuts::TMuonCuts(  int imu,
               float muPt_imu,
               float* muPt,
               float muEta_imu,
               float* muEta,
               int nMu,
               int muNumberOfValidPixelHits_imu,
               int muNumberOfValidTrkHits_imu,
               int muNumberOfValidTrkLayers_imu,
               int muNumberOfValidMuonHits_imu,
               int muStations_imu,
               int HLT_HLTIndex_18,
               int HLT_HLTIndex_19,
               int muTrg_imu_0,
               int muTrg_imu_1,
               float muChi2NDF_imu,
               float muD0_imu,
               float muDZ_imu,
               float muPFIsoR04_CH_imu,
               float muPFIsoR04_NH_imu,
               float muPFIsoR04_Pho_imu,
               float muPFIsoR04_PU_imu)
{
  imu_=imu;
  muPt_imu_=muPt_imu;
  muEta_imu_=muEta_imu;
  nMu_=nMu;
  muNumberOfValidPixelHits_imu_=muNumberOfValidPixelHits_imu;
  muNumberOfValidTrkHits_imu_=muNumberOfValidTrkHits_imu;
  muNumberOfValidTrkLayers_imu_=muNumberOfValidTrkLayers_imu;
  muNumberOfValidMuonHits_imu_=muNumberOfValidMuonHits_imu;
  muStations_imu_=muStations_imu;
  HLT_HLTIndex_18_=HLT_HLTIndex_18;
  muTrg_imu_0_=muTrg_imu_0;
  HLT_HLTIndex_19_=HLT_HLTIndex_19;
  muTrg_imu_1_=muTrg_imu_1;
  muChi2NDF_imu_=muChi2NDF_imu;
  muD0_imu_=muD0_imu;
  muDZ_imu_=muDZ_imu;
  muPFIsoR04_CH_imu_=muPFIsoR04_CH_imu;
  muPFIsoR04_NH_imu_=muPFIsoR04_NH_imu;
  muPFIsoR04_Pho_imu_=muPFIsoR04_Pho_imu;
  muPFIsoR04_PU_imu_=muPFIsoR04_PU_imu;
  for (int i=0; i<nMu; i++)
    {
      muPt_[i]=muPt[i];
      muEta_[i]=muEta[i];
    }
}

TMuonCuts::~TMuonCuts()
{
}

bool TMuonCuts::MoreMuonsVeto() 
{
  if (nMu_<2) return true;
  for (int i=0; i<nMu_; i++)
    {
      if (i==imu_) continue;
      if (muPt_[i]>extraMuPtCut_ && fabs(muEta_[i])<extraMuEtaCut_) return false;
    }
  return true;
}

bool TMuonCuts::PassedExceptKinematics() { 
  if (!MuTriggerMatch()) return false;
  if (!MuId()) return false; 
  if (!MuIsolation()) return false;
  return true;
}


bool TMuonCuts::Passed() { 
  if (!MuKinematics(muPt_imu_, muEta_imu_)) return false; 
  if (!PassedExceptKinematics()) return false;
  return true;
}

bool TMuonCuts::MuTriggerMatch()
{
  if (HLT_HLTIndex_19_ && muTrg_imu_1_) return true;
    //HLT_IsoMu24_v, muonTriggerMatchHLTIsoMu24
  if (HLT_HLTIndex_18_ && muTrg_imu_0_) return true;
    //HLT_IsoMu24_eta2p1_, muonTriggerMatchHLTIsoMu24eta2p1
  return false;
}

bool TMuonCuts::MuKinematics(float muPt, float muEta) 
{
  if (muPt<=muPtCut_) return false; 
  if (fabs(muEta)>=muEtaCut_) return false ;
  return true ; 
}

bool TMuonCuts::MuId () 
{
  if (muChi2NDF_imu_ >= muChi2NDFCut_) return false;
  if (muNumberOfValidMuonHits_imu_ <= muNOfValidMuonHitsCut_) return false;
  if (muNumberOfValidTrkHits_imu_ <= muNOfValidTrkHitsCut_) return false;
  if (fabs(muD0_imu_) >= impactParameterCut_) return false;
     //it might be muD0GV, muD0 or muDoVtx
  if (muStations_imu_<=muStationsCut_) return false;
  if (fabs(muDZ_imu_) >= longitudinalDistanceCut_) return false;
  if (muNumberOfValidPixelHits_imu_<=muNOfValidPixelHitsCut_) return false;
  if (muNumberOfValidTrkLayers_imu_<=muNOfValidTrkLayersCut_) return false;
  return true;
}

bool TMuonCuts::MuIsolation() {
  if (muPt_imu_ == 0) return false;
  float isolation;
  if (muPFIsoR04_NH_imu_+muPFIsoR04_Pho_imu_-0.5*muPFIsoR04_PU_imu_ < 0 )
    isolation=muPFIsoR04_CH_imu_/muPt_imu_;
  else isolation=(muPFIsoR04_CH_imu_+muPFIsoR04_NH_imu_
                  +muPFIsoR04_Pho_imu_-0.5*muPFIsoR04_PU_imu_)/muPt_imu_;
  if (isolation >= isoTightCut_) return false; 
  return true ; 
}


