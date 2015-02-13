#ifndef TTemplates_h
#define TTemplates_h

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

class TTemplates
{
  public:
    const static int nKinBinsMax=50;
    struct TemplatesPars{
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
      TFile* fFakeRef;
      TTree* treeTrue;
      TTree* treeFake;
      TTree* treeData;
      TTree* treeSign;//treeSign has to be consistent with treeFake
      TTree* treeFakeRef;
      int nKinBins;
      float kinBinLims[nKinBinsMax];
      int nFitBins[nKinBinsMax][2];
      float minVarFit[nKinBinsMax][2];
      float maxVarFit[nKinBinsMax][2];
      float unitOrigFit[nKinBinsMax][2];
      float sideband[nKinBinsMax][2];
      float sidebandUp[nKinBinsMax][2];
      bool combineTrueTempl[nKinBinsMax][2];
      bool combineFakeTempl[nKinBinsMax][2];
      TCut cutAdd;
      TCut cutBarrel;
      TCut cutEndcap;
      TCut cutNominal[2];
      TCut cutNominalExceptSidebandVar[2];
      TCut cutSidebandVarNominalRange[2];
      TCut cutWeight;
      bool noLeakSubtr;
      bool sumOverHist;
        // the method how to extract yields from fit:
        // sum over histogram or apply efficiency
      bool showTreeRef;
      TFile* fOutForSave;
      TString strFileOutName;
      TString strTrueYieldsTot[3];
      TString strTrueYields1D[3];
      TString strFakeYieldsTot[3];
      TString strFakeYields1D[3];
      TString strPlotsDir;
      TString strPlotsBaseName;
    };//end of struct TemplatesPars
    TTemplates();
    TTemplates(TemplatesPars pars);
    virtual ~TTemplates();


    void ComputeBackground(bool noPrint=0, bool noPlot=0);
    void PrintPars();
    TString StrLabelEta(int etaBin);
    TString StrLabelKin(int ikin, int nKinBins, float* kinBinLims, TString varKin);


  protected:

    enum {_BARREL=0, _ENDCAP=1, _COMMON=2};
    // should properly match the enum in TConfiguration.h
    //added here to make class independent on TConfiguration  

    TemplatesPars _pars;  
    
    TFile* _fOutTemp;

    TString StrLabelKin(int kinBin);

    void SetPars(TemplatesPars pars);
    bool ComputeBackgroundOne(int ikin, int ieta, bool noPrint=0);

    double _nFakeYieldsVal[nKinBinsMax][3];
    double _nFakeYieldsErr[nKinBinsMax][3];
    double _nTrueYieldsVal[nKinBinsMax][3];
    double _nTrueYieldsErr[nKinBinsMax][3];

  private:


    bool SetHists(int kinBin, int etaBin, bool noPrint=0);
    void RandomizeTemplates(int ikin, int ieta);

    void FitOne(int kinBin, int etaBin, bool noPrint=0, bool noPlot=0);
    void PrintYieldsAndChi2();
    void PrintHistogramsBinByBin(TH1D* hist[nKinBinsMax][3]);
    void PrintOneHistogramBinByBin(TH1D* hist[nKinBinsMax][3], int ikin, int ieta);
    void ComputeYieldOneKinBin(int ikin, int ieta, bool noPrint=0);
    void ComputeOneYield(int ikin, int ieta, bool noPrint, bool isTrueGamma, TH1D* hist[2], double*  nYieldsVal,double* nYieldsErr,double* nFromFitVal, double* nFromFitErr);
    float EffFromTree(int ikin, int ieta, bool noPrint, bool isTrueGamma);
    float EffFromSum(int ikin, int ieta, bool noPrint, TH1D* hist);
 //   void ComputeYieldOne(TH1D* hFake, double nFakeVal, double nFakeErr, double& nFakeYieldVal, double& nFakeYieldErr,int ieta, int ikin, bool isTrue, bool noPrint=0);
//    float EffPhoChIsoCorr(int ikin, int ieta, bool isTrue);

    void SetTemplate(bool isTrueGamma, TH1D* hTemplate, TCut cut, bool noPrint=0, TH1D* hLeak=0);

    void NewHistograms(int ikin, int ieta, bool noPrint);
    void SetFakeTemplate(int ikin, int ieta, bool noPrint);
    void SetTrueTemplate(int ikin, int ieta, bool noPrint);
    void SetDataAndSignHists(int ikin, int ieta, bool noPrint);
    bool CheckTemplates(int ikin, int ieta, bool badBins[150], bool noPrint);
    bool RebinTemplates(int ikin, int ieta, bool badBins[150], bool noPrint);
    void DeleteHistograms(int ikin, int ieta);

    void PlotTemplates();
    void PlotOneTemplate(int kinBin, int etaBin);

    void SaveYields();

    TCut SidebandCut(int ikin, int ieta);
    TCut SidebandVarNominalCut(int ieta1);
    TCut FitVarFitRangeCut(int ikin, int ieta);
    TCut CutKinBin(int kinBin);
    TCut CutEtaBin(int etaBin);

    TRandom _random;

    TH1D* _hTrue[nKinBinsMax][3];
    TH1D* _hFake[nKinBinsMax][3];
    TH1D* _hFakeMCtruth[nKinBinsMax][3];
    TH1D* _hLeak[nKinBinsMax][3];
    TH1D* _hSign[nKinBinsMax][3];
    double leakFraction[nKinBinsMax][3];
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


#endif //TTemplates_h
