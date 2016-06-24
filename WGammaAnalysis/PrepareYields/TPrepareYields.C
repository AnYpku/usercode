#include "TPrepareYields.h"
  //this class

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>

#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TCut.h"
#include "TCanvas.h"
#include "THStack.h"
#include "TVectorD.h"
#include "TLine.h"
#include "TLatex.h"
#include "TBox.h"

TPrepareYields::TPrepareYields()
{
}

TPrepareYields::~TPrepareYields()
{
}

void TPrepareYields::SetPars(PrepareYieldsPars pars)
{
  _pyPars=pars;

  //set _pyPars.cutKin
  TString strCutKin=_pyPars.varKin;
  strCutKin+=">=";
  strCutKin+=_pyPars.kinBinLims[0];
  strCutKin+=" && ";
  strCutKin+=_pyPars.varKin;
  strCutKin+="<=";
  strCutKin+=_pyPars.kinBinLims[_pyPars.nKinBins];
  _pyPars.cutKin=strCutKin;
  std::cout<<"_cutKin="<<_pyPars.cutKin.GetTitle()<<std::endl;

  _pyPars.fOut=new TFile(_pyPars.strFileOut,"recreate");
}// end of SetPars

bool TPrepareYields::SetOneYieldSource(int sourceType, TString name, TString label, int color, TString fileName, TString treeName)
{
  TString strYields1D[3];
  TString strYieldsTot[3];
  for (int ieta=_BARREL; ieta<=_COMMON; ieta++){
    strYields1D[ieta]="hist"+name+"_1D_"+StrLabelEta(ieta);
    strYieldsTot[ieta]="hist"+name+"_Tot_"+StrLabelEta(ieta);
  }
  bool isOk = SetOneYieldSource(sourceType, name, label, color, fileName, treeName, strYields1D, strYieldsTot);
  return isOk;
}// end of SetOneYieldSource

