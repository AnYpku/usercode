#include "TFile.h"
#include "TH1F.h"
#include "TString.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "TLine.h"
#include "TLatex.h"

#include "../Configuration/TConfiguration.h"

#include <iostream>
#include <iomanip>

void SetOttosHists(TFile* fOut, TH1F* h[2]);
float DifferenceInOneBin(TH1F* h1, TH1F* h2, int ib);
void PrintLatexCS(TH1F* hTheory, TH1F* hMuonStat, TH1F* hElectronStat, TH1F* hMuonSyst, TH1F* hElectronSyst);

TString StrProcess(int vg, int ch){
  TConfiguration c;
  if (vg==c.W_GAMMA && ch==c.MUON) return "#mu#nu#gamma";
  if (vg==c.W_GAMMA && ch==c.ELECTRON) return "e#nu#gamma";
  if (vg==c.Z_GAMMA && ch==c.MUON) return "#mu#mu#gamma";
  if (vg==c.Z_GAMMA && ch==c.ELECTRON) return "ee#gamma";
}// end of StrProcess(int vg, int ch)

void CompareCS(int vgamma)
{

  TFile* fCS[2]; //MUON, ELECTRON
  TFile* fCStheory; //MUON, ELECTRON
  TH1F* hMeasured1D[2];
  TH1F* hMeasured1D_Stat[2];
  TH1F* hMeasured1D_Syst[2];
  TH1F* hTheory1D[2];
  TH1F* hOtto1D[2];//Z_GAMMA only

  TH1F* hMeasuredTotal[2];
  float thTotalCSval[2]={0.0,0.0};
  

  TConfiguration config;
  TFile* fOut=new TFile("fOut.root","recreate");
  SetOttosHists(fOut,hOtto1D);

  for (int ich=config.MUON; ich<=config.ELECTRON; ich++){

    fCStheory=new TFile(config.GetAccXEffFileName(config.BOTH_CHANNELS, vgamma));
    std::cout<<"fCStheory="<<config.GetAccXEffFileName(config.BOTH_CHANNELS, vgamma)<<std::endl;
    fCS[ich]=new TFile(config.GetCrossSectionFileName(ich, vgamma));
    std::cout<<"fCS="<<config.GetCrossSectionFileName(ich, vgamma)<<std::endl;
    hMeasured1D[ich]=(TH1F*)fCS[ich]->Get(config.GetCSname(ich, config.ONEDI));
    hMeasured1D_Stat[ich]=(TH1F*)fCS[ich]->Get(config.GetCSname(ich, config.ONEDI)+TString("_errStat"));
    hMeasured1D_Syst[ich]=(TH1F*)fCS[ich]->Get(config.GetCSname(ich, config.ONEDI)+TString("_errSyst"));
    hMeasuredTotal[ich]=(TH1F*)fCS[ich]->Get(config.GetCSname(ich, config.TOTAL));
    hTheory1D[ich]=(TH1F*)fCStheory->Get(config.GetTheoryCSname(config.ONEDI));
    hTheory1D[ich]->SetLineWidth(2);
    hMeasured1D[ich]->SetLineWidth(2);
    hOtto1D[ich]->SetLineWidth(2);

    for (int ib=1; ib<=hTheory1D[ich]->GetNbinsX(); ib++){
      float cont = hTheory1D[ich]->GetBinContent(ib);
      float err = hTheory1D[ich]->GetBinError(ib);
      hTheory1D[ich]->SetBinContent(ib,1000*cont);
      hTheory1D[ich]->SetBinError(ib,1000*err);
    }// end of loop over ib

    for (int ib=2; ib<=hTheory1D[ich]->GetNbinsX(); ib++){
      thTotalCSval[ich]+=hTheory1D[ich]->GetBinContent(ib)*hTheory1D[ich]->GetBinWidth(ib);
    }//end of ib

  }//end of loop over ich

  

  PrintLatexCS(hTheory1D[0],hMeasured1D_Stat[0],hMeasured1D_Stat[1],hMeasured1D_Syst[0],hMeasured1D_Syst[1] );

  hMeasured1D[config.MUON]->SetLineColor(4);
  hMeasured1D[config.MUON]->SetMarkerColor(4);
  hMeasured1D[config.ELECTRON]->SetLineColor(809);
  hMeasured1D[config.ELECTRON]->SetLineStyle(7);
  hMeasured1D[config.ELECTRON]->SetMarkerColor(809);
  hTheory1D[config.MUON]->SetLineColor(6);
  hTheory1D[config.ELECTRON]->SetLineColor(2);
  hOtto1D[config.MUON]->SetLineColor(7);
  hOtto1D[config.ELECTRON]->SetLineColor(8);

  TLegend* leg;
  if (vgamma==config.Z_GAMMA) leg = new TLegend(0.50,0.60,0.88,0.85);
  if (vgamma==config.W_GAMMA) leg = new TLegend(0.50,0.70,0.88,0.85);

  for (int ich=config.MUON; ich<=config.ELECTRON; ich++)
    leg->AddEntry(hMeasured1D[ich],TString("Data ")+StrProcess(vgamma,ich),"l");
  for (int ich=config.MUON; ich<=config.MUON; ich++)
    leg->AddEntry(hTheory1D[ich],("NLO theory"),"l");
  if (vgamma==config.Z_GAMMA){
    for (int ich=config.MUON; ich<=config.ELECTRON; ich++)
      leg->AddEntry(hOtto1D[ich],TString("CMS published ")+StrProcess(vgamma,ich),"l");
  }

  leg->SetFillColor(0);  
  leg->SetLineColor(0);



  TString txt_CMS_Preliminary = "#font[52]{Work in progress, CMS 2012, #sqrt{s}=8 TeV, L=19.6 fb^{-1}}"; 
  //"#scale[1.4]{#font[61]{CMS}} #font[52]{Preliminary}";
  TString strHeader=txt_CMS_Preliminary;

  // differential cross section

  TString canvTitle = "c_CS_Compare_";
  canvTitle+=config.StrVgType(vgamma);
  TCanvas* canv = new TCanvas(canvTitle,canvTitle,600,600);
  canv->cd();
  canv->SetLogx();  canv->SetLogy();
  canv->SetLeftMargin(0.15);
  strHeader = txt_CMS_Preliminary;
  hMeasured1D[config.MUON]->SetTitle("");
  hMeasured1D[config.MUON]->GetXaxis()->SetTitle("p_{T}^{#gamma}, GeV/c");
  if (vgamma==config.W_GAMMA) hMeasured1D[config.MUON]->GetYaxis()->SetRangeUser(0.1,1000);
  if (vgamma==config.Z_GAMMA) hMeasured1D[config.MUON]->GetYaxis()->SetRangeUser(0.01,250);
  hMeasured1D[config.MUON]->GetXaxis()->SetRangeUser(15,500);
  hMeasured1D[config.ELECTRON]->GetXaxis()->SetRangeUser(15,500);
  hMeasured1D[config.MUON]->GetYaxis()->SetTitle("d#sigma/dP_{T}^{#gamma}, fb/GeV");
  hMeasured1D[config.MUON]->GetYaxis()->SetTitleOffset(1.6);
  hMeasured1D[config.MUON]->GetYaxis()->SetMoreLogLabels(0);
  hMeasured1D[config.MUON]->Draw("EP");
  hMeasured1D[config.ELECTRON]->Draw("EP same");
  hTheory1D[config.MUON]->Draw("EP same");
  //  hTheory1D[config.ELECTRON]->Draw("EP same");
  if (vgamma==config.Z_GAMMA){
    hOtto1D[config.MUON]->Draw("EP same");
    hOtto1D[config.ELECTRON]->Draw("EP same");
  }
  leg->Draw("same");
  TLatex* text = new TLatex(0.15,0.93,strHeader);
  text->SetNDC();
  text->SetTextSize(0.04);
  text->Draw("same");

  canv->SaveAs(TString("compareCS")+config.StrVgType(vgamma)+TString(".png"));
  canv->SaveAs(TString("compareCS")+config.StrVgType(vgamma)+TString(".pdf"));

  // ratio muon/electron

  TString canvTitle1 = "c_CS_Ratio_";
  canvTitle1+=config.StrVgType(vgamma);
  TCanvas* canv1 = new TCanvas(canvTitle1,canvTitle1,600,600);
  canv1->SetLogx();
  canv1->SetLeftMargin(0.15);
  TH1F* hRatioMuEle = (TH1F*)hMeasured1D[config.MUON]->Clone("hRatioMuEle");
  hRatioMuEle->Divide(hMeasured1D[config.ELECTRON]);
  hRatioMuEle->SetLineWidth(2);
  hRatioMuEle->SetLineColor(1);
  hRatioMuEle->GetXaxis()->SetRangeUser(15,500);
  hRatioMuEle->GetXaxis()->SetTitle("p_{T}^{#gamma}, GeV/c");
  hRatioMuEle->GetYaxis()->SetTitle("(d#sigma_{#mu}/dP_{T}^{#gamma}):(d#sigma_{e}/dP_{T}^{#gamma})");
  hRatioMuEle->GetYaxis()->SetTitleOffset(1.6);
//  hRatioMuEle->SetTitle(txt_CMS_Preliminary+(" d#sigma/dP_{T}^{#gamma} muon/electron, ")+TString(config.StrVgType(vgamma)));
//  TLegend* leg1 = new TLegend(0.55,0.80,0.90,0.90);
//  leg1->AddEntry(hRatioMuEle,"mu/ele","l");
  hRatioMuEle->GetYaxis()->SetRangeUser(0.5,1.5);
  hRatioMuEle->SetTitle("");
  hRatioMuEle->Draw("EP");
//  leg1->Draw("same");
  hRatioMuEle->Draw("EP same");
  TLine* line1 = new TLine(15,1,500,1);
  line1->SetLineWidth(2);
  line1->SetLineStyle(9);
  line1->Draw("same");
  strHeader = txt_CMS_Preliminary; 
  TLatex* text1 = new TLatex(0.15,0.93,strHeader);
  text1->SetNDC();
  text1->SetTextSize(0.04);
  text1->Draw("same");
  //  TLatex* textVg = new TLatex(0.70,0.80,config.StrVgType(vgamma));
  //  textVg->SetNDC();
  //  textVg->SetTextSize(0.04);
  //  textVg->Draw("same");
  canv1->SaveAs(TString("compareCSratio")+config.StrVgType(vgamma)+TString(".png"));
  canv1->SaveAs(TString("compareCSratio")+config.StrVgType(vgamma)+TString(".pdf"));

  // ratio measured/Otto

  if (vgamma==config.Z_GAMMA){
    TString canvTitle2 = "c_CS_Ratio_Otto_";
    canvTitle2+=config.StrVgType(vgamma);
    TCanvas* canv2 = new TCanvas(canvTitle2,canvTitle2,600,600);
    canv2->SetLogx();
    canv2->SetLeftMargin(0.15);
    TH1F* hRatioMuOtto = (TH1F*)hMeasured1D[config.MUON]->Clone("hRatioMuOtto");
    //hRatioMuOtto->SetTitle(txt_CMS_Preliminary+" Z#gamma check: d#sigma/dP_{T}^{#gamma} meas./approved ");
    hRatioMuOtto->SetTitle(txt_CMS_Preliminary);
    hRatioMuOtto->GetYaxis()->SetRangeUser(0.50,1.50);
    hRatioMuOtto->Divide(hOtto1D[config.MUON]);
    hRatioMuOtto->SetLineWidth(2);
    hRatioMuOtto->SetLineColor(4);   
    hRatioMuOtto->SetMarkerColor(4);  
    TH1F* hRatioEleOtto = (TH1F*)hMeasured1D[config.ELECTRON]->Clone("hRatioEleOtto");
    hRatioEleOtto->Divide(hOtto1D[config.ELECTRON]);
    hRatioEleOtto->SetLineWidth(3);
    hRatioEleOtto->SetLineColor(809); 
    hRatioEleOtto->SetMarkerColor(809); 
    TLegend* leg2 = new TLegend(0.50,0.78,0.70,0.88);
    leg2->SetFillColor(0);
    leg2->SetLineColor(0);
    leg2->AddEntry(hRatioMuOtto,TString("Data ")+StrProcess(vgamma,config.MUON),"l");
    leg2->AddEntry(hRatioEleOtto,TString("Data ")+StrProcess(vgamma,config.ELECTRON),"l");  
    hRatioMuOtto->GetXaxis()->SetTitle("p_{T}^{#gamma}, GeV/c");
    hRatioMuOtto->Draw("EP");
    line1->Draw("same");
    leg2->Draw("same");
    hRatioMuOtto->GetXaxis()->SetRangeUser(15,500);
    hRatioEleOtto->GetXaxis()->SetRangeUser(15,500);
    hRatioMuOtto->GetYaxis()->SetRangeUser(0.3,3.0);
    hRatioEleOtto->GetYaxis()->SetRangeUser(0.3,3.0);
    hRatioMuOtto->Draw("EP same");
    hRatioEleOtto->SetLineStyle(7);
    hRatioEleOtto->Draw("EP same");
    canv2->SaveAs(TString("compareCSratioOtto")+config.StrVgType(vgamma)+TString(".png"));
    canv2->SaveAs(TString("compareCSratioOtto")+config.StrVgType(vgamma)+TString(".pdf"));
  } // end of (vgamma==config.Z_GAMMA)




  // ratio measured/MCbased

    TString canvTitle3 = "c_CS_Ratio_Theory_";
    canvTitle3+=config.StrVgType(vgamma);
    TCanvas* canv3 = new TCanvas(canvTitle3,canvTitle3,600,600);
    canv3->SetLogx();
    canv3->SetLeftMargin(0.15);
    TH1F* hRatioMuTheory = (TH1F*)hMeasured1D[config.MUON]->Clone("hRatioMuTheory");
    TH1F* hRatioEleTheory = (TH1F*)hMeasured1D[config.ELECTRON]->Clone("hRatioEleTheory");
    hRatioEleTheory->SetTitle("");
//    hRatioEleTheory->SetTitle(txt_CMS_Preliminary+TString(" d#sigma/dP_{T}^{#gamma}, meas./MC-based, ")+config.StrVgType(vgamma));
//    hRatioMuTheory->GetYaxis()->SetRangeUser(0.60,1.40);
    hRatioMuTheory->Divide(hTheory1D[config.MUON]);
    hRatioMuTheory->SetLineWidth(2);
//    hRatioMuTheory->SetLineColor(4);   
//    hRatioMuTheory->SetMarkerColor(4); 
    hRatioEleTheory->Divide(hTheory1D[config.ELECTRON]);
    hRatioEleTheory->SetLineWidth(3);
//    hRatioEleTheory->SetLineColor(809); 
//    hRatioEleTheory->SetMarkerColor(809); 
    TLegend* leg3 = new TLegend(0.50,0.78,0.70,0.88);
    leg3->SetLineWidth(0);
    leg3->SetLineColor(0);
    leg3->AddEntry(hRatioMuTheory,TString("Data ")+StrProcess(vgamma,config.MUON),"l");
    leg3->AddEntry(hRatioEleTheory,TString("Data ")+StrProcess(vgamma,config.ELECTRON),"l");  
    leg3->SetFillColor(0);
    hRatioMuTheory->GetXaxis()->SetRangeUser(15,500);
    hRatioMuTheory->GetYaxis()->SetRangeUser(0.3,2.7);
    hRatioEleTheory->GetXaxis()->SetTitle("p_{T}^{#gamma}, GeV/c");
    hRatioEleTheory->GetXaxis()->SetRangeUser(15,500);
    hRatioEleTheory->GetYaxis()->SetRangeUser(0.3,2.7);
    hRatioEleTheory->GetYaxis()->SetTitle("(d#sigma_{meas.}/dP_{T}^{#gamma}):(d#sigma_{MC-based}/dP_{T}^{#gamma})");
    hRatioEleTheory->GetYaxis()->SetTitleOffset(1.6);
    hRatioEleTheory->Draw("EP");
    line1->Draw("same");
    leg3->Draw("same");
    hRatioMuTheory->Draw("EP same");
    hRatioEleTheory->SetLineStyle(7);
    hRatioEleTheory->Draw("EP same");
    strHeader = txt_CMS_Preliminary;
    TLatex* text3 = new TLatex(0.15,0.93,strHeader);
    text3->SetNDC();
    text3->SetTextSize(0.04);
    text3->Draw("same");
    canv1->SaveAs(TString("compareCSratio")+config.StrVgType(vgamma)+TString(".png"));
    canv1->SaveAs(TString("compareCSratio")+config.StrVgType(vgamma)+TString(".pdf"));
    canv3->SaveAs(TString("compareCSratioTheory")+config.StrVgType(vgamma)+TString(".png"));
    canv3->SaveAs(TString("compareCSratioTheory")+config.StrVgType(vgamma)+TString(".pdf"));

  std::cout<<std::endl;
  std::cout<<"Total Cross section: "<<std::endl;
  std::cout<<"theory mu: "<<thTotalCSval[0]<<", MC-based ele: "<<thTotalCSval[1]<<", meas mu: "<<hMeasuredTotal[0]->GetBinContent(1)<<"+-"<<hMeasuredTotal[0]->GetBinError(1)<<", meas ele: "<<hMeasuredTotal[1]->GetBinContent(1)<<"+-"<<hMeasuredTotal[1]->GetBinError(1)<<std::endl;
  if (vgamma==config.Z_GAMMA) std::cout<<"Otto mu: 2066+-"<<0.055*2066<<", Otto ele: 2087+-"<<0.058*2087<<std::endl;
  std::cout<<std::endl;

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
  const int nBins=13;
  float lims[nBins+1]=      { 10, 15,  20,  25,  30,    35,    45,   55,   65,   75,   85,  95,  120,  500};
  float csMuoVal[nBins]=     { 0,  912, 491, 227, 128.7, 124.5, 69.9, 36.3, 17.6, 17.1, 9.2, 16.6, 15.1};
  float csMuoErrPercent[nBins]={0,  5.3, 5.6, 6.5, 7.4,   7,     8.6,  11.8, 16,  15.6, 19.8,13.2, 10.8};
  float csEleVal[nBins]=     { 0, 902, 485, 258, 144.2, 115.4, 77.1, 28.5, 22,   10.5, 11.1,15.8, 17.6};
  float csEleErrPercent[nBins]={0, 5.5, 6,   7,    8.2,   8.1,   9.5, 14.5, 16.1, 22.5, 22.3,15.9, 12.3};
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

  //Print cross section



}// end of SetOttosHists


