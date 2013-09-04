#ifndef TPhotonScaleFactor_h
#define TPhotonScaleFactor_h

#include "TPhotonCuts.h"
 //this class
#include "TFile.h"
#include "TH1D.h"
#include "TString.h"
 //ROOT

class TPhotonScaleFactor
{
  public:
    TPhotonScaleFactor();
    virtual ~TPhotonScaleFactor();

    TString GetPhotonScaleFactorHistName(float pt, float eta, int WP);

    void SetScaleFactor(float pt, float eta, int WP);
    void SetScaleFactorExceptElectronVeto(float pt, float eta, int WP);
    void SetScaleFactorElectronVeto(float pt, float eta, int WP);
      //see https://twiki.cern.ch/twiki/bin/view/CMS/CutBasedPhotonID2012
      //for reference

    void SetScaleFactorExceptElectronVetoPt15to20(float pt, float eta, int WP);
    void SetScaleFactorExceptElectronVetoPt20toInf(float pt, float eta, int WP);
    void SetScaleFactorElectronVetoBarrel(float pt, int WP);
    void SetScaleFactorElectronVetoEndcap(float pt, int WP);

    void SetScaleFactorExceptElectronVetoKnownHist(TH1D *h, float pt);

    float GetScaleFactor();
    float GetScaleFactorErr(); 


  private:
    float scaleFactorExceptElectronVeto_;
    float scaleFactorExceptElectronVetoErr_;
    float scaleFactorElectronVeto_;
    float scaleFactorElectronVetoErr_;
    float scaleFactor_;
    float scaleFactorErr_;

    TPhotonCuts emptyPhoton_;

    TFile* fSFPt15to20_;
    TFile* fSFPt20toInf_;
    TH1D*  hSFPt15to20Loose_Barrel1_;
    TH1D*  hSFPt15to20Loose_Endcap1_;
    TH1D*  hSFPt15to20Medium_Barrel1_;
    TH1D*  hSFPt15to20Medium_Endcap1_;
    TH1D*  hSFPt15to20Tight_Barrel1_;
    TH1D*  hSFPt15to20Tight_Endcap1_;
    TH1D*  hSFPt20toInfLoose_Barrel1_;
    TH1D*  hSFPt20toInfLoose_Endcap1_;
    TH1D*  hSFPt20toInfMedium_Barrel1_;
    TH1D*  hSFPt20toInfMedium_Endcap1_;
    TH1D*  hSFPt20toInfTight_Barrel1_;
    TH1D*  hSFPt20toInfTight_Endcap1_;
    TH1D*  hSFPt20toInfLoose_Barrel2_;
    TH1D*  hSFPt20toInfLoose_Endcap2_;
    TH1D*  hSFPt20toInfMedium_Barrel2_;
    TH1D*  hSFPt20toInfMedium_Endcap2_;
    TH1D*  hSFPt20toInfTight_Barrel2_;
    TH1D*  hSFPt20toInfTight_Endcap2_;
};
#endif //TPhotonScaleFactor_h