bool TPrepareYields::SetOneYieldSource(int sourceType, TString name, TString label, int color, TString fileName, TString treeName, TString strYieldsName1D[3], TString strYieldsNameTot[3])
{

  YieldsSource source;

  // Set directly passed parameters 
  source.sourceType=sourceType;
  source.name=name;
  source.label=label;
  source.color=color;
  for (int ieta=_BARREL; ieta<=_COMMON; ieta++){
    source.strYieldsName1D[ieta]=strYieldsName1D[ieta];
    source.strYieldsNameTot[ieta]=strYieldsNameTot[ieta];
  }

  // Set tree 
  TFile* f = new TFile(fileName);
  if (!f->IsOpen()){
    std::cout<<"ERROR in TPrepareYields::SetOneYieldSource: file "<<fileName<<" can't be open"<<std::endl;
    return 0;
  }
  source.tr = (TTree*)f->Get(treeName);

  
  TString strWeight=_pyPars.varWeight;
  TCut cutW(strWeight);

  TString strWeightBlind=strWeight;
  if (source.sourceType!=DATA){
    strWeightBlind+="*";
    strWeightBlind+=_pyPars.blindFraction;//0.05 if BLIND_PRESCALE, 1 if UNBLIND or BLIND_COMBINE
  }
  std::cout<<"strWeightBlind="<<strWeightBlind<<", blind frac = "<<_pyPars.blindFraction<<std::endl;
  TCut cutWblind(strWeightBlind);

  _pyPars.fOut->cd();

  TCut cutEtog="1";
  if (source.name=="DYjets_to_ll_etog") cutEtog="pho_genEle_dRMin<0.4";
  if (source.name=="DYjets_to_ll_jtog") cutEtog="!(pho_genEle_dRMin<0.4)";

  for (int ieta=_BARREL; ieta<=_COMMON; ieta++){
    //Set 1D yields
    source.hist[ieta] = new TH1F(source.strYieldsName1D[ieta],source.strYieldsName1D[ieta],_pyPars.nKinBins,_pyPars.kinBinLims);
    source.hist[ieta]->Sumw2();
    source.tr->Draw(_pyPars.varKin+TString(">>")+source.strYieldsName1D[ieta],(CutEta(ieta) && _pyPars.cutAdd && cutEtog)*cutW,"goff");

    TString blindName=source.strYieldsName1D[ieta]+TString("_blind");
    source.histBlind[ieta] = new TH1F(blindName,blindName,_pyPars.nKinBins,_pyPars.kinBinLims);
    source.histBlind[ieta]->Sumw2();
    source.tr->Draw(_pyPars.varKin+TString(">>")+blindName,(CutEta(ieta) && _pyPars.cutAdd && cutEtog)*cutWblind,"goff");

    source.hist[ieta]->SetLineColor(source.color);
    source.hist[ieta]->SetFillColor(source.color);
    if (source.sourceType==DATA) source.hist[ieta]->SetFillColor(0);
    source.hist[ieta]->SetMarkerColor(source.color);
    source.hist[ieta]->SetStats(0);

    source.histBlind[ieta]->SetLineColor(source.color);
    source.histBlind[ieta]->SetFillColor(source.color);
    if (source.sourceType==DATA) source.hist[ieta]->SetFillColor(0);
    source.histBlind[ieta]->SetMarkerColor(source.color);
    source.histBlind[ieta]->SetStats(0);

    if (source.sourceType==SIGMC){
      _sigMCGenYields[ieta]=new TH1F(_pyPars.strYieldsName1D_SignalMCGenBins[ieta],_pyPars.strYieldsName1D_SignalMCGenBins[ieta],_pyPars.nKinBins,_pyPars.kinBinLims);
      _sigMCGenYields[ieta]->Sumw2();
      source.tr->Draw(_pyPars.varKinGen+TString(">>")+_pyPars.strYieldsName1D_SignalMCGenBins[ieta],(CutEta(ieta) && _pyPars.cutAdd && cutEtog)*cutW,"goff");
    }// end of if (source.sourceType==SIGMC)

    if (source.sourceType==BKGMC_ETOG){
      float scale=1;
      if (ieta==0) scale=_pyPars.etogScaleB;
      if (ieta==1) scale=_pyPars.etogScaleE;
      source.hist[ieta]->Scale(scale);
      source.histBlind[ieta]->Scale(scale);
    }// end of if (source.sourceType==ETOG)

    //Set total yield
    TH1F* floatingHist = new TH1F("floatingHist","hist for temprorary storage of total yields",1,source.tr->GetMinimum(_pyPars.varKin)-1,source.tr->GetMaximum(_pyPars.varKin)+1);//inputFileNumber
    floatingHist->Sumw2();
    source.tr->Draw(_pyPars.varKin+TString(">>floatingHist"),(CutEta(ieta) && _pyPars.cutKin && _pyPars.cutAdd)*cutW,"goff");
    source.yieldTotVal[ieta] = floatingHist->GetBinContent(1);
    source.yieldTotErr[ieta] = floatingHist->GetBinError(1);
    //std::cout<<"val+-err="<<source.yieldTotVal[ieta]<<"+-"<<source.yieldTotErr[ieta]<<std::endl;
    delete floatingHist;
  }

  _sources.push_back(source);

  return 1;
}// end of SetOneYieldSource

void TPrepareYields::PlotPrintSave()
{

  for (int ieta=_BARREL; ieta<=_COMMON; ieta++){
    PrepareMCtruth(ieta, BKGMC_TRUE);
    PrepareMCtruth(ieta, BKGMC_FAKE);
  }


  //Plot
  for (int ieta=_BARREL; ieta<=_COMMON; ieta++){
    CompareTotalDATAvsMC(ieta);
  }

  //Print Yields
  for (int is=0; is<_sources.size(); is++){
    for (int ieta=_BARREL; ieta<=_COMMON; ieta++){
      
      PrintYieldsOne(_sources[is].name+TString(", ")+StrLabelEta(ieta), _sources[is].yieldTotVal[ieta], _sources[is].yieldTotErr[ieta], _sources[is].hist[ieta]);
    }
  }//end of loop over is

}// end of PlotPrintSave()


