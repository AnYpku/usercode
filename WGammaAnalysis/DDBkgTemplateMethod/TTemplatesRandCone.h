#ifndef TTemplatesRandCone_h
#define TTemplatesRandCone_h

#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "THStack.h"
#include "TCut.h"
#include "TString.h"
#include "RooPlot.h"
#include "TRandom.h"

#include <vector>

//#include "../Configuration/TConfiguration.h"
//#include "../Configuration/TAllInputSamples.h"
//#include "../Include/TPhotonCuts.h"

class TTemplatesRandCone
{
  public:
    const static int nKinBinsMax=50;
    struct TemplatesRandConePars{
      TString varKin;
      TString varTrueTempl;
      TString varFakeTempl;
      TString varSideband;
      TString varFit; 
      TString varPhoEta;
      TString varWeight;
      TFile* fTrue;
      TFile* fFake;
      TFile* fData;
      TFile* fSign;//treeSign has to be consistent with treeFake
      TTree* treeTrue;
      TTree* treeFake;
      TTree* treeData;
      TTree* treeSign;//treeSign has to be consistent with treeFake
      int nKinBins;
      float kinBinLims[nKinBinsMax];
      int nFitBins[nKinBinsMax][2];
      float minVarFit[nKinBinsMax][2];
      float maxVarFit[nKinBinsMax][2];
      float sideband[nKinBinsMax][2];
      float sidebandUp[nKinBinsMax][2];
      bool combineTrueTempl[nKinBinsMax][2];
      bool combineFakeTempl[nKinBinsMax][2];
      TCut cutAdd;
      TCut cutBarrel;
      TCut cutEndcap;
      TCut cutNominal;
      TCut cutNominalExceptSidebandVar;
      TCut cutSidebandVarNominalRange;
      TCut cutWeight;
      bool noLeakSubtr;
      TFile* fOutForSave;
      TString strFileOutName;
      TString strTrueYieldsTot[3];
      TString strTrueYields1D[3];
      TString strFakeYieldsTot[3];
      TString strFakeYields1D[3];
    };//end of struct TemplatesRandConePars
    TTemplatesRandCone();
    TTemplatesRandCone(TemplatesRandConePars pars);
    virtual ~TTemplatesRandCone();


    void ComputeBackground(bool noPrint=0, bool noPlot=0);
    void PrintPars();

  protected:

    enum {_BARREL=0, _ENDCAP=1, _COMMON=2};
    // should properly match the enum in TConfiguration.h
    //added here to make class independent on TConfiguration  

    TemplatesRandConePars _pars;  

    void SetPars(TemplatesRandConePars pars);
    void ComputeBackgroundOne(int ikin, int ieta, bool noPrint=0);

    double _nFakeYieldsVal[nKinBinsMax][3];
    double _nFakeYieldsErr[nKinBinsMax][3];
    double _nTrueYieldsVal[nKinBinsMax][3];
    double _nTrueYieldsErr[nKinBinsMax][3];

    TString StrLabelEta(int etaBin);
    TString StrLabelKin(int kinBin);

  private:


    void SetHists(int kinBin, int etaBin, bool noPrint=0);
    void RandomizeTemplates(int ikin, int ieta);
    void DeleteHists(int kinBin, int etaBin);

    void FitOne(int kinBin, int etaBin, bool noPrint=0, bool noPlot=0);
    void PrintYieldsAndChi2();
    void PrintHistogramsBinByBin(TH1D* hist[nKinBinsMax][3]);
    void PrintOneHistogramBinByBin(TH1D* hist[nKinBinsMax][3], int ikin, int ieta);
    void ComputeYieldOneKinBin(int ikin, int ieta, bool noPrint=0);
    void ComputeOneYield(int ikin, int ieta, bool noPrint, bool isTrueGamma,double*  nYieldsVal,double* nYieldsErr,double* nFromFitVal, double* nFromFitErr);
 //   void ComputeYieldOne(TH1D* hFake, double nFakeVal, double nFakeErr, double& nFakeYieldVal, double& nFakeYieldErr,int ieta, int ikin, bool isTrue, bool noPrint=0);
//    float EffPhoChIsoCorr(int ikin, int ieta, bool isTrue);

    void SetTemplate(bool isTrueGamma, TH1D* hTemplate, TCut cut, bool noPrint=0, TH1D* hLeak=0);

    void PlotTemplates();
    void PlotOneTemplate(int kinBin, int etaBin);

    void SaveYields();

    TCut SidebandCut(int ikin, int ieta);
    TCut SidebandVarNominalCut();
    TCut FitVarFitRangeCut(int ikin, int ieta);
    TCut CutKinBin(int kinBin);
    TCut CutEtaBin(int etaBin);

    TRandom _random;

    TH1D* _hTrue[nKinBinsMax][3];
    TH1D* _hFake[nKinBinsMax][3];
    TH1D* _hLeak[nKinBinsMax][3];
    TH1D* _hTrueReference[nKinBinsMax][3];
    TH1D* _hFakeReference[nKinBinsMax][3];
    TH1D* _hData[nKinBinsMax][3];
    TH1D* _hSumm[nKinBinsMax][3];
    TH1D* _hRatio[nKinBinsMax][3];
    RooPlot* _plotter[nKinBinsMax][3];
    double _chi2ToNDF[nKinBinsMax][3];

    double _nFakeFromFitVal[nKinBinsMax][3];
    double _nFakeFromFitErr[nKinBinsMax][3];
    double _nTrueFromFitVal[nKinBinsMax][3];
    double _nTrueFromFitErr[nKinBinsMax][3];

    const static int _nToys=100;


};


#endif //TTemplatesRandCone_h
