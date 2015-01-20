#include "TFile.h"
#include "TCut.h"
#include "TTree.h"
#include "TString.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TLegend.h"
#include <iostream>

void DrawIsolationTemplatesOneCase(TString canvName, TTree* trVg, TTree* trVj, TCut cutNoWeight, TCut cutWeight)
{
  TString strVgRand=TString("h_VgRand")+canvName;
  TString strVgSCR=TString("h_VgSCR")+canvName;
  TString strVjRand=TString("h_VjRand")+canvName;
  TH1F* h_VgRand = new TH1F(strVgRand,strVgRand,9,-0.9,8.1);
  TH1F* h_VgSCR = new TH1F(strVgSCR,strVgSCR,9,-0.9,8.1);
  TH1F* h_VjRand = new TH1F(strVjRand,strVjRand,9,-0.9,8.1);
  trVg->Draw(TString("phoRandConeChIso04Corr>>")+strVgRand,cutNoWeight*cutWeight,"goff");
  trVg->Draw(TString("phoSCRChIso04Corr>>")+strVgSCR,cutNoWeight*cutWeight,"goff");
  trVj->Draw(TString("phoRandConeChIso04Corr>>")+strVjRand,cutNoWeight*cutWeight,"goff");
  std::cout<<"trVg->GetEntries()="<<trVg->GetEntries()<<", trVj->GetEntries()"<<trVj->GetEntries()<<std::endl;
  h_VgRand->Print();
  h_VjRand->Print();
  h_VgSCR->Print();
  TCanvas* canv = new TCanvas(canvName, canvName);
  float integr1=h_VgRand->GetSumOfWeights();
  float integr2=h_VjRand->GetSumOfWeights();
  float integr3=h_VgSCR->GetSumOfWeights();
  for (int ib=1; ib<=h_VgRand->GetNbinsX(); ib++){

    float cont=h_VgRand->GetBinContent(ib);
    float err=h_VgRand->GetBinError(ib);

    h_VgRand->SetBinContent(ib,cont/integr1);
    h_VgRand->SetBinError(ib,err/integr1);
    std::cout<<"h_VgRand: ib="<<ib<<", cont/integr=("<<cont<<"+-"<<err<<")/"<<integr1<<"="<<cont/integr1<<"+-"<<err/integr1<<", normalized="<<h_VgRand->GetBinContent(ib)<<"+-"<<h_VgRand->GetBinError(ib)<<std::endl;

    cont=h_VjRand->GetBinContent(ib);
    err=h_VjRand->GetBinError(ib);
    h_VjRand->SetBinContent(ib,cont/integr2);
    h_VjRand->SetBinError(ib,err/integr2);

    cont=h_VgSCR->GetBinContent(ib);
    err=h_VgSCR->GetBinError(ib);
    h_VgSCR->SetBinContent(ib,cont/integr3);
    h_VgSCR->SetBinError(ib,err/integr3);
  }

  float max=0;
  if (h_VgRand->GetMaximum()>max) max=h_VgRand->GetMaximum();
  if (h_VgSCR->GetMaximum()>max) max=h_VgSCR->GetMaximum();
  if (h_VjRand->GetMaximum()>max) max=h_VjRand->GetMaximum();

  h_VgRand->SetLineWidth(2);
  h_VgRand->SetLineColor(1);
  h_VgRand->GetYaxis()->SetRangeUser(0,1.1*max);
  h_VgRand->GetXaxis()->SetTitle("pho ch Iso");
  h_VgSCR->SetLineWidth(2);
  h_VgSCR->SetLineColor(2);
  h_VjRand->SetLineWidth(2);
  h_VjRand->SetLineColor(4);
  canv->cd();
  h_VgRand->SetStats(0);
  h_VgRand->SetTitle(canvName);
  h_VgRand->Draw("EP");
  h_VgSCR->Draw("EP same");
  h_VjRand->Draw("EP same");

  TLegend* leg = new TLegend(0.50,0.65,0.90,0.90);
  leg->AddEntry(h_VgSCR,"Vg, SCR","l");
  leg->AddEntry(h_VgRand,"Vg, RandCone","l");
  leg->AddEntry(h_VjRand,"Vjets, RandCone","l");
  leg->Draw("same");
  canv->SaveAs(TString("../WGammaOutput/MUON_WGamma/Plots/QuickChecks/")+canvName+TString(".png"));
  canv->SaveAs(TString("../WGammaOutput/MUON_WGamma/Plots/QuickChecks/")+canvName+TString(".pdf"));
  canv->SaveAs(TString("../WGammaOutput/MUON_WGamma/Plots/QuickChecks/")+canvName+TString(".root"));
}//end of DrawIsolationTemplatesOneCase

