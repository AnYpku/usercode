#include "TPuReweight.h"
  //this class
#include "TFile.h"
#include "TH1D.h"
#include "TH1F.h"
  //root classes

#include <iostream>
#include <string>
  //C++ classes

TPuReweight::TPuReweight(){
}

TPuReweight::TPuReweight(string fileNameData, string fileNameMc){
  fPuData_ = new TFile(fileNameData.c_str());
  bool hasPuHist = fPuData_->GetListOfKeys()->Contains("pileup");
  if (hasPuHist) hPuData_=(TH1D*)fPuData_->Get("pileup");
  else std::cout<<"ERROR detected in TPuReweight::TPuReweight: no histogram pileup in file "<<fileNameData<<std::endl;
  fPuMc_ = new TFile(fileNameMc.c_str());
  fPuMc_->cd("ggNtuplizer");
  hasPuHist = gDirectory->GetListOfKeys()->Contains("hPU");
  if (hasPuHist) hPuMc_=(TH1F*)gDirectory->Get("hPU");
  else std::cout<<"ERROR detected in TPuReweight::TPuReweight: no histogram hPU in file "<<fileNameMc<<std::endl;
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
  return 1;
}
