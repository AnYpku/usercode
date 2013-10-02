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
                    float phoPFChIso_ipho,
                    float phoPFNeuIso_ipho,
                    float phoPFPhoIso_ipho,
                    float rho2012)
{
  phoEleVeto_ipho_=phoEleVeto_ipho;
  phoEt_ipho_=phoEt_ipho;
  phoEta_ipho_=phoEta_ipho;
  phoSCEt_ipho_=phoSCEt_ipho;
  phoSCEta_ipho_=phoSCEta_ipho;
  phoHoverE12_ipho_=phoHoverE12_ipho;
  phoSigmaIEtaIEta_ipho_=phoSigmaIEtaIEta_ipho;
  phoPFChIso_ipho_=phoPFChIso_ipho;
  phoPFNeuIso_ipho_=phoPFNeuIso_ipho;
  phoPFPhoIso_ipho_=phoPFPhoIso_ipho;
  rho2012_=rho2012;

  //calculation of NeuIso and PhoIso cuts:
  for (int wp=0; wp<3; wp++)
    {
      phoPFNeuIsoBarrelCut_[wp]=phoPFNeuIsoBarrelCutA_[wp]+phoSCEt_ipho_*phoPFNeuIsoBarrelCutB_[wp];
      phoPFNeuIsoEndcapCut_[wp]=phoPFNeuIsoEndcapCutA_[wp]+phoSCEt_ipho_*phoPFNeuIsoEndcapCutB_[wp];
      phoPFPhoIsoBarrelCut_[wp]=phoPFPhoIsoBarrelCutA_[wp]+phoSCEt_ipho_*phoPFPhoIsoBarrelCutB_[wp];
      phoPFPhoIsoEndcapCut_[wp]=phoPFPhoIsoEndcapCutA_[wp]+phoSCEt_ipho_*phoPFPhoIsoEndcapCutB_[wp];     
    }
}

TPhotonCuts::~TPhotonCuts()
{
}

bool TPhotonCuts::Passed(bool doSigmaIEtaIEtaCut, bool doPhoChIsoCut)
{
  if (!PhoKinematics()) return false;
  if (!SimpleCutBasedPhotonID2012(doSigmaIEtaIEtaCut,doPhoChIsoCut)) return false;
  return true;
}

bool TPhotonCuts::PassedExceptKinematics(bool doSigmaIEtaIEtaCut, bool doPhoChIsoCut)
{
  if (!SimpleCutBasedPhotonID2012(doSigmaIEtaIEtaCut,doPhoChIsoCut)) return false;
  return true;
}

bool TPhotonCuts::PhoKinematics(float phoPt, float phoEta)
{
  if (phoPt<phoPtCut_) return false;
  if (!IsBarrel(phoEta) && !IsEndcap(phoEta)) return false;
  return true;
}

bool TPhotonCuts::PhoKinematics()
{
  return PhoKinematics(phoSCEt_ipho_, phoSCEta_ipho_);
}

bool TPhotonCuts::SimpleCutBasedPhotonID2012(bool doSigmaIEtaIEtaCut, bool doPhoChIsoCut)
{
  if (phoEleVeto_ipho_) return false;
  if (!IsBarrel() && !IsEndcap()) return false;
  float isoCorr;
  if (IsBarrel())
    {
      if (phoHoverE12_ipho_>phoHoverE12BarrelCut_[WP_]) return false;
      isoCorr = PFIsoCorr(phoPFNeuIso_ipho_, rho2012_, EffAreaNeutral());
      if (isoCorr>phoPFNeuIsoBarrelCut_[WP_]) return false;
      isoCorr = PFIsoCorr(phoPFPhoIso_ipho_, rho2012_, EffAreaPhotons());
      if (isoCorr>phoPFPhoIsoBarrelCut_[WP_]) return false;
    }
  if (IsEndcap())
    {
      if (phoHoverE12_ipho_>phoHoverE12EndcapCut_[WP_]) return false;
      isoCorr = PFIsoCorr(phoPFNeuIso_ipho_, rho2012_, EffAreaNeutral());
      if (phoPFNeuIso_ipho_>phoPFNeuIsoEndcapCut_[WP_]) return false;
      isoCorr = PFIsoCorr(phoPFPhoIso_ipho_, rho2012_, EffAreaPhotons());
      if (phoPFPhoIso_ipho_>phoPFPhoIsoEndcapCut_[WP_]) return false;
    }

  if (doSigmaIEtaIEtaCut && !CutSigmaIEtaIEta()) return false;
  if (doPhoChIsoCut && !CutPhoChIso()) return false;
  return true;
}

bool TPhotonCuts::CutPhoChIso(float phoChIso, float rho2012, float eta)
{
  float isoCorr = PFIsoCorr(phoChIso, rho2012, EffAreaCharged(eta));
  if (IsBarrel(eta))
    if (isoCorr<=phoPFChIsoBarrelCut_[WP_]) return true;
  else if (IsEndcap(eta))
    if (isoCorr<=phoPFChIsoEndcapCut_[WP_]) return true;
  return false;
}

