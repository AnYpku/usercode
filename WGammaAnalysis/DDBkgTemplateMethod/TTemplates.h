#ifndef TTemplates_h
#define TTemplates_h

#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"

#include "../Configuration/TConfiguration.h"

class TTemplates
{
  public:
    TTemplates(int channel);
    virtual ~TTemplates();
    void ComputeBackground();
    void SetHists();
    void PrintHists();
    void FitOne(int ptBin, int etaBin);
    void PrintChi2();
    void SaveBkgYields();
    enum {BARREL, ENDCAP};
      //eta bin: 0 - barrel, 1 - endcap

  private:
    TConfiguration config_;
    int channel_;
    TFile* fSig_;
    TFile* fBkg_;
    TFile* fOut_;
    TH1F** hBarrelData_;
    TH1F** hBarrelSignal_;
    TH1F** hBarrelBkg_;
    TH1F** hBarrelLeakage_;
    TH1F** hEndcapData_;
    TH1F** hEndcapSignal_;
    TH1F** hEndcapBkg_;
    TH1F** hEndcapLeakage_;
    TFile* fOutForSave_;
    TH1F* yieldsBkgB_;
    TH1F* yieldsBkgE_;
    TH1F* fractionsBkgB_;
    TH1F* fractionsBkgE_;
    vector <float> chi2Barrel_;
    vector <float> chi2Endcap_;
    vector <float> bkgFractionBarrel_;
    vector <float> bkgFractionEndcap_;
    vector <float> bkgFractionBarrelErr_;
    vector <float> bkgFractionEndcapErr_;
    const static float sIEtaIEtaBarrelMin_ = 0.007;
    const static float sIEtaIEtaBarrelMax_ = 0.020;
    const static float sIEtaIEtaEndcapMin_ = 0.020;
    const static float sIEtaIEtaEndcapMax_ = 0.050;
    const static int   nBinsBarrel_ = 20;
    const static int   nBinsEndcap_ = 30;
    //TTree** trData_;
    //TTree** trSignal_;
    //TTree** trBkg_;
    //TTree** trLeakage_;
};


#endif //TTemplates_h