void TPrepareYields::CompareTotalDATAvsMC(int ieta)
{
  _pyPars.fOut->cd();
  TString canvName="TotalDATAvsMC";
  canvName+=StrLabelEta(ieta);
  _canvTotalDATAvsMC[ieta]= new TCanvas(canvName,canvName,800,650);
  TLegend* legend;
  if (_pyPars.varKin=="Mleplep" || _pyPars.varKin=="lep2PhoDeltaR")
    legend = new TLegend(0.25,0.50,0.50,0.90);
  else legend = new TLegend(0.65,0.50,0.90,0.90);
  legend->SetFillColor(0);
  THStack* mcHists = new THStack("mcHistsTot","DATA vs MC");

  int isData=-1;
  int isSign=-1;
  for (int is=0; is<_sources.size(); is++){
    if (_sources[is].sourceType==DATA) isData=is;
    if (_sources[is].sourceType==SIGMC) isSign=is;
  }

  TH1F* hSum;
  bool hSumStarted=0;
  for (int is=0; is<_sources.size(); is++){
     std::cout<<"CompareTotalDATAvsMC: processing isource "<<_sources[is].name<<std::endl;
    if (is==isData || is==isSign) continue;
    mcHists->Add(_sources[is].histBlind[ieta]);
    if (!hSumStarted){
      hSum=(TH1F*)_sources[is].histBlind[ieta]->Clone("hSum_WholeMC");
      hSumStarted=1;
    }
    else hSum->Add(_sources[is].histBlind[ieta]);
    legend->AddEntry(_sources[is].histBlind[ieta],_sources[is].label);
  }//end of loop over is

  if (isSign!=-1) {
    mcHists->Add(_sources[isSign].histBlind[ieta]);  
    legend->AddEntry(_sources[isSign].histBlind[ieta],_sources[isSign].label);  
    if (!hSumStarted){
      hSum=(TH1F*)_sources[isSign].histBlind[ieta]->Clone("hSum_WholeMC");
      hSumStarted=1;
    }
    else hSum->Add(_sources[isSign].histBlind[ieta]);
  }

  if (isData==-1){
    std::cout<<"ERROR in TPrepareYields::CompareTotalDATAvsMC: no data to compare with"<<std::endl;
    return;
  }

  legend->AddEntry(_sources[isData].hist[ieta],"data");

  _sources[isData].hist[ieta]->SetTitle("");
  CompareStackVsHist("Total DATA vs total MC", _sources[isData].hist[ieta], hSum, legend, _canvTotalDATAvsMC[ieta], 1, mcHists);

  _canvTotalDATAvsMC[ieta]->Write();
}// end of CompareTotalDATAvsMC

void TPrepareYields::CompareStackVsHist(TString plotTitle, TH1F* hist1, TH1F* hist2, TLegend* legend, TCanvas* canv, bool isStack, THStack* stack)
{
  TH1F* hists[1];
  hists[0]=hist1;
  CompareStackVsHist(plotTitle, 1, hists, hist2, legend, canv, isStack, stack);
}

