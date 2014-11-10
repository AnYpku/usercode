#ifndef TPrepareYields_h
#define TPrepareYields_h

#include "../Configuration/TConfiguration.h"
#include "../Configuration/TAllInputSamples.h"
#include "../Include/TPhotonCuts.h"

#include "TH1F.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TTree.h"
#include "THStack.h"
#include "TLatex.h"

#include <vector>

class TPrepareYields
{
  public:
    TPrepareYields(int year,int channel, int vgamma, int blind, TString varKin, int nKinBins, float* kinBinLims, int phoWP=TPhotonCuts::WP_MEDIUM, bool noDDBkgComputation=0);
    virtual ~TPrepareYields();
    void PrepareYields();
    void SetYields();
    void SetYieldsOneSource(int iSource, int ieta);
    void SetYieldsDATA(TTree* tr, TString yieldsName, int iSource, int ieta);
    void SetYieldsSIGMC(TTree* tr, TString yieldsName, TString yieldsGenName, int iSource, int ieta);
    void SetYieldsBKGMC_all(TTree* tr, TString yieldsName, int iSource, int ieta);
    void SetYieldsBKGMC_true(TTree* tr, TString yieldsName, int iSource, int ieta);
    void SetYieldsBKGMC_fake(TTree* tr, TString yieldsName, int iSource, int ieta);
    void SetYieldsDDBkgTemplate(int ieta);
    void SubtractBackground();
    void CompareFakeBkgDDvsMC(int ieta);
    void CompareTrueBkgDDvsMC(int ieta);
    void CompareTotalDATAvsDDFakePlusTrue(int ieta);
    void CompareTotalDATAvsSIGplusBKG(int ieta);
    void CompareTotalDATAvsMC(int ieta);
    void CompareSignalDATAvsMC(int ieta);
    void CompareStackVsHist(TString plotTitle, TH1F* hist1, TH1F* hist2, TLegend* legend, TCanvas* canv, bool isStack=0, THStack* stack=0);
    void PrintYields();
    void PrintYieldsOne(TString strYieldType, float totVal, float totErr, TH1F* yieldHist);
    void StoreYields();

    void SetTotalYield(TTree* tr, TCut cut, float& val, float& err);
    TCut CutEta(int ieta);
    TString StrLabelEta(int ieta);

  private:
    TConfiguration _config;
    int _channel;
    int _vgamma;
    int _blind;
    bool _noDDBkgComputation;
    TAllInputSamples* _INPUT;
    TPhotonCuts _emptyPhoton;
//    TCut _cutAdd;
    TCut _cutWeight;
    TCut _cutKin;

    bool _doLogX;
    bool _doLogY;

    TString _varKin;
    int _nKinBins;
    float* _kinBinLims;
    TString _varKinLabel;

    TFile* _fOut;

    TH1F* _dataYields[3];
    float _dataYieldTot[3];
    float _dataYieldTotErr[3];
    TString _strDATAName;
    TString _strDATALabel;
    int _colorDATA;
    
    TH1F* _sigMCYields[3];
    float _sigMCYieldTot[3];
    float _sigMCYieldTotErr[3];
    TString _strSIGMCName;
    TString _strSIGMCLabel;
    int _colorSIGMC;


    TH1F* _sigMCGenYields[3];
    float _sigMCGenYieldTot[3];
    float _sigMCGenYieldTotErr[3];

    vector <TH1F*> _vecBkgMCYields[3];
    vector <float> _vecBkgMCYieldTot[3];
    vector <float> _vecBkgMCYieldTotErr[3];
    vector <TString> _vecStrBkgMCNames;
    vector <TString> _vecStrBkgLabels;
    vector <int> _colorsBkg;

    vector <TH1F*> _vecBkgMCTrueGammaYields[3];
    vector <float> _vecBkgMCTrueGammaYieldTot[3];
    vector <float> _vecBkgMCTrueGammaYieldTotErr[3];
    vector <TString> _vecStrBkgMCTrueNames;
    vector <TString> _vecStrBkgTrueLabels;
    vector <int> _colorsBkgTrue;

    vector <TH1F*> _vecBkgMCFakeGammaYields[3];
    vector <float> _vecBkgMCFakeGammaYieldTot[3];
    vector <float> _vecBkgMCFakeGammaYieldTotErr[3];
    vector <TString> _vecStrBkgMCFakeNames;
    vector <TString> _vecStrBkgFakeLabels;
    vector <int> _colorsBkgFake;

    TH1F* _DDFakeGammaYields[3];
    float _DDFakeGammaYieldTot[3];
    float _DDFakeGammaYieldTotErr[3];

    TH1F* _DDTrueGammaYields[3];
    float _DDTrueGammaYieldTot[3];
    float _DDTrueGammaYieldTotErr[3];


    TH1F* _signalDataYields[3];
    float _signalDataYieldTot[3];
    float _signalDataYieldTotErr[3];

    TH1F* _floatingHist;


 //   TLegend* _legend;

    TCanvas* _canvFakeDDvsMC[3];
    TCanvas* _canvTrueDDvsMC[3];
    TCanvas* _canvTotalDATAvsDDFakePlusTrue[3];
    TCanvas* _canvTotalDATAvsSIGplusBKG1[3];
    TCanvas* _canvTotalDATAvsSIGplusBKG2[3];
    TCanvas* _canvSignalDATAvsMC[3];
    TCanvas* _canvTotalDATAvsMC[3];
    

};


#endif //TPrepareYields_h
