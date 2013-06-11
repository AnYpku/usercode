#ifndef TPuReweight_h
#define TPuReweight_h

#include "TFile.h"
#include "TH1D.h"

class TPuReweight
{
  public:
    TPuReweight();
    TPuReweight(string fileName, string histName, string fileNameMc, string histNameMc);
    virtual ~TPuReweight();
    float GetPuWeightMc(int nPu);
  private:
    TFile *fPuData_;
    TH1D *hPuData_;
    TFile *fPuMc_;
    TH1D *hPuMc_;

};
#endif