void DrawIsolationTemplates()
{
  TFile f_Zg("../WGammaOutput/MUON_ZGamma/PreliminaryForTemplateMethodSelected/selected_ZGammaSIGMC.root");
  TFile f_Zjets("../WGammaOutput/MUON_ZGamma/PreliminaryForTemplateMethodSelected/selected_ZGammaBKGMC_DYjets_to_ll.root");
  TFile f_Wg("../WGammaOutput/MUON_WGamma/PreliminaryForTemplateMethodSelected/selected_WGammaSIGMC.root");
  TFile f_Wjets("../WGammaOutput/MUON_WGamma/PreliminaryForTemplateMethodSelected/selected_WGammaBKGMC_Wjets_to_lnu.root");

  TTree* tr_Zg = (TTree*)f_Zg.Get("selectedEvents");
  TTree* tr_Zjets = (TTree*)f_Zjets.Get("selectedEvents");
  TTree* tr_Wg = (TTree*)f_Wg.Get("selectedEvents");
  TTree* tr_Wjets = (TTree*)f_Wjets.Get("selectedEvents");
  
  TCut cutB="phoSCEta<1.4442 && phoSCEta>-1.4442";
  TCut cutE="(phoSCEta > 1.566 && phoSCEta < 2.5)||(phoSCEta <-1.566 && phoSCEta >-2.5)";
  TCut cutSihihB="phoSigmaIEtaIEta<0.011";
  TCut cutSihihE="phoSigmaIEtaIEta<0.033";
  TCut cutWeight="weight";
  TCut cutPtFirstBin="phoEt>15 && phoEt<20";

  TFile* f = new TFile("fOut1.root","recreate");
  DrawIsolationTemplatesOneCase("signalTempl_Iso_Zg_Barrel_pt15_20", tr_Zg, tr_Zjets, cutB && cutSihihB && cutPtFirstBin, cutWeight);
  DrawIsolationTemplatesOneCase("signalTempl_Iso_Zg_Endcap_pt15_20", tr_Zg, tr_Zjets, cutE && cutSihihE && cutPtFirstBin, cutWeight);
  DrawIsolationTemplatesOneCase("signalTempl_Iso_Wg_Barrel_pt15_20", tr_Wg, tr_Wjets, cutB && cutSihihB && cutPtFirstBin, cutWeight);
  DrawIsolationTemplatesOneCase("signalTempl_Iso_Wg_Endcap_pt15_20", tr_Wg, tr_Wjets, cutE && cutSihihE && cutPtFirstBin, cutWeight);
}

