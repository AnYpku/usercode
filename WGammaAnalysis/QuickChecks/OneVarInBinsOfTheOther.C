#include "TTree.h"
#include "TString.h"
#include "TCut.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TLegend.h"

#include <iostream>

bool IsCharDigit(char c)
{
  if (c=='0' || c=='1' || c=='2' || c=='3' || 
      c=='4' || c=='5' || c=='6' || c=='7' || 
      c=='8' || c=='9')
    return 1;
  return 0;
}

void OneVarInBinsOfTheOther(TString canvName, TString axName, TTree* tr, TString var1, TCut cutNotWeight, TCut cutWeight, TString var2, int nBinsVar2, float* binLimsVar2, int nBinsHist, float histMin, float histMax)
{
  if (nBinsVar2>9) {
    std::cout<<"nBinsVar2="<<nBinsVar2<<", but can't exceed 9; if you want to check more than 9 bins, split into two or more plots"<<std::endl;
    return;
  }
  TH1F* hist[nBinsVar2]; 
  TCanvas* canv = new TCanvas(canvName,canvName);
  TLegend* leg = new TLegend(0.55,0.55,1.00,0.90);
  float max=0;
  for (int ib=0; ib<nBinsVar2; ib++){
    TString strCutBin = var2+TString(">=");
    strCutBin+=binLimsVar2[ib];
    strCutBin+=TString(" && ")+var2+TString("<=");
    strCutBin+=binLimsVar2[ib+1];
    TCut cutBin(strCutBin);
    TString histName = TString("h")+var1;
    histName+=ib;
    hist[ib] = new TH1F(histName,histName,nBinsHist,histMin,histMax);
    tr->Draw(var1+TString(">>")+histName,(cutNotWeight+cutBin)*cutWeight,"goff");
    hist[ib]->SetLineWidth(2);
    //hist[ib]->SetLineStyle(ib+1);
    //hist[ib]->SetLineStyle(1);
    int color = ib+1;
    if (color==5) color=13;
    if (color==8) color=49;
    hist[ib]->SetLineColor(color);
    float sum = 0;
    for (int ibh=1; ibh<=hist[ib]->GetNbinsX();ibh++)
      sum+=hist[ib]->GetBinContent(ibh);

    for (int ibh=1; ibh<=hist[ib]->GetNbinsX();ibh++){
      float cont=hist[ib]->GetBinContent(ibh);
      float err=hist[ib]->GetBinError(ibh);
      hist[ib]->SetBinContent(ibh,1.0*cont/sum);
      hist[ib]->SetBinError(ibh,1.0*err/sum);
    }
    if (max<hist[ib]->GetMaximum()) max=hist[ib]->GetMaximum();
    TString name="";
    name+=binLimsVar2[ib];
    name+=" < ";
    name+=var2;
    name+=" < ";
    name+=binLimsVar2[ib+1];
   
    int indDot=name.Index(".");
    int length=name.Length();
    int dAfterDot=0;
    for (int ind=1; ind<length; ind++){
      if (name(ind-1)=='.') dAfterDot=1;
      else if (dAfterDot && IsCharDigit(name(ind))){
        dAfterDot++;
        if (dAfterDot>3){ 
        name.Replace(ind,1,"");
        ind=ind-1;
        }
      }
      else dAfterDot=0;
    }
    name.ReplaceAll("Corr","");
    name.ReplaceAll("pho","");    
    name.ReplaceAll("SigmaIEtaIEta","sihih"); 
    leg->AddEntry(hist[ib],name,"l");
  }

  for (int ib=0; ib<nBinsVar2; ib++){
    if (ib==0){
      hist[ib]->SetStats(0);
      hist[ib]->SetTitle(canvName);
      hist[ib]->GetXaxis()->SetTitle(axName);
      hist[ib]->GetYaxis()->SetRangeUser(0,1.1*max);
      hist[ib]->Draw("EP");
    }
    else hist[ib]->Draw("EP same");
  }
  leg->Draw("same");
  canv->SaveAs(TString("../WGammaOutput/MUON_WGamma/Plots/QuickChecks/")+canvName+TString(".png"));
  canv->SaveAs(TString("../WGammaOutput/MUON_WGamma/Plots/QuickChecks/")+canvName+TString(".pdf"));
  canv->SaveAs(TString("../WGammaOutput/MUON_WGamma/Plots/QuickChecks/")+canvName+TString(".root"));
}

