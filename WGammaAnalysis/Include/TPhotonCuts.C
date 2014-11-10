#include "TPhotonCuts.h"
  //this class
#include "math.h"
#include <iostream>
  //standard C++ class
#include "../Configuration/TConfiguration.h"
  //this package
#include "TFile.h"
#include "TH1D.h"
#include "TString.h"
#include "TCut.h"
 //ROOT

TPhotonCuts::TPhotonCuts()
{
}

TPhotonCuts::~TPhotonCuts()
{
}

bool TPhotonCuts::PassedKinematics(float phoPt, float phoEta, bool& ifPassedPt, bool& ifPassedEta)
{
  ifPassedPt=true; ifPassedEta=true;
  if (phoPt<_phoPtCut) ifPassedPt=false;
  if (!IsBarrel(phoEta) && !IsEndcap(phoEta)) ifPassedEta=false;
  return (ifPassedPt && ifPassedEta);
}

float TPhotonCuts::EffAreaCharged(float eta)
{
  float EA=0;
  if (fabs(eta)<1.0)        EA=0.012;
  else if (fabs(eta)<1.479) EA=0.010;
  else if (fabs(eta)<2.0)   EA=0.014;
  else if (fabs(eta)<2.2)   EA=0.012;
  else if (fabs(eta)<2.3)   EA=0.016;
  else if (fabs(eta)<2.4)   EA=0.020;
  else                      EA=0.012;
//  std::cout<<"EffAreaCharged: eta="<<eta<<", EA="<<EA<<std::endl;
  return EA;
}

float TPhotonCuts::EffAreaNeutral(float eta)
{
  float EA=0;
  if (fabs(eta)<1.0)        EA=0.030;
  else if (fabs(eta)<1.479) EA=0.057;
  else if (fabs(eta)<2.0)   EA=0.039;
  else if (fabs(eta)<2.2)   EA=0.015;
  else if (fabs(eta)<2.3)   EA=0.024;
  else if (fabs(eta)<2.4)   EA=0.039;
  else                      EA=0.072;
//  std::cout<<"EffAreaNeutral: eta="<<eta<<", EA="<<EA<<std::endl;
  return EA;
}
float TPhotonCuts::EffAreaPhotons(float eta)
{
  float EA=0;
  if (fabs(eta)<1.0)        EA=0.148;
  else if (fabs(eta)<1.479) EA=0.130;
  else if (fabs(eta)<2.0)   EA=0.112;
  else if (fabs(eta)<2.2)   EA=0.216;
  else if (fabs(eta)<2.3)   EA=0.262;
  else if (fabs(eta)<2.4)   EA=0.260;
  else                      EA=0.266;
//  std::cout<<"EffAreaPhotons: eta="<<eta<<", EA="<<EA<<std::endl;
  return EA;
}


float TPhotonCuts::IsoCorr(float iso, float rho, float EA)
{
//  std::cout<<"iso-rho*EA="<<iso<<"-"<<rho<<"*"<<EA<<"="<<iso-rho*EA<<std::endl;
  if (iso-rho*EA>0.0) return iso-rho*EA;
  return 0.0;
}

bool TPhotonCuts::IsBarrel(float phoEta)
{
  if (fabs(phoEta)<1.4442) return true;
  return false;
}

bool TPhotonCuts::IsEndcap(float phoEta)
{
  if (fabs(phoEta)>1.566 && fabs(phoEta)<2.5) return true;
  return false;
}

float TPhotonCuts::GetPhoChIsoCorr(float phoChIso, float rho2012, float eta)
{
  return IsoCorr(phoChIso, rho2012, EffAreaCharged(eta));
}

float TPhotonCuts::GetPhoNeuIsoCorr(float phoNeuIso, float rho2012, float eta)
{
  return IsoCorr(phoNeuIso, rho2012, EffAreaNeutral(eta));
}

float TPhotonCuts::GetPhoPhoIsoCorr(float phoPhoIso, float rho2012, float eta)
{
  return IsoCorr(phoPhoIso, rho2012, EffAreaPhotons(eta));
}

float TPhotonCuts::GetPhoEcalIsoDR04Corr(float iso, float rho2011, float eta)
{
  if (IsBarrel(eta)) return (iso-rho2011*0.183);
  else if(IsEndcap(eta)) return (iso-rho2011*0.090);
  return iso;
}

float TPhotonCuts::GetPhoTrkIsoHollowDR04Corr(float iso, float rho2011, float eta)
{
  if (IsBarrel(eta)) return (iso-rho2011*0.0167);
  else if(IsEndcap(eta)) return (iso-rho2011*0.032);
  return iso;
}

float TPhotonCuts::GetPhoHcalIsoDR04Corr(float iso, float rho2011, float eta)
{
  if (IsBarrel(eta)) return (iso-rho2011*0.062);
  else if(IsEndcap(eta)) return (iso-rho2011*0.180);
  return iso;
}

