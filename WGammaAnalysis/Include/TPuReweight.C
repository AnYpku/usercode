#include "TPuReweight.h"

TPuReweight::TPuReweight(){
}

TPuReweight::TPuReweight(string fileNameData, string histNameData, string fileNameMc, string histNameMc){
  fPuData_ = new TFile(fileNameData.c_str());
  hPuData_=(TH1D*)fPuData_->Get(histNameData.c_str());
  fPuMc_ = new TFile(fileNameMc.c_str());
  hPuMc_=(TH1D*)fPuMc_->Get(histNameMc.c_str());
}

TPuReweight::~TPuReweight(){
  hPuData_=0;
  delete (fPuData_);
  hPuMc_=0;
  delete (fPuMc_);
}

float TPuReweight::GetPuWeightMc(int nPu){
  int binData = hPuData_->FindBin(nPu);
  int binMc = hPuMc_->FindBin(nPu);
  return (hPuData_->GetBinContent(binData))/(hPuMc_->GetBinContent(binMc));
}
