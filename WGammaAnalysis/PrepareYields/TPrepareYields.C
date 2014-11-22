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
  _pars=pars;

  //set _pars.cutKin
  TString strCutKin=_pars.varKin;
  strCutKin+=">=";
  strCutKin+=_pars.kinBinLims[0];
  strCutKin+=" && ";
  strCutKin+=_pars.varKin;
  strCutKin+="<=";
  strCutKin+=_pars.kinBinLims[_pars.nKinBins];
  _pars.cutKin=strCutKin;
  std::cout<<"_cutKin="<<_pars.cutKin.GetTitle()<<std::endl;

  //set _pars.cutWeight
  TString strWeight=_pars.varWeight;
  strWeight+="*";
  strWeight+=_pars.blindFraction;//0.05 if blinded, 1 if unblinded
  _pars.cutWeight=strWeight;

  _pars.fOut=new TFile(_pars.strFileOut,"recreate");
}

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
}

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

  TCut cutW="1";
  if (source.sourceType!=DATA)
    cutW=_pars.cutWeight;

  _pars.fOut->cd();

  for (int ieta=_BARREL; ieta<=_COMMON; ieta++){
    //Set 1D yields
    source.hist[ieta] = new TH1F(source.strYieldsName1D[ieta],source.strYieldsName1D[ieta],_pars.nKinBins,_pars.kinBinLims);
    source.hist[ieta]->Sumw2();
    source.tr->Draw(_pars.varKin+TString(">>")+source.strYieldsName1D[ieta],(CutEta(ieta))*cutW,"goff");
    source.hist[ieta]->SetLineColor(source.color);
    source.hist[ieta]->SetFillColor(source.color);
    if (source.sourceType==DATA) source.hist[ieta]->SetFillColor(0);
    source.hist[ieta]->SetMarkerColor(source.color);
    source.hist[ieta]->SetStats(0);

    //Set total yield
    TH1F* floatingHist = new TH1F("floatingHist","hist for temprorary storage of total yields",1,source.tr->GetMinimum(_pars.varKin)-1,source.tr->GetMaximum(_pars.varKin)+1);//inputFileNumber
    floatingHist->Sumw2();
    source.tr->Draw(_pars.varKin+TString(">>floatingHist"),(CutEta(ieta) && _pars.cutKin)*cutW,"goff");
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

  //Plot
  for (int ieta=_BARREL; ieta<=_COMMON; ieta++){
    CompareTotalDATAvsMC(ieta);
  }

  //Print Yields
  for (int is=0; is<_sources.size(); is++){
    for (int ieta=_BARREL; ieta<=_COMMON; ieta++){
      PrintYieldsOne(_sources[is].name+TString(": "), _sources[is].yieldTotVal[ieta], _sources[is].yieldTotErr[ieta], _sources[is].hist[ieta]);
    }
  }//end of loop over is

}


void TPrepareYields::CompareTotalDATAvsMC(int ieta)
{
  _pars.fOut->cd();
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
    mcHists->Add(_sources[is].hist[ieta]);
    if (!hSumStarted){
      hSum=(TH1F*)_sources[is].hist[ieta]->Clone("hSum_WholeMC");
      hSumStarted=1;
    }
    else hSum->Add(_sources[is].hist[ieta]);
    legend->AddEntry(_sources[is].hist[ieta],_sources[is].label);
  }//end of loop over is

  if (isSign!=-1) {
    mcHists->Add(_sources[isSign].hist[ieta]);  
    legend->AddEntry(_sources[isSign].hist[ieta],_sources[isSign].label);  
    if (!hSumStarted){
      hSum=(TH1F*)_sources[isSign].hist[ieta]->Clone("hSum_WholeMC");
      hSumStarted=1;
    }
    else hSum->Add(_sources[isSign].hist[ieta]);
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
  if (_pars.doLogX) pad1->SetLogx();
  if (_pars.doLogY) pad1->SetLogy();
  pad1->cd();

  float max = hist1->GetMaximum();
  if (hist2->GetMaximum()>max)
    max=hist2->GetMaximum();
  float min = 0.3*hist1->GetMinimum();
  if (min>0.3*hist2->GetMinimum())
    min=0.3*hist2->GetMinimum();
  if (!_pars.doLogX) min=0;
  max=1.2*max;

  if (isStack) hist2->SetLineColor(2);
  if (isStack) hist2->SetFillStyle(0);
  hist1->GetYaxis()->SetRangeUser(min,max);
  hist1->SetStats(0);
  if (_pars.doLogY) {hist1->GetYaxis()->SetMoreLogLabels(); hist1->GetYaxis()->SetNoExponent();}
  hist1->Draw("P");
  if (isStack) stack->Draw("HIST same"); 
  else hist2->Draw("HIST same");

  hist1->SetLineWidth(2);
  hist1->Draw("EP same");
  legend->Draw("same");

  TLatex* latexTitle = new TLatex(0.15,0.95,plotTitle);
  latexTitle->SetNDC();
  latexTitle->Draw("same");

  pad2->cd();
  if (_pars.doLogX) pad2->SetLogx();
  TString hRName = "hRatio";
  hRName+=hist1->GetTitle();
  hRName+=hist2->GetTitle();
  TH1F* hRatio = (TH1F*)hist1->Clone(hRName);
  hRatio->Divide(hist2);
  //bool isDiv = hRatio->Divide(hist2);
  //if (!isDiv){
  //  std::cout<<"Can't divide histograms"<<std::endl;
  //  return;
 // }
  max=1;min=1;
  for (int ib=1; ib<=hRatio->GetNbinsX(); ib++){
    if (hRatio->GetBinContent(ib)+hRatio->GetBinError(ib)>max) 
      max=hRatio->GetBinContent(ib)+hRatio->GetBinError(ib);
    if (hRatio->GetBinContent(ib)-hRatio->GetBinError(ib)<min) 
      min=hRatio->GetBinContent(ib)-hRatio->GetBinError(ib);
  }

  std::cout<<"ratio min = "<<min<<", ratio max = "<<max<<std::endl;
  hRatio->GetYaxis()->SetRangeUser(min,max);
  hRatio->GetYaxis()->SetLabelSize(0.1);
  hRatio->GetXaxis()->SetLabelSize(0.1);
  hRatio->GetXaxis()->SetTitleOffset(1.0);
  hRatio->GetXaxis()->SetTitleSize(0.12);
  hRatio->GetXaxis()->SetMoreLogLabels();
  hRatio->GetXaxis()->SetNoExponent();

  hRatio->Draw();
  hRatio->SetTitle(TString("; ")+_pars.varKinLabel+TString(" ;"));
  int nBins = hRatio->GetNbinsX();
  TLine* line = new TLine(hRatio->GetBinLowEdge(1),1,hRatio->GetBinLowEdge(nBins)+hRatio->GetBinWidth(nBins),1);
  line->SetLineWidth(2);
  line->SetLineStyle(9);
  line->Draw("same");

  TString nameForSave="canv";
  nameForSave+="_";
  nameForSave+=_pars.varKin;
  nameForSave+="_";
  nameForSave+=canv->GetTitle();
  nameForSave.ReplaceAll(" ","");
  nameForSave+=".png";
  canv->SaveAs(nameForSave);
}// end of CompareStackVsHist


TCut TPrepareYields::CutEta(int ieta)
{
  if (ieta==_BARREL) return _pars.cutBarrel;
  if (ieta==_ENDCAP) return _pars.cutEndcap;
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
