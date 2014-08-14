#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TCut.h"
#include "TText.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "TString.h"
#include "TLine.h"
#include <iostream>
#include <iomanip>

void PlotOneComparison(TCanvas* canv, TH1F* hZg, TH1F* hWg, TH1F* hRatio)
{
  TString cName = canv->GetTitle();
  canv->Divide(1,2);
  TPad* pad1 = (TPad*)canv->GetPad(1);
  TPad* pad2 = (TPad*)canv->GetPad(2);
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
  float maxValue=0;
  bool isWFirst=1;
  for (int ib=1; ib<=hWg->GetNbinsX(); ib++){
    if (hWg->GetBinContent(ib)/hWg->GetSumOfWeights()>maxValue)
      maxValue=hWg->GetBinContent(ib)/hWg->GetSumOfWeights();
  }
  for (int ib=1; ib<=hZg->GetNbinsX(); ib++){
    if (hZg->GetBinContent(ib)/hZg->GetSumOfWeights()>maxValue){
      maxValue=hZg->GetBinContent(ib)/hZg->GetSumOfWeights();
      isWFirst=0;
    }
  }
  TString strVar=hWg->GetTitle();
  strVar.ReplaceAll("hWg","Zg and Wg: ");
  hWg->SetTitle("");
  hZg->SetTitle("");
  hWg->SetStats(0);
  hZg->SetStats(0);
  hRatio->SetTitle("");
  hWg->SetLineWidth(2);
  hWg->SetLineColor(2);
  hZg->SetLineWidth(2);
  hZg->SetLineColor(4);
  if (isWFirst){
    hWg->DrawNormalized();
    hZg->SetLineStyle(9);
    hZg->DrawNormalized("same");
  }
  else{
    hZg->DrawNormalized();
    hWg->SetLineStyle(9);
    hWg->DrawNormalized("same");
  }
  TText* tTitle1 = new TText(0.15,0.95,strVar);
  tTitle1->SetNDC();
  tTitle1->Draw("same");
  TLegend* leg = new TLegend(0.65,0.65,0.85,0.85);
  leg->AddEntry(hZg,"Zg","l");
  leg->AddEntry(hWg,"Wg","l");
  leg->Draw("same");
  pad2->cd();
  std::cout<<"hRatio "<<hRatio->GetTitle()<<std::endl;
  for (int ib=1; ib<=hRatio->GetNbinsX(); ib++){
    float x=hWg->GetBinContent(ib)*hZg->GetSumOfWeights();
    float y=hZg->GetBinContent(ib)*hWg->GetSumOfWeights();
    float ex=sqrt(hWg->GetBinContent(ib))*hZg->GetSumOfWeights();
    float ey=sqrt(hZg->GetBinContent(ib))*hWg->GetSumOfWeights();
    if (y==0){ hRatio->SetBinContent(ib,0); hRatio->SetBinError(ib,0);}
    else {
      hRatio->SetBinContent(ib,x/y);
      hRatio->SetBinError(ib,sqrt(x*x*ey*ey+y*y*ex*ex)/(y*y));
    }
    std::cout<<hRatio->GetBinContent(ib)<<", ";
  }
  std::cout<<std::endl;
  //TCanvas* canv1 = new TCanvas(cName+TString("ratio"),cName+TString("ratio"));
  TLine* line = new TLine(hRatio->GetBinLowEdge(1),1,hRatio->GetBinLowEdge(hRatio->GetNbinsX())+hRatio->GetBinWidth(hRatio->GetNbinsX()),1);
  line->SetLineStyle(9);
  hRatio->SetLineWidth(2);
  hRatio->SetStats(0);
  hRatio->GetYaxis()->SetLabelSize(0.1);
  hRatio->GetXaxis()->SetLabelSize(0.1);
  hRatio->GetXaxis()->SetTitleOffset(1.0);
  hRatio->GetXaxis()->SetTitleSize(0.12);
  hRatio->Draw("E");
  line->Draw("same");
  canv->SaveAs(cName+TString(".png"));
}

