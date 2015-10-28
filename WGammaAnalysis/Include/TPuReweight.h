#ifndef TPuReweight_h
#define TPuReweight_h

#include "TFile.h"
#include "TH1D.h"
#include "TH1F.h"
#include "TTree.h"
#include "TString.h"
  //root classes
#include <vector>
  //C++ classes

class TPuReweight
{
  public:
    TPuReweight();
    TPuReweight(TString fileName, TString fileNameMc);
    virtual ~TPuReweight();
    float GetPuWeightMc(float puTrue);
  private:
    TFile *_fPuData;
    TH1F *_hDataPU;
    TFile *_fPuMC;
    TTree *_trMC;
    TH1F *_hMCPU;
    float _puWeightArray[101];
    float _npu_probs[101];
    float _npu_estimated[101];

};
#endif
