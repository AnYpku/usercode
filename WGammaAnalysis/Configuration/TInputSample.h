#ifndef TInputSample_h
#define TInputSample_h

#include <string>
#include "TConfiguration.h"

class TInputSample
{
  public:
    TInputSample();
    virtual ~TInputSample();
    
    //void GetInputSample(TString configFile, int channel, int sample, TString bkgName="");
    void CalcLuminocities();
    void GetFileSelectedNames();
    void Print();

    enum {MUON, ELECTRON, BOTH};
    enum {DATA, SIGMC, BKGMC};

    int channelTotal_;
    int sample_;
    int nFiles_;
    string sourceName_;
    string sourceLatexLabel_;
    int color_;
    bool isSharedCS_;
    float lumiTotal_;
    float csTotal_;
    string fileSelected_;
    string fileSelectedDebug_;

    vector<string> fileNames_;
    vector<float> cs_;
    vector<float> factor_;
    vector<int> channelEachFile_;

    vector<float> lumiEachFile_;
    vector <float> lumiWeights_;
   
};

#endif //#ifndef TSample_h