void TPrepareYields::CompareStackVsHist(TString plotTitle, int nHists1, TH1F* hist1[_nHistsMax], TH1F* hist2, TLegend* legend, TCanvas* canv, bool isStack, THStack* stack)
//if isStack then hist2 must be sum of histograms in the stack
{

  std::cout<<std::endl;
  std::cout<<"###########"<<std::endl;
  std::cout<<"CompareStackVsHist; canv = "<<canv->GetTitle()<<std::endl;

  canv->Divide(1,2);
  TPad* pad1 = (TPad*)canv->GetPad(1);
  TPad* pad2 = (TPad*)canv->GetPad(2);
  pad1->SetPad(0,0.3,1.0,1.0);
  pad2->SetPad(0,0,  1.0,0.28);
  pad1->SetLeftMargin(0.10);
  pad1->SetTopMargin(0.08);
  pad1->SetRightMargin(0.07);
  pad1->SetBottomMargin(0.01); // All X axis labels and titles are thus cut off
  pad2->SetLeftMargin(0.10);
  pad2->SetTopMargin(0.01);
  pad2->SetRightMargin(0.07);
  pad2->SetBottomMargin(0.45);
  if (_pyPars.doLogX) pad1->SetLogx();
  if (_pyPars.doLogY) pad1->SetLogy();
  pad1->cd();

  // Divide over bin width
  for (int ih=0; ih<nHists1; ih++){
    hist1[ih]->Print();
    hist1[ih]->SetLineWidth(2);
    hist1[ih]->SetMarkerStyle(24);
    hist1[ih]->SetMarkerColor(hist1[ih]->GetLineColor());
    std::cout<<"(hist1[ih]) h->GetSumOfWeights()="<<hist1[ih]->GetSumOfWeights()<<std::endl;
    DivideOverBinWidth(hist1[ih]);
    std::cout<<"(hist1[ih]) h->GetSumOfWeights()="<<hist1[ih]->GetSumOfWeights()<<std::endl;
  }// end of loop over ih
  if (isStack){ 
    int nHists = stack->GetHists()->GetEntries();
    for (int ih=0; ih<nHists; ih++){
      std::cout<<"(from stack) h->GetSumOfWeights()="<<(TH1F*)stack->GetHists()->At(ih)<<std::endl;
      DivideOverBinWidth((TH1F*)stack->GetHists()->At(ih));
      ((TH1F*)stack->GetHists()->At(ih))->Print();
      std::cout<<"(from stack) h->GetSumOfWeights()="<<(TH1F*)stack->GetHists()->At(ih)<<std::endl;
    }//end of loop over ih
  }// end of isStack
  hist2->SetLineWidth(2);  
  hist2->Print();
  std::cout<<"(hist2) h->GetSumOfWeights()="<<hist2->GetSumOfWeights()<<std::endl;
  DivideOverBinWidth(hist2);
  std::cout<<"(hist2) h->GetSumOfWeights()="<<hist2->GetSumOfWeights()<<std::endl;



  float max = hist2->GetMaximum();
  float min = hist2->GetMinimum();
  for (int ih=0; ih<nHists1; ih++){
    if (hist1[ih]->GetMaximum()>max)
      max=hist1[ih]->GetMaximum();
    if (hist1[ih]->GetMinimum()<min)
      min=hist1[ih]->GetMinimum();
  }
  std::cout<<"min="<<min<<", max="<<max<<std::endl;
  if (_pyPars.doLogY) {min=min/10;}
  else min=min/2;
  if (min<=0){
    if (_pyPars.doLogY) {min=0.001;}
    else min=0;
  }
  max=1.2*max;


  if (isStack) hist2->SetLineColor(2);
  if (isStack) hist2->SetFillStyle(0);
  hist1[0]->GetYaxis()->SetRangeUser(min,max);
  hist1[0]->GetYaxis()->SetTitle("N_{events}/(bin width)");
  hist1[0]->GetYaxis()->SetTitleOffset(1.0);
  hist1[0]->SetStats(0);
  if (_pyPars.doLogY) {hist1[0]->GetYaxis()->SetMoreLogLabels(0); hist1[0]->GetYaxis()->SetNoExponent(); }
  hist1[0]->Draw("P");
  if (isStack){ 
    stack->Draw("HIST same"); 
  }// end of isStack
  else{hist2->Draw("EP same");}
 
  for (int ih=0; ih<nHists1; ih++){
    hist1[ih]->Draw("EP same");
  }
  hist1[0]->GetYaxis()->Draw("same");
  hist1[0]->GetXaxis()->Draw("same");
  float lineX=hist1[0]->GetBinLowEdge(1)+hist1[0]->GetBinWidth(1);
  if (_pyPars.varKin=="phoEt"){
    TLine* l1 = new TLine(lineX, min, lineX, max);
    l1->SetLineWidth(2);
    l1->Draw("same");
//    TBox* boxBlind1 = new TBox(45,min,500,max);
//    boxBlind1->SetLineWidth(2);
//    boxBlind1->SetLineColor(13);
//    boxBlind1->SetFillColor(13);
//    boxBlind1->SetFillStyle(3004);
//    boxBlind1->Draw("same");
    TText* textUnderflow = new TText(0.15,0.20,"underflow bin");
    textUnderflow->SetNDC();
    textUnderflow->SetTextAngle(90);//vertical
    textUnderflow->Draw("same");
  }
  legend->Draw("same");
  pad1->RedrawAxis();



  plotTitle=canv->GetTitle();
  plotTitle+=TString(" ")+_pyPars.strPlotsBaseName;
  plotTitle=PlotTitleReplaceAll(plotTitle);


  TLatex* latexTitle = new TLatex(0.15,0.95,plotTitle);
  latexTitle->SetNDC();
  latexTitle->Draw("same");

  pad2->cd();
  if (_pyPars.doLogX) pad2->SetLogx();
  TH1F* hRatio[nHists1];
  for (int ih=0; ih<nHists1; ih++){
    TString hRName = "hRatio";
    hRName+=hist1[ih]->GetTitle();
    hRName+=hist2->GetTitle();
    hRatio[ih] = (TH1F*)hist1[ih]->Clone(hRName);
    hRatio[ih]->Divide(hist2);
  }
  //bool isDiv = hRatio->Divide(hist2);
  //if (!isDiv){
  //  std::cout<<"Can't divide histograms"<<std::endl;
  //  return;
 // }
  max=1.1;min=0.9;

  for (int ih=0; ih<nHists1; ih++){  
    for (int ib=1; ib<=hRatio[ih]->GetNbinsX(); ib++){
      if (hRatio[ih]->GetBinContent(ib)+hRatio[ih]->GetBinError(ib)>max) 
        max=hRatio[ih]->GetBinContent(ib)+hRatio[ih]->GetBinError(ib);
      if (hRatio[ih]->GetBinContent(ib)-hRatio[ih]->GetBinError(ib)<min) 
        min=hRatio[ih]->GetBinContent(ib)-hRatio[ih]->GetBinError(ib);
    }// end of loop over ib
  }// end of loop over ih
  if (max>2) max=2;
  min=0.5; max=1.9;

  std::cout<<"ratio min = "<<min<<", ratio max = "<<max<<std::endl;
  hRatio[0]->GetYaxis()->SetRangeUser(min,max);
  hRatio[0]->GetYaxis()->SetLabelSize(0.1);
  hRatio[0]->GetXaxis()->SetLabelSize(0.1);
  hRatio[0]->GetXaxis()->SetTitleOffset(1.0);
  hRatio[0]->GetXaxis()->SetTitleSize(0.12);
  hRatio[0]->GetXaxis()->SetMoreLogLabels();
  hRatio[0]->GetXaxis()->SetNoExponent();

  hRatio[0]->Draw();
  hRatio[0]->SetTitle(TString("; ")+TitleOfXAxis()+TString(" ;"));

  for (int ih=0; ih<nHists1; ih++){
    hRatio[ih]->Draw("EP same");
  }

  int nBins = hRatio[0]->GetNbinsX();
  TLine* line = new TLine(hRatio[0]->GetBinLowEdge(1),1,hRatio[0]->GetBinLowEdge(nBins)+hRatio[0]->GetBinWidth(nBins),1);
  line->SetLineWidth(2);
  line->SetLineStyle(9);
  line->Draw("same");
  if (_pyPars.varKin=="phoEt"){
    TLine* l2 = new TLine(lineX, min, lineX, max);
    l2->SetLineWidth(2);
    l2->Draw("same");
//    TBox* boxBlind2 = new TBox(45,min,500,max);
//    boxBlind2->SetLineWidth(2);
//    boxBlind2->SetLineColor(13);
//    boxBlind2->SetFillColor(13);
//    boxBlind2->SetFillStyle(3004);
//    boxBlind2->Draw("same");
  }

  TString nameForSave=_pyPars.strPlotsDir;//+_pyPars.strPlotsBaseName;
  nameForSave+="c_";
  nameForSave+=canv->GetTitle();
  nameForSave+="_";
  nameForSave+=_pyPars.varKin;
  nameForSave+=_pyPars.strSelStage;

  if (_pyPars.etogScaleB<0.999 || _pyPars.etogScaleB>1.001 || 
      _pyPars.etogScaleE<0.999 || _pyPars.etogScaleE>1.001)
    nameForSave+="_etogScale";

  nameForSave.ReplaceAll(" ","");
  if (_pyPars.isMCclosure) nameForSave+="_MCclosure";
  std::cout<<"nameForSave="<<nameForSave<<std::endl;
  std::cout<<"pars.strSelStage="<<_pyPars.strSelStage<<std::endl;
  nameForSave+=".png";
  canv->SaveAs(nameForSave);
  nameForSave.ReplaceAll(".png",".pdf");
  canv->SaveAs(nameForSave);
  nameForSave.ReplaceAll(".pdf",".root");
  canv->SaveAs(nameForSave);

  // Multiply by bin width
  for (int ih=0; ih<nHists1; ih++){
    std::cout<<"(hist1[ih]) h->GetSumOfWeights()="<<hist1[ih]->GetSumOfWeights()<<std::endl;
    MultiplyByBinWidth(hist1[ih]);
    hist1[ih]->Print();
    std::cout<<"(hist1[ih]) h->GetSumOfWeights()="<<hist1[ih]->GetSumOfWeights()<<std::endl;
  }// end of loop over ih
  if (isStack){ 
    int nHists = stack->GetHists()->GetEntries();
    for (int ih=0; ih<nHists; ih++){
      std::cout<<"(from stack) h->GetSumOfWeights()="<<(TH1F*)stack->GetHists()->At(ih)<<std::endl;
      MultiplyByBinWidth((TH1F*)stack->GetHists()->At(ih));
      ((TH1F*)stack->GetHists()->At(ih))->Print();
      std::cout<<"(from stack) h->GetSumOfWeights()="<<(TH1F*)stack->GetHists()->At(ih)<<std::endl;
    }//end of loop over ih
  }// end of isStack
  std::cout<<"(hist2) h->GetSumOfWeights()="<<hist2->GetSumOfWeights()<<std::endl;
  MultiplyByBinWidth(hist2);
  hist2->Print();
  std::cout<<"(hist2) h->GetSumOfWeights()="<<hist2->GetSumOfWeights()<<std::endl;

  std::cout<<"###########"<<std::endl;
  std::cout<<std::endl;


}// end of CompareStackVsHist