void PlotSihihInIsoBins(TTree* tr_Zg, TTree* tr_Wg, TTree* tr_Zjets, TTree* tr_Wjets, TCut cutB, TCut cutE, TCut cutPtFirstBin, TCut cutWeight)
{
  ////////////////////////////////
  // begin
  // phoSigmaIEtaIEta in phoSCRChIso04Corr  bins
  TString var1 = "phoSigmaIEtaIEta";
  TString var2 = "phoPFChIsoCorr";
  const int nBinsVar2= 5;
  float binLimsVar2[nBinsVar2+1]={-0.1,1.5,2.6,5.0,15.0,100.0};

  int nBinsHist =24;
  float histMin = 0.019;
  float histMax = 0.067;


  OneVarInBinsOfTheOther("Zg_MC_Endcap_sihih_in_PFIso_bins_pt15_20", "pho sihih", tr_Zg, var1, cutE && cutPtFirstBin, cutWeight, var2, nBinsVar2, binLimsVar2, nBinsHist, histMin, histMax);
  OneVarInBinsOfTheOther("Zjets_MC_Endcap_sihih_in_PFIso_bins_pt15_20", "pho sihih", tr_Zjets, var1, cutE && cutPtFirstBin, cutWeight, var2, nBinsVar2, binLimsVar2, nBinsHist, histMin, histMax);
  OneVarInBinsOfTheOther("Wg_MC_Endcap_sihih_in_PFIso_bins_pt15_20", "pho sihih", tr_Wg, var1, cutE && cutPtFirstBin, cutWeight, var2, nBinsVar2, binLimsVar2, nBinsHist, histMin, histMax);
  OneVarInBinsOfTheOther("Wjets_MC_Endcap_sihih_in_PFIso_bins_pt15_20", "pho sihih", tr_Wjets, var1, cutE && cutPtFirstBin, cutWeight, var2, nBinsVar2, binLimsVar2, nBinsHist, histMin, histMax);

  nBinsHist =17;
  histMin = 0.005;
  histMax = 0.022;

  OneVarInBinsOfTheOther("Zg_MC_Barrel_sihih_in_PFIso_bins_pt15_20", "pho sihih", tr_Zg, var1, cutB && cutPtFirstBin, cutWeight, var2, nBinsVar2, binLimsVar2, nBinsHist, histMin, histMax);
  OneVarInBinsOfTheOther("Zjets_MC_Barrel_sihih_in_PFIso_bins_pt15_20", "pho sihih", tr_Zjets, var1, cutB && cutPtFirstBin, cutWeight, var2, nBinsVar2, binLimsVar2, nBinsHist, histMin, histMax);
  OneVarInBinsOfTheOther("Wg_MC_Barrel_sihih_in_PFIso_bins_pt15_20", "pho sihih", tr_Wg, var1, cutB && cutPtFirstBin, cutWeight, var2, nBinsVar2, binLimsVar2, nBinsHist, histMin, histMax);
  OneVarInBinsOfTheOther("Wjets_MC_Barrel_sihih_in_PFIso_bins_pt15_20", "pho sihih", tr_Wjets, var1, cutB && cutPtFirstBin, cutWeight, var2, nBinsVar2, binLimsVar2, nBinsHist, histMin, histMax);
  // phoSigmaIEtaIEta in phoSCRChIso04Corr  bins
  // end
  ////////////////////////////////



  ////////////////////////////////
  // begin
  // phoSigmaIEtaIEta in phoSCRChIso04Corr  bins

  var2 = "phoSCRChIso04Corr";

  nBinsHist =24;
  histMin = 0.019;
  histMax = 0.067;

  OneVarInBinsOfTheOther("Zg_MC_Endcap_sihih_in_Iso_bins_pt15_20", "pho sihih", tr_Zg, var1, cutE && cutPtFirstBin, cutWeight, var2, nBinsVar2, binLimsVar2, nBinsHist, histMin, histMax);
  OneVarInBinsOfTheOther("Zjets_MC_Endcap_sihih_in_Iso_bins_pt15_20", "pho sihih", tr_Zjets, var1, cutE && cutPtFirstBin, cutWeight, var2, nBinsVar2, binLimsVar2, nBinsHist, histMin, histMax);
  OneVarInBinsOfTheOther("Wg_MC_Endcap_sihih_in_Iso_bins_pt15_20", "pho sihih", tr_Wg, var1, cutE && cutPtFirstBin, cutWeight, var2, nBinsVar2, binLimsVar2, nBinsHist, histMin, histMax);
  OneVarInBinsOfTheOther("Wjets_MC_Endcap_sihih_in_Iso_bins_pt15_20", "pho sihih", tr_Wjets, var1, cutE && cutPtFirstBin, cutWeight, var2, nBinsVar2, binLimsVar2, nBinsHist, histMin, histMax);

  nBinsHist =17;
  histMin = 0.005;
  histMax = 0.022;

  OneVarInBinsOfTheOther("Zg_MC_Barrel_sihih_in_Iso_bins_pt15_20", "pho sihih", tr_Zg, var1, cutB && cutPtFirstBin, cutWeight, var2, nBinsVar2, binLimsVar2, nBinsHist, histMin, histMax);
  OneVarInBinsOfTheOther("Zjets_MC_Barrel_sihih_in_Iso_bins_pt15_20", "pho sihih", tr_Zjets, var1, cutB && cutPtFirstBin, cutWeight, var2, nBinsVar2, binLimsVar2, nBinsHist, histMin, histMax);
  OneVarInBinsOfTheOther("Wg_MC_Barrel_sihih_in_Iso_bins_pt15_20", "pho sihih", tr_Wg, var1, cutB && cutPtFirstBin, cutWeight, var2, nBinsVar2, binLimsVar2, nBinsHist, histMin, histMax);
  OneVarInBinsOfTheOther("Wjets_MC_Barrel_sihih_in_Iso_bins_pt15_20", "pho sihih", tr_Wjets, var1, cutB && cutPtFirstBin, cutWeight, var2, nBinsVar2, binLimsVar2, nBinsHist, histMin, histMax);

  // phoSigmaIEtaIEta in phoSCRChIso04Corr  bins
  // end
  ////////////////////////////////
}// end of PlotSihihInIsoBins()

