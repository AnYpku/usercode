#include "TPhotonScaleFactor.h"
  //this class
#include "math.h"
#include "string.h"
#include <iostream>
  //standard C++ class
#include "../Configuration/TConfiguration.h"
#include "TPhotonCuts.h"
  //this package
#include "TFile.h"
#include "TH1D.h"
#include "TString.h"
 //ROOT

TPhotonScaleFactor::TPhotonScaleFactor()
{

  TConfiguration config;
  fSFPt15to20_ = new TFile ((config.GetPhotonScaleFactorsFileNamePt15to20GeV()).c_str());
  fSFPt20toInf_ = new TFile ((config.GetPhotonScaleFactorsFileNamePt20toInfGeV()).c_str());

  hSFPt15to20Loose_Barrel1_ = (TH1D*)fSFPt15to20_->Get("Loose_Barrel1");
  hSFPt15to20Loose_Endcap1_ = (TH1D*)fSFPt15to20_->Get("Loose_Endcap1");
  hSFPt15to20Medium_Barrel1_ = (TH1D*)fSFPt15to20_->Get("Medium_Barrel1");
  hSFPt15to20Medium_Endcap1_ = (TH1D*)fSFPt15to20_->Get("Medium_Endcap1");
  hSFPt15to20Tight_Barrel1_ = (TH1D*)fSFPt15to20_->Get("Tight_Barrel1");
  hSFPt15to20Tight_Endcap1_ = (TH1D*)fSFPt15to20_->Get("Tight_Endcap1");

  hSFPt20toInfLoose_Barrel1_ = (TH1D*)fSFPt20toInf_->Get("Loose_Barrel1");
  hSFPt20toInfLoose_Endcap1_ = (TH1D*)fSFPt20toInf_->Get("Loose_Endcap1");
  hSFPt20toInfMedium_Barrel1_ = (TH1D*)fSFPt20toInf_->Get("Medium_Barrel1");
  hSFPt20toInfMedium_Endcap1_ = (TH1D*)fSFPt20toInf_->Get("Medium_Endcap1");
  hSFPt20toInfTight_Barrel1_ = (TH1D*)fSFPt20toInf_->Get("Tight_Barrel1");
  hSFPt20toInfTight_Endcap1_ = (TH1D*)fSFPt20toInf_->Get("Tight_Endcap1");

  hSFPt20toInfLoose_Barrel2_ = (TH1D*)fSFPt20toInf_->Get("Loose_Barrel2");
  hSFPt20toInfLoose_Endcap2_ = (TH1D*)fSFPt20toInf_->Get("Loose_Endcap2");
  hSFPt20toInfMedium_Barrel2_ = (TH1D*)fSFPt20toInf_->Get("Medium_Barrel2");
  hSFPt20toInfMedium_Endcap2_ = (TH1D*)fSFPt20toInf_->Get("Medium_Endcap2");
  hSFPt20toInfTight_Barrel2_ = (TH1D*)fSFPt20toInf_->Get("Tight_Barrel2");
  hSFPt20toInfTight_Endcap2_ = (TH1D*)fSFPt20toInf_->Get("Tight_Endcap2");

}

TPhotonScaleFactor::~TPhotonScaleFactor()
{
}

void TPhotonScaleFactor::SetScaleFactor(float pt, float eta, int WP)
{
  SetScaleFactorExceptElectronVeto(pt,eta,WP);
  SetScaleFactorElectronVeto(pt,eta,WP);
  scaleFactor_=scaleFactorExceptElectronVeto_*scaleFactorElectronVeto_;
  scaleFactorErr_=
    scaleFactorExceptElectronVeto_*scaleFactorExceptElectronVeto_*
    scaleFactorElectronVetoErr_*scaleFactorElectronVetoErr_ +
    scaleFactorExceptElectronVetoErr_*scaleFactorExceptElectronVetoErr_*
    scaleFactorElectronVeto_*scaleFactorElectronVeto_;
  scaleFactorErr_=sqrt(scaleFactorErr_);
}

float TPhotonScaleFactor::GetScaleFactor()
{
  return scaleFactor_;
}

float TPhotonScaleFactor::GetScaleFactorErr()
{
  return scaleFactorErr_;
}

float TPhotonScaleFactor::GetScaleFactorExceptElectronVeto()
{
  return scaleFactorExceptElectronVeto_;
}

