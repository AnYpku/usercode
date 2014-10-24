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
  _fSFPt15to20 = new TFile (config.GetPhotonScaleFactorsFileNamePt15to20GeV());
  _fSFPt20toInf = new TFile (config.GetPhotonScaleFactorsFileNamePt20toInfGeV());

  _hSFPt15to20Loose_Barrel1 = (TH1D*)_fSFPt15to20->Get("Loose_Barrel1");
  _hSFPt15to20Loose_Endcap1 = (TH1D*)_fSFPt15to20->Get("Loose_Endcap1");
  _hSFPt15to20Medium_Barrel1 = (TH1D*)_fSFPt15to20->Get("Medium_Barrel1");
  _hSFPt15to20Medium_Endcap1 = (TH1D*)_fSFPt15to20->Get("Medium_Endcap1");
  _hSFPt15to20Tight_Barrel1 = (TH1D*)_fSFPt15to20->Get("Tight_Barrel1");
  _hSFPt15to20Tight_Endcap1 = (TH1D*)_fSFPt15to20->Get("Tight_Endcap1");

  _hSFPt20toInfLoose_Barrel1 = (TH1D*)_fSFPt20toInf->Get("Loose_Barrel1");
  _hSFPt20toInfLoose_Endcap1 = (TH1D*)_fSFPt20toInf->Get("Loose_Endcap1");
  _hSFPt20toInfMedium_Barrel1 = (TH1D*)_fSFPt20toInf->Get("Medium_Barrel1");
  _hSFPt20toInfMedium_Endcap1 = (TH1D*)_fSFPt20toInf->Get("Medium_Endcap1");
  _hSFPt20toInfTight_Barrel1 = (TH1D*)_fSFPt20toInf->Get("Tight_Barrel1");
  _hSFPt20toInfTight_Endcap1 = (TH1D*)_fSFPt20toInf->Get("Tight_Endcap1");

  _hSFPt20toInfLoose_Barrel2 = (TH1D*)_fSFPt20toInf->Get("Loose_Barrel2");
  _hSFPt20toInfLoose_Endcap2 = (TH1D*)_fSFPt20toInf->Get("Loose_Endcap2");
  _hSFPt20toInfMedium_Barrel2 = (TH1D*)_fSFPt20toInf->Get("Medium_Barrel2");
  _hSFPt20toInfMedium_Endcap2 = (TH1D*)_fSFPt20toInf->Get("Medium_Endcap2");
  _hSFPt20toInfTight_Barrel2 = (TH1D*)_fSFPt20toInf->Get("Tight_Barrel2");
  _hSFPt20toInfTight_Endcap2 = (TH1D*)_fSFPt20toInf->Get("Tight_Endcap2");

}

TPhotonScaleFactor::~TPhotonScaleFactor()
{
}

void TPhotonScaleFactor::SetScaleFactor(float pt, float eta, int WP)
{
  SetScaleFactorExceptElectronVeto(pt,eta,WP);
  SetScaleFactorElectronVeto(pt,eta,WP);
  _scaleFactor=_scaleFactorExceptElectronVeto*_scaleFactorElectronVeto;
  _scaleFactorErr=
    _scaleFactorExceptElectronVeto*_scaleFactorExceptElectronVeto*
    _scaleFactorElectronVetoErr*_scaleFactorElectronVetoErr +
    _scaleFactorExceptElectronVetoErr*_scaleFactorExceptElectronVetoErr*
    _scaleFactorElectronVeto*_scaleFactorElectronVeto;
  _scaleFactorErr=sqrt(_scaleFactorErr);
}

float TPhotonScaleFactor::GetScaleFactor()
{
  return _scaleFactor;
}

float TPhotonScaleFactor::GetScaleFactorErr()
{
  return _scaleFactorErr;
}

float TPhotonScaleFactor::GetScaleFactorExceptElectronVeto()
{
  return _scaleFactorExceptElectronVeto;
}

float TPhotonScaleFactor::GetScaleFactorExceptElectronVetoErr()
{
  return _scaleFactorExceptElectronVetoErr;
}

float TPhotonScaleFactor::GetScaleFactorElectronVeto()
{
  return _scaleFactorElectronVeto;
}

