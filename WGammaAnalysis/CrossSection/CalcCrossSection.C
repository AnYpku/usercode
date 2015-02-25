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
#include "TLegend.h"
#include "TLine.h"
  //ROOT class

CalcCrossSection::CalcCrossSection(int channel, int vgamma, int blind, string configFile)
{
  _INPUT = new TAllInputSamples(channel, vgamma, configFile);
  _channel=channel;
  _vgamma=vgamma;
  _blind=blind;
  _fOut = new TFile("fCrossSection.root","recreate");
  for (int iSource=0; iSource<_INPUT->nSources_; iSource++){
       if (_INPUT->allInputs_[iSource].sample_==_config.DATA){
           _lumi=_INPUT->allInputs_[iSource].lumiTotal_;
           if (blind==_config.BLIND_PRESCALE) 
             _lumi=_lumi/_config.GetBlindPrescale();
           break;
       }
  }// end of iSource
}// end of CalcCrossSection

CalcCrossSection::~CalcCrossSection()
{

}// end of ~CalcCrossSection()

void CalcCrossSection::Calc()
{
  GetSignalYields();
    ApplyUnfolding();
  ApplyAccXEff();
  DivideOverLumi();
  DivideOverBinWidth();
    Plot();
}// Calc()

void CalcCrossSection::GetSignalYields()
{
  TFile* fSig = new TFile(_config.GetYieldsFileName(_channel, _vgamma, _config.TEMPL_CHISO, "phoEt"));
  std::cout<<"file with signal yields: "<<_config.GetYieldsFileName(_channel,_vgamma,_config.TEMPL_CHISO,"phoEt")<<std::endl;
  std::cout<<"total yields: "<<_config.GetYieldsBkgSubtrDataName(_config.TOTAL)<<std::endl;
  std::cout<<"1D yields: "<<_config.GetYieldsBkgSubtrDataName(_config.ONEDI)<<std::endl;
  _signalYieldTotal=(TH1F*)fSig->Get(_config.GetYieldsBkgSubtrDataName(_config.TOTAL));
  _signalYields1D=(TH1F*)fSig->Get(_config.GetYieldsBkgSubtrDataName(_config.ONEDI));
  _fOut->cd();
  _signalYieldTotal->Write("signalYieldTotal"); 
  _signalYields1D->Write("signalYield1D");
  Print("Signal Yields:");
}// end of GetSignalYields()

void CalcCrossSection::ApplyUnfolding()
{
  Unfolding unf(_channel,_vgamma);
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
}// end of ApplyUnfolding()

void CalcCrossSection::ApplyAccXEff()
{
  TFile* f = new TFile(_config.GetAccXEffFileName(_channel,_vgamma));
  TH1F* hAccXEffTot = (TH1F*)f->Get(_config.GetAccXEffName(_config.TOTAL));
  TH1F* hAccXEff1D = (TH1F*)f->Get(_config.GetAccXEffName(_config.ONEDI));
  _signalYieldTotal->Divide(hAccXEffTot);
  _signalYields1D->Divide(hAccXEff1D);
  Print("Acc X Eff Corrected Yields:");
}// end of ApplyAccXEff()

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
}// end of DivideOverLumi()

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
}// end of DivideOverBinWidth()

