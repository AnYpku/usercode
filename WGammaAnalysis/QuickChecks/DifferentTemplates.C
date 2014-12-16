
#include "../Include/TPhotonCuts.h"
#include "../Configuration/TConfiguration.h"

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>

#include "TFile.h"
#include "TString.h"
#include "TCut.h"
#include "TCanvas.h"
#include "THStack.h"
#include "TVectorD.h"
#include "TLine.h"
#include "TLatex.h"
#include "TLegend.h"

void Plot(TString strCanvName, TString strRandCone, TString strChIsoFromFSR, TString strSihihFromFSR, bool doYutaro, THStack* hStack[2], TLegend* leg[2]);

TCut CutEta(int ieta)
{
TConfiguration config;
TPhotonCuts photon;
  if (ieta==config.BARREL) return photon.RangeBarrel();
  if (ieta==config.ENDCAP) return photon.RangeEndcap();
  return "1";
}

TString StrLabelEta(int ieta)
{
TConfiguration config;
TPhotonCuts photon;
  if (ieta==config.BARREL) return "_Barrel_";
  if (ieta==config.ENDCAP) return "_Endcap_";
  return "_EtaCommon_";
}

void DifferentTemplates()
{
  THStack *hStackWg[2], *hStackZg[2];
  TLegend *legWg[2], *legZg[2];
    legWg[0]=new TLegend(0.55,0.55,0.95,0.90);
    legZg[0]=new TLegend(0.55,0.75,0.95,0.90);
    legWg[1]=new TLegend(0.35,0.55,0.95,0.90);
    legZg[1]=new TLegend(0.35,0.55,0.95,0.90);

//  TFile* fWg = new TFile();
//  TFile* fZgWg = new TFile();
//  TFile* fWgtaunu = new TFile();
  TFile* fWg = new TFile("../WGammaOutput/MUON/FullySelected/selected_WGammaSIGMC.root");
  TTree* trWg=(TTree*)fWg->Get("selectedEvents");
  TH1F* hWgB=new TH1F("hWgB","hZgB",5,15,40);
  TH1F* hWgE=new TH1F("hWgE","hZgE",5,15,40);
  trWg->Draw("phoEt>>hWgB","(sqrt(phoSCEta*phoSCEta)<1.4442)*weight","goff");
  trWg->Draw("phoEt>>hWgE","(sqrt(phoSCEta*phoSCEta)>1.5)*weight","goff");
  hWgB->SetFillColor(634);
  hWgE->SetFillColor(634);
  hWgB->SetLineColor(634);
  hWgE->SetLineColor(634);
  TFile* fZgWg = new TFile("../WGammaOutput/MUON/FullySelected/selected_WGammaBKGMC_Zg.root");
  TTree* trZgWg=(TTree*)fZgWg->Get("selectedEvents");
  TH1F* hZgWgB=new TH1F("hZgWgB","hZgWgB",5,15,40);
  TH1F* hZgWgE=new TH1F("hZgWgE","hZgWgE",5,15,40);
  trZgWg->Draw("phoEt>>hZgWgB","(sqrt(phoSCEta*phoSCEta)<1.4442)*weight","goff");
  trZgWg->Draw("phoEt>>hZgWgE","(sqrt(phoSCEta*phoSCEta)>1.5)*weight","goff");
  hZgWgB->SetFillColor(879);
  hZgWgE->SetFillColor(879);
  hZgWgB->SetLineColor(879);
  hZgWgE->SetLineColor(879);
  TFile* fWgtaunu = new TFile("../WGammaOutput/MUON/FullySelected/selected_WGammaBKGMC_Wg_to_taunu.root");
  TTree* trWgtaunu=(TTree*)fWgtaunu->Get("selectedEvents");
  TH1F* hWgtaunuB=new TH1F("hWgtaunuB","hWgtaunuB",5,15,40);
  TH1F* hWgtaunuE=new TH1F("hWgtaunuE","hWgtaunuE",5,15,40);
  trWgtaunu->Draw("phoEt>>hWgtaunuB","(sqrt(phoSCEta*phoSCEta)<1.4442)*weight","goff");
  trWgtaunu->Draw("phoEt>>hWgtaunuE","(sqrt(phoSCEta*phoSCEta)>1.5)*weight","goff");
  hWgtaunuB->SetFillColor(401);
  hWgtaunuE->SetFillColor(401);
  hWgtaunuB->SetLineColor(401);
  hWgtaunuE->SetLineColor(401);
  hStackWg[0]=new THStack("hStackWgB","hStackWgB");
  hStackWg[1]=new THStack("hStackWgE","hStackWgE");
  hStackWg[0]->Add(hWgB);
  hStackWg[1]->Add(hWgE);
  hStackWg[0]->Add(hZgWgB);
  hStackWg[1]->Add(hZgWgE);
  hStackWg[0]->Add(hWgtaunuB);
  hStackWg[1]->Add(hWgtaunuE);
  legWg[0]->AddEntry(hWgB,"W#gamma#rightarrow#mu#nu#gamma","f");
  legWg[0]->AddEntry(hZgWgB,"Z#gamma","f");
  legWg[0]->AddEntry(hWgtaunuB,"W#gamma#rightarrow#tau#nu#gamma","f");
  Plot("Wg_DifferentTemplates",
       "../WGammaOutput/MUON/YieldsAndBackground/DDTemplate_WGamma_phoEt_RandCone.root",
       "../WGammaOutput/MUON/YieldsAndBackground/DDTemplate_WGamma_phoEt_chIsoFSR.root",
       "../WGammaOutput/MUON/YieldsAndBackground/DDTemplate_WGamma_phoEt_Sihih_UpSideband.root",
       1, hStackWg, legWg);
  TFile* fZg = new TFile("../WGammaOutput/MUON/FullySelected/selected_ZGammaSIGMC.root");
  TTree* trZg=(TTree*)fZg->Get("selectedEvents");
  TH1F* hZgB=new TH1F("hZgB","hZgB",5,15,40);
  TH1F* hZgE=new TH1F("hZgE","hZgE",5,15,40);
  trZg->Draw("phoEt>>hZgB","(sqrt(phoSCEta*phoSCEta)<1.4442)*weight","goff");
  trZg->Draw("phoEt>>hZgE","(sqrt(phoSCEta*phoSCEta)>1.5)*weight","goff");
  hZgB->SetFillColor(879);
  hZgE->SetFillColor(879);
  hZgB->SetLineColor(879);
  hZgE->SetLineColor(879);
  hStackZg[0]=new THStack("hStackZgB","hStackZgB");
  hStackZg[1]=new THStack("hStackZgE","hStackZgE");
  hStackZg[0]->Add(hZgB);
  hStackZg[1]->Add(hZgE);
  legZg[0]->AddEntry(hZgB,"Z#gamma","f");
  Plot("Zg_DifferentTemplates",
       "../WGammaOutput/MUON/YieldsAndBackground/DDTemplate_ZGamma_phoEt_RandCone.root",
       "../WGammaOutput/MUON/YieldsAndBackground/DDTemplate_ZGamma_phoEt_chIsoFSR.root",
       "../WGammaOutput/MUON/YieldsAndBackground/DDTemplate_ZGamma_phoEt_Sihih_UpSideband.root",
       0, hStackZg, legZg);
}// end of DifferentTemplates()

