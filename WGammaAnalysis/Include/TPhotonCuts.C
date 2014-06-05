#include "TPhotonCuts.h"
  //this class
#include "math.h"
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

bool TPhotonCuts::PassedKinematics(float phoPt, float phoEta)
{
  if (phoPt<_phoPtCut) return false;
  if (!IsBarrel(phoEta) && !IsEndcap(phoEta)) return false;
  return true;
}

float TPhotonCuts::EffAreaCharged(float eta)
{
  if (fabs(eta)<1.0)   return 0.012;
  if (fabs(eta)<1.479) return 0.010;
  if (fabs(eta)<2.0)   return 0.014;
  if (fabs(eta)<2.2)   return 0.012;
  if (fabs(eta)<2.3)   return 0.016;
  if (fabs(eta)<2.4)   return 0.020;
                       return 0.012;
}

float TPhotonCuts::EffAreaNeutral(float eta)
{
  if (fabs(eta)<1.0)   return 0.030;
  if (fabs(eta)<1.479) return 0.057;
  if (fabs(eta)<2.0)   return 0.039;
  if (fabs(eta)<2.2)   return 0.015;
  if (fabs(eta)<2.3)   return 0.024;
  if (fabs(eta)<2.4)   return 0.039;
                                return 0.072;
}
float TPhotonCuts::EffAreaPhotons(float eta)
{
  if (fabs(eta)<1.0)   return 0.148;
  if (fabs(eta)<1.479) return 0.130;
  if (fabs(eta)<2.0)   return 0.112;
  if (fabs(eta)<2.2)   return 0.216;
  if (fabs(eta)<2.3)   return 0.262;
  if (fabs(eta)<2.4)   return 0.260;
                                return 0.266;
}


