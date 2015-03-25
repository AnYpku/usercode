#include "TFile.h"
#include "TTree.h"
#include "TCut.h"
#include "TString.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TLegend.h"

TCut CutPt(float lowPt, float upPt)
{
  TString strPt = "phoEt>";
  strPt+=lowPt;
  strPt+= "&& phoEt<=";
  strPt+=upPt;
  TCut cut(strPt);
  return cut;  
}// end of CutPt

void NormalizeHist(TH1F* h)
{
  int Nb=h->GetNbinsX();
  float norm = 0;
  for (int ib=1; ib<=Nb; ib++){
    norm+=h->GetBinContent(ib);
  }// end of ib
  for (int ib=1; ib<=Nb; ib++){
    float cont = h->GetBinContent(ib);
    float err = h->GetBinError(ib);
    h->SetBinContent(ib,cont/norm);
    h->SetBinError(ib,err/norm);
  }// end of ib
}// end of NomalizeHist

void DrawTrueOrFake(TString strTrueOrFake, TString varIso, TCut cutSihih[2], TTree* trWgMuo, TTree* trZgMuo, TTree* trWgEle, TTree* trZgEle)
{
// varIso - phoRandConeChIsoCorr for True, phoSCRChIsoCorr for fake
  const int nPtBins = 7;
  const double lims[nPtBins+1]={15,20,25,30,35,45,55,500};
  TCut cutEta[2];
  cutEta[0]="phoSCEta>-1.4442 && phoSCEta<1.4442";
  cutEta[1]="(phoSCEta>-2.5 && phoSCEta<-1.566)||(phoSCEta<2.5 && phoSCEta>1.566)";

  TH1F* hChIsoWgMuo[nPtBins][2];
  TH1F* hChIsoWgEle[nPtBins][2];
  TH1F* hChIsoZgMuo[nPtBins][2];
  TH1F* hChIsoZgEle[nPtBins][2];


  TFile* fOut = new TFile(TString("fOut_")+strTrueOrFake+TString(".root"),"recreate"); 

  for (int ipt=0; ipt<nPtBins; ipt++){
    TCut cutPt =  CutPt(lims[ipt],lims[ipt+1]);
    for (int ieta=0; ieta<=1; ieta++){
      TCut cut = cutPt && cutEta[ieta] && cutSihih[ieta];
      TString strAffix="_pt";
      strAffix+=lims[ipt];
      strAffix+="to";
      strAffix+=lims[ipt+1];
      if (ieta==0) strAffix+="_Barrel_";
      if (ieta==1) strAffix+="_Endcap_";

      int nBins=21;
      float low = -0.9;
      float up = 20.1;

      if (strTrueOrFake=="True") {up=7.1; nBins=8;};

      hChIsoWgMuo[ipt][ieta] = new TH1F(TString("hWgMuo")+strAffix+strTrueOrFake,"WGamma, MUON",nBins,low,up);
      hChIsoZgMuo[ipt][ieta] = new TH1F(TString("hZgMuo")+strAffix+strTrueOrFake,"ZGamma, MUON",nBins,low,up);
      hChIsoWgEle[ipt][ieta] = new TH1F(TString("hWgEle")+strAffix+strTrueOrFake,"WGamma, ELECTRON",nBins,low,up);
      hChIsoZgEle[ipt][ieta] = new TH1F(TString("hZgEle")+strAffix+strTrueOrFake,"ZGamma, ELECTRON",nBins,low,up);

      trWgMuo->Draw(varIso+TString(">>")+hChIsoWgMuo[ipt][ieta]->GetName(),cut,"goff");
      trZgMuo->Draw(varIso+TString(">>")+hChIsoZgMuo[ipt][ieta]->GetName(),cut,"goff");
      trWgEle->Draw(varIso+TString(">>")+hChIsoWgEle[ipt][ieta]->GetName(),cut,"goff");
      trZgEle->Draw(varIso+TString(">>")+hChIsoZgEle[ipt][ieta]->GetName(),cut,"goff");


      hChIsoWgMuo[ipt][ieta]->SetLineWidth(2);
      hChIsoWgMuo[ipt][ieta]->SetLineColor(1);
      hChIsoZgMuo[ipt][ieta]->SetLineWidth(2);
      hChIsoZgMuo[ipt][ieta]->SetLineColor(2);
      hChIsoWgEle[ipt][ieta]->SetLineWidth(2);
      hChIsoWgEle[ipt][ieta]->SetLineColor(3);
      hChIsoZgEle[ipt][ieta]->SetLineWidth(2);
      hChIsoZgEle[ipt][ieta]->SetLineColor(4);

      NormalizeHist(hChIsoWgMuo[ipt][ieta]);
      NormalizeHist(hChIsoZgMuo[ipt][ieta]);
      NormalizeHist(hChIsoWgEle[ipt][ieta]);
      NormalizeHist(hChIsoZgEle[ipt][ieta]);

      TCanvas* canv = new TCanvas(TString("c")+strTrueOrFake+strAffix,TString("c")+strTrueOrFake+strAffix, 600, 600);

      float max = 0;
      for (int ib=1; ib<=hChIsoWgMuo[ipt][ieta]->GetNbinsX(); ib++){
        if (hChIsoWgMuo[ipt][ieta]->GetBinContent(ib)>max) max=hChIsoWgMuo[ipt][ieta]->GetBinContent(ib);
        if (hChIsoZgMuo[ipt][ieta]->GetBinContent(ib)>max) max=hChIsoZgMuo[ipt][ieta]->GetBinContent(ib);
        if (hChIsoWgEle[ipt][ieta]->GetBinContent(ib)>max) max=hChIsoWgEle[ipt][ieta]->GetBinContent(ib);
        if (hChIsoZgEle[ipt][ieta]->GetBinContent(ib)>max) max=hChIsoZgEle[ipt][ieta]->GetBinContent(ib);
      }// end of ib

      hChIsoWgMuo[ipt][ieta]->SetStats(0);
      hChIsoWgMuo[ipt][ieta]->GetYaxis()->SetRangeUser(0.0,1.1*max);
      hChIsoWgMuo[ipt][ieta]->GetXaxis()->SetTitle(varIso);
      hChIsoWgMuo[ipt][ieta]->Draw("EP");
      hChIsoZgMuo[ipt][ieta]->Draw("EP same");
      hChIsoWgEle[ipt][ieta]->Draw("EP same");
      hChIsoZgEle[ipt][ieta]->Draw("EP same");

      TLegend* leg = new TLegend(0.5,0.5,0.9,0.9);
      leg->AddEntry(hChIsoWgMuo[ipt][ieta],hChIsoWgMuo[ipt][ieta]->GetTitle(),"l");
      leg->AddEntry(hChIsoZgMuo[ipt][ieta],hChIsoZgMuo[ipt][ieta]->GetTitle(),"l");
      leg->AddEntry(hChIsoWgEle[ipt][ieta],hChIsoWgEle[ipt][ieta]->GetTitle(),"l");
      leg->AddEntry(hChIsoZgEle[ipt][ieta],hChIsoZgEle[ipt][ieta]->GetTitle(),"l");
      leg->Draw("same");

      hChIsoWgMuo[ipt][ieta]->SetTitle(strTrueOrFake+strAffix);
      hChIsoWgMuo[ipt][ieta]->Draw("EP same");

      TString strSave="../WGammaOutput/MUON_WGamma/Plots/QuickChecks/";
      strSave+=canv->GetName()+TString(".png");

      canv->SaveAs(strSave);

    }// end of loop over ieta
  }//end of loop over ipt  

}// end of DrawTrueOrFake