void DrawSihihTemplatesOneCase(TString canvName, TTree* trZg, TTree* trWg, TTree* trZgFSR, TCut cutNoWeight, TCut cutWeight, int nbins, float min, float max)
{
  TString strZg=TString("h_Zg")+canvName;
  TString strWg=TString("h_Wg")+canvName;
  TString strZgFSR=TString("h_ZgFSR")+canvName;
  TH1F* h_Zg = new TH1F(strZg,strZg,nbins,min,max);
  TH1F* h_Wg = new TH1F(strWg,strWg,nbins,min,max);
  TH1F* h_ZgFSR = new TH1F(strZgFSR,strZgFSR,nbins,min,max);
  trZg->Draw(TString("phoSigmaIEtaIEta>>")+strZg,cutNoWeight*cutWeight,"goff");
  trWg->Draw(TString("phoSigmaIEtaIEta>>")+strWg,cutNoWeight*cutWeight,"goff");
  trZgFSR->Draw(TString("phoSigmaIEtaIEta>>")+strZgFSR,cutNoWeight*cutWeight,"goff");
  TCanvas* canv = new TCanvas(canvName, canvName);
  float integr1=h_Zg->GetSumOfWeights();
  float integr2=h_Wg->GetSumOfWeights();
  float integr3=h_ZgFSR->GetSumOfWeights();
  h_Zg->Print();
  h_Wg->Print();
  h_ZgFSR->Print();
  for (int ib=1; ib<=h_Zg->GetNbinsX(); ib++){

    float cont=h_Zg->GetBinContent(ib);
    float err=h_Zg->GetBinError(ib);
    h_Zg->SetBinContent(ib,cont/integr1);
    h_Zg->SetBinError(ib,err/integr1);
//    std::cout<<"h_Zg: ib="<<ib<<", cont/integr=("<<cont<<"+-"<<err<<")/"<<integr1<<"="<<cont/integr1<<"+-"<<err/integr1<<", normalized="<<h_Zg->GetBinContent(ib)<<"+-"<<h_Zg->GetBinError(ib)<<std::endl;

    cont=h_Wg->GetBinContent(ib);
    err=h_Wg->GetBinError(ib);
    h_Wg->SetBinContent(ib,cont/integr2);
    h_Wg->SetBinError(ib,err/integr2);

    cont=h_ZgFSR->GetBinContent(ib);
    err=h_ZgFSR->GetBinError(ib);
    h_ZgFSR->SetBinContent(ib,cont/integr3);
    h_ZgFSR->SetBinError(ib,err/integr3);

  }

  float maxH=0;
  if (h_Zg->GetMaximum()>maxH) maxH=h_Zg->GetMaximum();
  if (h_Wg->GetMaximum()>maxH) maxH=h_Wg->GetMaximum();
  if (h_ZgFSR->GetMaximum()>maxH) maxH=h_ZgFSR->GetMaximum();

  h_Zg->SetLineWidth(2);
  h_Zg->SetLineColor(1);
  h_Zg->GetYaxis()->SetRangeUser(0,1.1*maxH);
  h_Zg->GetXaxis()->SetTitle("phoSigmaIEtaIEta");
  h_Wg->SetLineWidth(2);
  h_Wg->SetLineColor(2);
  h_ZgFSR->SetLineWidth(2);
  h_ZgFSR->SetLineColor(4);
  canv->cd();
  h_Zg->SetStats(0);
  h_Zg->SetTitle(canvName);

  h_Zg->Draw("EP");
  h_Wg->Draw("EP same");
  h_ZgFSR->Draw("EP same");

  TLegend* leg = new TLegend(0.50,0.65,0.90,0.90);
  leg->AddEntry(h_Wg,"Wg","l");
  leg->AddEntry(h_Zg,"Zg","l");
  leg->AddEntry(h_ZgFSR,"Zg FSR","l");
  leg->Draw("same");

  canv->SaveAs(TString("../WGammaOutput/MUON_WGamma/Plots/QuickChecks/")+canvName+TString(".png"));
  canv->SaveAs(TString("../WGammaOutput/MUON_WGamma/Plots/QuickChecks/")+canvName+TString(".pdf"));
  canv->SaveAs(TString("../WGammaOutput/MUON_WGamma/Plots/QuickChecks/")+canvName+TString(".root"));
}//end of DrawSihihTemplatesOneCase

void DrawSihihTemplates()
{
  TFile f_Zg("../WGammaOutput/MUON_ZGamma/PreliminaryForTemplateMethodSelected/selected_ZGammaSIGMC.root");
  TFile f_Wg("../WGammaOutput/MUON_WGamma/PreliminaryForTemplateMethodSelected/selected_WGammaSIGMC.root");
  TFile f_ZgFSR("../WGammaOutput/MUON_ZGamma/FsrSelected/selected_ZGammaSIGMC.root");

  TTree* tr_Zg = (TTree*)f_Zg.Get("selectedEvents");
  TTree* tr_Wg = (TTree*)f_Wg.Get("selectedEvents");
  TTree* tr_ZgFSR = (TTree*)f_ZgFSR.Get("selectedEvents");
  
  TCut cutB="phoSCEta<1.4442 && phoSCEta>-1.4442";
  TCut cutE="(phoSCEta > 1.566 && phoSCEta < 2.5)||(phoSCEta <-1.566 && phoSCEta >-2.5)";
  TCut cutWeight="weight";
  TCut cutPtFirstBin="phoEt>15 && phoEt<20";

  TFile* f = new TFile("fOut2.root","recreate");
  DrawSihihTemplatesOneCase("signalTempl_sihih_Barrel_pt15_20", tr_Zg, tr_Wg, tr_ZgFSR, cutB && cutPtFirstBin, cutWeight, 17, 0.005, 0.022);
  DrawSihihTemplatesOneCase("signalTempl_sihih_Endcap_pt15_20", tr_Zg, tr_Wg, tr_ZgFSR, cutE && cutPtFirstBin, cutWeight, 24, 0.019, 0.067);
}
