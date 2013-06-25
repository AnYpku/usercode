#include "CalcCrossSection.h" 
  //this class
#include "../Configuration/TConfiguration.h" 
#include "../Configuration/TInputSample.h"
#include "../Configuration/TAllInputSamples.h"
  //this package
  //currently in this package
#include <string>
#include <iostream>
  //standard C++ class
#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
  //ROOT class

CalcCrossSection::CalcCrossSection(int channel, string configFile)
{

  INPUT_ = new TAllInputSamples(channel, configFile);

  channel_=channel;

  vecPhoPtLimits_ = config_.GetPhoPtBinsLimits();
  for (int i=0; i<config_.GetNPhoPtBins(); i++){
    signalYields1D_.push_back(0.0);
    signalYieldsErr1D_.push_back(0.0);
  }
  signalYieldTotal_=0;
  signalYieldErrTotal_=0;
}

CalcCrossSection::~CalcCrossSection()
{

}

void CalcCrossSection::Calc(string cut)
{
  GetSignalYields(cut);
  ApplyAccAndEff();
  DivideOverLumi();
  DivideOverBinWidth();
  SaveOutput();
}

void CalcCrossSection::GetSignalYields(string cut)
{
  for (int i=0; i<config_.GetNPhoPtBins(); i++){
    signalYields1D_.push_back(0.0);
    signalYieldsErr1D_.push_back(0.0);
  }
  signalYieldTotal_=0;
  signalYieldErrTotal_=0;

  int nSources = INPUT_->nSources_;
  for (int iSource=0; iSource<nSources; iSource++)
    {
       int sample = INPUT_->allInputs_[iSource].sample_;
       if (sample==TInputSample::SIGMC) continue;
       TString fileName = (TString)(INPUT_->allInputs_[iSource].fileSelected_).c_str();
       TFile f(fileName);
       TTree* tr = (TTree *)f.Get("selectedEvents");

       TH1F *hist = new TH1F("hist","hist",10,tr->GetMinimum("phoEt"),tr->GetMaximum("phoEt"));
       tr->Draw("phoEt>>hist","("+TString(cut.c_str())+")*weight","goff");
       TH1F *histErr = new TH1F("histErr","histErr",10,tr->GetMinimum("phoEt"),tr->GetMaximum("phoEt"));
       tr->Draw("phoEt>>histErr","("+TString(cut.c_str())+")*weight*weight","goff");

       if (sample==TInputSample::DATA) 
         signalYieldTotal_+=hist->Integral();
       else if (sample==TInputSample::BKGMC)
         signalYieldTotal_-=hist->Integral();

       signalYieldErrTotal_+=histErr->Integral();
       delete hist;
       delete histErr;

       for (int i=0; i<config_.GetNPhoPtBins(); i++){
         TH1F *hist1D = new TH1F("hist1D","hist1D",10,vecPhoPtLimits_[i],vecPhoPtLimits_[i+1]);
         tr->Draw("phoEt>>hist1D","("+TString(cut.c_str())+")*weight","goff");
         TH1F *hist1DErr = new TH1F("hist1DErr","hist1DErr",10,vecPhoPtLimits_[i],vecPhoPtLimits_[i+1]);
         tr->Draw("phoEt>>hist1DErr","("+TString(cut.c_str())+")*weight*weight","goff");

         if (sample==TInputSample::DATA) 
           signalYields1D_[i]+=hist1D->Integral();
         else if (sample==TInputSample::BKGMC)
           signalYields1D_[i]-=hist1D->Integral();

         signalYieldsErr1D_[i]+=hist1DErr->Integral();
         delete hist1D;
         delete hist1DErr;
       } //loop over phoEt bins ends: for (int i=0; i<config_.GetNPhoPtBins(); i++)
                
    } //loop over iSource ends
  signalYieldErrTotal_=sqrt(signalYieldErrTotal_);
  for (int i=0; i<config_.GetNPhoPtBins(); i++)
    signalYieldsErr1D_[i]=sqrt(signalYieldsErr1D_[i]);
  std::cout<<"Raw yields:"<<std::endl;
  std::cout<<"Total:"<<signalYieldTotal_<<"+-"<<signalYieldErrTotal_<<std::endl;
  for (int i=0; i<config_.GetNPhoPtBins(); i++)
    std::cout<<"phoEt "<<vecPhoPtLimits_[i]<<"-"<<vecPhoPtLimits_[i+1]<<" GeV: "<<signalYields1D_[i]<<"+-"<<signalYieldsErr1D_[i]<<std::endl;
}

void CalcCrossSection::ApplyAccAndEff()
{

}

void CalcCrossSection::DivideOverLumi()
{

}

void CalcCrossSection::DivideOverBinWidth()
{

}

void CalcCrossSection::SaveOutput()
{

}