TString TPrepareYields::PlotTitleReplaceAll(TString strTitle){

  TString plotTitle = strTitle;

  plotTitle.ReplaceAll("c_","");
  plotTitle.ReplaceAll("TEMPL_","");
  plotTitle.ReplaceAll("UNblind_","");
  plotTitle.ReplaceAll("blindPRESCALE_","");
  plotTitle.ReplaceAll("blindCOMBINED_","");
  plotTitle.ReplaceAll("_"," ");
  plotTitle.ReplaceAll("  "," ");
  plotTitle.ReplaceAll("TrueDDvsMC","Real #gamma DD-fits vs MC.");
  plotTitle.ReplaceAll("FakeDDvsMC","Fake #gamma DD-fits vs MC.");
  plotTitle.ReplaceAll("TotalDATAvsMC","DATA vs MC.");
  plotTitle.ReplaceAll("BkgSubtrDATAvsSIGMC","(DATA - BKG) vs SIGMC.");
  plotTitle.ReplaceAll("DATAvsBkgPlusSigMC","DATA vs (BKG + SIGMC).");
  plotTitle.ReplaceAll("DATAvsDDsum","DATA vs DD fits ");
  /*
  plotTitle.ReplaceAll("CHISO Endcap MUON WGamma","Endcap");
  plotTitle.ReplaceAll("CHISO Barrel MUON WGamma","Barrel");
  plotTitle.ReplaceAll("CHISO Endcap MUON ZGamma","Endcap");
  plotTitle.ReplaceAll("CHISO Barrel MUON ZGamma","Barrel");
  plotTitle.ReplaceAll("CHISO Endcap ELECTRON WGamma","Endcap");
  plotTitle.ReplaceAll("CHISO Barrel ELECTRON WGamma","Barrel");
  plotTitle.ReplaceAll("CHISO Endcap ELECTRON ZGamma","Endcap");
  plotTitle.ReplaceAll("CHISO Barrel ELECTRON ZGamma","Barrel");
  plotTitle.ReplaceAll("CHISO EtaCommon MUON WGamma","EB+EE");
  plotTitle.ReplaceAll("CHISO EtaCommon MUON ZGamma","EB+EE");
  plotTitle.ReplaceAll("CHISO EtaCommon ELECTRON WGamma","EB+EE");
  plotTitle.ReplaceAll("CHISO EtaCommon ELECTRON ZGamma","EB+EE");

  plotTitle.ReplaceAll("SIHIH Endcap MUON WGamma","Endcap");
  plotTitle.ReplaceAll("SIHIH Barrel MUON WGamma","Barrel");
  plotTitle.ReplaceAll("SIHIH Endcap MUON ZGamma","Endcap");
  plotTitle.ReplaceAll("SIHIH Barrel MUON ZGamma","Barrel");
  plotTitle.ReplaceAll("SIHIH Endcap ELECTRON WGamma","Endcap");
  plotTitle.ReplaceAll("SIHIH Barrel ELECTRON WGamma","Barrel");
  plotTitle.ReplaceAll("SIHIH Endcap ELECTRON ZGamma","Endcap");
  plotTitle.ReplaceAll("SIHIH Barrel ELECTRON ZGamma","Barrel");
  plotTitle.ReplaceAll("SIHIH EtaCommon MUON WGamma","EB+EE");
  plotTitle.ReplaceAll("SIHIH EtaCommon MUON ZGamma","EB+EE");
  plotTitle.ReplaceAll("SIHIH EtaCommon ELECTRON WGamma","EB+EE");
  plotTitle.ReplaceAll("SIHIH EtaCommon ELECTRON ZGamma","EB+EE");
  */
  plotTitle.ReplaceAll("Endcap MUON WGamma","Endcap");
  plotTitle.ReplaceAll("Barrel MUON WGamma","Barrel");
  plotTitle.ReplaceAll("Endcap MUON ZGamma","Endcap");
  plotTitle.ReplaceAll("Barrel MUON ZGamma","Barrel");
  plotTitle.ReplaceAll("Endcap ELECTRON WGamma","Endcap");
  plotTitle.ReplaceAll("Barrel ELECTRON WGamma","Barrel");
  plotTitle.ReplaceAll("Endcap ELECTRON ZGamma","Endcap");
  plotTitle.ReplaceAll("Barrel ELECTRON ZGamma","Barrel");
  plotTitle.ReplaceAll("EtaCommon MUON WGamma","EB+EE");
  plotTitle.ReplaceAll("EtaCommon MUON ZGamma","EB+EE");
  plotTitle.ReplaceAll("EtaCommon ELECTRON WGamma","EB+EE");
  plotTitle.ReplaceAll("EtaCommon ELECTRON ZGamma","EB+EE");

  plotTitle.ReplaceAll("EtaCommon","EB+EE");
  plotTitle.ReplaceAll("ChannelsMERGED","#mu+e");
  plotTitle.ReplaceAll("CHISO","");
  plotTitle.ReplaceAll("SIHIH","");

  plotTitle.ReplaceAll("WGamma","W#gamma");
  plotTitle.ReplaceAll("ZGamma","Z#gamma");

  return plotTitle;

}// end of PlotTitleReplaceAll