void CalcCrossSection::Plot()
{
  TString canvTitle="c_CS_";
  canvTitle+=_config.StrChannel(_channel);
  canvTitle+="_";
  canvTitle+=_config.StrVgType(_vgamma);
  canvTitle+="_";
  canvTitle+=_config.StrBlindType(_blind);
  TCanvas* canv=new TCanvas(canvTitle,canvTitle,800,800);
  canv->Divide(2,1);
  TPad* pad1 = (TPad*)canv->GetPad(1);
  TPad* pad2 = (TPad*)canv->GetPad(2);
  pad1->SetLogx();
  pad1->SetLogy();
  pad2->SetLogx();
  pad1->SetPad(0,0.3,1.0,1.0);
  pad2->SetPad(0,0,  1.0,0.28);
  pad1->SetLeftMargin(0.18);
  pad1->SetTopMargin(0.08);
  pad1->SetRightMargin(0.07);
  pad1->SetBottomMargin(0.01); // All X axis labels and titles are thus cut off
  pad2->SetLeftMargin(0.18);
  pad2->SetTopMargin(0.01);
  pad2->SetRightMargin(0.07);
  pad2->SetBottomMargin(0.45);
 TString fName=_config.GetAccXEffFileName(_channel, _vgamma);
  TFile* fTheory = new TFile(fName);
  TH1F* hTheory = (TH1F*)fTheory->Get(_config.GetTheoryCSname(_config.ONEDI));
  // Multiply by 1000, pb -> fb
  for (int ib=1; ib<=_signalYields1D->GetNbinsX(); ib++){
    float cont; float err;
    cont = _signalYields1D->GetBinContent(ib);
    err = _signalYields1D->GetBinError(ib);
    _signalYields1D->SetBinContent(ib,1000*cont);
    _signalYields1D->SetBinError(ib,1000*err);
    cont = hTheory->GetBinContent(ib);
    err = hTheory->GetBinError(ib);
    hTheory->SetBinContent(ib,1000*cont);
    hTheory->SetBinError(ib,1000*err);
  }// end of loop over ib
   Print("Total (pb) and differential(fb/GeV) cross sections:");
  pad1->cd();
  _signalYields1D->GetXaxis()->SetNoExponent();
  _signalYields1D->GetXaxis()->SetMoreLogLabels();
  _signalYields1D->SetLineWidth(2);
  TString strTitle="PRELIMINARY: d#sigma/dP_{T}^{#gamma}, fb/GeV, ";
  strTitle+=_config.StrChannel(_channel);
  strTitle+=" ";
  strTitle+=_config.StrVgType(_vgamma);
  strTitle+=" ";
  strTitle+=_config.StrBlindType(_blind);
  _signalYields1D->SetTitle(strTitle);
  _signalYields1D->SetStats(0);
  _signalYields1D->Draw();
  // Plot theory Cross section
  hTheory->SetLineWidth(2);
  hTheory->SetLineColor(2);
  hTheory->Draw("same");
  _signalYields1D->Draw("same");
  TLegend *leg = new TLegend(0.70,0.70,0.95,0.95);
  leg->AddEntry(_signalYields1D,"measured CS","l");
  leg->AddEntry(hTheory,"theory CS","l");
  leg->Draw("same");

  pad2->cd();
  TH1F* hRatio = (TH1F*) _signalYields1D->Clone("hRatio");
  hRatio->Divide(hTheory);
  float max=1.1; float min=0.9;
  for (int ib=1; ib<=hRatio->GetNbinsX(); ib++){
    if (hRatio->GetBinContent(ib)+hRatio->GetBinError(ib)>max) 
      max=hRatio->GetBinContent(ib)+hRatio->GetBinError(ib);
    if (hRatio->GetBinContent(ib)-hRatio->GetBinError(ib)<min) 
      min=hRatio->GetBinContent(ib)-hRatio->GetBinError(ib);
  }// end of loop over ib
  hRatio->SetStats(0);
  hRatio->SetLineWidth(2);
  hRatio->GetYaxis()->SetRangeUser(min,max);
  hRatio->GetYaxis()->SetLabelSize(0.1);
  hRatio->GetXaxis()->SetLabelSize(0.1);
  hRatio->GetXaxis()->SetTitleOffset(1.0);
  hRatio->GetXaxis()->SetTitleSize(0.12);
  hRatio->GetXaxis()->SetMoreLogLabels();
  hRatio->GetXaxis()->SetNoExponent();
  hRatio->SetTitle(";phoEt, GeV;");
  hRatio->Draw();

  int nBins = hRatio->GetNbinsX();
  TLine* line = new TLine(hRatio->GetBinLowEdge(1),1,hRatio->GetBinLowEdge(nBins)+hRatio->GetBinWidth(nBins),1);
  line->SetLineWidth(2);
  line->SetLineStyle(9);
  line->Draw("same");

  TString nameSave=_config.GetPlotsDirName(_channel,_vgamma,_config.PLOTS_CROSS_SECTION);
  nameSave+=canv->GetTitle();
  nameSave+=".png";
  canv->SaveAs(nameSave);
  nameSave.ReplaceAll(".png",".pdf");
  canv->SaveAs(nameSave);
  nameSave.ReplaceAll(".pdf",".root");
  canv->SaveAs(nameSave);
  
}// end of Plot()

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
}// end of Print()