TCut TPhotonCuts::CutPhoChIso(TString phoChIso, TString eta)
{
  TCut cutB = IsBarrel(eta);
  TCut cutE = IsEndcap(eta);
  TString cutIsoBStr = phoChIso+" <= ";
  cutIsoBStr+=phoPFChIsoBarrelCut_[WP_];
  TCut cutIsoB(cutIsoBStr); 
  TString cutIsoEStr = phoChIso+" <= ";
  cutIsoEStr+=phoPFChIsoEndcapCut_[WP_];
  TCut cutIsoE(cutIsoEStr); 
  TCut cut = (cutB && cutIsoBStr) || (cutE && cutIsoEStr);
  return cut;
}

bool TPhotonCuts::CutPhoChIso()
{
  return CutPhoChIso(phoPFChIso_ipho_, rho2012_,phoEta_ipho_);
}

bool TPhotonCuts::CutSigmaIEtaIEta(float eta, float sigmaIEtaIEta)
{
  if ( IsEndcap(eta) && (sigmaIEtaIEta<=phoSigmaIEtaIEtaEndcapCut_[WP_]) ) 
    return true;
  if ( IsBarrel(eta) && (sigmaIEtaIEta<=phoSigmaIEtaIEtaBarrelCut_[WP_]) ) 
    return true;
  return false;
}

TCut TPhotonCuts::CutSigmaIEtaIEta(TString sigmaIEtaIEta, TString eta)
{
  TString cutBStr=sigmaIEtaIEta;
  cutBStr+="<=";
  cutBStr+=phoSigmaIEtaIEtaBarrelCut_[WP_];
  TCut cutB(cutBStr);
  TString cutEStr=sigmaIEtaIEta;
  cutEStr+="<=";
  cutEStr+=phoSigmaIEtaIEtaEndcapCut_[WP_];
  TCut cutE(cutEStr);
  TCut cut = (cutB && IsBarrel(eta)) || (cutE && IsEndcap(eta));
  return cut;
}

bool TPhotonCuts::CutSigmaIEtaIEta()
{
  return CutSigmaIEtaIEta(phoSCEta_ipho_,phoSigmaIEtaIEta_ipho_);
}

float TPhotonCuts::GetPhoPFChIsoCorr(float phoChIso, float rho2012, float eta)
{
  return PFIsoCorr(phoChIso, rho2012, EffAreaCharged(eta));
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
float TPhotonCuts::EffAreaCharged()
{
  return EffAreaCharged(phoEta_ipho_);
}
float TPhotonCuts::EffAreaNeutral()
{
  if (fabs(phoEta_ipho_)<1.0)   return 0.030;
  if (fabs(phoEta_ipho_)<1.479) return 0.057;
  if (fabs(phoEta_ipho_)<2.0)   return 0.039;
  if (fabs(phoEta_ipho_)<2.2)   return 0.015;
  if (fabs(phoEta_ipho_)<2.3)   return 0.024;
  if (fabs(phoEta_ipho_)<2.4)   return 0.039;
                                return 0.072;
}
float TPhotonCuts::EffAreaPhotons()
{
  if (fabs(phoEta_ipho_)<1.0)   return 0.148;
  if (fabs(phoEta_ipho_)<1.479) return 0.130;
  if (fabs(phoEta_ipho_)<2.0)   return 0.112;
  if (fabs(phoEta_ipho_)<2.2)   return 0.216;
  if (fabs(phoEta_ipho_)<2.3)   return 0.262;
  if (fabs(phoEta_ipho_)<2.4)   return 0.260;
                                return 0.266;
}


float TPhotonCuts::PFIsoCorr(float PFIso, float rho, float EA)
{
  if (PFIso-rho*EA>0.0) return PFIso-rho*EA;
  return 0.0;
}

bool TPhotonCuts::IsBarrel(float phoEta)
{
  if (fabs(phoEta)<1.442) return true;
  return false;
}

bool TPhotonCuts::IsEndcap(float phoEta)
{
  if (fabs(phoEta)>1.566 && fabs(phoSCEta_ipho_)<2.5) return true;
  return false;
}

TCut TPhotonCuts::IsBarrel(TString phoEtaStr)
{
  TString str=phoEtaStr+"<1.442 && "+phoEtaStr+">-1.442";
  TCut cut(str);
  return cut;
}

TCut TPhotonCuts::IsEndcap(TString phoEtaStr)
{
  TString str1 = phoEtaStr+"> 1.566 && "+phoEtaStr+"< 2.5";
  TString str2 = phoEtaStr+"<-1.566 && "+phoEtaStr+">-2.5";
  TCut cut1(str1);
  TCut cut2(str2);
  TCut cut = cut1 || cut2;
  return cut;
}

bool TPhotonCuts::IsBarrel()
{
    return IsBarrel(phoSCEta_ipho_);
}

bool TPhotonCuts::IsEndcap()
{
    return IsEndcap(phoSCEta_ipho_);
}
