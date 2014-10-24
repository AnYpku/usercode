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
    float GetScaleFactorExceptElectronVeto();
    float GetScaleFactorExceptElectronVetoErr();
    float GetScaleFactorElectronVeto();
    float GetScaleFactorElectronVetoErr();  


  private:
    float _scaleFactorExceptElectronVeto;
    float _scaleFactorExceptElectronVetoErr;
    float _scaleFactorElectronVeto;
    float _scaleFactorElectronVetoErr;
    float _scaleFactor;
    float _scaleFactorErr;

    TPhotonCuts _emptyPhoton;

    TFile* _fSFPt15to20;
    TFile* _fSFPt20toInf;
    TH1D*  _hSFPt15to20Loose_Barrel1;
    TH1D*  _hSFPt15to20Loose_Endcap1;
    TH1D*  _hSFPt15to20Medium_Barrel1;
    TH1D*  _hSFPt15to20Medium_Endcap1;
    TH1D*  _hSFPt15to20Tight_Barrel1;
    TH1D*  _hSFPt15to20Tight_Endcap1;
    TH1D*  _hSFPt20toInfLoose_Barrel1;
    TH1D*  _hSFPt20toInfLoose_Endcap1;
    TH1D*  _hSFPt20toInfMedium_Barrel1;
    TH1D*  _hSFPt20toInfMedium_Endcap1;
    TH1D*  _hSFPt20toInfTight_Barrel1;
    TH1D*  _hSFPt20toInfTight_Endcap1;
    TH1D*  _hSFPt20toInfLoose_Barrel2;
    TH1D*  _hSFPt20toInfLoose_Endcap2;
    TH1D*  _hSFPt20toInfMedium_Barrel2;
    TH1D*  _hSFPt20toInfMedium_Endcap2;
    TH1D*  _hSFPt20toInfTight_Barrel2;
    TH1D*  _hSFPt20toInfTight_Endcap2;
};
#endif //TPhotonScaleFactor_h