float TPhotonScaleFactor::GetScaleFactorExceptElectronVetoErr()
{
  return scaleFactorExceptElectronVetoErr_;
}

float TPhotonScaleFactor::GetScaleFactorElectronVeto()
{
  return scaleFactorElectronVeto_;
}

float TPhotonScaleFactor::GetScaleFactorElectronVetoErr()
{
  return scaleFactorElectronVetoErr_;
}

void TPhotonScaleFactor::SetScaleFactorExceptElectronVeto(float pt, float eta, int WP)
{
  if (pt<=15){
    scaleFactorExceptElectronVeto_=1.0;
    scaleFactorExceptElectronVetoErr_=0.0;
  }
  else if (pt>15 && pt<20){
    SetScaleFactorExceptElectronVetoPt15to20(pt, eta, WP);
  }
  else if (pt>=20){
    SetScaleFactorExceptElectronVetoPt20toInf(pt, eta, WP);
  }
}

void TPhotonScaleFactor::SetScaleFactorExceptElectronVetoPt15to20(float pt, float eta, int WP)
{
    if (emptyPhoton_.IsBarrel(eta)){
      if      (WP==emptyPhoton_.WP_LOOSE)
        SetScaleFactorExceptElectronVetoKnownHist(hSFPt15to20Loose_Barrel1_,pt);
      else if (WP==emptyPhoton_.WP_MEDIUM)
        SetScaleFactorExceptElectronVetoKnownHist(hSFPt15to20Medium_Barrel1_,pt);
      else if (WP==emptyPhoton_.WP_TIGHT)
        SetScaleFactorExceptElectronVetoKnownHist(hSFPt15to20Tight_Barrel1_,pt);
    }
    if (emptyPhoton_.IsEndcap(eta)){
      if      (WP==emptyPhoton_.WP_LOOSE)
        SetScaleFactorExceptElectronVetoKnownHist(hSFPt15to20Loose_Endcap1_,pt);
      else if (WP==emptyPhoton_.WP_MEDIUM)
        SetScaleFactorExceptElectronVetoKnownHist(hSFPt15to20Medium_Endcap1_,pt);
      else if (WP==emptyPhoton_.WP_TIGHT)
        SetScaleFactorExceptElectronVetoKnownHist(hSFPt15to20Tight_Endcap1_,pt);
    }
}

void TPhotonScaleFactor::SetScaleFactorExceptElectronVetoPt20toInf(float pt, float eta, int WP)
{
    scaleFactorExceptElectronVeto_=1.0;
    scaleFactorExceptElectronVetoErr_=0.0;
    if (emptyPhoton_.IsBarrel(eta) && fabs(eta)<0.8){
      if      (WP==emptyPhoton_.WP_LOOSE)
        SetScaleFactorExceptElectronVetoKnownHist(hSFPt20toInfLoose_Barrel1_,pt);
      else if (WP==emptyPhoton_.WP_MEDIUM)
        SetScaleFactorExceptElectronVetoKnownHist(hSFPt20toInfMedium_Barrel1_,pt);
      else if (WP==emptyPhoton_.WP_TIGHT)
        SetScaleFactorExceptElectronVetoKnownHist(hSFPt20toInfTight_Barrel1_,pt);
    }
    else if (emptyPhoton_.IsBarrel(eta) && fabs(eta)>=0.8){
      if      (WP==emptyPhoton_.WP_LOOSE)
        SetScaleFactorExceptElectronVetoKnownHist(hSFPt20toInfLoose_Barrel2_,pt);
      else if (WP==emptyPhoton_.WP_MEDIUM)
        SetScaleFactorExceptElectronVetoKnownHist(hSFPt20toInfMedium_Barrel2_,pt);
      else if (WP==emptyPhoton_.WP_TIGHT)
        SetScaleFactorExceptElectronVetoKnownHist(hSFPt20toInfTight_Barrel2_,pt);
    }
    else if (emptyPhoton_.IsEndcap(eta) && fabs(eta)<2.0){
      if      (WP==emptyPhoton_.WP_LOOSE)
        SetScaleFactorExceptElectronVetoKnownHist(hSFPt20toInfLoose_Endcap1_,pt);
      else if (WP==emptyPhoton_.WP_MEDIUM)
        SetScaleFactorExceptElectronVetoKnownHist(hSFPt20toInfMedium_Endcap1_,pt);
      else if (WP==emptyPhoton_.WP_TIGHT)
        SetScaleFactorExceptElectronVetoKnownHist(hSFPt20toInfTight_Endcap1_,pt);
    }
    else if (emptyPhoton_.IsEndcap(eta) && fabs(eta)>=2.0){
      if      (WP==emptyPhoton_.WP_LOOSE)
        SetScaleFactorExceptElectronVetoKnownHist(hSFPt20toInfLoose_Endcap2_,pt);
      else if (WP==emptyPhoton_.WP_MEDIUM)
        SetScaleFactorExceptElectronVetoKnownHist(hSFPt20toInfMedium_Endcap2_,pt);
      else if (WP==emptyPhoton_.WP_TIGHT)
        SetScaleFactorExceptElectronVetoKnownHist(hSFPt20toInfTight_Endcap2_,pt);
    }
}

