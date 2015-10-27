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
    TFile *fPuData_;
    TH1F *hDataPU_;
    TFile *fPuMc_;
    TTree *trMC_;
    TH1F *hMCPU_;
    float _puWeightArray[101];

};
#endif
