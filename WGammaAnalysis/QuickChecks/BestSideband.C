#include "TTree.h"
#include "TString.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TFile.h" 
#include "TCanvas.h" 
#include "TGraph.h" 
#include "TCut.h" 
#include <iostream> 

void PlotHistsAndRatio(TCanvas* c1, TH1D* hData, TH1D* hSign);
void BestSideband(TTree* trData, TTree* trSign)
{
  TCut cutWeight("weight");
  TCut cutBarrel("phoSCEta>-1.4442 && phoSCEta<1.4442");
  TCut cutEndcap("(phoSCEta>-2.5 && phoSCEta<-1.566)||(phoSCEta<2.5 && phoSCEta>1.566)");
  TCut cutPhoSigmaIEtaIEta("phoSigmaIEtaIEta<1");
  TCut cutPt[10];
  cutPt[0]="phoEt>15";
  cutPt[1]="phoEt>15 && phoEt<20";
  cutPt[2]="phoEt>20 && phoEt<25";
  cutPt[3]="phoEt>25 && phoEt<30";
  cutPt[4]="phoEt>30 && phoEt<35";
  cutPt[5]="phoEt>35 && phoEt<40";
  cutPt[6]="phoEt>40 && phoEt<55";
  cutPt[7]="phoEt>55 && phoEt<75";
  cutPt[8]="phoEt>75 && phoEt<95";
  cutPt[9]="phoEt>95 && phoEt<500";
  
  TH1D* hDataB = new TH1D("hDataB", "hDataB", 20, 0.005,0.025);
  TH1D* hSignB = new TH1D("hSignB", "hSignB", 20, 0.005,0.025);
  hSignB->Sumw2();

  TH1D* hDataE = new TH1D("hDataE", "hDataE", 24, 0.019,0.067);
  TH1D* hSignE = new TH1D("hSignE", "hSignE", 24, 0.019,0.067);
  hSignE->Sumw2();

  for (int iptB=0; iptB<2; iptB++){
    trData->Draw("phoSigmaIEtaIEta>>hDataB",(cutPt[iptB] && cutBarrel)*cutWeight);
    trData->Draw("phoSigmaIEtaIEta>>hDataE",(cutPt[iptB] && cutEndcap)*cutWeight);

    trSign->Draw("phoSigmaIEtaIEta>>hSignB",(cutPt[iptB] && cutBarrel)*cutWeight);
    trSign->Draw("phoSigmaIEtaIEta>>hSignE",(cutPt[iptB] && cutEndcap)*cutWeight);

    TString cBName="cBestSidebandBarrel_";
    cBName+=cutPt[iptB].GetTitle();
    cBName.ReplaceAll(" && phoEt<","_");
    cBName.ReplaceAll(">","_");
    cBName.ReplaceAll("(","");
    cBName.ReplaceAll(")","");
    TString cEName="cBestSidebandEndcap_";
    cEName+=cutPt[iptB].GetTitle();
    cEName.ReplaceAll(" && phoEt<","_");
    cEName.ReplaceAll(">","_");
    cEName.ReplaceAll("(","");
    cEName.ReplaceAll(")","");
    TCanvas* cB = new TCanvas(cBName,cBName);
    TCanvas* cE = new TCanvas(cEName,cEName);

    PlotHistsAndRatio(cB, hDataB, hSignB);
    PlotHistsAndRatio(cE, hDataE, hSignE);
  }
}

void PlotHistsAndRatio(TCanvas* c1, TH1D* hData, TH1D* hSign)
{

  TString strRatio=hSign->GetName();
  strRatio+="_over_";
  strRatio+=hData->GetName();

  TH1D* hRatio=(TH1D*)hSign->Clone(strRatio);
  hRatio->Sumw2();  

  hRatio->Divide(hData);

    c1->Divide(1,2);
    TPad* pad1 = (TPad*)c1->GetPad(1);
    TPad* pad2 = (TPad*)c1->GetPad(2);
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

    pad1->cd();

    hData->SetTitle("");
    hSign->SetTitle("");
    hData->SetLineWidth(2);
    hData->Draw("HIST");
    hSign->SetLineColor(2);
    hSign->SetLineWidth(2);
    hSign->Draw("HIST same");
    
    pad2->cd();
    hRatio->SetLineWidth(2);
    hRatio->SetStats(0);
    hRatio->GetYaxis()->SetLabelSize(0.1);
    hRatio->GetXaxis()->SetLabelSize(0.1);
    hRatio->GetXaxis()->SetTitleOffset(1.0);
    hRatio->GetXaxis()->SetTitleSize(0.12);
    hRatio->SetTitle(TString("; pho #sigma_i#etai#eta ;"));
    hRatio->Draw();
   
    TString cName=c1->GetName();
    cName+=".png";
    c1->SaveAs(cName);

}
