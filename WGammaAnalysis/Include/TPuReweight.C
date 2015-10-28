#include "TPuReweight.h"
  //this class
#include "TFile.h"
#include "TH1D.h"
#include "TH1F.h"
#include "TString.h"
  //root classes

#include <iostream>
#include <string>
#include <vector>
  //C++ classes

TPuReweight::TPuReweight(){
}

TPuReweight::TPuReweight(TString fileNameData, TString fileNameMC){
  _fPuData = new TFile(fileNameData);
  bool hasPuHist = _fPuData->GetListOfKeys()->Contains("pileup");
  if (hasPuHist) _hDataPU=(TH1F*)_fPuData->Get("pileup");
  else {
    std::cout<<"ERROR detected in TPuReweight::TPuReweight: no histogram pileup in file "<<fileNameData<<std::endl;
    return;
  }
  _fPuMC = new TFile(fileNameMC);
  _fPuMC->cd("ggNtuplizer");
  _hMCPU   = (TH1F*)gDirectory->Get("hPU");

  for (int ipu=0; ipu<101; ipu++)
    _npu_probs[ipu]=_hMCPU->GetBinContent(ipu+1)/_hMCPU->GetEntries();

  double s = 0;
  for (int ipu=0; ipu<101; ipu++){
    _npu_estimated[ipu] = _hDataPU->GetBinContent(_hDataPU->GetXaxis()->FindBin(ipu));
    s+=_npu_estimated[ipu];
  }

  for (int ipu=0; ipu<101; ipu++){
    _npu_estimated[ipu] = _npu_estimated[ipu]/s;
    if (_npu_probs[ipu]!=0) _puWeightArray[ipu]=_npu_estimated[ipu]/_npu_probs[ipu];
    else _puWeightArray[ipu]=0;
    std::cout<<"ipu="<<ipu<<", npu_probs="<<_npu_probs[ipu]<<", npu_estimated="<<_npu_estimated[ipu]<<", puWeight="<<_puWeightArray[ipu]<<std::endl;
  }
}// end of TPuReweight

TPuReweight::~TPuReweight(){
  _hDataPU=0;
  delete (_fPuData);
  _hMCPU=0;
  _trMC=0;
  delete (_fPuMC);
}// end of ~TPuReweight

float TPuReweight::GetPuWeightMc(float puTrue){
  float nTruePU = puTrue;
//  float puSize = 101;
  int iPU = (int)nTruePU;//(nTruePU*puSize/100.0);
//  std::cout<<"puTrue="<<puTrue<<", iPU="<<iPU<<", _puWeightArray[iPU]="<<_puWeightArray[iPU]<<std::endl;
  return _puWeightArray[iPU];
}// end of GetPuWeightMc
