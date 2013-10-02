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

  for(int i = 0; i < hDataPU_->GetNbinsX(); i++){ 
    if ( hMCPU_->GetBinContent(i + 1) > 0 ) 
      puWeightArray_.push_back(hDataPU_->GetBinContent(i + 1)/hMCPU_->GetBinContent(i + 1));
    else
      puWeightArray_.push_back(0);
  }

}

TPuReweight::~TPuReweight(){
  hDataPU_=0;
  delete (fPuData_);
  hMCPU_=0;
  trMC_=0;
  delete (fPuMc_);
}

float TPuReweight::GetPuWeightMc(float puTrue){
  float nTruePU = puTrue;
  float puSize = puWeightArray_.size();
  float iPU = nTruePU*puSize/100.0;
  return puWeightArray_.at((int)iPU);
}
