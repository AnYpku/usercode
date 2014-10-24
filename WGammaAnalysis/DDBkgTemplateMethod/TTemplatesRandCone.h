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

#include "../Configuration/TConfiguration.h"
#include "../Configuration/TAllInputSamples.h"
#include "../Include/TPhotonCuts.h"

class TTemplatesRandCone
{
  public:
    TTemplatesRandCone(int channel, int vgamma, int phoWP, TString varKin, int nKinBins, float* kinBinLims, bool isMetCutOptimization);
    virtual ~TTemplatesRandCone();


    void ComputeBackground(bool noPrint=0, bool noPlot=0);
    void ComputeBackgroundOne(int ikin, bool noPrint=0, bool noPlot=0);
    void SetHists(int kinBin, int etaBin, bool noPrint=0);
    void RandomizeTemplates(int ikin, int ieta);
    void DeleteHists(int kinBin, int etaBin);

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
    int _vgamma;
    int _channel;
    bool _isMetCutOptimization;
    int _selectionStage;

    TCut _cutWeight;

    int _phoWP;
    TAllInputSamples* _INPUT;
    TFile* _fData;
    TTree* _treeData;

    TString _varKin;
    int _nKinBins;
    float* _kinBinLims;

    TRandom _random;

    TH1D* _hTrue[_nBinsMax][3];
    TH1D* _hFake[_nBinsMax][3];
    TH1D* _hData[_nBinsMax][3];
    TH1D* _hSumm[_nBinsMax][3];
    TH1D* _hRatio[_nBinsMax][3];
    RooPlot* _plotter[_nBinsMax][3];
    double _chi2ToNDF[_nBinsMax][3];

    double _nFakeFromFitVal[_nBinsMax][3];
    double _nFakeFromFitErr[_nBinsMax][3];
    double _nTrueFromFitVal[_nBinsMax][3];
    double _nTrueFromFitErr[_nBinsMax][3];

    const static int _nToys=100;
    //double _nFakeFromFitToyVal[_nBinsMax][3][_nToys];
    //double _nTrueFromFitToyVal[_nBinsMax][3][_nToys];

    int _nFitBins[_nBinsMax][3];

    double _nFakeYieldsVal[_nBinsMax][3];
    double _nFakeYieldsErr[_nBinsMax][3];
    double _nTrueYieldsVal[_nBinsMax][3];
    double _nTrueYieldsErr[_nBinsMax][3];
    // strictly, Fake and True are not background and signal
    // but Fake is fake gamma Fake
    // and True is signal plus true gamma Fake

    TFile* _fOutForSave;

    float _maxVarFit[_nBinsMax][3];

};


#endif //TTemplatesRandCone_h