void CompareTrueGammaDistributions()
{
  TFile fZg("../WGammaOutput/MUON/VeryPreliminarySelected/selectedBKGMC_Zg.root");
  TFile fWg("../WGammaOutput/MUON/VeryPreliminarySelected/selectedSIGMC.root");
  TTree* trZg = (TTree*)fZg.Get("selectedEvents");
  TTree* trWg = (TTree*)fWg.Get("selectedEvents");

  TH1F* hZgSigmaB = new TH1F("hZgSigmaB","hZgSigmaB",16,0.005,0.025);
  TH1F* hWgSigmaB = new TH1F("hWgSigmaB","hWgSigmaB",16,0.005,0.025);
  TH1F* hRatioSigmaB = new TH1F("hRatioSigmaB","hRatioSigmaB",16,0.005,0.025);

  TH1F* hZgSigmaE = new TH1F("hZgSigmaE","hZgSigmaE",16,0.015,0.065);
  TH1F* hWgSigmaE = new TH1F("hWgSigmaE","hWgSigmaE",16,0.015,0.065);
  TH1F* hRatioSigmaE = new TH1F("hRatioSigmaE","hRatioSigmaE",16,0.015,0.065);

  TH1F* hZgIsoB = new TH1F("hZgIsoB","hZgIsoB",24,0,12);
  TH1F* hWgIsoB = new TH1F("hWgIsoB","hWgIsoB",24,0,12);
  TH1F* hRatioIsoB = new TH1F("hRatioIsoB","hRatioIsoB",24,0,12);

  TH1F* hZgIsoE = new TH1F("hZgIsoE","hZgIsoE",24,0,12);
  TH1F* hWgIsoE = new TH1F("hWgIsoE","hWgIsoE",24,0,12);
  TH1F* hRatioIsoE = new TH1F("hRatioIsoE","hRatioIsoE",24,0,12);

  TCut cutB="phoEta>-1.4442 && phoEta<1.4442";
  TCut cutE="(phoEta>-2.5 && phoEta<-1.566)||(phoEta<2.5 && phoEta>1.566)";
  TCut cutTrueGamma="phoGenPID==22 && phoGenMomPID>=-24 && phoGenMomPID<=24";
  TCut cutPhoEt="phoEt>15";
  TCut cutBase=cutPhoEt && cutTrueGamma;
  TCut cutWeight="weight";

  trZg->Draw("phoSigmaIEtaIEta>>hZgSigmaB",(cutBase&&cutB)*cutWeight,"goff");
  trZg->Draw("phoSigmaIEtaIEta>>hZgSigmaE",(cutBase&&cutE)*cutWeight,"goff");
  trWg->Draw("phoSigmaIEtaIEta>>hWgSigmaB",(cutBase&&cutB)*cutWeight,"goff");
  trWg->Draw("phoSigmaIEtaIEta>>hWgSigmaE",(cutBase&&cutE)*cutWeight,"goff");
  trZg->Draw("phoPFChIsoCorr>>hZgIsoB",(cutBase&&cutB)*cutWeight,"goff");
  trZg->Draw("phoPFChIsoCorr>>hZgIsoE",(cutBase&&cutE)*cutWeight,"goff");
  trWg->Draw("phoPFChIsoCorr>>hWgIsoB",(cutBase&&cutB)*cutWeight,"goff");
  trWg->Draw("phoPFChIsoCorr>>hWgIsoE",(cutBase&&cutE)*cutWeight,"goff");
 
  TCanvas* c1=new TCanvas("cSigmaB","cSigmaB");
  PlotOneComparison(c1, hZgSigmaB, hWgSigmaB, hRatioSigmaB);
  hRatioSigmaB->Print();
  TCanvas* c2=new TCanvas("cSigmaE","cSigmaE");
  PlotOneComparison(c2, hZgSigmaE, hWgSigmaE, hRatioSigmaE);
  hRatioSigmaE->Print();
  TCanvas* c3=new TCanvas("cIsoB","cIsoB");
  PlotOneComparison(c3, hZgIsoB, hWgIsoB, hRatioIsoB);
  hRatioIsoB->Print();
  TCanvas* c4=new TCanvas("cIsoE","cIsoE");
  PlotOneComparison(c4, hZgIsoE, hWgIsoE, hRatioIsoE);
  hRatioIsoE->Print();
}

float ErrorOfRate(float num, float den)
{
  float err;
  return sqrt(num*(den-num)*(den-num)+den*num*num)/(den*den);
}

void PrintOneStepOfEfficiency(TString strHead, int nCutB, int nFullB, int nCutE, int nFullE)
{
  std::cout<<strHead<<": ";
  std::cout<<std::setw(5);
  std::cout<<std::setprecision(2);
  if (nFullB==0) std::cout<<"B: "<<nFullB<<"=0"<<"; ";
  else std::cout<<"B: "<<nCutB<<"/"<<nFullB<<"="<<1.0*nCutB/nFullB<<"+-"<<ErrorOfRate(nCutB,nFullB)<<"; ";
  if (nFullE==0) std::cout<<"E: "<<nFullE<<"=0"<<"; ";
  else std::cout<<"E: "<<nCutE<<"/"<<nFullE<<"="<<1.0*nCutE/nFullE<<"+-"<<ErrorOfRate(nCutE,nFullE)<<"; ";
  if ((nFullB+nFullE)==0) std::cout<<"T: "<<nFullB+nFullE<<"=0"<<"";
  else std::cout<<"T: "<<(nCutB+nCutE)<<"/"<<(nFullB+nFullE)<<"="<<1.0*(nCutB+nCutE)/(nFullB+nFullE)<<"+-"<<ErrorOfRate(nCutB+nCutE,nFullB+nFullE)<<";";
  std::cout<<std::endl;
}

