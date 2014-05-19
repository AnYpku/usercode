#include "TMuonCuts.h" //class of this package
#include "math.h" //general C++ class
#include <iostream> //general C++ class
#include <vector> //general C++ class
#include "TMath.h" //ROOT class
#include "TCut.h" //ROOT class

TMuonCuts::TMuonCuts()
{
}

TMuonCuts::TMuonCuts(  int imu,
               float muPt_imu,
               vector <float> *muPt,
               float muEta_imu,
               vector <float> *muEta,
               int nMu,
               int muNumberOfValidPixelHits_imu,
               int muNumberOfValidTrkHits_imu,
               int muNumberOfValidTrkLayers_imu,
               int muNumberOfValidMuonHits_imu,
               int muStations_imu,
               int HLT_HLTIndex_18,
               int HLT_HLTIndex_19,
               int muTrg_imu,
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
  muTrg_imu_=muTrg_imu;
  HLT_HLTIndex_19_=HLT_HLTIndex_19;
  muChi2NDF_imu_=muChi2NDF_imu;
  muD0_imu_=muD0_imu;
  muDZ_imu_=muDZ_imu;
  muPFIsoR04_CH_imu_=muPFIsoR04_CH_imu;
  muPFIsoR04_NH_imu_=muPFIsoR04_NH_imu;
  muPFIsoR04_Pho_imu_=muPFIsoR04_Pho_imu;
  muPFIsoR04_PU_imu_=muPFIsoR04_PU_imu;
  for (int i=0; i<nMu; i++)
    {
      muPt_.push_back(muPt->at(i));
      muEta_.push_back(muEta->at(i));
    }
}

TMuonCuts::~TMuonCuts()
{
}

//bool TMuonCuts::MoreMuonsVeto() 
//{
//  if (HasMoreMuons(nMu_, imu_, muPt_, muEta_)) return false;
//  return true;
//}


bool TMuonCuts::HasMoreMuons(int nMu, int imu, vector <float> *muPt, vector <float> *muEta) 
{
  if (nMu<2) return false;
  for (int i=0; i<nMu; i++)
    {
      if (i==imu) continue;
      else if (muPt->at(i)>extraMuPtCut_ && fabs(muEta->at(i))<extraMuEtaCut_) return true;
    }
  return false;
}

bool TMuonCuts::PassedExceptKinematics() { 
  if (!MuTriggerMatch()) return false;
  if (!MuId()) return false; 
  if (!MuIsolationPassed()) return false;
  return true;
}


bool TMuonCuts::Passed() { 
  if (!PassedKinematics(muPt_imu_, muEta_imu_)) return false; 
  if (!PassedExceptKinematics()) return false;
  return true;
}

bool TMuonCuts::MuTriggerMatch()
{
  if (HLT_HLTIndex_19_ &&   (((muTrg_imu_&(1<<1)))==(1<<1)) ) return true;
    //HLT_IsoMu24_v, muonTriggerMatchHLTIsoMu24 (1st bit of muTrg[imu])
  if (HLT_HLTIndex_18_ && (((muTrg_imu_&(1<<0)))==(1<<0))) return true;
    //HLT_IsoMu24_eta2p1_, muonTriggerMatchHLTIsoMu24eta2p1 (0th bit of muTrg[imu])
  return false;
}

bool TMuonCuts::PassedKinematics(float muPt, float muEta) 
{
  if (muPt<=muPtCut_) return false; 
  if (fabs(muEta)>=muEtaCut_) return false ;
  return true ; 
}

bool TMuonCuts::MuId() 
{
  return MuId(muChi2NDF_imu_, muD0_imu_, muDZ_imu_, 
                muNumberOfValidMuonHits_imu_, muNumberOfValidTrkHits_imu_, 
                muNumberOfValidPixelHits_imu_, muNumberOfValidTrkLayers_imu_, 
                muStations_imu_);
}

bool TMuonCuts::MuId(float muChi2NDF, float muD0, float muDZ, 
                int muNumberOfValidMuonHits, int muNumberOfValidTrkHits, 
                int muNumberOfValidPixelHits, int muNumberOfValidTrkLayers, 
                int muStations) 
{
  if (muChi2NDF >= muChi2NDFCut_) return false;
  if (muNumberOfValidMuonHits <= muNOfValidMuonHitsCut_) return false;
  if (muNumberOfValidTrkHits <= muNOfValidTrkHitsCut_) return false;
  if (fabs(muD0) >= impactParameterCut_) return false;
     //it might be muD0GV, muD0 or muDoVtx
  if (muStations<=muStationsCut_) return false;
  if (fabs(muDZ) >= longitudinalDistanceCut_) return false;
  if (muNumberOfValidPixelHits<=muNOfValidPixelHitsCut_) return false;
  if (muNumberOfValidTrkLayers<=muNOfValidTrkLayersCut_) return false;
  return true;
}

bool TMuonCuts::MuIsolationPassed() {
  if (muPt_imu_ == 0) return false;
  float isolation = MuIsolation(muPt_imu_, 
                 muPFIsoR04_NH_imu_, muPFIsoR04_Pho_imu_, 
                 muPFIsoR04_PU_imu_, muPFIsoR04_CH_imu_);
//  if (muPFIsoR04_NH_imu_+muPFIsoR04_Pho_imu_-0.5*muPFIsoR04_PU_imu_ < 0 )
//    isolation=muPFIsoR04_CH_imu_/muPt_imu_;
//  else isolation=(muPFIsoR04_CH_imu_+muPFIsoR04_NH_imu_
//                  +muPFIsoR04_Pho_imu_-0.5*muPFIsoR04_PU_imu_)/muPt_imu_;
  if (isolation >= isoTightCut_) return false; 
  return true ; 
}

float TMuonCuts::MuIsolation(float muPt, 
                 float muPFIsoR04_NH, float muPFIsoR04_Pho, 
                 float muPFIsoR04_PU,float muPFIsoR04_CH) {
  if (muPt == 0) return 10000;
  float isolation;
  if (muPFIsoR04_NH+muPFIsoR04_Pho-0.5*muPFIsoR04_PU < 0 )
    isolation=muPFIsoR04_CH/muPt;
  else isolation=(muPFIsoR04_CH+muPFIsoR04_NH
                  +muPFIsoR04_Pho-0.5*muPFIsoR04_PU)/muPt;
//  if (isolation >= isoTightCut_) return false; 
  return isolation; 
}

TCut TMuonCuts::RangeId(){
  return "leptonId";
}

TCut TMuonCuts::RangeIsolation(){
  TString strCut="leptonIsolation<";
  strCut+=isoTightCut_;
  TCut cut(strCut);
  return cut;
}

TCut TMuonCuts::RangeTriggerMatch(){
  TCut cutTrg1="HLT_IsoMu24_v && trgMatchIsoMu24";
  TCut cutTrg2="HLT_IsoMu24_eta2p1_ && trgMatchIsoMu24eta2p1";
  return (cutTrg1||cutTrg2);
}

TCut TMuonCuts::RangeMuon(bool doIsoCut, bool doIdCut, bool doTrgCut){
  TCut cut="1";
  if (doTrgCut) cut=cut && RangeTriggerMatch();
  if (doIdCut)  cut=cut && RangeId();
  if (doIsoCut) cut=cut && RangeIsolation();
  return cut;
}