TString TPrepareYields::TitleOfXAxis(){

  if (_pyPars.varKin=="phoEt") return "P_{T}^{#gamma}, GeV";
  if (_pyPars.varKin=="WMt") return "M_{T}^{W}, GeV";
  if (_pyPars.varKin=="Mleplep") return "M_{ll}, GeV";
  if (_pyPars.varKin=="Mpholeplep") return "M_{ll#gamma}, GeV";
  if (_pyPars.varKin=="Mpholep1") return "M_{l#gamma}, GeV";
  if (_pyPars.varKin=="lep1PhoDeltaR") return "#DeltaR(l,#gamma)";
  if (_pyPars.varKin=="lep2PhoDeltaR") return "#DeltaR(l_{2},#gamma)";
  if (_pyPars.varKin=="phoPFChIsoCorr") return "I_{chHad}^{#gamma}, GeV";
  if (_pyPars.varKin=="phoSCRChIsoCorr") return "I_{chHad-SCR}^{#gamma}, GeV";
  if (_pyPars.varKin=="phoRandConeChIsoCorr") return "I_{chHad-RandCone}^{#gamma}, GeV";
  if (_pyPars.varKin=="phoSCEta") return "#eta^{#gamma}";

  return _pyPars.varKin;

}// end of PlotTitleReplaceAll


