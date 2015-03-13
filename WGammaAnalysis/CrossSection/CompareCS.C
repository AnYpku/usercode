#include "TFile.h"
#include "TH1F.h"
#include "TString.h"
#include "TLegend.h"
#include "TCanvas.h"

#include "../Configuration/TConfiguration.h"

#include <iostream>

void SetOttosHists(TFile* fOut, TH1F* h[2]);
float DifferenceInOneBin(TH1F* h1, TH1F* h2, int ib);

void CompareCS(int vgamma)
{
  TFile* fCS[2]; //MUON, ELECTRON
  TH1F* hMeasured1D[2];
  TH1F* hTheory1D[2];
  TH1F* hOtto1D[2];//Z_GAMMA only

  TConfiguration config;
  TFile* fOut=new TFile("fOut.root","recreate");
  SetOttosHists(fOut,hOtto1D);

  for (int ich=config.MUON; ich<=config.ELECTRON; ich++){
    fCS[ich]=new TFile(config.GetCrossSectionFileName(ich, vgamma));
    hMeasured1D[ich]=(TH1F*)fCS[ich]->Get(config.GetCSname(ich, config.ONEDI));
    hTheory1D[ich]=(TH1F*)fCS[ich]->Get(config.GetTheoryCSname(config.ONEDI));
    hTheory1D[ich]->SetLineWidth(2);
    hMeasured1D[ich]->SetLineWidth(2);
    hOtto1D[ich]->SetLineWidth(2);
  }//end of loop over ich

  hMeasured1D[config.MUON]->SetLineColor(1);
  hMeasured1D[config.ELECTRON]->SetLineColor(4);
  hTheory1D[config.MUON]->SetLineColor(2);
  hTheory1D[config.ELECTRON]->SetLineColor(6);
  hOtto1D[config.MUON]->SetLineColor(7);
  hOtto1D[config.ELECTRON]->SetLineColor(8);

  TLegend* leg = new TLegend(0.55,0.55,0.90,0.90);
  for (int ich=config.MUON; ich<=config.ELECTRON; ich++)
    leg->AddEntry(hMeasured1D[ich],config.StrChannel(ich)+TString(", measured"),"l");
  for (int ich=config.MUON; ich<=config.ELECTRON; ich++)
    leg->AddEntry(hTheory1D[ich],config.StrChannel(ich)+TString(", theory"),"l");
  if (vgamma==config.Z_GAMMA){
    for (int ich=config.MUON; ich<=config.ELECTRON; ich++)
      leg->AddEntry(hOtto1D[ich],config.StrChannel(ich)+TString(", Otto"),"l");
  }

  TString canvTitle = "c_CS_Compare_";
  canvTitle+=config.StrVgType(vgamma);
  TCanvas* canv = new TCanvas(canvTitle,canvTitle,600,600);
  canv->cd();
  canv->SetLogx();  canv->SetLogy();
  hMeasured1D[config.MUON]->SetTitle(("PRELIMINARY: d#sigma/dP_{T}^{#gamma}, fb/GeV, ")+TString(config.StrVgType(vgamma)));
  hMeasured1D[config.MUON]->Draw("EP");
  hMeasured1D[config.ELECTRON]->Draw("EP same");
  hTheory1D[config.MUON]->Draw("EP same");
  hTheory1D[config.ELECTRON]->Draw("EP same");
  if (vgamma==config.Z_GAMMA){
    hOtto1D[config.MUON]->Draw("EP same");
    hOtto1D[config.ELECTRON]->Draw("EP same");
  }
  leg->Draw("same");

  canv->SaveAs(TString("compareCS")+config.StrVgType(vgamma)+TString(".png"));
  canv->SaveAs(TString("compareCS")+config.StrVgType(vgamma)+TString(".pdf"));

  std::cout<<"MUO/ELE,   MEAS/THE(MUO), MEAS/THE(ELE) ";
  if (vgamma==config.Z_GAMMA) std::cout<<"MEAS/OTTO(MUO), MEAS/OTTO(ELE) ";
  std::cout<<std::endl;
  for (int ib=1; ib<=hMeasured1D[0]->GetNbinsX(); ib++){
    std::cout<<DifferenceInOneBin(hMeasured1D[0],hMeasured1D[1],ib)<<", ";
    std::cout<<DifferenceInOneBin(hMeasured1D[0],hTheory1D[0],ib)<<", ";
    std::cout<<DifferenceInOneBin(hMeasured1D[1],hTheory1D[1],ib)<<", ";
    if (vgamma==config.Z_GAMMA) {
      std::cout<<DifferenceInOneBin(hMeasured1D[0],hOtto1D[0],ib)<<", ";
      std::cout<<DifferenceInOneBin(hMeasured1D[1],hOtto1D[1],ib)<<", ";
    }
    std::cout<<std::endl;
  }//end of loop over ib

}//end of CompareCS

float DifferenceInOneBin(TH1F* h1, TH1F* h2, int ib)
{
  if (h1->GetNbinsX()<ib) return -1;
  if (h2->GetNbinsX()<ib) return -1;
  float den = h1->GetBinContent(ib)+h2->GetBinContent(ib);
  if (den<=0) return -1;
  return fabs(100*(h1->GetBinContent(ib)-h2->GetBinContent(ib))/den);
}//end of DifferenceInOneBin

void SetOttosHists(TFile* fOut, TH1F* h[2])
{
  //  TFile* fOut=new TFile("fOut.root","recreate");
  const int nBins=12;
  float lims[nBins+1]=      { 15,  20,  25,  30,    35,    45,   55,   65,   75,   85,  95,  120,  500};
  float csMuoVal[nBins]=     {  912, 491, 227, 128.7, 124.5, 69.9, 36.3, 17.6, 17.1, 9.2, 16.6, 15.1};
  float csMuoErrPercent[nBins]={ 5.3, 5.6, 6.5, 7.4,   7,     8.6,  11.8, 16,  15.6, 19.8,13.2, 10.8};
  float csEleVal[nBins]=     {  902, 485, 258, 144.2, 115.4, 77.1, 28.5, 22,   10.5, 11.1,15.8, 17.6};
  float csEleErrPercent[nBins]={5.5, 6,   7,    8.2,   8.1,   9.5, 14.5, 16.1, 22.5, 22.3,15.9, 12.3};
  fOut->cd();
  h[0] = new TH1F("hMuoOtto", "MUON, Otto", nBins, lims);
  h[1] = new TH1F("hEleOtto", "ELECTRON Otto", nBins,lims);
  std::cout<<"Otto's:"<<std::endl;
  for (int ib=1; ib<=nBins; ib++){
    float binWidth=lims[ib]-lims[ib-1];
    h[0]->SetBinContent(ib,csMuoVal[ib-1]/binWidth);
    h[1]->SetBinContent(ib,csEleVal[ib-1]/binWidth);
    h[1]->SetBinError(ib,csMuoErrPercent[ib-1]*csMuoVal[ib-1]/(100*binWidth));
    h[1]->SetBinError(ib,csEleErrPercent[ib-1]*csEleVal[ib-1]/(100*binWidth));
    std::cout<<lims[ib-1]<<"-"<<lims[ib]<<": "<<csMuoVal[ib-1]<<"/"<<binWidth<<"; "<<csEleVal[ib-1]<<"/"<<binWidth<<std::endl;
  }// emd of loop over ib
  h[0]->Print();
}// end of SetOttosHists
