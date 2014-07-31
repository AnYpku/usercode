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
#include "../Include/TPhotonCuts.h"

class TTemplates
{
  public:
    TTemplates(int year, int channel, int blind, int phoWP, TString varFit, TString varSideband, TString varKin, int nKinBins, float* kinBinLims, bool noAdjustBinning=0, bool noPhoPFChIsoCut=0, bool isMetCutOptimization=0);
    virtual ~TTemplates();

    enum {NO_CLOSURE_TEST=0, CLOSURE_TEST_1, CLOSURE_TEST_2, CLOSURE_TEST_3};

    void ComputeBackground(bool noPrint=0, bool noPlot=0);
    void ComputeBackgroundOne(int ikin, bool noPrint=0, bool noPlot=0);
    void AdjustFitBinning(int kinBin, int etaBin, bool noPrint, bool noPlot);
    bool IsNewChi2ToNDFBetter(float newChi2, float oldChi2);
    void SetThreeHists(int kinBin, int etaBin, bool noPrint=0);
    void DeleteThreeHists(int kinBin, int etaBin);

    void FitOne(int kinBin, int etaBin, bool noPrint=0, bool noPlot=0);
    void PrintYieldsAndChi2();
    const static int _nBinsMax=50;
    void PrintHistogramsBinByBin(TH1D* hist[_nBinsMax][3]);
    void PrintOneHistogramBinByBin(TH1D* hist[_nBinsMax][3], int ikin, int ieta);
    void ComputeYieldOneKinBin(int ikin, bool noPrint=0);
    void ComputeYieldOne(TH1D* hFake, double nFakeVal, double nFakeErr, double& nFakeYieldVal, double& nFakeYieldErr,int ieta, int ikin, bool isTrue, bool noPrint=0);
    float EffPhoChIsoCorr(int ikin, int ieta, bool isTrue);

    void SetTrueTemplate(TH1D* hTrue, TCut cut, bool noPrint=0);
    void SetFakeTemplate(TH1D* hFake, TH1D* hLeak, TH1D* hLeakTemp, TCut cut, bool noPrint=0);

    void PlotTemplates();
    void PlotOneTemplate(int kinBin, int etaBin);

    void SaveYields();

    TCut SidebandCut();
    TCut SidebandVarNominalCut();
    TCut CutKinBin(int kinBin);
    TCut CutEtaBin(int etaBin);
    TString StrLabelEta(int etaBin);
    TString StrLabelKin(int kinBin);

    double ValueCutNominalVarFit(int etaBin);

  private:
    TConfiguration _config;
    TPhotonCuts _photon;
    int _year;
    int _channel;
    int _blind;
    bool _isMetCutOptimization;
    bool _noAdjustBinning;
    bool _noPhoPFChIsoCut;
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

    int _iter;

    TString _varFit;
    TString _varSideband;
    TString _labelVarFit;

    TH1D* _hTrue[_nBinsMax][3];
    TH1D* _hFake[_nBinsMax][3];
    TH1D* _hLeak[_nBinsMax][3];
    TH1D* _hData[_nBinsMax][3];
    TH1D* _hSumm[_nBinsMax][3];
    TH1D* _hRatio[_nBinsMax][3];
    RooPlot* _plotter[_nBinsMax][3];
    double _chi2ToNDF[_nBinsMax][3];
    double _nFakeFromFitVal[_nBinsMax][3];
    double _nFakeFromFitErr[_nBinsMax][3];
    double _nTrueFromFitVal[_nBinsMax][3];
    double _nTrueFromFitErr[_nBinsMax][3];
    int _nBinsLeft[_nBinsMax][3];

    double _nFakeYieldsVal[_nBinsMax][3];
    double _nFakeYieldsErr[_nBinsMax][3];
    double _nTrueYieldsVal[_nBinsMax][3];
    double _nTrueYieldsErr[_nBinsMax][3];
    // strictly, Fake and True are not background and signal
    // but Fake is fake gamma Fake
    // and True is signal plus true gamma Fake

    TFile* _fOutForSave;

//    vector <double> chi2Barrel_;
//    vector <double> chi2Endcap_;
//    vector <double> FakeFractionBarrel_;
//    vector <double> FakeFractionEndcap_;
//    vector <double> FakeFractionBarrelErr_;
//    vector <double> FakeFractionEndcapErr_;
    float _minVarFit[3]; //sIEtaIEtaBarrelMin_ = 0.007;
    float _maxVarFit[_nBinsMax][3];

    //TTree** trData_;
    //TTree** trSignal_;
    //TTree** trFake_;
    //TTree** trLeakage_;
};

const static int   _nOfnBinsLeft=5;
const static float _nBinsLeftVariants[_nOfnBinsLeft]={4,2,8,5,1};
const static int   _nOfnRightLimits=5;
const static float _nPhoSCRChIsoRightLimits[_nOfnRightLimits]={14,12,10,8,6};

//const static int   _nOfnBinsLeft=1;
//const static float _nBinsLeftVariants[_nOfnBinsLeft]={4};
//const static int   _nOfnRightLimits=1;
//const static float _nPhoSCRChIsoRightLimits[_nOfnRightLimits]={6};


#endif //TTemplates_h
