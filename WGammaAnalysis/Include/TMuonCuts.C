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

bool TMuonCuts::PassedKinematics(float muPt, float muEta, bool& ifPassedPt, bool& ifPassedEta) 
{
  ifPassedPt=true; ifPassedEta=true;
  if (muPt<=_muPtCut) ifPassedPt=false; 
  if (fabs(muEta)>=_muEtaCut) ifPassedEta=false;
  return (ifPassedPt && ifPassedEta); 
}

bool TMuonCuts::MuId(int year, float muChi2NDF, float muD0, float muDZ, 
                int muNumberOfValidMuonHits, int muNumberOfValidTrkHits, 
                int muNumberOfValidPixelHits, int muNumberOfValidTrkLayers, 
                int muStations, int muType) 
{
  if (muType != _muType2012) return false;
  if (muChi2NDF >= _muChi2NDFCut) return false;
  if (muNumberOfValidMuonHits <= _muNOfValidMuonHitsCut) return false;
//  if (muNumberOfValidTrkHits <= _muNOfValidTrkHitsCut) return false;
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

TCut TMuonCuts::RangeTriggerMatch(int vgamma, int ilep){
  TString strLep="";
  strLep+=ilep;
  TCut cutTrg="1";
  if (vgamma==_config.W_GAMMA){
    TString strHLT1="HLT_IsoMu24_eta2p1_"; 
    TString strMuTrg1=TString("trgMatch")+strLep+TString("IsoMu24eta2p1");
    TString strHLT2="HLT_IsoMu24_v"; 
    TString strMuTrg2=TString("trgMatch")+strLep+TString("IsoMu24");
    cutTrg=RangeTriggerOne(strHLT1,strMuTrg1) || 
           RangeTriggerOne(strHLT2,strMuTrg2);
  }
  else if (vgamma==_config.Z_GAMMA){
    TString strHLT="HLT_Mu22_Mu8_v"; 
    TString strMuTrg=TString("trgMatch")+strLep+TString("Mu22Mu8");
    cutTrg=RangeTriggerOne(strHLT,strMuTrg);
  }
  return (cutTrg);
}

TCut TMuonCuts::RangeTriggerOne(TString strHLT, TString strMuTrg){
  TCut cutHLT(strHLT);
  TCut cutMuTrg(strMuTrg);
  TCut cutTrg(cutHLT && cutMuTrg);
  return cutTrg;
}

TCut TMuonCuts::RangeMuon(int year, int vgamma, int ilep, bool doIsoCut, bool doIdCut, bool doTrgCut){
  TCut cut="1";
  if (doTrgCut) cut=cut && RangeTriggerMatch(vgamma,ilep);
  if (doIdCut)  cut=cut && RangeId(year,ilep);
  if (doIsoCut) cut=cut && RangeIsolation(year,ilep);
  return cut;
}

