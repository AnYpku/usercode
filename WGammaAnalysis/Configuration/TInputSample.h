#ifndef TInputSample_h
#define TInputSample_h

#include <string>
#include "TString.h"
#include "TConfiguration.h"

class TInputSample
{
  public:
    TInputSample();
    virtual ~TInputSample();
    
    //void GetInputSample(TString configFile, int channel, int sample, TString bkgName="");
    void CalcLuminocities();
 //   void GetFileSelectedNames();
    void Print();

    //enum {MUON, ELECTRON, BOTH};
       //shifted to TConfiguration.h
//    enum {DATA, SIGMC, BKGMC};

    int channel_;
    int vgamma_;
    int sample_;
    int nFiles_;
    TString sourceName_;
    TString sourceLatexLabel_;
    int color_;
    bool isSharedCS_;
    float lumiTotal_;
    float csTotal_;
//    TString fileSelected_;
//    TString fileSelectedDebug_;

    vector<TString> fileNames_;
    vector<float> cs_;
    vector<float> factor_;

    vector<float> lumiEachFile_;
    vector <float> lumiWeights_;
   
};

#endif //#ifndef TSample_h
