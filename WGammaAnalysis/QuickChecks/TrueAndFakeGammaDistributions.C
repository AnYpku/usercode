#include "TTree.h"
#include "TString.h"
#include "TCut.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TLegend.h"

#include <iostream>

struct CompareEntry{
  TTree* tr;
  TCut cut;
  TString label;
  int color;
  int lineStyle;
};

void CompareDistributions(TFile* fOut, TString varCompare, TString etaStr, TString plotName, int nHists, CompareEntry* comp, int nBins, float* binLims)
{
  fOut->cd();
  TH1F* hist[nHists]; 
  TString canvName=plotName+TString("_")+varCompare+TString("_")+etaStr;
/*
  canvName+=comp[0].cut.GetTitle();
  canvName.ReplaceAll(".","");
  canvName.ReplaceAll("|","");
  canvName.ReplaceAll("&","");
  canvName.ReplaceAll("+","");
  canvName.ReplaceAll("-","");
  canvName.ReplaceAll("=","");
  canvName.ReplaceAll("!","");
  canvName.ReplaceAll("*","");
  canvName.ReplaceAll("/","");
  canvName.ReplaceAll("%","");
  canvName.ReplaceAll(" ","");
  canvName.ReplaceAll("(","");
  canvName.ReplaceAll(")","");
  canvName.ReplaceAll(">","");
  canvName.ReplaceAll("<","");
*/
  TCanvas* canv = new TCanvas(canvName,canvName);
  TCanvas* canvScale;
  TString canvNameScale;
  if (varCompare=="phoSCRChIsoCorr"){
    canvNameScale = canvName+TString("_Scale");
    canvScale=new TCanvas(canvNameScale,canvNameScale);
  }
  TLegend* leg = new TLegend(0.7,0.7,1,1);
  for (int ic=0; ic<nHists; ic++){
    TString histName = TString("h")+canvName;
    histName+=ic;
    hist[ic] = new TH1F(histName,histName,nBins,binLims);
    comp[ic].tr->Draw(varCompare+TString(">>")+histName,comp[ic].cut,"goff");
    hist[ic]->SetLineWidth(2);
    hist[ic]->SetLineStyle(comp[ic].lineStyle);
    hist[ic]->SetLineColor(comp[ic].color);
    if (ic==0){
      canv->cd();
      hist[ic]->SetTitle(plotName);
      hist[ic]->SetStats(0);
      hist[ic]->DrawNormalized();
      if (varCompare=="phoSCRChIsoCorr"){
        canvScale->cd();
        hist[ic]->GetXaxis()->SetRangeUser(0.21,5);
        hist[ic]->DrawNormalized();
      }
    }
    else {
      canv->cd();
      hist[ic]->DrawNormalized("same");
      if (varCompare=="phoSCRChIsoCorr"){
        canvScale->cd();
        hist[ic]->GetXaxis()->SetRangeUser(0.21,5);
        hist[ic]->DrawNormalized("same");
      }      
    }
    leg->AddEntry(hist[ic],comp[ic].label,"l");
  }
  leg->SetFillColor(0);
  canv->cd();
  leg->Draw("same");
  if (varCompare=="phoSCRChIsoCorr"){
    canvScale->cd();
    leg->Draw("same");
  }
  canv->SaveAs(canvName+TString(".png"));
  if (varCompare=="phoSCRChIsoCorr")
    canvScale->SaveAs(canvNameScale+TString(".png"));
}