float TPhotonCuts::GetPhoSigmaIEtaIEtaCutB(int year, int wp)
{
  if (year==2012) return _phoSigmaIEtaIEtaBarrelCut2012[wp];
  else if (year==2011) return _phoSigmaIEtaIEtaBarrelCut2011;
  return _phoSigmaIEtaIEtaBarrelCut2012[wp];
}

float TPhotonCuts::GetPhoSigmaIEtaIEtaCutE(int year, int wp)
{
  if (year==2012) return _phoSigmaIEtaIEtaEndcapCut2012[wp];
  else if (year==2011) return _phoSigmaIEtaIEtaEndcapCut2011;
  return _phoSigmaIEtaIEtaEndcapCut2012[wp];
}

float TPhotonCuts::GetPhoSigmaIEtaIEtaCutLeftB()
{
  return _phoSigmaIEtaIEtaBarrelCutLeft;
}

float TPhotonCuts::GetPhoSCRChIsoCorrCut()
{
  return _phoSCRChIsoCorrCut;
}

float TPhotonCuts::GetPhoSigmaIEtaIEtaCutLeftE()
{
  return _phoSigmaIEtaIEtaEndcapCutLeft;
}


float TPhotonCuts::GetOneIsolationCutB(int year, int wp, int isoType, float phoEt)
{
  if (year==2012) return (_phoIsoBarrelCut2012[isoType][2*wp]+phoEt*_phoIsoBarrelCut2012[isoType][2*wp+1]);
  else if (year==2011) return (_phoIsoCut2011[isoType][0]+phoEt*_phoIsoCut2011[isoType][1]);
  return _phoIsoBarrelCut2012[isoType][2*wp+1]+phoEt*_phoIsoBarrelCut2012[isoType][2*wp+1]; 
}

float TPhotonCuts::GetOneIsolationCutE(int year, int wp, int isoType, float phoEt)
{
  if (year==2012) return (_phoIsoEndcapCut2012[isoType][2*wp]+phoEt*_phoIsoEndcapCut2012[isoType][2*wp+1]);
  else if (year==2011) return (_phoIsoCut2011[isoType][0]+phoEt*_phoIsoCut2011[isoType][1]);
  return _phoIsoEndcapCut2012[isoType][2*wp+1]+phoEt*_phoIsoEndcapCut2012[isoType][2*wp+1]; 
}

TCut TPhotonCuts::RangeBarrel()
{
  TString str="phoSCEta<1.4442 && phoSCEta>-1.4442";
  TCut cut(str);
  return cut;
}

TCut TPhotonCuts::RangeEndcap()
{
  TString str1 = "phoSCEta > 1.566 && phoSCEta < 2.5";
  TString str2 = "phoSCEta <-1.566 && phoSCEta >-2.5";
  TCut cut1(str1);
  TCut cut2(str2);
  TCut cut = cut1 || cut2;
  return cut;
}

TCut TPhotonCuts::RangeSigmaIEtaIEta(int year, int wp)
{
  float sigmaCutB;
  float sigmaCutE;
  if (year==2011){
    sigmaCutB=_phoSigmaIEtaIEtaBarrelCut2011;
    sigmaCutE=_phoSigmaIEtaIEtaEndcapCut2011;
  }
  else if (year==2012){
    sigmaCutB=_phoSigmaIEtaIEtaBarrelCut2012[wp];
    sigmaCutE=_phoSigmaIEtaIEtaEndcapCut2012[wp];
  }
  else{
    std::cout<<"ERROR in TPhotonCuts::RangeSigmaIEtaIEta: year=="<<year<<", but cuts are known only for year==2011 and year==2012"<<std::endl;
  }

  TString cutBStr="phoSigmaIEtaIEta<=";
  cutBStr+=sigmaCutB;
  cutBStr+="&& phoSigmaIEtaIEta>=";
  cutBStr+=_phoSigmaIEtaIEtaBarrelCutLeft;
  TCut cutB(cutBStr);
  TString cutEStr="phoSigmaIEtaIEta<=";
  cutEStr+=sigmaCutE;
  cutEStr+="&& phoSigmaIEtaIEta>=";
  cutEStr+=_phoSigmaIEtaIEtaEndcapCutLeft;
  TCut cutE(cutEStr);
  TCut cut = (cutB && RangeBarrel()) || (cutE && RangeEndcap());
  return cut;
}

TCut TPhotonCuts::SidebandSigmaIEtaIEta()
{
  TString cutBStr="phoSigmaIEtaIEta";
  cutBStr+=">=";
  cutBStr+=_phoSigmaIEtaIEtaBarrelSideband;
  TCut cutB(cutBStr);
  TString cutEStr="phoSigmaIEtaIEta";
  cutEStr+=">=";
  cutEStr+=_phoSigmaIEtaIEtaEndcapSideband;
  TCut cutE(cutEStr);
  TCut cut = (cutB && RangeBarrel()) || (cutE && RangeEndcap());
  return cut;
}