float TPhotonCuts::IsoCorr(float iso, float rho, float EA)
{
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
/*
bool TPhotonCuts::IsBarrel()
{
    return IsBarrel(phoSCEta_ipho_);
}

bool TPhotonCuts::IsEndcap()
{
    return IsEndcap(phoSCEta_ipho_);
}
*/
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

float TPhotonCuts::GetPhoSigmaIEtaIEtaCutB(int wp)
{
  return phoSigmaIEtaIEtaBarrelCut_[wp];
}

float TPhotonCuts::GetPhoSigmaIEtaIEtaCutE(int wp)
{
  return phoSigmaIEtaIEtaEndcapCut_[wp];
}

float TPhotonCuts::GetPhoSigmaIEtaIEtaCutLeftB()
{
  return phoSigmaIEtaIEtaBarrelCutLeft_;
}

float TPhotonCuts::GetPhoSigmaIEtaIEtaCutLeftE()
{
  return phoSigmaIEtaIEtaEndcapCutLeft_;
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

TCut TPhotonCuts::RangeSigmaIEtaIEta(int wp)
{
  TString cutBStr="phoSigmaIEtaIEta<=";
  cutBStr+=phoSigmaIEtaIEtaBarrelCut_[wp];
  cutBStr+="&& phoSigmaIEtaIEta>=";
  cutBStr+=phoSigmaIEtaIEtaBarrelCutLeft_;
  TCut cutB(cutBStr);
  TString cutEStr="phoSigmaIEtaIEta<=";
  cutEStr+=phoSigmaIEtaIEtaEndcapCut_[wp];
  cutEStr+="&& phoSigmaIEtaIEta>=";
  cutEStr+=phoSigmaIEtaIEtaEndcapCutLeft_;
  TCut cutE(cutEStr);
  TCut cut = (cutB && RangeBarrel()) || (cutE && RangeEndcap());
  return cut;
}

TCut TPhotonCuts::SidebandSigmaIEtaIEta()
{
  TString cutBStr="phoSigmaIEtaIEta";
  cutBStr+=">=";
  cutBStr+=phoSigmaIEtaIEtaBarrelSideband_;
  TCut cutB(cutBStr);
  TString cutEStr="phoSigmaIEtaIEta";
  cutEStr+=">=";
  cutEStr+=phoSigmaIEtaIEtaEndcapSideband_;
  TCut cutE(cutEStr);
  TCut cut = (cutB && RangeBarrel()) || (cutE && RangeEndcap());
  return cut;
}


TCut TPhotonCuts::RangePhoChIso(TString strIso, int wp)
//strIso = "phoSCRChIsoCorr" or "phoPFChIsoCorr
{
  TCut cutB = RangeBarrel();
  TCut cutE = RangeEndcap();
  TString cutIsoBStr = strIso;
  cutIsoBStr+=" <= ";
  cutIsoBStr+=phoChIsoBarrelCut_[wp];
  TCut cutIsoB(cutIsoBStr); 
  TString cutIsoEStr = strIso;
  cutIsoEStr+=" <= ";
  cutIsoEStr+=phoChIsoEndcapCut_[wp];
  TCut cutIsoE(cutIsoEStr); 
  TCut cut = (cutB && cutIsoBStr) || (cutE && cutIsoEStr);
  return cut;
}


TCut TPhotonCuts::SidebandPhoChIso(TString strIso)
//strIso = "phoSCRChIsoCorr" or "phoPFChIsoCorr
{
  TCut cutB = RangeBarrel();
  TCut cutE = RangeEndcap();
  TString cutIsoBStr = strIso;
  cutIsoBStr+=" >= ";
  cutIsoBStr+=phoChIsoBarrelSideband_;
  TCut cutIsoB(cutIsoBStr); 
  TString cutIsoEStr = strIso;
  cutIsoEStr+=" >= ";
  cutIsoEStr+=phoChIsoEndcapSideband_;
  TCut cutIsoE(cutIsoEStr); 
  TCut cut = (cutB && cutIsoBStr) || (cutE && cutIsoEStr);
  return cut;
}

TCut TPhotonCuts::RangePhoNeuIso(TString strIso, int wp)
//strChIso = "phoSCRChNeuCorr" or "phoPFNeuIsoCorr
{
  TCut cutB = RangeBarrel();
  TCut cutE = RangeEndcap();
  TString cutIsoBStr = strIso;
  cutIsoBStr+=" <= ";
  cutIsoBStr+=phoNeuIsoBarrelCutA_[wp];
  cutIsoBStr+=" + phoSCEt*";
  cutIsoBStr+=phoNeuIsoBarrelCutB_[wp];
  TCut cutIsoB(cutIsoBStr); 
  TString cutIsoEStr = strIso;
  cutIsoEStr+=" <= ";
  cutIsoEStr+=phoNeuIsoEndcapCutA_[wp];
  cutIsoEStr+=" + phoSCEt*";
  cutIsoEStr+=phoNeuIsoEndcapCutB_[wp];
  TCut cutIsoE(cutIsoEStr); 
  TCut cut = (cutB && cutIsoBStr) || (cutE && cutIsoEStr);
  return cut;
}

TCut TPhotonCuts::RangePhoPhoIso(TString strIso, int wp)
//strIso = "phoSCRPhoIsoCorr" or "phoPFPhoIsoCorr
{
  TCut cutB = RangeBarrel();
  TCut cutE = RangeEndcap();
  TString cutIsoBStr = strIso;
  cutIsoBStr+=" <= ";
  cutIsoBStr+=phoPhoIsoBarrelCutA_[wp];
  cutIsoBStr+=" + phoSCEt*";
  cutIsoBStr+=phoPhoIsoBarrelCutB_[wp];
  TCut cutIsoB(cutIsoBStr); 
  TString cutIsoEStr = strIso;
  cutIsoEStr+=" <= ";
  cutIsoEStr+=phoPhoIsoEndcapCutA_[wp];
  cutIsoEStr+=" + phoSCEt*";
  cutIsoEStr+=phoPhoIsoEndcapCutB_[wp];
  TCut cutIsoE(cutIsoEStr); 
  TCut cut = (cutB && cutIsoBStr) || (cutE && cutIsoEStr);
  return cut;
}

TCut TPhotonCuts::RangeHoverE12()
{
  TString cutStr="phoHoverE12<=";
  cutStr+=_phoHoverE12Cut;
  TCut cut(cutStr);
  return cut;
}

TCut TPhotonCuts::RangePhoEleVeto()
{
  TCut cut="phoElectronVeto==0";
  return cut;
}

TCut TPhotonCuts::RangePhoton(int wp, TString isoBase, 
           bool doSigmaIEtaIEtaCut, bool doChIsoCut, 
           bool doNeuIsoCut, bool doPhoIsoCut, 
           bool doHoverE12Cut, bool doElectronVetoCut)
{
  TCut cut="1";
  if (doSigmaIEtaIEtaCut) cut = cut && RangeSigmaIEtaIEta(wp);
  if (doHoverE12Cut) cut = cut && RangeHoverE12();
  if (doElectronVetoCut) cut = cut && RangePhoEleVeto();
  if (doChIsoCut){
    TString strIso="pho"+isoBase+"ChIsoCorr";
    cut = cut && RangePhoChIso(strIso,wp);
  }
  if (doNeuIsoCut){
    TString strIso="pho"+isoBase+"NeuIsoCorr";
    cut = cut && RangePhoNeuIso(strIso,wp);
  }
  if (doPhoIsoCut){
    TString strIso="pho"+isoBase+"PhoIsoCorr";
    cut = cut && RangePhoPhoIso(strIso,wp);
  }
  return cut;
}

TCut TPhotonCuts::RangeGenTruePhoton()
{
  TCut cutGamma="phoGenPID==22";
  TCut cutMom="phoGenMomPID>=-24 && phoGenMomPID<=24";
  TCut cut = cutGamma && cutMom;
  return cut;
}
TCut TPhotonCuts::RangeGenFakePhoton()
{
  return (!RangeGenTruePhoton());
}
