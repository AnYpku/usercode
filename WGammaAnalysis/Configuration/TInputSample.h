#ifndef TInputSample_h
#define TInputSample_h

#include "TString.h"

class TInputSample
{
  public:
    TInputSample();
    TInputSample(int nFilesIn, 
      TString sourceNameIn,
      TString sourceLatexLabelIn,
      int colorIn,
      float lumiDataTotalIn,
      TString* fileNamesIn,
      float* lumiDataIn,
      float* lumiMCIn,
      float* csMCIn,
      bool* isSkimmedIn    );
    virtual ~TInputSample();
    
    void Print();

    const static int nFilesMax_=15;   
 
    int nFiles_;
    TString sourceName_;
    TString sourceLatexLabel_;
    int color_;
    float lumiDataTotal_;

    TString fileNames_[nFilesMax_];
    float lumiData_[nFilesMax_];
    float lumiMC_[nFilesMax_];
    float csMC_[nFilesMax_];
    bool isSkimmed_[nFilesMax_];    
};

#endif //#ifndef TSample_h