void PlotIsoInSihihBins(TTree* tr_Zg, TTree* tr_Wg, TTree* tr_Zjets, TTree* tr_Wjets, TCut cutB, TCut cutE, TCut cutPtFirstBin, TCut cutWeight)
{

  ////////////////////////////////
  // begin
  // phoPFChIsoCorr in phoSigmaIEtaIEta  bins

  TString var1 = "phoPFChIsoCorr";
  TString var2 = "phoSigmaIEtaIEta";
  const int nBinsVar2B= 3;
  float binLimsVar2B[nBinsVar2B+1]={0.0051,0.0111,0.0141,0.0201};
  const int nBinsVar2E= 3;
  float binLimsVar2E[nBinsVar2B+1]={0.0191,0.0331,0.0411,0.0611};
  int nBinsHist=21;
  float histMin = -0.9;
  float histMax = 20.1;

  OneVarInBinsOfTheOther("Zg_MC_Endcap_Iso_in_sihih_bins_pt15_20_PF", "pho _PF ch Iso", tr_Zg, var1, cutE && cutPtFirstBin, cutWeight, var2, nBinsVar2E, binLimsVar2E, nBinsHist, histMin, histMax);
  OneVarInBinsOfTheOther("Zjets_MC_Endcap_Iso_in_sihih_bins_pt15_20_PF", "pho _PF ch Iso", tr_Zjets, var1, cutE && cutPtFirstBin, cutWeight, var2, nBinsVar2E, binLimsVar2E, nBinsHist, histMin, histMax);
  OneVarInBinsOfTheOther("Wg_MC_Endcap_Iso_in_sihih_bins_pt15_20_PF", "pho _PF ch Iso", tr_Wg, var1, cutE && cutPtFirstBin, cutWeight, var2, nBinsVar2E, binLimsVar2E, nBinsHist, histMin, histMax);
  OneVarInBinsOfTheOther("Wjets_MC_Endcap_Iso_in_sihih_bins_pt15_20_PF", "pho _PF ch Iso", tr_Wjets, var1, cutE && cutPtFirstBin, cutWeight, var2, nBinsVar2E, binLimsVar2E, nBinsHist, histMin, histMax);

  OneVarInBinsOfTheOther("Zg_MC_Barrel_Iso_in_sihih_bins_pt15_20_PF", "pho _PF ch Iso", tr_Zg, var1, cutB && cutPtFirstBin, cutWeight, var2, nBinsVar2B, binLimsVar2B, nBinsHist, histMin, histMax);
  OneVarInBinsOfTheOther("Zjets_MC_Barrel_Iso_in_sihih_bins_pt15_20_PF", "pho _PF ch Iso", tr_Zjets, var1, cutB && cutPtFirstBin, cutWeight, var2, nBinsVar2B, binLimsVar2B, nBinsHist, histMin, histMax);
  OneVarInBinsOfTheOther("Wg_MC_Barrel_Iso_in_sihih_bins_pt15_20_PF", "pho _PF ch Iso", tr_Wg, var1, cutB && cutPtFirstBin, cutWeight, var2, nBinsVar2B, binLimsVar2B, nBinsHist, histMin, histMax);
  OneVarInBinsOfTheOther("Wjets_MC_Barrel_Iso_in_sihih_bins_pt15_20_PF", "pho _PF ch Iso", tr_Wjets, var1, cutB && cutPtFirstBin, cutWeight, var2, nBinsVar2B, binLimsVar2B, nBinsHist, histMin, histMax);

  // phoPFChIsoCorr in phoSigmaIEtaIEta  bins
  // end
  ////////////////////////////////



  ////////////////////////////////
  // begin
  // phoSCRChIso04Corr in phoSigmaIEtaIEta  bins
  var1 = "phoSCRChIso04Corr";

  OneVarInBinsOfTheOther("Zg_MC_Endcap_Iso_in_sihih_bins_pt15_20", "pho SCR ch Iso", tr_Zg, var1, cutE && cutPtFirstBin, cutWeight, var2, nBinsVar2E, binLimsVar2E, nBinsHist, histMin, histMax);
  OneVarInBinsOfTheOther("Zjets_MC_Endcap_Iso_in_sihih_bins_pt15_20", "pho SCR ch Iso", tr_Zjets, var1, cutE && cutPtFirstBin, cutWeight, var2, nBinsVar2E, binLimsVar2E, nBinsHist, histMin, histMax);
  OneVarInBinsOfTheOther("Wg_MC_Endcap_Iso_in_sihih_bins_pt15_20", "pho SCR ch Iso", tr_Wg, var1, cutE && cutPtFirstBin, cutWeight, var2, nBinsVar2E, binLimsVar2E, nBinsHist, histMin, histMax);
  OneVarInBinsOfTheOther("Wjets_MC_Endcap_Iso_in_sihih_bins_pt15_20", "pho SCR ch Iso", tr_Wjets, var1, cutE && cutPtFirstBin, cutWeight, var2, nBinsVar2E, binLimsVar2E, nBinsHist, histMin, histMax);

  OneVarInBinsOfTheOther("Zg_MC_Barrel_Iso_in_sihih_bins_pt15_20", "pho SCR ch Iso", tr_Zg, var1, cutB && cutPtFirstBin, cutWeight, var2, nBinsVar2B, binLimsVar2B, nBinsHist, histMin, histMax);
  OneVarInBinsOfTheOther("Zjets_MC_Barrel_Iso_in_sihih_bins_pt15_20", "pho SCR ch Iso", tr_Zjets, var1, cutB && cutPtFirstBin, cutWeight, var2, nBinsVar2B, binLimsVar2B, nBinsHist, histMin, histMax);
  OneVarInBinsOfTheOther("Wg_MC_Barrel_Iso_in_sihih_bins_pt15_20", "pho SCR ch Iso", tr_Wg, var1, cutB && cutPtFirstBin, cutWeight, var2, nBinsVar2B, binLimsVar2B, nBinsHist, histMin, histMax);
  OneVarInBinsOfTheOther("Wjets_MC_Barrel_Iso_in_sihih_bins_pt15_20", "pho SCR ch Iso", tr_Wjets, var1, cutB && cutPtFirstBin, cutWeight, var2, nBinsVar2B, binLimsVar2B, nBinsHist, histMin, histMax);

  // phoSCRChIso04Corr in phoSigmaIEtaIEta  bins
  // end
  ////////////////////////////////

}// end of PlotIsoInSihihBins()

