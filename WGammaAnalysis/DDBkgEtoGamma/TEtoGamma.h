#ifndef TEtoGamma_h
#define TEtoGamma_h

#include "../Configuration/TConfiguration.h"

#include "TH1F.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TTree.h"
#include "THStack.h"
#include "TLatex.h"
#include "TCut.h"

#include <vector>

class TEtoGamma
{
  public:

  TEtoGamma(TConfiguration::AnalysisParameters &anPars, int selStage, bool isMCclosure);
    virtual ~TEtoGamma();
    void ComputePlotSave();

  private:
    TFile* _fOut;

    enum {_DATA_EtoGAMMA_ENR, _ZJETS_EtoGAMMA_ENR, _ZJETS_NOM, _SIGMC_NOM, _SIGMC_ENR, _WGAMMATAUNU_ENR, _ZGAMMA_ENR, _TTJETS_ENR, _WJETS_ENR};

    struct Yield{
      int num;
      TH1F* hist[2];
      TString fName;
      TTree* tr;
//      float cont;
//      float err;
    };
    Yield _yield[10];

    TString FName(int inum);
    TString HName(int inum, int ieta);
    void SetYields(int inum);

    // etaBin: BARREL, ENDCAP
    TH1F* _yieldDDEtoGamma[3];// data driven estimate
    TH1F* _hScale[2]; // nEst/nMC
    TH1F* _hZmassData[2][4][50];// BARREL or ENDCAP, ietaFine, ikin
    float _NsigVal[4];//ietaFine
    float _NsigErr[4];//ietaFine

    TConfiguration _conf;
    TCut _cutAdd;
    TString _varKin;
    int _nKinBins;
    float _kinBinLims[100];

    int _nFineEtaBins;

    int _selStage;

    bool _isMCclosure;

    void WriteToFile();
    void ComputeBkg();
    TTree* GetTree(TString strFileName);
    void FitMeg(int ikin, int ieta, int ietaFine, TH1F* Z_mass, TCut cutEtoGenr, TString saveas);
    TCut CutEtaFine(int ieta, int ietaFine);
    float EtaMin(int ieta, int ietaFine);
    float EtaMax(int ieta, int ietaFine);
    TString StrEtaFine(int ieta, int ietaFine);

    float _start_CMS_alpha;
    float _start_CMS_beta;
    float _start_CMS_gamma;
    float _start_CMS_peak;
    float _start_mean_gau;
    float _start_sigma_gau;
};

const float _ZmassMin = 40;
const float _ZmassMax=200;
const int _ZmassNbins[5]={40,32,20,16,10};

#endif //TEtoGamma_h