TCut TPrepareYields::CutEta(int ieta)
{
  if (ieta==_BARREL) return _pyPars.cutBarrel;
  if (ieta==_ENDCAP) return _pyPars.cutEndcap;
  return "1";
}

TString TPrepareYields::StrLabelEta(int ieta)
{
  if (ieta==_BARREL) return "_Barrel_";
  if (ieta==_ENDCAP) return "_Endcap_";
  return "_EtaCommon_";
}

void TPrepareYields::PrintYieldsOne(TString strYieldType, float totVal, float totErr, TH1F* hist)
{
  std::cout<<std::setprecision(0);
  std::cout<<std::setw(5);
  std::cout<<std::fixed;
  std::cout<<strYieldType<<": Tot: "<<totVal<<"+-"<<totErr<<"; (";
  for (int ib=1; ib<=hist->GetNbinsX(); ib++){
    std::cout<<hist->GetBinContent(ib)<<"+-"<<hist->GetBinError(ib)<<", ";
  }
  std::cout<<" sum="<<hist->GetSumOfWeights()<<" ) "<<std::endl;
}

void TPrepareYields::PrepareMCtruth(int ieta, int bkgType)
{

  _pyPars.fOut->cd(); 

  TH1F* hSum;
  bool hSumStarted=0;
  TString hSumName="hSum_MCtruth_";
  hSumName+=StrLabelEta(ieta);
  if (bkgType==BKGMC_TRUE) hSumName+="TrueGamma";
  if (bkgType==BKGMC_FAKE) hSumName+="FakeGamma";

  for (int is=0; is<_sources.size(); is++){
    if (bkgType==BKGMC_TRUE)
      if (_sources[is].sourceType!=BKGMC_TRUE 
            && _sources[is].sourceType!=SIGMC) continue;
    if (bkgType==BKGMC_FAKE)
      if (_sources[is].sourceType!=BKGMC_FAKE) continue;
    if (!hSumStarted){
      hSum=(TH1F*)_sources[is].hist[ieta]->Clone(hSumName);
      hSumStarted=1;
    }
    else hSum->Add(_sources[is].hist[ieta]);
  }//end of loop over is

  hSum->SetTitle(hSumName);
  hSum->Write(hSumName);

}// end of PrepareTrueGammaMCtruth

