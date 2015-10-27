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

TPuReweight::TPuReweight(TString fileNameData, TString fileNameMc){
  fPuData_ = new TFile(fileNameData);
  bool hasPuHist = fPuData_->GetListOfKeys()->Contains("pileup");
  if (hasPuHist) hDataPU_=(TH1F*)fPuData_->Get("pileup");
  else {
    std::cout<<"ERROR detected in TPuReweight::TPuReweight: no histogram pileup in file "<<fileNameData<<std::endl;
    return;
  }
  fPuMc_ = new TFile(fileNameMc);
  fPuMc_->cd("ggNtuplizer");
  hMCPU_   = (TH1F*)hDataPU_->Clone();
  hMCPU_->Reset();
  hMCPU_->SetName("pileupMC");
  bool hasTree = gDirectory->GetListOfKeys()->Contains("EventTree");
  if (hasTree) trMC_=(TTree*)gDirectory->Get("EventTree");
  else {
    std::cout<<"ERROR detected in TPuReweight::TPuReweight: no tree EventTree in file "<<fileNameMc<<std::endl;
    return;
  }
  trMC_->Draw("puTrue[1]>>pileupMC","","goff");
    //"goff" option suppresses generating the graphics
  hDataPU_->Scale(1./hDataPU_->Integral(-1,1000));
  hMCPU_->Scale(1./hMCPU_->Integral(-1,1000));
  std::cout<<"hDataPU_:"<<std::endl;
  hDataPU_->Print();
  std::cout<<"hMCPU_:"<<std::endl;
  hMCPU_->Print();
  for(int i = 0; i < hDataPU_->GetNbinsX(); i++){ 
    if ( hMCPU_->GetBinContent(i + 1) > 0 ) 
      _puWeightArray[i+1] = hDataPU_->GetBinContent(i + 1)/hMCPU_->GetBinContent(i + 1);
    else
      _puWeightArray[i+1] = 0;
    std::cout<<"i="<<i<<", puWeightArray = "<<_puWeightArray[i+1]<<std::endl;
  }// end of loop over i
  for(int i = hDataPU_->GetNbinsX(); i<101; i++){ 
      _puWeightArray[i+1] = 0;
    std::cout<<"i="<<i<<", puWeightArray = "<<_puWeightArray[i+1]<<std::endl;
  }// end of loop over i
}// end of TPuReweight

TPuReweight::~TPuReweight(){
  hDataPU_=0;
  delete (fPuData_);
  hMCPU_=0;
  trMC_=0;
  delete (fPuMc_);
}// end of ~TPuReweight

float TPuReweight::GetPuWeightMc(float puTrue){
//  float nTruePU = puTrue;
//  float puSize = puWeightArray_.size();
  int iPU = (int)puTrue;//nTruePU*puSize/100.0;
//  std::cout<<"puTrue="<<puTrue<<", iPU="<<iPU<<", _puWeightArray[iPU]="<<_puWeightArray[iPU]<<std::endl;
  return _puWeightArray[iPU];
}// end of GetPuWeightMc
