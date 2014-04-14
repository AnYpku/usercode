#ifndef TTemplates_h
#define TTemplates_h

#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TCut.h"
#include "TString.h"

#include <vector>

#include "../Configuration/TConfiguration.h"
#include "../Configuration/TAllInputSamples.h"

class TTemplates
{
  public:
    TTemplates(int channel);
    virtual ~TTemplates();
    void ComputeBackground(bool noPrint=0, bool noPlot=0);
    void ComputeBackgroundOne(int ipt, bool noPrint=0, bool noPlot=0);
    void PrepareTemplatesOnePtBin(int ipt, bool noPrint=0);
    void SetThreeHists(int ptBin, int etaBin, bool noPrint=0);
    void DeleteThreeHists(int ptBin, int etaBin);

    void FitOne(int ptBin, int etaBin, bool noPrint=0, bool noPlot=0);
    void PrintBkgYieldsAndChi2();
    void ComputeBkgYieldOnePtBin(int ipt, bool noPrint=0);
    void ComputeBkgYieldOne(TH1D* hBkgr, double nBkgrVal, double nBkgrErr, double& nBkgrYieldVal, double& nBkgrYieldErr,int ieta, int ipt, bool noPrint=0);

    void SetSignalTemplate(TH1D* hSign, TCut cut);
    void SetBackgrTemplate(TH1D* hBkgr, TH1D* hLeak, TH1D* hLeakTemp, TCut cut, bool noPrint=0);

    void SaveBkgYields();

    TCut SidebandCut(TString varSideband);
    TCut CutPtBin(int ptBin);
    TCut CutEtaBin(int etaBin);
    TString StrLabelEta(int etaBin);
    TString StrLabelPt(int ptBin);

    double ValueCutNominalVarFit(TString varFit, int etaBin);
    void FractionOfSidebandVarInRange(int ipt, int ieta, double& frac, double& fracErr);

    void SelectOptimalNBinsLeft();

  private:
    TConfiguration _config;
    int _channel;
    TAllInputSamples* _INPUT;
    TFile* _fSign;
    TFile* _fData;
    vector <TFile*> _vecFBkg;
//    TFile* fOut_;

    TTree* _treeSign;
    TTree* _treeData;
    vector <TTree*> _vecTreeBkg;

    vector <float> _phoPtBinLimits;

    TString _varFit;
    TString _varSideband;
    TString _labelVarFit;

    const static int _nBinsMax=20;
    TH1D* _hSign[_nBinsMax][3];
    TH1D* _hBkgr[_nBinsMax][3];
    TH1D* _hLeak[_nBinsMax][3];
    double _chi2ToNDF[_nBinsMax][3];
    double _nBkgrVal[_nBinsMax][3];
    double _nBkgrErr[_nBinsMax][3];
    int _nBinsLeft[_nBinsMax][3];
    double _nBkgrYieldsVal[_nBinsMax];
    double _nBkgrYieldsErr[_nBinsMax];

//    double _nDataYieldsVal[_nBinsMax];
//    double _nDataYieldsErr[_nBinsMax];
//    double _nSignYieldsVal[_nBinsMax];
//    double _nSignYieldsErr[_nBinsMax];

    TH1D* _hSignTot[3];
    TH1D* _hBkgrTot[3];
    TH1D* _hLeakTot[3];
    double _chi2ToNDFTot[3];
    double _nBkgrValTot[3];
    double _nBkgrErrTot[3];
    int _nBinsLeftTot[3];
    double _nBkgrYieldsValTot;
    double _nBkgrYieldsErrTot;

    TFile* _fOutForSave;

//    vector <double> chi2Barrel_;
//    vector <double> chi2Endcap_;
//    vector <double> bkgFractionBarrel_;
//    vector <double> bkgFractionEndcap_;
//    vector <double> bkgFractionBarrelErr_;
//    vector <double> bkgFractionEndcapErr_;
    double _minVarFitB; //sIEtaIEtaBarrelMin_ = 0.007;
    double _maxVarFitB; //sIEtaIEtaBarrelMax_ = 0.020;
    double _minVarFitE; //sIEtaIEtaEndcapMin_ = 0.020;
    double _maxVarFitE; //sIEtaIEtaEndcapMax_ = 0.050;
    //TTree** trData_;
    //TTree** trSignal_;
    //TTree** trBkg_;
    //TTree** trLeakage_;
};


#endif //TTemplates_h