void TPhotonScaleFactor::SetScaleFactorExceptElectronVetoKnownHist(TH1D *h, float pt)
{
   if (pt>=(h->GetBinCenter(h->GetNbinsX())+h->GetBinWidth(h->GetNbinsX()))){
     scaleFactorExceptElectronVeto_=h->GetBinContent(h->GetNbinsX());
     scaleFactorExceptElectronVetoErr_=h->GetBinError(h->GetNbinsX());
   }
   else if (pt<=h->GetBinLowEdge(1)){
     scaleFactorExceptElectronVeto_=1.0;
     scaleFactorExceptElectronVetoErr_=0.0;
   }
   else {
     scaleFactorExceptElectronVeto_=h->GetBinContent(h->FindBin(pt));
     scaleFactorExceptElectronVetoErr_=h->GetBinError(h->FindBin(pt));
   }
}

void TPhotonScaleFactor::SetScaleFactorElectronVeto(float pt, float eta, int WP)
{
  //from https://twiki.cern.ch/twiki/bin/view/CMS/CutBasedPhotonID2012#Simple_counting_efficiency_scale
  scaleFactorElectronVeto_=1.0;
  scaleFactorElectronVetoErr_=0.0;
  if      (emptyPhoton_.IsBarrel(eta))
    SetScaleFactorElectronVetoBarrel(pt, WP);
  else if (emptyPhoton_.IsEndcap(eta))
    SetScaleFactorElectronVetoEndcap(pt, WP);
}

void TPhotonScaleFactor::SetScaleFactorElectronVetoBarrel(float pt, int WP)
{
    if      (pt>15 && pt<20){
      if      (WP==emptyPhoton_.WP_LOOSE){
        scaleFactorElectronVeto_= 0.9910;
        scaleFactorElectronVetoErr_= 0.0040;
      }
      else if (WP==emptyPhoton_.WP_MEDIUM){
        scaleFactorElectronVeto_= 0.9936;
        scaleFactorElectronVetoErr_= 0.0041;
      }
      else if (WP==emptyPhoton_.WP_TIGHT){
        scaleFactorElectronVeto_= 0.9939;
        scaleFactorElectronVetoErr_= 0.0043;
      }
    }
    else if (pt>20 && pt<30){
      if      (WP==emptyPhoton_.WP_LOOSE){
        scaleFactorElectronVeto_= 0.9996;
        scaleFactorElectronVetoErr_= 0.0059;
      }
      else if (WP==emptyPhoton_.WP_MEDIUM){
        scaleFactorElectronVeto_= 1.0022;
        scaleFactorElectronVetoErr_= 0.0062;
      }
      else if (WP==emptyPhoton_.WP_TIGHT){
        scaleFactorElectronVeto_= 1.0032;
        scaleFactorElectronVetoErr_= 0.0066;
      }
    }
    else if (pt>30 && pt<40){
      if      (WP==emptyPhoton_.WP_LOOSE){
        scaleFactorElectronVeto_= 1.0081;
        scaleFactorElectronVetoErr_= 0.0154;
      }
      else if (WP==emptyPhoton_.WP_MEDIUM){
        scaleFactorElectronVeto_= 1.0148;
        scaleFactorElectronVetoErr_= 0.0153;
      }
      else if (WP==emptyPhoton_.WP_TIGHT){
        scaleFactorElectronVeto_= 0.9919;
        scaleFactorElectronVetoErr_= 0.0057;
      }
    }
    else if (pt>15){
      if      (WP==emptyPhoton_.WP_LOOSE){
        scaleFactorElectronVeto_= 0.9957;
        scaleFactorElectronVetoErr_= 0.0037;
      }
      else if (WP==emptyPhoton_.WP_MEDIUM){
        scaleFactorElectronVeto_= 0.9992;
        scaleFactorElectronVetoErr_= 0.0038;
      }
      else if (WP==emptyPhoton_.WP_TIGHT){
        scaleFactorElectronVeto_= 0.9969;
        scaleFactorElectronVetoErr_= 0.0035;
      }
   }
}

