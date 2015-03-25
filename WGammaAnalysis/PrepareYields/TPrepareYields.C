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

  for (int ieta=_BARREL; ieta<=_COMMON; ieta++){
    //Set 1D yields
    source.hist[ieta] = new TH1F(source.strYieldsName1D[ieta],source.strYieldsName1D[ieta],_pyPars.nKinBins,_pyPars.kinBinLims);
    source.hist[ieta]->Sumw2();
    source.tr->Draw(_pyPars.varKin+TString(">>")+source.strYieldsName1D[ieta],(CutEta(ieta) && _pyPars.cutAdd)*cutW,"goff");

    TString blindName=source.strYieldsName1D[ieta]+TString("_blind");
    source.histBlind[ieta] = new TH1F(blindName,blindName,_pyPars.nKinBins,_pyPars.kinBinLims);
    source.histBlind[ieta]->Sumw2();
    source.tr->Draw(_pyPars.varKin+TString(">>")+blindName,(CutEta(ieta) && _pyPars.cutAdd)*cutWblind,"goff");

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
      source.tr->Draw(_pyPars.varKinGen+TString(">>")+_pyPars.strYieldsName1D_SignalMCGenBins[ieta],(CutEta(ieta) && _pyPars.cutAdd)*cutW,"goff");
    }

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
      PrintYieldsOne(_sources[is].name+TString(": "), _sources[is].yieldTotVal[ieta], _sources[is].yieldTotErr[ieta], _sources[is].hist[ieta]);
      PrintYieldsOne(_sources[is].name+TString(": "), _sources[is].yieldTotVal[ieta], _sources[is].yieldTotErr[ieta], _sources[is].histBlind[ieta]);
    }
  }//end of loop over is

}


void TPrepareYields::CompareTotalDATAvsMC(int ieta)
{
  _pyPars.fOut->cd();
  TString canvName="TotalDATAvsMC";
  canvName+=StrLabelEta(ieta);
  _canvTotalDATAvsMC[ieta]= new TCanvas(canvName,canvName,800,800);
  TLegend* legend = new TLegend(0.7,0.7,0.95,0.95);
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
  if (_pyPars.doLogX) pad1->SetLogx();
  if (_pyPars.doLogY) pad1->SetLogy();
  pad1->cd();

  float max = hist2->GetMaximum();
  float min;
  for (int ih=0; ih<nHists1; ih++){
    if (hist1[ih]->GetMaximum()>max)
      max=hist1[ih]->GetMaximum();
  }

  if (_pyPars.doLogY) min=1;
  else min=0;
//  float min = 0.3*hist1->GetMinimum();
//  if (min>0.3*hist2->GetMinimum())
//    min=0.3*hist2->GetMinimum();
  if (!_pyPars.doLogX) min=0;
  max=1.2*max;

  if (isStack) hist2->SetLineColor(2);
  if (isStack) hist2->SetFillStyle(0);
  hist1[0]->GetYaxis()->SetRangeUser(min,max);
  hist1[0]->SetStats(0);
  if (_pyPars.doLogY) {hist1[0]->GetYaxis()->SetMoreLogLabels(); hist1[0]->GetYaxis()->SetNoExponent();}
  hist1[0]->Draw("P");
  if (isStack) stack->Draw("HIST same"); 
  else hist2->Draw("HIST same");
 
  for (int ih=0; ih<nHists1; ih++){
    hist1[ih]->SetLineWidth(2);
    hist1[ih]->Draw("EP same");
  }
  legend->Draw("same");

  plotTitle=canv->GetTitle();
  plotTitle+=TString(" ")+_pyPars.strPlotsBaseName;
  plotTitle.ReplaceAll("c_","");
  plotTitle.ReplaceAll("TEMPL_","");
  plotTitle.ReplaceAll("UNblind_","");
  plotTitle.ReplaceAll("blindPRESCALE_","");
  plotTitle.ReplaceAll("blindCOMBINED_","");
  plotTitle.ReplaceAll("_"," ");
  plotTitle.ReplaceAll("  "," ");
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

  std::cout<<"ratio min = "<<min<<", ratio max = "<<max<<std::endl;
  hRatio[0]->GetYaxis()->SetRangeUser(min,max);
  hRatio[0]->GetYaxis()->SetLabelSize(0.1);
  hRatio[0]->GetXaxis()->SetLabelSize(0.1);
  hRatio[0]->GetXaxis()->SetTitleOffset(1.0);
  hRatio[0]->GetXaxis()->SetTitleSize(0.12);
  hRatio[0]->GetXaxis()->SetMoreLogLabels();
  hRatio[0]->GetXaxis()->SetNoExponent();

  hRatio[0]->Draw();
  hRatio[0]->SetTitle(TString("; ")+_pyPars.varKinLabel+TString(" ;"));

  for (int ih=0; ih<nHists1; ih++){
    hRatio[ih]->Draw("EP same");
  }

  int nBins = hRatio[0]->GetNbinsX();
  TLine* line = new TLine(hRatio[0]->GetBinLowEdge(1),1,hRatio[0]->GetBinLowEdge(nBins)+hRatio[0]->GetBinWidth(nBins),1);
  line->SetLineWidth(2);
  line->SetLineStyle(9);
  line->Draw("same");

  TString nameForSave=_pyPars.strPlotsDir;//+_pyPars.strPlotsBaseName;
//  std::cout<<"_pyPars.strPlotsDir="<<_pyPars.strPlotsDir<<std::endl;
//  std::cout<<"_pyPars.strPlotsBaseName="<<_pyPars.strPlotsBaseName<<std::endl;
  nameForSave+="c_";
  nameForSave+=canv->GetTitle();
  nameForSave+="_";
  nameForSave+=_pyPars.varKin;
  nameForSave.ReplaceAll(" ","");
  if (_pyPars.isMCclosure) nameForSave+="_MCclosure";
  std::cout<<"nameForSave="<<nameForSave<<std::endl;
  nameForSave+=".png";
  canv->SaveAs(nameForSave);
  nameForSave.ReplaceAll(".png",".pdf");
  canv->SaveAs(nameForSave);
  nameForSave.ReplaceAll(".pdf",".root");
  canv->SaveAs(nameForSave);
}// end of CompareStackVsHist


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
  std::cout<<strYieldType<<": "<<totVal<<"+-"<<totErr<<"; ";
  for (int ib=1; ib<=hist->GetNbinsX(); ib++){
    std::cout<<hist->GetBinContent(ib)<<"+-"<<hist->GetBinError(ib)<<", ";
  }
  std::cout<<hist->GetSumOfWeights()<<std::endl;
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
