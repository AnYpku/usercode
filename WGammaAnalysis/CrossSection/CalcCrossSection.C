#include "CalcCrossSection.h" 
  //this class
#include "../Configuration/TConfiguration.h" 
#include "../Configuration/TInputSample.h"
#include "../Configuration/TAllInputSamples.h"
#include "../Include/TMathTools.h"
  //this package
  //currently in this package
#include <string>
#include <iostream>
  //standard C++ class
#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TVectorF.h"
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TAxis.h"
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
  PlotAndSaveOutput();
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
       if (sample==TInputSample::DATA)
         lumi_=INPUT_->allInputs_[iSource].lumiTotal_;
       TString fileName = (TString)(INPUT_->allInputs_[iSource].fileSelected_).c_str();
       TFile f(fileName);
       if (!f.IsOpen()) continue;
       bool hasTree = f.GetListOfKeys()->Contains("selectedEvents");
       if (!hasTree) continue;
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
  TString fName=(config_.GetAccEffDirName()).c_str();
  fName+=(config_.GetAccEffFileName()).c_str();
  TFile f(fName,"read");
  TVectorF* vacc = (TVectorF*)f.Get((config_.GetAcc1DName()).c_str());
  TVectorF* veff = (TVectorF*)f.Get((config_.GetEff1DName()).c_str());
  TVectorF* vaccErr = (TVectorF*)f.Get((config_.GetAccErr1DName()).c_str());
  TVectorF* veffErr = (TVectorF*)f.Get((config_.GetEffErr1DName()).c_str());
  TVectorF* acc = (TVectorF*)f.Get((config_.GetAccTotalName()).c_str());
  TVectorF* eff = (TVectorF*)f.Get((config_.GetEffTotalName()).c_str());
  TVectorF* accErr = (TVectorF*)f.Get((config_.GetAccErrTotalName()).c_str());
  TVectorF* effErr = (TVectorF*)f.Get((config_.GetEffErrTotalName()).c_str());

  TMathTools math;
  signalYieldErrTotal_=math.ErrOfThreeIndependent("x1/(x2*x3)",signalYieldTotal_,acc->operator()(0),eff->operator()(0),signalYieldErrTotal_,accErr->operator()(0),effErr->operator()(0));
  signalYieldTotal_=signalYieldTotal_/((acc->operator()(0))*(eff->operator()(0)));
  for (int i=0; i<config_.GetNPhoPtBins(); i++){
    signalYieldsErr1D_[i]=math.ErrOfThreeIndependent("x1/(x2*x3)",signalYields1D_[i],vacc->operator()(i),veff->operator()(i),signalYieldsErr1D_[i],vaccErr->operator()(i),veffErr->operator()(i));
    signalYields1D_[i]=signalYields1D_[i]/((vacc->operator()(i))*(veff->operator()(i)));
  }
  std::cout<<"Acc and eff corrected yields:"<<std::endl;
  std::cout<<"Total:"<<signalYieldTotal_<<"+-"<<signalYieldErrTotal_<<std::endl;
  for (int i=0; i<config_.GetNPhoPtBins(); i++)
    std::cout<<"phoEt "<<vecPhoPtLimits_[i]<<"-"<<vecPhoPtLimits_[i+1]<<" GeV: "<<signalYields1D_[i]<<"+-"<<signalYieldsErr1D_[i]<<std::endl;

}

void CalcCrossSection::DivideOverLumi()
{
  signalYieldTotal_=signalYieldTotal_/lumi_;
  signalYieldErrTotal_=signalYieldErrTotal_/lumi_;
  for (int i=0; i<config_.GetNPhoPtBins(); i++){
    signalYields1D_[i]=signalYields1D_[i]/lumi_;
    signalYieldsErr1D_[i]=signalYieldsErr1D_[i]/lumi_;
  }
  std::cout<<"yields over lumi:"<<std::endl;
  std::cout<<"Total:"<<signalYieldTotal_<<"+-"<<signalYieldErrTotal_<<std::endl;
  for (int i=0; i<config_.GetNPhoPtBins(); i++)
    std::cout<<"phoEt "<<vecPhoPtLimits_[i]<<"-"<<vecPhoPtLimits_[i+1]<<" GeV: "<<signalYields1D_[i]<<"+-"<<signalYieldsErr1D_[i]<<std::endl;
}

void CalcCrossSection::DivideOverBinWidth()
{
  for (int i=0; i<config_.GetNPhoPtBins(); i++){
    signalYields1D_[i]=signalYields1D_[i]/(vecPhoPtLimits_[i+1]-vecPhoPtLimits_[i]);
    signalYieldsErr1D_[i]=signalYieldsErr1D_[i]/(vecPhoPtLimits_[i+1]-vecPhoPtLimits_[i]);
  }
  std::cout<<"Cross section, pb:"<<std::endl;
  std::cout<<"Total:"<<signalYieldTotal_<<"+-"<<signalYieldErrTotal_<<std::endl;
  std::cout<<"Differential, pb/GeV:"<<std::endl;
  for (int i=0; i<config_.GetNPhoPtBins(); i++)
    std::cout<<"phoEt "<<vecPhoPtLimits_[i]<<"-"<<vecPhoPtLimits_[i+1]<<" GeV: "<<signalYields1D_[i]<<"+-"<<signalYieldsErr1D_[i]<<std::endl;
}

void CalcCrossSection::PlotAndSaveOutput()
{
  TVectorF csTotal(1);
  TVectorF csErrTotal(1);
  TVectorF cs1D(config_.GetNPhoPtBins());
  TVectorF csErr1D(config_.GetNPhoPtBins());

  csTotal[0]=signalYieldTotal_;
  csErrTotal[0]=signalYieldErrTotal_;
  for (int i=0; i<config_.GetNPhoPtBins(); i++){
    cs1D[i]=signalYields1D_[i];
    csErr1D[i]=signalYieldsErr1D_[i];
  }
  
  TVectorF phoBins(config_.GetNPhoPtBins());
  TVectorF phoBinsErr(config_.GetNPhoPtBins());
  for (int i=0; i<config_.GetNPhoPtBins(); i++){
    phoBins[i]=0.5*(vecPhoPtLimits_[i+1]+vecPhoPtLimits_[i]);
    phoBinsErr[i]=0.5*(vecPhoPtLimits_[i+1]-vecPhoPtLimits_[i]);
  }

  TCanvas c("cCS","cCS");
  TGraphErrors grCS(phoBins,cs1D,phoBinsErr,csErr1D);
  TAxis* xAx=grCS.GetXaxis();
  xAx->SetTitle("Pt_{#gamma}, GeV/c");
  grCS.GetXaxis()->SetMoreLogLabels(kTRUE);
  TAxis* yAx=grCS.GetYaxis();
  yAx->SetTitle("d#sigma/dPt_{#gamma}, pb x c/GeV");
  c.SetLogx();
  c.SetLogy();
  grCS.SetLineWidth(2);
  grCS.Draw("AP");
  c.SaveAs("cCS.png");
  c.SaveAs("cCS.root");

}