void PrintLatexCS(TH1F* hTheory, TH1F* hMuon_Stat, TH1F* hElectron_Stat,  TH1F* hMuon_Syst, TH1F* hElectron_Syst)
{
  std::cout<<"==============================="<<std::endl;
  std::cout<<"||||========== Print Latex"<<std::endl;

  std::cout<<"||||========== Table with theory vs muon vs electron"<<std::endl;

    std::cout<<"\\begin{table}[h]"<<std::endl;
    std::cout<<"  \\scriptsize"<<std::endl;
    std::cout<<"  \\begin{center}"<<std::endl;
    std::cout<<"  \\caption{Cross section and errors}"<<std::endl;
                                  // bin | val | stat err | syst Ich vs sihih
    std::cout<<"  \\begin{tabular}{|c|c|c|c|}"<<std::endl;
    std::cout<<"                     & \multicolumn{3}{|c|}{$d\\sigma/dP_{T}^{\\gamma}$, fb/GeV} \\\\ "<<std::endl;
    std::cout<<"     $P_T^{\\gamma}$, & NLO theory                          &  \multicolumn{2}{|c|}   measured      \\\\"<<std::endl;
    std::cout<<"    GeV              &  $W\\gamma\\rightarrow l\\nu\\gamma$ & $W\\gamma\\rightarrow \\mu\\nu\\gamma$  & $W\\gamma\\rightarrow e\\nu\\gamma$    \\\\ \\hline"<<std::endl;

    //loop over pt bins
    for (int ib=1; ib<=hTheory->GetNbinsX(); ib++){

      if (ib==1) std::cout<<"%";

      std::cout<<"    ";
      std::cout<<(int)hTheory->GetBinLowEdge(ib)<<"-"<<(int)hTheory->GetBinLowEdge(ib)+(int)hTheory->GetBinWidth(ib)<<" & ";

      float cont = hTheory->GetBinContent(ib);
      float pres=0;
      if (cont<10) pres=1; //0.1
      if (cont<1)  pres=2;
      if (cont<0.1) pres=3;

      std::cout<<hTheory->GetBinContent(ib)<<" & ";     
      std::cout<<hMuon_Stat->GetBinContent(ib)<<" $\\pm$ "<<hMuon_Stat->GetBinError(ib)<<" $\\pm$ "<<hMuon_Syst->GetBinError(ib)<<" & ";
      std::cout<<hElectron_Stat->GetBinContent(ib)<<" $\\pm$ "<<hElectron_Stat->GetBinError(ib)<<" $\\pm$ "<<hElectron_Syst->GetBinError(ib)<<"  ";

      //     std::cout<<std::setprecision(pres)<<_yCSarray[ERR_STAT].crossSection1D->GetBinError(ib)<<" \\pm ";
      //     std::cout<<std::setprecision(pres)<<_yCSarray[ERR_SYST_SUM].crossSection1D->GetBinError(ib)<<"$";

      std::cout<<" \\\\ \\hline"<<std::endl;
    }//end of loop over ib

    std::cout<<"  \\end{tabular}"<<std::endl;
    std::cout<<"  \\label{tab:sc_mc_vs_meas_";
    //    std::cout<<_config.StrChannel(_channel)<<"_"<<_config.StrVgType(_vgamma);
    std::cout<<"}"<<std::endl;
    std::cout<<"  \\end{center}"<<std::endl;
    std::cout<<"\\end{table}"<<std::endl;

  std::cout<<"|||| end of Print Latex"<<std::endl;
  std::cout<<"==============================="<<std::endl;

}//end of PrintLatexCS()