void RunForGivenVarAndEtaBin(TFile* fOut, TTree* trWg, TTree* trWj, TTree* trZg, TTree* trZj, TTree* trDa, TString varCompare, int nBins, float* binLims, TCut cutEta, TString etaStr, TCut sidebandMild, TCut sidebandDeep) 
{
  TCut cutWeight = "weight";
  TCut cutTrueGamma = "phoGenPID==22";
  TCut cutFSR = "(phoGenMomPID==13 || phoGenMomPID==-13) && (phoGenGMomPID==23 || phoGenGMomPID==24 || phoGenGMomPID==-24)";
  TCut cutFrompi0="phoGenMomPID==111";

  CompareEntry compWg;
  compWg.tr=trWg;
  compWg.cut=cutEta * cutWeight;
  compWg.label="Wg";
  compWg.color=2;
  compWg.lineStyle=9;
  CompareEntry compWj;
  compWj.tr=trWj;
  compWj.cut=cutEta * cutWeight;
  compWj.label="Wjets";
  compWj.color=4;
  compWj.lineStyle=1;
  CompareEntry compZg;
  compZg.tr=trZg;
  compZg.cut=cutEta * cutWeight;
  compZg.label="Zg";
  compZg.color=8;
  compZg.lineStyle=1;
  CompareEntry compZj;
  compZj.tr=trZj;
  compZj.cut=cutEta * cutWeight;
  compZj.label="DYjets";
  compZj.color=1;
  compZj.lineStyle=9;
  CompareEntry comps1[4]={compZg,compWg,compWj,compZj};
  CompareDistributions(fOut, varCompare, etaStr, "Wg_Wjets_Zg_DYjets", 4, comps1, nBins, binLims);

  compWj.cut=(cutTrueGamma && cutEta) * cutWeight;
  compZj.cut=(cutTrueGamma && cutEta) * cutWeight;

  CompareEntry comps2[4]={compZg,compWg,compWj,compZj};

  CompareDistributions(fOut, varCompare, etaStr, "Wg_Wjets_Zg_DYjets_TrueGamma", 4, comps2, nBins, binLims);

  compWg.cut=(cutTrueGamma && cutEta) * cutWeight;
  compZg.cut=(cutTrueGamma && cutEta) * cutWeight;
  compWj.cut=(cutTrueGamma && cutFSR && cutEta) * cutWeight;
  compZj.cut=(cutTrueGamma && cutFSR && cutEta) * cutWeight;
  compWj.label="Wjets FSR";
  compZj.label="DYjets FSR";

  CompareEntry comps3[4]={compZj,compWj,compZg,compWg};//Wg doesn't have many FSR
  CompareDistributions(fOut, varCompare, etaStr, "Wg_Wjets_Zg_DYjets_FSR", 4, comps3, nBins, binLims);

  CompareEntry compWjFake;
  compWjFake.tr=trWj;
  compWjFake.cut=(!cutTrueGamma && cutEta) * cutWeight;
  compWjFake.label="Wjets fakes";
  compWjFake.color=1;
  compWjFake.lineStyle=1;
  CompareEntry compWjpi0;
  compWjpi0.tr=trWj;
  compWjpi0.cut=(cutTrueGamma && cutFrompi0 && cutEta) * cutWeight;
  compWjpi0.label="Wjets from pi0";
  compWjpi0.color=2;
  compWjpi0.lineStyle=1;
  CompareEntry compWjFSR;
  compWjFSR.tr=trWj;
  compWjFSR.cut=(cutTrueGamma && cutFSR && cutEta) * cutWeight;
  compWjFSR.label="Wjets FSR";
  compWjFSR.color=4;
  compWjFSR.lineStyle=1;
  CompareEntry comps4[3]={compWjFSR,compWjFake,compWjpi0};
  CompareDistributions(fOut, varCompare, etaStr, "Wjets_fake_vs_pi0_vs_FSR", 3, comps4, nBins, binLims);

  CompareEntry compZjFake;
  compZjFake.tr=trZj;
  compZjFake.cut=(!cutTrueGamma && cutEta) * cutWeight;
  compZjFake.label="DYjets fakes";
  compZjFake.color=1;
  compZjFake.lineStyle=1;
  CompareEntry compZjpi0;
  compZjpi0.tr=trZj;
  compZjpi0.cut=(cutTrueGamma && cutFrompi0 && cutEta) * cutWeight;
  compZjpi0.label="DYjets from pi0";
  compZjpi0.color=2;
  compZjpi0.lineStyle=1;
  CompareEntry compZjFSR;
  compZjFSR.tr=trZj;
  compZjFSR.cut=(cutTrueGamma && cutFSR && cutEta) * cutWeight;
  compZjFSR.label="DYjets FSR";
  compZjFSR.color=4;
  compZjFSR.lineStyle=1;
  CompareEntry comps5[3]={compZjFSR,compZjFake,compZjpi0};
  CompareDistributions(fOut, varCompare, etaStr, "DYjets_fake_vs_pi0_vs_FSR", 3, comps5, nBins, binLims);

  compZjFake.color=28;
  compWjFake.lineStyle=10;
  compZjpi0.color=4;
  compWjpi0.lineStyle=10;
  CompareEntry comps6[4]={compZjpi0,compZjFake,compWjpi0,compWjFake};
  CompareDistributions(fOut, varCompare, etaStr, "DYjets_Wjets_fake_and_pi0", 4, comps6, nBins, binLims);  

  CompareEntry compData;
  compData.tr=trDa;
  compData.cut=(sidebandDeep)*cutWeight;
  compData.label=TString("Data, ")+sidebandDeep.GetTitle();
  compData.color=1;
  compData.lineStyle=1;
  CompareEntry compZjFakeAndpi0;
  compZjFakeAndpi0.tr=trZj;
  compZjFakeAndpi0.cut=(((cutTrueGamma && cutFrompi0)||(!cutTrueGamma)) && cutEta) * cutWeight;
  compZjFakeAndpi0.label="DYjets fake and from pi0";
  compZjFakeAndpi0.color=2;
  compZjpi0.lineStyle=1;
  CompareEntry comps7[4]={compZjpi0,compZjFake,compData,compZjFakeAndpi0};
  CompareDistributions(fOut, varCompare, etaStr,  "DataDeep_vs_DYjets_fake_and_pi0_and_comb", 4, comps7, nBins, binLims);  

  compData.cut=(sidebandMild)*cutWeight;
  compData.label=TString("Data, ")+sidebandMild.GetTitle();
  CompareEntry comps8[4]={compZjpi0,compZjFake,compData,compZjFakeAndpi0};
  CompareDistributions(fOut, varCompare, etaStr,  "DataMild_vs_DYjets_fake_and_pi0_and_comb", 4, comps8, nBins, binLims);

}