void CheckMuonVsElectronTemplates()
{
  TFile fWgMuo("../WGammaOutput/MUON_WGamma/PreliminaryForTemplateMethodSelected/selected_WGamma_UNblind_DATA.root");
  TFile fZgMuo("../WGammaOutput/MUON_ZGamma/PreliminaryForTemplateMethodSelected/selected_ZGamma_UNblind_DATA.root");
  TFile fWgEle("../WGammaOutput/ELECTRON_WGamma/PreliminaryForTemplateMethodSelected/selected_WGamma_UNblind_DATA.root");
  TFile fZgEle("../WGammaOutput/ELECTRON_ZGamma/PreliminaryForTemplateMethodSelected/selected_ZGamma_UNblind_DATA.root");

  TTree* trWgMuo = (TTree*)fWgMuo.Get("selectedEvents");
  TTree* trZgMuo = (TTree*)fZgMuo.Get("selectedEvents");
  TTree* trWgEle = (TTree*)fWgEle.Get("selectedEvents");
  TTree* trZgEle = (TTree*)fZgEle.Get("selectedEvents");

  TCut cutSihihTrue[2];
  TCut cutSihihFake[2];
  cutSihihTrue[0]="phoSigmaIEtaIEta<0.011";
  cutSihihTrue[1]="phoSigmaIEtaIEta<0.033";
  cutSihihFake[0]="phoSigmaIEtaIEta>0.011 && phoSigmaIEtaIEta<0.014";
  cutSihihFake[1]="phoSigmaIEtaIEta>0.033 && phoSigmaIEtaIEta<0.044";

  DrawTrueOrFake("True", "phoRandConeChIsoCorr", cutSihihTrue, trWgMuo, trZgMuo, trWgEle, trZgEle);
  DrawTrueOrFake("Fake", "phoSCRChIsoCorr",      cutSihihFake, trWgMuo, trZgMuo, trWgEle, trZgEle);

}// end of CheckMuonVsElectronTemplates