void PlotIsoAndSihihInPtBins(TTree* tr_Zg, TTree* tr_Wg, TTree* tr_Zjets, TTree* tr_Wjets, TCut cutB, TCut cutE, TCut cutWeight)
{
  ////////////////////////////////
  // begin
  // phoSCRChIso04Corr and phoSigmaIEtaIEta in different phoEt bins

  TString var1 = "phoSCRChIso04Corr";
  TString var2 = "phoEt";
  const int nBinsVar2 = 6;
  float binLimsVar2[nBinsVar2+1]={15,25,35,55,75,95,500};
  int nBinsHist=21;
  float histMin = -0.9;
  float histMax = 20.1;

  OneVarInBinsOfTheOther("Zg_MC_Endcap_Iso_in_phoEt_bins", "pho SCR ch Iso", tr_Zg, var1, cutE, cutWeight, var2, nBinsVar2, binLimsVar2, nBinsHist, histMin, histMax);
  OneVarInBinsOfTheOther("Zjets_MC_Endcap_Iso_in_phoEt_bins", "pho SCR ch Iso", tr_Zjets, var1, cutE, cutWeight, var2, nBinsVar2, binLimsVar2, nBinsHist, histMin, histMax);
  OneVarInBinsOfTheOther("Wg_MC_Endcap_Iso_in_phoEt_bins", "pho SCR ch Iso", tr_Wg, var1, cutE, cutWeight, var2, nBinsVar2, binLimsVar2, nBinsHist, histMin, histMax);
  OneVarInBinsOfTheOther("Wjets_MC_Endcap_Iso_in_phoEt_bins", "pho SCR ch Iso", tr_Wjets, var1, cutE, cutWeight, var2, nBinsVar2, binLimsVar2, nBinsHist, histMin, histMax);

  OneVarInBinsOfTheOther("Zg_MC_Barrel_Iso_in_phoEt_bins", "pho SCR ch Iso", tr_Zg, var1, cutB, cutWeight, var2, nBinsVar2, binLimsVar2, nBinsHist, histMin, histMax);
  OneVarInBinsOfTheOther("Zjets_MC_Barrel_Iso_in_phoEt_bins", "pho SCR ch Iso", tr_Zjets, var1, cutB, cutWeight, var2, nBinsVar2, binLimsVar2, nBinsHist, histMin, histMax);
  OneVarInBinsOfTheOther("Wg_MC_Barrel_Iso_in_phoEt_bins", "pho SCR ch Iso", tr_Wg, var1, cutB, cutWeight, var2, nBinsVar2, binLimsVar2, nBinsHist, histMin, histMax);
  OneVarInBinsOfTheOther("Wjets_MC_Barrel_Iso_in_phoEt_bins", "pho SCR ch Iso", tr_Wjets, var1, cutB, cutWeight, var2, nBinsVar2, binLimsVar2, nBinsHist, histMin, histMax);

  var1 = "phoSigmaIEtaIEta";
  nBinsHist=24;
  histMin = 0.019;
  histMax = 0.067;

  OneVarInBinsOfTheOther("Zg_MC_Endcap_sihih_in_phoEt_bins", "pho sihih", tr_Zg, var1, cutE, cutWeight, var2, nBinsVar2, binLimsVar2, nBinsHist, histMin, histMax);
  OneVarInBinsOfTheOther("Zjets_MC_Endcap_sihih_in_phoEt_bins", "pho sihih", tr_Zjets, var1, cutE, cutWeight, var2, nBinsVar2, binLimsVar2, nBinsHist, histMin, histMax);
  OneVarInBinsOfTheOther("Wg_MC_Endcap_sihih_in_phoEt_bins", "pho sihih", tr_Wg, var1, cutE, cutWeight, var2, nBinsVar2, binLimsVar2, nBinsHist, histMin, histMax);
  OneVarInBinsOfTheOther("Wjets_MC_Endcap_sihih_in_phoEt_bins", "pho sihih", tr_Wjets, var1, cutE, cutWeight, var2, nBinsVar2, binLimsVar2, nBinsHist, histMin, histMax);

  nBinsHist=17;
  histMin = 0.005;
  histMax = 0.022;

  OneVarInBinsOfTheOther("Zg_MC_Barrel_sihih_in_phoEt_bins", "pho sihih", tr_Zg, var1, cutB, cutWeight, var2, nBinsVar2, binLimsVar2, nBinsHist, histMin, histMax);
  OneVarInBinsOfTheOther("Zjets_MC_Barrel_sihih_in_phoEt_bins", "pho sihih", tr_Zjets, var1, cutB, cutWeight, var2, nBinsVar2, binLimsVar2, nBinsHist, histMin, histMax);
  OneVarInBinsOfTheOther("Wg_MC_Barrel_sihih_in_phoEt_bins", "pho sihih", tr_Wg, var1, cutB, cutWeight, var2, nBinsVar2, binLimsVar2, nBinsHist, histMin, histMax);
  OneVarInBinsOfTheOther("Wjets_MC_Barrel_sihih_in_phoEt_bins", "pho sihih", tr_Wjets, var1, cutB, cutWeight, var2, nBinsVar2, binLimsVar2, nBinsHist, histMin, histMax);

  // phoSCRChIso04Corr and phoSigmaIEtaIEta in different phoEt bins
  // end
  ////////////////////////////////
}// end of PlotIsoAndSihihInPtBins()