void RunComparisons()
{
  TFile f1("../WGammaOutput/MUON/PreliminaryForTemplateMethodSelected/selectedSIGMC.root");
  TTree* trWg = (TTree*)f1.Get("selectedEvents");
  TFile f2("../WGammaOutput/MUON/PreliminaryForTemplateMethodSelected/selectedBKGMC_Wjets_to_lnu.root");
  TTree* trWj = (TTree*)f2.Get("selectedEvents");
  TFile f3("../WGammaOutput/MUON/PreliminaryForTemplateMethodSelected/selectedBKGMC_Zg.root");
  TTree* trZg = (TTree*)f3.Get("selectedEvents");
  TFile f4("../WGammaOutput/MUON/PreliminaryForTemplateMethodSelected/selectedBKGMC_DYjets_to_ll.root");
  TTree* trZj = (TTree*)f4.Get("selectedEvents");
  TFile f0("../WGammaOutput/MUON/PreliminaryForTemplateMethodSelected/selectedDATA.root");
  TTree* trDa = (TTree*)f0.Get("selectedEvents");

  TFile* fOut=new TFile("fOutTrueAndFake.root","recreate");

  TCut cutBarrel = "phoSCEta>-1.442 && phoSCEta<1.442";
  TCut cutEndcap = "(phoSCEta>-2.5 && phoSCEta<-1.566)||(phoSCEta>1.566 && phoSCEta<2.5)";


  TCut sidebandDeep="phoSCRChIsoCorr>8";
  TCut sidebandMild="phoSCRChIsoCorr>2.6";
  TString varCompare="phoSigmaIEtaIEta";
  const int nBins1 = 16;
  float binLims1[nBins1+1]={0.006,0.007,0.008,0.009,0.010,0.011,0.012,0.013,0.014,0.015,0.016,0.017,0.018,0.019,0.020,0.021,0.022}; 

  RunForGivenVarAndEtaBin(fOut, trWg, trWj, trZg, trZj, trDa, varCompare, nBins1, binLims1, cutBarrel, "Barrel", sidebandMild, sidebandDeep);

  sidebandDeep="phoSCRChIsoCorr>8";
  sidebandMild="phoSCRChIsoCorr>2.3";
  varCompare="phoSigmaIEtaIEta";
  const int nBins2 = 12;
  float binLims2[nBins2+1]={0.019,0.023,0.027,0.031,0.035,0.039,0.043,0.047,0.051,0.055,0.059,0.063,0.067}; 

  RunForGivenVarAndEtaBin(fOut, trWg, trWj, trZg, trZj, trDa, varCompare, nBins2, binLims2, cutEndcap, "Endcap", sidebandMild, sidebandDeep);

//  TCut cutIso="phoSCRChIsoCorr>0";

  sidebandDeep="phoSigmaIEtaIEta>0.020";
  sidebandMild="phoSigmaIEtaIEta>0.012";
  varCompare="phoSCRChIsoCorr";
  const int nBins3 = 25;
  float binLims3[nBins3+1]={0,0.2,0.4,0.6,0.8,1,1.2,1.4,1.6,1.8,2,2.2,2.4,2.6,2.8,3,3.2,3.4,3.6,3.8,4,4.2,4.4,4.6,4.8,5}; 

  RunForGivenVarAndEtaBin(fOut, trWg, trWj, trZg, trZj, trDa, varCompare, nBins3, binLims3, cutBarrel, "Barrel", sidebandMild, sidebandDeep);

  sidebandDeep="phoSigmaIEtaIEta>0.060";
  sidebandMild="phoSigmaIEtaIEta>0.034";
  varCompare="phoSCRChIsoCorr";
  const int nBins4 = 25;
  float binLims4[nBins4+1]={0,0.2,0.4,0.6,0.8,1,1.2,1.4,1.6,1.8,2,2.2,2.4,2.6,2.8,3,3.2,3.4,3.6,3.8,4,4.2,4.4,4.6,4.8,5}; 

  RunForGivenVarAndEtaBin(fOut, trWg, trWj, trZg, trZj, trDa, varCompare, nBins4, binLims4, cutEndcap, "Endcap", sidebandMild, sidebandDeep);

}