TCut TPhotonCuts::RangeOneIsolation(int year, int wp, int isoType)
{
 // if (isoType==ISO_CHorTRK) return "1";
  TString strIso;
  if (year==2011 && isoType==ISO_CHorTRK) strIso="phoTrkIsoHollowDR04Corr";
  if (year==2012 && isoType==ISO_CHorTRK) strIso="phoPFChIsoCorr";
  if (year==2011 && isoType==ISO_NEUorHCAL) strIso="phoHcalIsoDR04Corr";
  if (year==2012 && isoType==ISO_NEUorHCAL) strIso="phoPFNeuIsoCorr";
  if (year==2011 && isoType==ISO_PHOorECAL) strIso="phoEcalIsoDR04Corr";
  if (year==2012 && isoType==ISO_PHOorECAL) strIso="phoPFPhoIsoCorr";
  float cutABarrel;
  float cutBBarrel;
  float cutAEndcap;
  float cutBEndcap;
  if (year==2012){
    cutABarrel=_phoIsoBarrelCut2012[isoType][2*wp];
    cutBBarrel=_phoIsoBarrelCut2012[isoType][2*wp+1];
    cutAEndcap=_phoIsoEndcapCut2012[isoType][2*wp];
    cutBEndcap=_phoIsoEndcapCut2012[isoType][2*wp+1];
  }
  else if (year==2011){
    cutABarrel=_phoIsoCut2011[isoType][0];
    cutBBarrel=_phoIsoCut2011[isoType][1];
    cutAEndcap=_phoIsoCut2011[isoType][0];
    cutBEndcap=_phoIsoCut2011[isoType][1];
  }

  TCut cutB = RangeBarrel();
  TCut cutE = RangeEndcap();
  TString cutIsoBStr = strIso;
  cutIsoBStr+=" < ";
  cutIsoBStr+=cutABarrel;
  cutIsoBStr+=" + phoEt*";
  cutIsoBStr+=cutBBarrel;
  TCut cutIsoB(cutIsoBStr); 
  TString cutIsoEStr = strIso;
  cutIsoEStr+=" < ";
  cutIsoEStr+=cutAEndcap;
  cutIsoEStr+=" + phoEt*";
  cutIsoEStr+=cutBEndcap;
  TCut cutIsoE(cutIsoEStr); 
  TCut cut = (cutB && cutIsoBStr) || (cutE && cutIsoEStr);
  return cut;
}

TCut TPhotonCuts::RangeHoverE(int year)
{
  TString cutStr="";
  if (year==2012) cutStr="phoHoverE12<=";
  else if (year==2011) cutStr="phoHoverE<=";
  cutStr+=_phoHoverECut;
  TCut cut(cutStr);
  return cut;
}

TCut TPhotonCuts::RangePhoEleVeto()
{
  TCut cut="phoElectronVeto==0";
  return cut;
}

TCut TPhotonCuts::RangePhoHasPixelSeed()
{
  TCut cut="phohasPixelSeed==0";
  return cut;
}

TCut TPhotonCuts::RangePhoton(int year, int wp,  
           bool doSigmaIEtaIEtaCut, bool doChOrTrkIsoCut, 
           bool doNeuOrHcalIsoCut, bool doPhoOrEcalIsoCut, 
           bool doHoverECut, bool doElectronVetoCut)
{
  //doChOrTrkIsoCut/doNeuOrHcalIsoCut/doPhoOrEcalIsoCut
  //means do charged/neutral/photon isolation cut for 2012 year
  //      do trkHollow/Hcal/Ecal isolation cut for 2011
  TCut cut="1";
  if (doSigmaIEtaIEtaCut) cut = cut && RangeSigmaIEtaIEta(year, wp);
  if (doHoverECut) cut = cut && RangeHoverE(year);
  if (doElectronVetoCut) cut = cut && RangePhoEleVeto();
  if (year==2011) cut = cut && RangePhoHasPixelSeed();
  if (doChOrTrkIsoCut){
    cut = cut && RangeOneIsolation(year,wp,ISO_CHorTRK);
  }
  if (doNeuOrHcalIsoCut){
    cut = cut && RangeOneIsolation(year,wp,ISO_NEUorHCAL);
  }
  if (doPhoOrEcalIsoCut){
    cut = cut && RangeOneIsolation(year,wp,ISO_PHOorECAL);
  }
  return cut;
}

TCut TPhotonCuts::RangeGenTruePhoton()
{
//  TCut cutGamma="phoGenPID==22";
//  TCut cutMom="phoGenMomPID>=-24 && phoGenMomPID<=24";
//  TCut cutParentage="phoGenParentage & 4";
//  TCut cut = cutGamma && cutMom;
//  return cut;
  return "1";
}
TCut TPhotonCuts::RangeGenFakePhoton()
{
  return (!RangeGenTruePhoton());
}
