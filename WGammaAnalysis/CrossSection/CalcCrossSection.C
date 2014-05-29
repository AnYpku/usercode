#include "CalcCrossSection.h" 
  //this class
#include "../Configuration/TConfiguration.h" 
#include "../Configuration/TInputSample.h"
#include "../Configuration/TAllInputSamples.h"
#include "../Include/TMathTools.h"
#include "../Unfolding/Unfolding.h"
  //this package
  //currently in this package
#include <string>
#include <iostream>
#include <iomanip>
  //standard C++ class
#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TVectorF.h"
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TAxis.h"
  //ROOT class

CalcCrossSection::CalcCrossSection(int channel, int blind, string configFile)
{
  _INPUT = new TAllInputSamples(channel, configFile);
  _channel=channel;
  _fOut = new TFile("fCrossSection.root","recreate");
  for (int iSource=0; iSource<_INPUT->nSources_; iSource++){
       if (_INPUT->allInputs_[iSource].sample_==_config.DATA){
           _lumi=_INPUT->allInputs_[iSource].lumiTotal_;
           if (blind==_config.BLIND_PRESCALE) 
             _lumi=_lumi/_config.GetBlindPrescale();
           break;
       }
  }
}

CalcCrossSection::~CalcCrossSection()
{

}

void CalcCrossSection::Calc()
{
  GetSignalYields();
  ApplyEfficiency();
  ApplyUnfolding();
  ApplyAcceptance();
  DivideOverLumi();
  DivideOverBinWidth();
  Plot();
}

void CalcCrossSection::GetSignalYields()
{
  TFile* fSig = new TFile(_config.GetYieldsFileName(_channel));
  std::cout<<"file with signal yields: "<<_config.GetYieldsFileName(_channel)<<std::endl;
  std::cout<<"total yields: "<<_config.GetYieldsSignalName(_config.TOTAL)<<std::endl;
  std::cout<<"1D yields: "<<_config.GetYieldsSignalName(_config.ONEDI)<<std::endl;
  _signalYieldTotal=(TH1F*)fSig->Get(_config.GetYieldsSignalName(_config.TOTAL));
  _signalYields1D=(TH1F*)fSig->Get(_config.GetYieldsSignalName(_config.ONEDI));
  _fOut->cd();
  _signalYieldTotal->Write("signalYieldTotal"); 
  _signalYields1D->Write("signalYield1D");
  Print("Signal Yields:");
}

void CalcCrossSection::ApplyUnfolding()
{
  Unfolding unf(_channel);
  bool isOk = unf.PrepareMigrationMatrix();
  if (!isOk){
    std::cout<<"ERROR: PrepareMigrationMatrix() for Unfolding failed"<<std::endl;
    return;
  }
//  TH1D sign1D;
//  _signalYields1D->Copy(sign1D);
  TH1D* signInput1D = (TH1D*)_signalYields1D;
  TH1D* signUnfolded1D = (TH1D*)signInput1D->Clone("hUnfolded");
  isOk = unf.ApplyRooUnfold(signInput1D,signUnfolded1D);
  if (!isOk){
    std::cout<<"ERROR: ApplyRooUnfold() for Unfolding failed"<<std::endl;
    return;
  } 
  unf.PlotAndStore();
  Print("Yields before Unfolding:");
  _signalYields1D=(TH1F*)signUnfolded1D;
  Print("Unfolded Yields:");
//  unf.PlotAndStore();
}

void CalcCrossSection::ApplyEfficiency()
{
  TFile* fEff = new TFile(_config.GetEffFileName(_channel));
  TH1F* hEffTot = (TH1F*)fEff->Get(_config.GetEffName(_config.TOTAL));
  TH1F* hEff1D = (TH1F*)fEff->Get(_config.GetEffName(_config.ONEDI));
  _signalYieldTotal->Divide(hEffTot);
  _signalYields1D->Divide(hEff1D);
  Print("Efficiency Corrected Yields:");
}

void CalcCrossSection::ApplyAcceptance()
{
  TFile* fAcc = new TFile(_config.GetAccFileName(_channel));
  TH1F* hAccTot = (TH1F*)fAcc->Get(_config.GetAccName(_config.TOTAL));
  TH1F* hAcc1D = (TH1F*)fAcc->Get(_config.GetAccName(_config.ONEDI));
  _signalYieldTotal->Divide(hAccTot);
  _signalYields1D->Divide(hAcc1D);
  Print("Acceptance Corrected Yields:");
}

void CalcCrossSection::DivideOverLumi()
{
  float cont=_signalYieldTotal->GetBinContent(1);
  float err=_signalYieldTotal->GetBinError(1);
  _signalYieldTotal->SetBinContent(1,cont/_lumi);
  _signalYieldTotal->SetBinError(1,err/_lumi);
  for (int ib=1; ib<=_signalYields1D->GetNbinsX(); ib++){
    cont=_signalYields1D->GetBinContent(ib);
    err=_signalYields1D->GetBinError(ib);
    _signalYields1D->SetBinContent(ib,cont/_lumi);
    _signalYields1D->SetBinError(ib,err/_lumi);
  }
  Print("Yields over Lumi (pb):");
}

void CalcCrossSection::DivideOverBinWidth()
{
  for (int ib=1; ib<=_signalYields1D->GetNbinsX(); ib++){
    float cont=_signalYields1D->GetBinContent(ib);
    float err=_signalYields1D->GetBinError(ib);
    float width=_signalYields1D->GetBinWidth(ib);
    _signalYields1D->SetBinContent(ib,cont/width);
    _signalYields1D->SetBinError(ib,err/width);
  }
  Print("Total (pb) and differential(pb/GeV) cross sections:");
}

void CalcCrossSection::Plot()
{
  TCanvas* canv=new TCanvas("cCS","cCS",1200,600);
  canv->Divide(1);
  TPad* pad1D = (TPad*)canv->GetPad(1);
  pad1D->SetLogx();
  pad1D->SetLogy();
  _signalYields1D->GetXaxis()->SetNoExponent();
  _signalYields1D->GetXaxis()->SetMoreLogLabels();
  _signalYields1D->SetLineWidth(2);
  _signalYields1D->SetTitle("PRELIMINARY: d#sigma/dP_{T}^{#gamma}, pb/GeV");
  pad1D->cd();
  _signalYields1D->Draw();
}

void CalcCrossSection::Print(TString strYields)
{
  std::cout<<std::endl;
  std::cout<<strYields<<std::endl;
  std::cout<<"tot: "<<_signalYieldTotal->GetBinContent(1)<<"+-"<<_signalYieldTotal->GetBinError(1)<<std::endl;
  std::cout<<"1D: "<<std::endl;
  for (int ib=1; ib<=_signalYields1D->GetNbinsX(); ib++){
    std::cout<<_signalYields1D->GetBinLowEdge(ib)<<"-"<<_signalYields1D->GetBinLowEdge(ib)+_signalYields1D->GetBinWidth(ib)<<": "<<_signalYields1D->GetBinContent(ib)<<"+-"<<_signalYields1D->GetBinError(ib)<<std::endl;
  }
  std::cout<<std::endl;
}