void TPhotonScaleFactor::SetScaleFactorElectronVetoEndcap(float pt, int WP)
{
    if      (pt>15 && pt<20){
      if      (WP==emptyPhoton_.WP_LOOSE){
        scaleFactorElectronVeto_= 0.9970;
        scaleFactorElectronVetoErr_= 0.0148;
      }
      else if (WP==emptyPhoton_.WP_MEDIUM){
        scaleFactorElectronVeto_= 1.0002;
        scaleFactorElectronVetoErr_= 0.0170;
      }
      else if (WP==emptyPhoton_.WP_TIGHT){
        scaleFactorElectronVeto_= 1.0046;
        scaleFactorElectronVetoErr_= 0.0178;
      }
    }
    else if (pt>20 && pt<30){
      if      (WP==emptyPhoton_.WP_LOOSE){
        scaleFactorElectronVeto_= 0.9911;
        scaleFactorElectronVetoErr_= 0.0169;
      }
      else if (WP==emptyPhoton_.WP_MEDIUM){
        scaleFactorElectronVeto_= 0.9912;
        scaleFactorElectronVetoErr_= 0.0188;
      }
      else if (WP==emptyPhoton_.WP_TIGHT){
        scaleFactorElectronVeto_= 0.9917;
        scaleFactorElectronVetoErr_= 0.0200;
      }
    }
    else if (pt>30 && pt<40){
      if      (WP==emptyPhoton_.WP_LOOSE){
        scaleFactorElectronVeto_= 0.9796;
        scaleFactorElectronVetoErr_= 0.0117;
      }
      else if (WP==emptyPhoton_.WP_MEDIUM){
        scaleFactorElectronVeto_= 0.9850;
        scaleFactorElectronVetoErr_= 0.0106;
      }
      else if (WP==emptyPhoton_.WP_TIGHT){
        scaleFactorElectronVeto_= 0.9837;
        scaleFactorElectronVetoErr_= 0.0114;
      }
    }
    else if (pt>15){
      if      (WP==emptyPhoton_.WP_LOOSE){
        scaleFactorElectronVeto_= 0.9919;
        scaleFactorElectronVetoErr_= 0.0094;
      }
      else if (WP==emptyPhoton_.WP_MEDIUM){
        scaleFactorElectronVeto_= 0.9939;
        scaleFactorElectronVetoErr_= 0.0106;
      }
      else if (WP==emptyPhoton_.WP_TIGHT){
        scaleFactorElectronVeto_= 0.9957;
        scaleFactorElectronVetoErr_= 0.0113;
      }
    }
}

TString TPhotonScaleFactor::GetPhotonScaleFactorHistName(float pt, float eta, int WP)
{

  TString histName="NONE";

  TString wpStr;//Loose, Medium or Tight
  TString etaStr;//Barrel or Endcap
  TString lastDigitStr;//1 or 2
  if      (WP==emptyPhoton_.WP_LOOSE)  wpStr="Loose";
  else if (WP==emptyPhoton_.WP_MEDIUM) wpStr="Medium";
  else if (WP==emptyPhoton_.WP_TIGHT)  wpStr="Tight";
  else return histName;

  if      (pt<15) return histName;
  else if (pt>=15 && pt<20){
    lastDigitStr="1";
    if (emptyPhoton_.IsBarrel(eta)) etaStr="Barrel";
    else if (emptyPhoton_.IsEndcap(eta)) etaStr="Endcap"; 
    else return histName;
  }
  else if (pt>=20){
    if (emptyPhoton_.IsBarrel(eta)){
      etaStr="Barrel";
      if (fabs(eta)<0.8) lastDigitStr="1";
      else lastDigitStr="2";
    }
    else if (emptyPhoton_.IsEndcap(eta)){
      etaStr="Endcap";
      if (fabs(eta)<2.0) lastDigitStr="1";
      else lastDigitStr="2";
    }
  }

  histName=wpStr;
  histName+="_";
  histName+=etaStr;
  histName+=lastDigitStr;

  return histName;
}
