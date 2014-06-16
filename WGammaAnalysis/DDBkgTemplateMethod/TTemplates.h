#ifndef TTemplates_h
#define TTemplates_h

#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "THStack.h"
#include "TCut.h"
#include "TString.h"
#include "RooPlot.h"

#include <vector>

#include "../Configuration/TConfiguration.h"
#include "../Configuration/TAllInputSamples.h"

class TTemplates
{
  public:
    TTemplates(int channel, int blind, int phoWP, TString varFit, TString varSideband, TString varKin, int nKinBins, float* kinBinLims, int* nBinsLeftB, float* maxVarFitB, int* nBinsLeftE, float* maxVarFitE, int nBinsLeftBTot, float maxVarFitBTot, int nBinsLeftETot, float maxVarFitETot, bool isMetCutOptimization=0);
    virtual ~TTemplates();

    enum {NO_CLOSURE_TEST=0, CLOSURE_TEST_1, CLOSURE_TEST_2, CLOSURE_TEST_3};

    void ComputeBackground(bool noPrint=0, bool noPlot=0);
    void ComputeBackgroundOne(int ikin, bool noPrint=0, bool noPlot=0);
    void SetThreeHists(int kinBin, int etaBin, bool noPrint=0);
    void DeleteThreeHists(int kinBin, int etaBin);

    void FitOne(int kinBin, int etaBin, bool noPrint=0, bool noPlot=0);
    void PrintBkgYieldsAndChi2();
    void ComputeBkgYieldOneKinBin(int ikin, bool noPrint=0);
    void ComputeYieldOne(TH1D* hBkgr, double nBkgrVal, double nBkgrErr, double& nBkgrYieldVal, double& nBkgrYieldErr,int ieta, int ikin, bool noPrint=0);

    void SetSignalTemplate(TH1D* hSign, TCut cut, bool noPrint=0);
    void SetBackgrTemplate(TH1D* hBkgr, TH1D* hLeak, TH1D* hLeakTemp, TCut cut, bool noPrint=0);

    void PlotTemplates();
    void PlotOneTemplate(int kinBin, int etaBin);

    void SaveBkgYields();

    TCut SidebandCut();
    TCut SidebandVarNominalCut();
    TCut CutKinBin(int kinBin);
    TCut CutEtaBin(int etaBin);
    TString StrLabelEta(int etaBin);
    TString StrLabelKin(int kinBin);

    double ValueCutNominalVarFit(int etaBin);

  private:
    TConfiguration _config;
    int _channel;
    int _blind;
    bool _isMetCutOptimization;
    int _selectionStage;

    int _isClosureTest;
    TCut _cutWeight;

    int _phoWP;
    TAllInputSamples* _INPUT;
    TFile* _fSign;
    TFile* _fData;
    vector <TFile*> _vecFBkg;
//    TFile* fOut_;

    TTree* _treeSign;
    TTree* _treeData;
    vector <TTree*> _vecTreeBkg;
    vector <TString> _vecBkgNames;
    vector <int> _vecBkgColors;

    TString _varKin;
    int _nKinBins;
    float* _kinBinLims;

    TString _varFit;
    TString _varSideband;
    TString _labelVarFit;

    const static int _nBinsMax=50;
    TH1D* _hSign[_nBinsMax][3];
    TH1D* _hBkgr[_nBinsMax][3];
    TH1D* _hLeak[_nBinsMax][3];
    TH1D* _hData[_nBinsMax][3];
    TH1D* _hSumm[_nBinsMax][3];
    TH1D* _hRatio[_nBinsMax][3];
    RooPlot* _plotter[_nBinsMax][3];
    double _chi2ToNDF[_nBinsMax][3];
    double _nBkgrFromFitVal[_nBinsMax][3];
    double _nBkgrFromFitErr[_nBinsMax][3];
    double _nSignFromFitVal[_nBinsMax][3];
    double _nSignFromFitErr[_nBinsMax][3];
    int _nBinsLeft[_nBinsMax][3];

    double _nBkgrYieldsVal[_nBinsMax];
    double _nBkgrYieldsErr[_nBinsMax];
    double _nSignYieldsVal[_nBinsMax];
    double _nSignYieldsErr[_nBinsMax];
    // strictly, bkgr and sign are not background and signal
    // but bkgr is fake gamma bkgr
    // and sign is signal plus true gamma bkgr

    TFile* _fOutForSave;

//    vector <double> chi2Barrel_;
//    vector <double> chi2Endcap_;
//    vector <double> bkgFractionBarrel_;
//    vector <double> bkgFractionEndcap_;
//    vector <double> bkgFractionBarrelErr_;
//    vector <double> bkgFractionEndcapErr_;
    float _minVarFit[3]; //sIEtaIEtaBarrelMin_ = 0.007;
    float _maxVarFit[_nBinsMax][3];

    //TTree** trData_;
    //TTree** trSignal_;
    //TTree** trBkg_;
    //TTree** trLeakage_;
};


#endif //TTemplates_h
