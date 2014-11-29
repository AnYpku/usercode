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

bool TMuonCuts::HasMoreMuons(TEventTree::InputTreeLeaves &leaf, int imu) 
{
  if (leaf.nMu<2) return false;
  for (int i=0; i<leaf.nMu; i++)
    {
      if (i==imu) continue;
      else if (leaf.muPt->at(i)>_extraMuPtCut && fabs(leaf.muEta->at(i))<_extraMuEtaCut) return true;
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

bool TMuonCuts::MuId(int year, TEventTree::InputTreeLeaves &leaf, int imu) 
{
  if (!(leaf.muType->at(imu) == 46)) return 0;
  if (!(leaf.muChi2NDF->at(imu) < 10)) return 0;
  if (!(leaf.muNumberOfValidMuonHits->at(imu)>0)) return 0;
  if ((year==2012) && !(fabs(leaf.muInnerD0GV->at(imu)) < 0.20)) return 0;
  if ((year==2012) && !(fabs(leaf.muInnerDzGV->at(imu)) < 0.50)) return 0;
  if ((year==2011) && !(fabs(leaf.muInnerD0GV->at(imu)) < 0.02)) return 0;
  if ((year==2011) && !(fabs(leaf.muInnerDzGV->at(imu)) < 0.10)) return 0;
     //it might be muD0GV, muD0 or muDoVtx
  if (!(leaf.muStations->at(imu)>1)) return 0;
  if (!(leaf.muNumberOfValidPixelHits->at(imu)>0)) return 0;
  if (!(leaf.muNumberOfValidTrkLayers->at(imu)>5)) return 0;

  // isolation are set up to be part of muon ID 
  // to be consistent with electron selection
  // (in POG they are separated)
  if ((year==2012) && !(MuIsolation2012(leaf,imu)<0.12)) return 0;
  if ((year==2011) && !(MuIsolation2012(leaf,imu)<0.10)) return 0;
  return 1;
}

float TMuonCuts::MuIsolation2012(TEventTree::InputTreeLeaves &leaf, int imu) {
  if (leaf.muPt->at(imu) == 0) return 10000;
  float isolation;
  if (leaf.muPFIsoR04_NH->at(imu)+leaf.muPFIsoR04_Pho->at(imu)-0.5*leaf.muPFIsoR04_PU->at(imu) < 0 )
    isolation=leaf.muPFIsoR04_CH->at(imu)/leaf.muPt->at(imu);
  else isolation=(leaf.muPFIsoR04_CH->at(imu)+leaf.muPFIsoR04_NH->at(imu)
                  +leaf.muPFIsoR04_Pho->at(imu)
                  -0.5*leaf.muPFIsoR04_PU->at(imu))/leaf.muPt->at(imu);
  return isolation; 
}

//  if ( (muIsoTrk[imu]+muIsoEcal[imu]+muIsoHcal[imu] - rho*TMath::Pi()*0.3*0.3) < 0.1*muPt[imu] ) {
float TMuonCuts::MuIsolation2011(TEventTree::InputTreeLeaves &leaf, int imu) {
//  if ( (muIsoTrk[imu]+muIsoEcal[imu]+muIsoHcal[imu] - rho*TMath::Pi()*0.3*0.3) < 0.1*muPt[imu] ) - from Senka's code
  if (leaf.muPt->at(imu) == 0) return 10000;
  float isolation=(leaf.muIsoTrk->at(imu)+leaf.muIsoHcal->at(imu)+leaf.muIsoEcal->at(imu)-leaf.rho2011*TMath::Pi()*0.3*0.3)/leaf.muPt->at(imu);
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
/*
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
*/
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

TCut TMuonCuts::RangeMuon(int year, int vgamma, int ilep, bool doIdCut, bool doTrgCut){
  TCut cut="1";
  if (doTrgCut) cut=cut && RangeTriggerMatch(vgamma,ilep);
  if (doIdCut)  cut=cut && RangeId(year,ilep);
  //if (doIsoCut) cut=cut && RangeIsolation(year,ilep);
  return cut;
}

