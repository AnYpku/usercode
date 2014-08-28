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
      else if (muPt->at(i)>_extraMuPtCut && fabs(muEta->at(i))<_extraMuEtaCut) return true;
    }
  return false;
}

bool TMuonCuts::PassedKinematics(float muPt, float muEta) 
{
  if (muPt<=_muPtCut) return false; 
  if (fabs(muEta)>=_muEtaCut) return false ;
  return true ; 
}

bool TMuonCuts::MuId(int year, float muChi2NDF, float muD0, float muDZ, 
                int muNumberOfValidMuonHits, int muNumberOfValidTrkHits, 
                int muNumberOfValidPixelHits, int muNumberOfValidTrkLayers, 
                int muStations, int muType) 
{
  if (muType != _muType2012) return false;
  if (muChi2NDF >= _muChi2NDFCut) return false;
  if (muNumberOfValidMuonHits <= _muNOfValidMuonHitsCut) return false;
  if (muNumberOfValidTrkHits <= _muNOfValidTrkHitsCut) return false;
  if ((year==2012) && (fabs(muD0) >= _D0Cut2012)) return false;
  if ((year==2012) && (fabs(muDZ) >= _DZCut2012)) return false;
  if ((year==2011) && (fabs(muD0) >= _D0Cut2011)) return false;
  if ((year==2011) && fabs(muDZ) >= _DZCut2011) return false;
     //it might be muD0GV, muD0 or muDoVtx
  if (muStations<=_muStationsCut) return false;
  if (muNumberOfValidPixelHits<=_muNOfValidPixelHitsCut) return false;
  if (muNumberOfValidTrkLayers<=_muNOfValidTrkLayersCut) return false;
  return true;
}

float TMuonCuts::MuIsolation2012(float muPt, 
                 float muPFIsoR04_NH, float muPFIsoR04_Pho, 
                 float muPFIsoR04_PU,float muPFIsoR04_CH) {
  if (muPt == 0) return 10000;
  float isolation;
  if (muPFIsoR04_NH+muPFIsoR04_Pho-0.5*muPFIsoR04_PU < 0 )
    isolation=muPFIsoR04_CH/muPt;
  else isolation=(muPFIsoR04_CH+muPFIsoR04_NH
                  +muPFIsoR04_Pho-0.5*muPFIsoR04_PU)/muPt;
  return isolation; 
}

//  if ( (muIsoTrk[imu]+muIsoEcal[imu]+muIsoHcal[imu] - rho*TMath::Pi()*0.3*0.3) < 0.1*muPt[imu] ) {
float TMuonCuts::MuIsolation2011(float muPt, 
                 float muIsoTrk, float muIsoEcal, 
                 float muIsoHcal,float rho2011) {
//  if ( (muIsoTrk[imu]+muIsoEcal[imu]+muIsoHcal[imu] - rho*TMath::Pi()*0.3*0.3) < 0.1*muPt[imu] ) - from Senka's code
  if (muPt == 0) return 10000;
  float isolation=(muIsoTrk+muIsoHcal+muIsoEcal-rho2011*TMath::Pi()*0.3*0.3)/muPt;
  return isolation; 
}

TCut TMuonCuts::RangeId(int year, int ilep){
  TString lepName="lepton";
  lepName+=ilep;
  lepName+="Id";
  lepName+=year;
  TCut cut(lepName);
  return cut;
}

TCut TMuonCuts::RangeIsolation(int year, int ilep){
  TString strCut="lepton";
  strCut+=ilep;
  strCut+="Isolation";
  strCut+=year;
  strCut+="<";
  if (year==2012){
    strCut+=_isoTightCut2012;
  }
  else if (year==2011){
    strCut+=_isoCut2011;
  }
  TCut cut(strCut);
  return cut;
}

TCut TMuonCuts::RangeTriggerMatch(int ilep){
  TString strCut1="HLT_IsoMu24_v && trgMatch";
  strCut1+=ilep;
  strCut1+="IsoMu24";
  TCut cutTrg1(strCut1);
  TString strCut2="HLT_IsoMu24_eta2p1_ && trgMatch";
  strCut2+=ilep;
  strCut2+="IsoMu24eta2p1";
  TCut cutTrg2(strCut2);
  return (cutTrg1||cutTrg2);
}

TCut TMuonCuts::RangeMuon(int year, int ilep, bool doIsoCut, bool doIdCut, bool doTrgCut){
  TCut cut="1";
  if (doTrgCut) cut=cut && RangeTriggerMatch(ilep);
  if (doIdCut)  cut=cut && RangeId(year,ilep);
  if (doIsoCut) cut=cut && RangeIsolation(year,ilep);
  return cut;
}

