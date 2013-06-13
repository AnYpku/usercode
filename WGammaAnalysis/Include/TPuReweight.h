#ifndef TPuReweight_h
#define TPuReweight_h

#include "TFile.h"
#include "TH1D.h"
#include "TH1F.h"
#include "TTree.h"
  //root classes
#include <vector>
  //C++ classes

class TPuReweight
{
  public:
    TPuReweight();
    TPuReweight(string fileName, string fileNameMc);
    virtual ~TPuReweight();
    float GetPuWeightMc(float puTrue);
  private:
    TFile *fPuData_;
    TH1F *hDataPU_;
    TFile *fPuMc_;
    TTree *trMC_;
    TH1F *hMCPU_;
    vector <float> puWeightArray_;

};
#endif