float TPhotonScaleFactor::GetScaleFactorElectronVetoErr()
{
  return _scaleFactorElectronVetoErr;
}

void TPhotonScaleFactor::SetScaleFactorExceptElectronVeto(float pt, float eta, int WP)
{
  if (pt<=15){
    _scaleFactorExceptElectronVeto=1.0;
    _scaleFactorExceptElectronVetoErr=0.0;
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
    if (_emptyPhoton.IsBarrel(eta)){
      if      (WP==_emptyPhoton.WP_LOOSE)
        SetScaleFactorExceptElectronVetoKnownHist(_hSFPt15to20Loose_Barrel1,pt);
      else if (WP==_emptyPhoton.WP_MEDIUM)
        SetScaleFactorExceptElectronVetoKnownHist(_hSFPt15to20Medium_Barrel1,pt);
      else if (WP==_emptyPhoton.WP_TIGHT)
        SetScaleFactorExceptElectronVetoKnownHist(_hSFPt15to20Tight_Barrel1,pt);
    }
    if (_emptyPhoton.IsEndcap(eta)){
      if      (WP==_emptyPhoton.WP_LOOSE)
        SetScaleFactorExceptElectronVetoKnownHist(_hSFPt15to20Loose_Endcap1,pt);
      else if (WP==_emptyPhoton.WP_MEDIUM)
        SetScaleFactorExceptElectronVetoKnownHist(_hSFPt15to20Medium_Endcap1,pt);
      else if (WP==_emptyPhoton.WP_TIGHT)
        SetScaleFactorExceptElectronVetoKnownHist(_hSFPt15to20Tight_Endcap1,pt);
    }
}

void TPhotonScaleFactor::SetScaleFactorExceptElectronVetoPt20toInf(float pt, float eta, int WP)
{
    _scaleFactorExceptElectronVeto=1.0;
    _scaleFactorExceptElectronVetoErr=0.0;
    if (_emptyPhoton.IsBarrel(eta) && fabs(eta)<0.8){
      if      (WP==_emptyPhoton.WP_LOOSE)
        SetScaleFactorExceptElectronVetoKnownHist(_hSFPt20toInfLoose_Barrel1,pt);
      else if (WP==_emptyPhoton.WP_MEDIUM)
        SetScaleFactorExceptElectronVetoKnownHist(_hSFPt20toInfMedium_Barrel1,pt);
      else if (WP==_emptyPhoton.WP_TIGHT)
        SetScaleFactorExceptElectronVetoKnownHist(_hSFPt20toInfTight_Barrel1,pt);
    }
    else if (_emptyPhoton.IsBarrel(eta) && fabs(eta)>=0.8){
      if      (WP==_emptyPhoton.WP_LOOSE)
        SetScaleFactorExceptElectronVetoKnownHist(_hSFPt20toInfLoose_Barrel2,pt);
      else if (WP==_emptyPhoton.WP_MEDIUM)
        SetScaleFactorExceptElectronVetoKnownHist(_hSFPt20toInfMedium_Barrel2,pt);
      else if (WP==_emptyPhoton.WP_TIGHT)
        SetScaleFactorExceptElectronVetoKnownHist(_hSFPt20toInfTight_Barrel2,pt);
    }
    else if (_emptyPhoton.IsEndcap(eta) && fabs(eta)<2.0){
      if      (WP==_emptyPhoton.WP_LOOSE)
        SetScaleFactorExceptElectronVetoKnownHist(_hSFPt20toInfLoose_Endcap1,pt);
      else if (WP==_emptyPhoton.WP_MEDIUM)
        SetScaleFactorExceptElectronVetoKnownHist(_hSFPt20toInfMedium_Endcap1,pt);
      else if (WP==_emptyPhoton.WP_TIGHT)
        SetScaleFactorExceptElectronVetoKnownHist(_hSFPt20toInfTight_Endcap1,pt);
    }
    else if (_emptyPhoton.IsEndcap(eta) && fabs(eta)>=2.0){
      if      (WP==_emptyPhoton.WP_LOOSE)
        SetScaleFactorExceptElectronVetoKnownHist(_hSFPt20toInfLoose_Endcap2,pt);
      else if (WP==_emptyPhoton.WP_MEDIUM)
        SetScaleFactorExceptElectronVetoKnownHist(_hSFPt20toInfMedium_Endcap2,pt);
      else if (WP==_emptyPhoton.WP_TIGHT)
        SetScaleFactorExceptElectronVetoKnownHist(_hSFPt20toInfTight_Endcap2,pt);
    }
}

void TPhotonScaleFactor::SetScaleFactorExceptElectronVetoKnownHist(TH1D *h, float pt)
{
   if (pt>=(h->GetBinCenter(h->GetNbinsX())+h->GetBinWidth(h->GetNbinsX()))){
     _scaleFactorExceptElectronVeto=h->GetBinContent(h->GetNbinsX());
     _scaleFactorExceptElectronVetoErr=h->GetBinError(h->GetNbinsX());
   }
   else if (pt<=h->GetBinLowEdge(1)){
     _scaleFactorExceptElectronVeto=1.0;
     _scaleFactorExceptElectronVetoErr=0.0;
   }
   else {
     _scaleFactorExceptElectronVeto=h->GetBinContent(h->FindBin(pt));
     _scaleFactorExceptElectronVetoErr=h->GetBinError(h->FindBin(pt));
   }
}

void TPhotonScaleFactor::SetScaleFactorElectronVeto(float pt, float eta, int WP)
{
  //from https://twiki.cern.ch/twiki/bin/view/CMS/CutBasedPhotonID2012#Simple_counting_efficiency_scale
  _scaleFactorElectronVeto=1.0;
  _scaleFactorElectronVetoErr=0.0;
  if      (_emptyPhoton.IsBarrel(eta))
    SetScaleFactorElectronVetoBarrel(pt, WP);
  else if (_emptyPhoton.IsEndcap(eta))
    SetScaleFactorElectronVetoEndcap(pt, WP);
}

void TPhotonScaleFactor::SetScaleFactorElectronVetoBarrel(float pt, int WP)
{
    if      (pt>15 && pt<20){
      if      (WP==_emptyPhoton.WP_LOOSE){
        _scaleFactorElectronVeto= 0.9910;
        _scaleFactorElectronVetoErr= 0.0040;
      }
      else if (WP==_emptyPhoton.WP_MEDIUM){
        _scaleFactorElectronVeto= 0.9936;
        _scaleFactorElectronVetoErr= 0.0041;
      }
      else if (WP==_emptyPhoton.WP_TIGHT){
        _scaleFactorElectronVeto= 0.9939;
        _scaleFactorElectronVetoErr= 0.0043;
      }
    }
    else if (pt>20 && pt<30){
      if      (WP==_emptyPhoton.WP_LOOSE){
        _scaleFactorElectronVeto= 0.9996;
        _scaleFactorElectronVetoErr= 0.0059;
      }
      else if (WP==_emptyPhoton.WP_MEDIUM){
        _scaleFactorElectronVeto= 1.0022;
        _scaleFactorElectronVetoErr= 0.0062;
      }
      else if (WP==_emptyPhoton.WP_TIGHT){
        _scaleFactorElectronVeto= 1.0032;
        _scaleFactorElectronVetoErr= 0.0066;
      }
    }
    else if (pt>30 && pt<40){
      if      (WP==_emptyPhoton.WP_LOOSE){
        _scaleFactorElectronVeto= 1.0081;
        _scaleFactorElectronVetoErr= 0.0154;
      }
      else if (WP==_emptyPhoton.WP_MEDIUM){
        _scaleFactorElectronVeto= 1.0148;
        _scaleFactorElectronVetoErr= 0.0153;
      }
      else if (WP==_emptyPhoton.WP_TIGHT){
        _scaleFactorElectronVeto= 0.9919;
        _scaleFactorElectronVetoErr= 0.0057;
      }
    }
    else if (pt>15){
      if      (WP==_emptyPhoton.WP_LOOSE){
        _scaleFactorElectronVeto= 0.9957;
        _scaleFactorElectronVetoErr= 0.0037;
      }
      else if (WP==_emptyPhoton.WP_MEDIUM){
        _scaleFactorElectronVeto= 0.9992;
        _scaleFactorElectronVetoErr= 0.0038;
      }
      else if (WP==_emptyPhoton.WP_TIGHT){
        _scaleFactorElectronVeto= 0.9969;
        _scaleFactorElectronVetoErr= 0.0035;
      }
   }
}

void TPhotonScaleFactor::SetScaleFactorElectronVetoEndcap(float pt, int WP)
{
    if      (pt>15 && pt<20){
      if      (WP==_emptyPhoton.WP_LOOSE){
        _scaleFactorElectronVeto= 0.9970;
        _scaleFactorElectronVetoErr= 0.0148;
      }
      else if (WP==_emptyPhoton.WP_MEDIUM){
        _scaleFactorElectronVeto= 1.0002;
        _scaleFactorElectronVetoErr= 0.0170;
      }
      else if (WP==_emptyPhoton.WP_TIGHT){
        _scaleFactorElectronVeto= 1.0046;
        _scaleFactorElectronVetoErr= 0.0178;
      }
    }
    else if (pt>20 && pt<30){
      if      (WP==_emptyPhoton.WP_LOOSE){
        _scaleFactorElectronVeto= 0.9911;
        _scaleFactorElectronVetoErr= 0.0169;
      }
      else if (WP==_emptyPhoton.WP_MEDIUM){
        _scaleFactorElectronVeto= 0.9912;
        _scaleFactorElectronVetoErr= 0.0188;
      }
      else if (WP==_emptyPhoton.WP_TIGHT){
        _scaleFactorElectronVeto= 0.9917;
        _scaleFactorElectronVetoErr= 0.0200;
      }
    }
    else if (pt>30 && pt<40){
      if      (WP==_emptyPhoton.WP_LOOSE){
        _scaleFactorElectronVeto= 0.9796;
        _scaleFactorElectronVetoErr= 0.0117;
      }
      else if (WP==_emptyPhoton.WP_MEDIUM){
        _scaleFactorElectronVeto= 0.9850;
        _scaleFactorElectronVetoErr= 0.0106;
      }
      else if (WP==_emptyPhoton.WP_TIGHT){
        _scaleFactorElectronVeto= 0.9837;
        _scaleFactorElectronVetoErr= 0.0114;
      }
    }
    else if (pt>15){
      if      (WP==_emptyPhoton.WP_LOOSE){
        _scaleFactorElectronVeto= 0.9919;
        _scaleFactorElectronVetoErr= 0.0094;
      }
      else if (WP==_emptyPhoton.WP_MEDIUM){
        _scaleFactorElectronVeto= 0.9939;
        _scaleFactorElectronVetoErr= 0.0106;
      }
      else if (WP==_emptyPhoton.WP_TIGHT){
        _scaleFactorElectronVeto= 0.9957;
        _scaleFactorElectronVetoErr= 0.0113;
      }
    }
}

TString TPhotonScaleFactor::GetPhotonScaleFactorHistName(float pt, float eta, int WP)
{

  TString histName="NONE";

  TString wpStr;//Loose, Medium or Tight
  TString etaStr;//Barrel or Endcap
  TString lastDigitStr;//1 or 2
  if      (WP==_emptyPhoton.WP_LOOSE)  wpStr="Loose";
  else if (WP==_emptyPhoton.WP_MEDIUM) wpStr="Medium";
  else if (WP==_emptyPhoton.WP_TIGHT)  wpStr="Tight";
  else return histName;

  if      (pt<15) return histName;
  else if (pt>=15 && pt<20){
    lastDigitStr="1";
    if (_emptyPhoton.IsBarrel(eta)) etaStr="Barrel";
    else if (_emptyPhoton.IsEndcap(eta)) etaStr="Endcap"; 
    else return histName;
  }
  else if (pt>=20){
    if (_emptyPhoton.IsBarrel(eta)){
      etaStr="Barrel";
      if (fabs(eta)<0.8) lastDigitStr="1";
      else lastDigitStr="2";
    }
    else if (_emptyPhoton.IsEndcap(eta)){
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
