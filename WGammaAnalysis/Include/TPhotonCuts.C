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

TPhotonCuts::TPhotonCuts(    int phoEleVeto_ipho,
                    float phoEt_ipho,
                    float phoEta_ipho,
                    float phoSCEt_ipho,
                    float phoSCEta_ipho,
                    float phoHoverE12_ipho,
                    float phoSigmaIEtaIEta_ipho,
                    float phoChIso_ipho,
                    float phoNeuIso_ipho,
                    float phoPhoIso_ipho,
                    float rho2012)
{
  phoEleVeto_ipho_=phoEleVeto_ipho;
  phoEt_ipho_=phoEt_ipho;
  phoEta_ipho_=phoEta_ipho;
  phoSCEt_ipho_=phoSCEt_ipho;
  phoSCEta_ipho_=phoSCEta_ipho;
  phoHoverE12_ipho_=phoHoverE12_ipho;
  phoSigmaIEtaIEta_ipho_=phoSigmaIEtaIEta_ipho;
  phoChIso_ipho_=phoChIso_ipho;
  phoNeuIso_ipho_=phoNeuIso_ipho;
  phoPhoIso_ipho_=phoPhoIso_ipho;
  rho2012_=rho2012;

  //calculation of NeuIso and PhoIso cuts:
  for (int wp=0; wp<3; wp++)
    {
      phoNeuIsoBarrelCut_[wp]=phoNeuIsoBarrelCutA_[wp]+phoSCEt_ipho_*phoNeuIsoBarrelCutB_[wp];
      phoNeuIsoEndcapCut_[wp]=phoNeuIsoEndcapCutA_[wp]+phoSCEt_ipho_*phoNeuIsoEndcapCutB_[wp];
      phoPhoIsoBarrelCut_[wp]=phoPhoIsoBarrelCutA_[wp]+phoSCEt_ipho_*phoPhoIsoBarrelCutB_[wp];
      phoPhoIsoEndcapCut_[wp]=phoPhoIsoEndcapCutA_[wp]+phoSCEt_ipho_*phoPhoIsoEndcapCutB_[wp];     
    }
}

TPhotonCuts::~TPhotonCuts()
{
}

bool TPhotonCuts::Passed(int wp, bool doSigmaIEtaIEtaCut, bool doPhoChIsoCut)
{
  if (!PhoKinematics()) return false;
  if (!SimpleCutBasedPhotonID2012(wp,doSigmaIEtaIEtaCut,doPhoChIsoCut)) return false;
  return true;
}

bool TPhotonCuts::PassedExceptKinematics(int wp, bool doSigmaIEtaIEtaCut, bool doPhoChIsoCut)
{
  if (!SimpleCutBasedPhotonID2012(wp,doSigmaIEtaIEtaCut,doPhoChIsoCut)) return false;
  return true;
}

bool TPhotonCuts::PassedKinematics(float phoPt, float phoEta)
{
  if (phoPt<phoPtCut_) return false;
  if (!IsBarrel(phoEta) && !IsEndcap(phoEta)) return false;
  return true;
}

bool TPhotonCuts::PhoKinematics()
{
  return PassedKinematics(phoSCEt_ipho_, phoSCEta_ipho_);
}

bool TPhotonCuts::SimpleCutBasedPhotonID2012(int wp, bool doSigmaIEtaIEtaCut, bool doPhoChIsoCut)
{
  if (phoEleVeto_ipho_) return false;
  if (!IsBarrel() && !IsEndcap()) return false;
  float isoCorr;
  if (IsBarrel())
    {
      if (phoHoverE12_ipho_>_phoHoverE12Cut) return false;
      isoCorr = IsoCorr(phoNeuIso_ipho_, rho2012_, EffAreaNeutral());
      if (isoCorr>phoNeuIsoBarrelCut_[wp]) return false;
      isoCorr = IsoCorr(phoPhoIso_ipho_, rho2012_, EffAreaPhotons());
      if (isoCorr>phoPhoIsoBarrelCut_[wp]) return false;
    }
  if (IsEndcap())
    {
      if (phoHoverE12_ipho_>_phoHoverE12Cut) return false;
      isoCorr = IsoCorr(phoNeuIso_ipho_, rho2012_, EffAreaNeutral());
      if (phoNeuIso_ipho_>phoNeuIsoEndcapCut_[wp]) return false;
      isoCorr = IsoCorr(phoPhoIso_ipho_, rho2012_, EffAreaPhotons());
      if (phoPhoIso_ipho_>phoPhoIsoEndcapCut_[wp]) return false;
    }

  if (doSigmaIEtaIEtaCut && !CutSigmaIEtaIEta(wp)) return false;
  if (doPhoChIsoCut && !CutPhoChIso(wp)) return false;
  return true;
}

bool TPhotonCuts::CutPhoChIso(int wp, float phoChIso, float rho2012, float eta)
{
  float isoCorr = IsoCorr(phoChIso, rho2012, EffAreaCharged(eta));
  if (IsBarrel(eta))
    if (isoCorr<=phoChIsoBarrelCut_[wp]) return true;
  else if (IsEndcap(eta))
    if (isoCorr<=phoChIsoEndcapCut_[wp]) return true;
  return false;
}

bool TPhotonCuts::CutPhoChIso(int wp)
{
  return CutPhoChIso(wp, phoChIso_ipho_, rho2012_,phoEta_ipho_);
}

bool TPhotonCuts::CutSigmaIEtaIEta(int wp, float eta, float sigmaIEtaIEta)
{
  if ( IsEndcap(eta) && (sigmaIEtaIEta<=phoSigmaIEtaIEtaEndcapCut_[wp]) 
       && sigmaIEtaIEta>=phoSigmaIEtaIEtaEndcapCutLeft_) 
    return true;
  if ( IsBarrel(eta) && (sigmaIEtaIEta<=phoSigmaIEtaIEtaBarrelCut_[wp]) 
       && sigmaIEtaIEta>=phoSigmaIEtaIEtaBarrelCutLeft_) 
    return true;
  return false;
}

bool TPhotonCuts::CutSigmaIEtaIEta(int wp)
{
  return CutSigmaIEtaIEta(wp, phoSCEta_ipho_,phoSigmaIEtaIEta_ipho_);
}

float TPhotonCuts::EffAreaCharged()
{
  return EffAreaCharged(phoEta_ipho_);
}

float TPhotonCuts::EffAreaNeutral()
{
  return EffAreaNeutral(phoEta_ipho_);
}

float TPhotonCuts::EffAreaPhotons()
{
  return EffAreaPhotons(phoEta_ipho_);
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
  if (fabs(phoEta)<1.442) return true;
  return false;
}

bool TPhotonCuts::IsEndcap(float phoEta)
{
  if (fabs(phoEta)>1.566 && fabs(phoEta)<2.5) return true;
  return false;
}

bool TPhotonCuts::IsBarrel()
{
    return IsBarrel(phoSCEta_ipho_);
}

bool TPhotonCuts::IsEndcap()
{
    return IsEndcap(phoSCEta_ipho_);
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

int TPhotonCuts::GetWP()
{
  return WP_;
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
  TString str="phoSCEta<1.442 && phoSCEta>-1.442";
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