void Plot(TString strCanvName, TString strRandCone, TString strChIsoFromFSR, TString strSihihFromFSR, bool doYutaro, THStack* hStack[2], TLegend* leg[2])
{
TConfiguration config;
TPhotonCuts photon;
  TFile* fRC = new TFile(strRandCone);
  TH1F* hRC[2];
  hRC[0] = (TH1F*)fRC->Get("yieldsDDTrue1DB");
  hRC[1] = (TH1F*)fRC->Get("yieldsDDTrue1DE");
  TFile* fChIsoFSR = new TFile(strChIsoFromFSR);
  TH1F* hChIsoFSR[2];
  hChIsoFSR[0] = (TH1F*)fChIsoFSR->Get("yieldsDDTrue1DB");
  hChIsoFSR[1] = (TH1F*)fChIsoFSR->Get("yieldsDDTrue1DE");
  TFile* fSihihFSR = new TFile(strSihihFromFSR);
  TH1F* hSihihFSR[2];
  hSihihFSR[0] = (TH1F*)fSihihFSR->Get("yieldsDDTrue1DB");
  hSihihFSR[1] = (TH1F*)fSihihFSR->Get("yieldsDDTrue1DE");
  TH1D* hPur[2]; TH1F *hPurBins[2], *hData[2], *hYutaro[2];
  if (doYutaro){
    TFile* fPurity = new TFile("../purity.root");
    TFile* fObsData = new TFile("../WGammaOutput/MUON/FullySelected/selected_WGamma_UNblind_DATA.root");
    TTree* tr = (TTree*)fObsData->Get("selectedEvents");
    hData[0]=(TH1F*)hSihihFSR[0]->Clone("hDataB");
    tr->Draw("phoEt>>hDataB",CutEta(0),"goff");
    hData[1]=(TH1F*)hSihihFSR[1]->Clone("hDataE");
    tr->Draw("phoEt>>hDataE",CutEta(1),"goff");
    hPur[0]=(TH1D*)fPurity->Get("barrel_photon_purity");
    hPur[1]=(TH1D*)fPurity->Get("endcap_photon_purity");
    for (int ieta=config.BARREL; ieta<=config.ENDCAP; ieta++){
      hYutaro[ieta]=(TH1F*)hData[ieta]->Clone("hYutaro"+StrLabelEta(ieta));
      hPurBins[ieta]=(TH1F*)hData[ieta]->Clone("hPurBins"+StrLabelEta(ieta));
      for (int ib=1; ib<=hYutaro[ieta]->GetNbinsX(); ib++){        
        hPurBins[ieta]->SetBinContent(ib,hPur[ieta]->GetBinContent(ib));
        hPurBins[ieta]->SetBinError(ib,hPur[ieta]->GetBinError(ib));
      }
      hYutaro[ieta]->Multiply(hPurBins[ieta]);
    }
  }
  TCanvas* canv = new TCanvas(strCanvName,strCanvName,400,800);
  canv->Divide(1,2);
  for (int ieta=config.BARREL; ieta<=config.ENDCAP; ieta++){
    canv->cd(ieta+1);
    hRC[ieta]->SetLineColor(1);
    hChIsoFSR[ieta]->SetLineColor(2);
    hSihihFSR[ieta]->SetLineColor(8);
    hRC[ieta]->SetLineWidth(2);
    hChIsoFSR[ieta]->SetLineWidth(2);
    hSihihFSR[ieta]->SetLineWidth(2);
    float max = hSihihFSR[ieta]->GetMaximum();
    if (doYutaro) max=hYutaro[ieta]->GetMaximum();
    hSihihFSR[ieta]->GetYaxis()->SetRangeUser(0,1.2*max);
    hSihihFSR[ieta]->GetXaxis()->SetTitle("photon pt, GeV");
    hSihihFSR[ieta]->SetStats(0);
    TString strTitle; 
    if (strCanvName.Contains("Wg")) strTitle="Wg, MUON, "+StrLabelEta(ieta);
    if (strCanvName.Contains("Zg")) strTitle="Zg, MUON, "+StrLabelEta(ieta);
    hSihihFSR[ieta]->SetTitle(strTitle);
    hSihihFSR[ieta]->Draw("EP");
    hStack[ieta]->Draw("same");
    hSihihFSR[ieta]->Draw("EP same");
    hRC[ieta]->Draw("EP same");
    hChIsoFSR[ieta]->SetLineStyle(7);
    hChIsoFSR[ieta]->Draw("EP same");
    if (doYutaro) {
      hYutaro[ieta]->SetLineColor(4);
      hYutaro[ieta]->SetLineWidth(2);
      hYutaro[ieta]->SetLineStyle(7);
      hYutaro[ieta]->Draw("EP same");
    }
    if (ieta==config.BARREL) leg[0]->Draw("same");
    if (ieta==config.ENDCAP){
      leg[1]->AddEntry(hChIsoFSR[1],"I_ch templ, FSR", "l");
      leg[1]->AddEntry(hRC[1],"I_ch templ, Rand cone", "l");
      leg[1]->AddEntry(hSihihFSR[0],"#sigmai#etai#eta templ, Katya", "l");
      if (doYutaro) leg[1]->AddEntry(hYutaro[0],"#sigmai#etai#eta templ, Yutaro", "l");
      leg[1]->Draw("same");
    }
  }
}// end of Plot()


