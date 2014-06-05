#include "TMuonCuts.h" //class of this package
#include "math.h" //general C++ class
#include <iostream> //general C++ class
#include <vector> //general C++ class
#include "TMath.h" //ROOT class
#include "TCut.h" //ROOT class

TMuonCuts::TMuonCuts()
{
}

TMuonCuts::~TMuonCuts()
{
}




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

bool TMuonCuts::PassedKinematics(float muPt, float muEta) 
{
  if (muPt<=muPtCut_) return false; 
  if (fabs(muEta)>=muEtaCut_) return false ;
  return true ; 
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