void CompareSomePhotonEfficiencies()
{
  TFile fZg("../WGammaOutput/MUON/PreliminaryForTemplateMethodSelected/selectedBKGMC_Zg.root");
  TFile fWg("../WGammaOutput/MUON/PreliminaryForTemplateMethodSelected/selectedSIGMC.root");
  TTree* trZg = (TTree*)fZg.Get("selectedEvents");
  TTree* trWg = (TTree*)fWg.Get("selectedEvents");
  TCut cutB="phoEta>-1.4442 && phoEta<1.4442";
  TCut cutE="(phoEta>-2.5 && phoEta<-1.566)||(phoEta<2.5 && phoEta>1.566)";
  TCut cutTrueGamma="phoGenPID==22 && phoGenMomPID>=-24 && phoGenMomPID<=24";
  TCut cutPhoEt="phoEt>15";
  TCut cutPhoSigmaIEtaIEtaMediumB="phoSigmaIEtaIEta<0.011";
  TCut cutPhoSigmaIEtaIEtaMediumE="phoSigmaIEtaIEta<0.033";
  TCut cutPhoPFChIsoCorrMediumB="phoPFChIsoCorr<1.5";
  TCut cutPhoPFChIsoCorrMediumE="phoPFChIsoCorr<1.2";
  TCut cutBase=cutPhoEt && cutTrueGamma;

  int nZgB=trZg->GetEntries(cutBase && cutB);
  int nWgB=trWg->GetEntries(cutBase && cutB);
  int nZgE=trZg->GetEntries(cutBase && cutE);
  int nWgE=trWg->GetEntries(cutBase && cutE);

  int nZgBSigma=trZg->GetEntries(cutBase && cutB && cutPhoSigmaIEtaIEtaMediumB);
  int nWgBSigma=trWg->GetEntries(cutBase && cutB && cutPhoSigmaIEtaIEtaMediumB);
  int nZgESigma=trZg->GetEntries(cutBase && cutE && cutPhoSigmaIEtaIEtaMediumE);
  int nWgESigma=trWg->GetEntries(cutBase && cutE && cutPhoSigmaIEtaIEtaMediumE);

  int nZgBIso=trZg->GetEntries(cutBase && cutB && cutPhoPFChIsoCorrMediumB);
  int nWgBIso=trWg->GetEntries(cutBase && cutB && cutPhoPFChIsoCorrMediumB);
  int nZgEIso=trZg->GetEntries(cutBase && cutE && cutPhoPFChIsoCorrMediumE);
  int nWgEIso=trWg->GetEntries(cutBase && cutE && cutPhoPFChIsoCorrMediumE);

  int nZgBSigmaIso=trZg->GetEntries(cutBase && cutB && cutPhoSigmaIEtaIEtaMediumB && cutPhoPFChIsoCorrMediumB);
  int nWgBSigmaIso=trWg->GetEntries(cutBase && cutB && cutPhoSigmaIEtaIEtaMediumB && cutPhoPFChIsoCorrMediumB);
  int nZgESigmaIso=trZg->GetEntries(cutBase && cutE && cutPhoSigmaIEtaIEtaMediumE && cutPhoPFChIsoCorrMediumE);
  int nWgESigmaIso=trWg->GetEntries(cutBase && cutE && cutPhoSigmaIEtaIEtaMediumE && cutPhoPFChIsoCorrMediumE);

  std::cout<<std::endl;
  PrintOneStepOfEfficiency("rate after phoSigmaIEtaIEta, Zg", nZgBSigma, nZgB, nZgESigma, nZgE);
  PrintOneStepOfEfficiency("rate after Iso after Sigma , Zg", nZgBSigmaIso, nZgBSigma, nZgESigmaIso, nZgESigma);
  PrintOneStepOfEfficiency("rate after phoPFChIsoCorr  , Zg", nZgBIso, nZgB, nZgEIso, nZgE);
  PrintOneStepOfEfficiency("rate after Sigma after Iso , Zg", nZgBSigmaIso, nZgBIso, nZgESigmaIso, nZgEIso);
  PrintOneStepOfEfficiency("rate after Sigma X IsoCorr , Zg", nZgBSigmaIso, nZgB, nZgESigmaIso, nZgE);
  std::cout<<std::endl;
  PrintOneStepOfEfficiency("rate after phoSigmaIEtaIEta, Wg", nWgBSigma, nWgB, nWgESigma, nWgE);
  PrintOneStepOfEfficiency("rate after Iso after Sigma , Wg", nWgBSigmaIso, nWgBSigma, nWgESigmaIso, nWgESigma);
  PrintOneStepOfEfficiency("rate after phoPFChIsoCorr  , Wg", nWgBIso, nWgB, nWgEIso, nWgE);
  PrintOneStepOfEfficiency("rate after Sigma after Iso , Wg", nWgBSigmaIso, nWgBIso, nWgESigmaIso, nWgEIso);
  PrintOneStepOfEfficiency("rate after Sigma X IsoCorr , Wg", nWgBSigmaIso, nWgB, nWgESigmaIso, nWgE);
  std::cout<<std::endl;
}