void TPrepareYields::DivideOverBinWidth(TH1F* h){

    std::cout<<h->GetName()<<" ( "<<h->GetTitle()<<" ) will be divided over bin width"<<std::endl;
    std::cout<<"h->GetSumOfWeights()="<<h->GetSumOfWeights()<<std::endl;
    for (int ib=1; ib<=h->GetNbinsX(); ib++){
      float cont,err,width;
      cont = h->GetBinContent(ib);
      err = h->GetBinError(ib);
      width = h->GetBinWidth(ib);
      h->SetBinContent(ib,cont/width);
      h->SetBinError(ib,err/width);
    }// end of loop over ib
    std::cout<<"h->GetSumOfWeights()="<<h->GetSumOfWeights()<<std::endl;

}// end of DivideOverBinWidth

void TPrepareYields::MultiplyByBinWidth(TH1F* h){

    std::cout<<h->GetName()<<" ( "<<h->GetTitle()<<" ) will be multiplied by bin width"<<std::endl;
    std::cout<<"h->GetSumOfWeights()="<<h->GetSumOfWeights()<<std::endl;
    for (int ib=1; ib<=h->GetNbinsX(); ib++){
      float cont,err,width;
      cont = h->GetBinContent(ib);
      err = h->GetBinError(ib);
      width = h->GetBinWidth(ib);
      h->SetBinContent(ib,cont*width);
      h->SetBinError(ib,err*width);
    }// end of loop over ib
    std::cout<<"h->GetSumOfWeights()="<<h->GetSumOfWeights()<<std::endl;

}// end of MultiplyByBinWidth
