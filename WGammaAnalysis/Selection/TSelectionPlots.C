#include "TSelectionPlots.h"
  //this class
#include "../Configuration/TInputSample.h"
#include "../Configuration/TAllInputSamples.h"
#include "../Configuration/TConfiguration.h"
#include "../Include/TMuonCuts.h"
#include "../Include/TPhotonCuts.h"
#include "../Include/TFullCuts.h"
  //this package
#include <TString.h>
#include <TFile.h>
#include <TTree.h>
#include <TTree.h>
#include <TCanvas.h>
#include <THStack.h>
#include <TCut.h>
#include <TPad.h>
  //root classes
#include <iostream>
#include <sstream>
#include <string>
  //standard C++

TSelectionPlots::TSelectionPlots()
{
  _dataIndex=-1;
  _sigmcIndex=-1;
  _taunuIndex=-1;
  _wjetsIndex=-1;
  _zgIndex=-1;
  _dyjetsIndex=-1;
  _ttgIndex=-1;
  _ttjetsIndex=-1;
}

TSelectionPlots::~TSelectionPlots()
{
}

bool TSelectionPlots::GetTrees(int channel, int vgamma, int blind, string confFile, string strSources)
{

  stringstream ss(strSources);
  vector <TString> names;
  TString name;
  while (ss >> name) 
    names.push_back(name);
  int nNames = names.size();

  TAllInputSamples INPUT(channel, vgamma, confFile);
  TConfiguration config;

  _nSources = 0;
  for (int iSource=0; iSource<INPUT.nSources_; iSource++){
    TString fileName = (TString)(config.GetSelectedName(config.VERY_PRELIMINARY, channel, vgamma, blind,INPUT.allInputs_[iSource].sample_,INPUT.allInputs_[iSource].sourceName_)) ;
//      TString fileName = (TString)(config.GetSelectedName(config.FULLY, channel, blind, INPUT.allInputs_[iSource].sample_,INPUT.allInputs_[iSource].sourceName_)) ;
    bool doThisSource=0;

    if (INPUT.allInputs_[iSource].sample_!=TConfiguration::SIGMC) continue;

    for (int j=0; j<nNames; j++){
      if (names[j]==INPUT.allInputs_[iSource].sourceName_)
        doThisSource=1; 
    }
    if (strSources=="ALL")
      doThisSource=1;

    if (!doThisSource) continue;
    _nSources++;
      
    _file.push_back(new TFile(fileName,"READ"));
    _hasHist.push_back(_file.back()->IsOpen());

    if (!_hasHist.back()){
      std::cout<<"file "<<_file.back()->GetName()<<" can't be opened"<<std::endl;
      _tree.push_back(0);
      _colors.push_back(0);
      _isData.push_back(0);
      _isSigMC.push_back(0);
      _sourceLabel.push_back("");
      continue;
    }

    _tree.push_back((TTree*)_file.back()->Get("selectedEvents"));
    _colors.push_back(INPUT.allInputs_[iSource].color_);

    if (INPUT.allInputs_[iSource].sample_ == TConfiguration::DATA)
      _isData.push_back(1);
    else
      _isData.push_back(0);

    if (INPUT.allInputs_[iSource].sample_ == TConfiguration::SIGMC)
      _isSigMC.push_back(1);
    else
      _isSigMC.push_back(0);

    _sourceLabel.push_back(INPUT.allInputs_[iSource].sourceLatexLabel_);
    _sourceName.push_back(INPUT.allInputs_[iSource].sourceName_);
  }//loop over iSource

  return 1;
}

void TSelectionPlots::SelectionEfficiencyInStages(int year, int vgamma, int wp, TString strIsoBase)
{

  TMuonCuts muon;
  TPhotonCuts photon;
  TFullCuts fullCuts;
  TConfiguration config;
  int nSources=_hasHist.size();
  int nTot[nSources];
  int nPass[nSources];
  float eff[nSources];

  int nCuts=7;
  TCut cutExtra[nCuts];
  TString strDescr[nCuts];


  TCut cut="event==6865278";
//  TCut cut="event==6870283";
//  TCut cut="event==6883214";
//  TCut cut="event==7537047";
//  TCut cut="event==6958805";



  cut = cut && photon.RangeBarrel();

  cutExtra[0]=photon.RangeSigmaIEtaIEta(year, wp);
  cutExtra[1]=photon.RangeHoverE(year);
  cutExtra[2]=photon.RangePhoEleVeto();
  cutExtra[3]="1";//photon.RangeOneIsolation(year,wp,photon.ISO_CHorTRK);
  cutExtra[4]=photon.RangeOneIsolation(year,wp,photon.ISO_NEUorHCAL);
  cutExtra[5]=photon.RangeOneIsolation(year,wp,photon.ISO_PHOorECAL);
  cutExtra[6]=fullCuts.RangeMetRelatedCut(year,config.ELECTRON);


  for (int ic=0; ic<7; ic++){
    SelectionEfficiencyOneStage(nSources, nTot, nPass, eff, cut, cutExtra[ic], strDescr[ic]);
    cut = cut && cutExtra[ic];
  }

}

void TSelectionPlots::SelectionEfficiencyOneStage(int nSources, int* nTot, int* nPass, float* eff, TCut cut, TCut cutExtra, TString strCutDescription)
{
  std::cout<<std::endl;
  std::cout<<"cut: "<<strCutDescription<<" ( "<<cutExtra.GetTitle()<<" )"<<std::endl;
//  for (int is=0; is<nSources; is++){
  for (int is=0; is<1; is++){
    if (!_hasHist[is]) {nTot[is]=0; nPass[is]=0; eff[is]=0; continue;}
    nTot[is]=_tree[is]->GetEntries(cut);
    if (nTot[is]==0){std::cout<<_sourceName[is]<<": nTot="<<nTot[is]<<std::endl;continue;} 
    nPass[is]=_tree[is]->GetEntries(cut && cutExtra);
    eff[is]=1.0*nPass[is]/nTot[is];
    std::cout<<_sourceName[is]<<": nTot="<<nTot[is]<<", nPass="<<nPass[is]<<", eff="<<eff[is]<<std::endl;        
  }
  std::cout<<std::endl; 
}

bool TSelectionPlots::SetHistograms(int blind, TString plotVar, int nBins, float* binLimits, TCut cutWhole, TCut cutCutted)
{
  TCanvas* c = new TCanvas("cSetHistograms","cSetHistograms");
  _legend = new TLegend(0.7,0.7,0.95,0.95);
  for (unsigned int iHist=0; iHist<_hasHist.size(); iHist++){

      SetSourceIndex(iHist);

      TString histName=StrHistName(plotVar,iHist);

      if (!_hasHist[iHist]){
        _hist.push_back(new TH1F(histName, plotVar , nBins, binLimits));
        continue;
      }
      _file[iHist]->cd();
      _hist.push_back(new TH1F(histName, plotVar , nBins, binLimits));
      _tree[iHist]->Draw(plotVar+TString(">>") + histName,cutWhole);
      TConfiguration config;
      if (!_isData[iHist] && blind==config.BLIND_PRESCALE) 
        _hist.back()->Scale(1.0/config.GetBlindPrescale());
      _hist.back()->SetTitle(plotVar);
      SetHistProperties(_hist.back(), iHist, 0);

     _legend->AddEntry(_hist.back(), _sourceLabel[iHist] );
/*
     int nBinFirst = 0;
     int nBinLast = nBins-1;
     for (int ib=0; ib<=nBins; ib++){
       if (binLimits[ib]<=cutMin) nBinFirst=ib;
       if (binLimits[ib]>=cutMax){
         nBinLast=ib;
         break;
       }
     }
     float binLimitsCutted[nBinLast-nBinFirst+1];
     for (int ib=0; ib<nBinLast-nBinFirst+1; ib++){
       binLimitsCutted[ib]=binLimits[ib+nBinFirst];
     }
*/
     histName=histName+TString("Cutted");
     _histCutted.push_back(new TH1F(histName, plotVar , nBins, binLimits));
     _tree[iHist]->Draw(plotVar+TString(">>") + histName,cutCutted);
      if (!_isData[iHist] && blind==config.BLIND_PRESCALE) 
        _histCutted.back()->Scale(1.0/config.GetBlindPrescale());
     _histCutted.back()->SetTitle(plotVar);
     SetHistProperties(_histCutted.back(), iHist, 1);
  }//end of loop over hists

  std::cout<<"indeces: wjets="<<_wjetsIndex<<", Wg_to_taunu="<<_taunuIndex<<", sigmc="<<_sigmcIndex<<", dyjets="<<_dyjetsIndex<<", Zg="<<_zgIndex<<", ttjets="<<_ttjetsIndex<<", ttg="<<_ttgIndex<<", multibosons="<<_multibosonsIndex<<std::endl;

  //subtracting smth+gamma from smth+jets
  HistSubtractions(_wjetsIndex,  _taunuIndex);
  HistSubtractions(_wjetsIndex,  _sigmcIndex);
  HistSubtractions(_dyjetsIndex, _zgIndex);
  HistSubtractions(_ttjetsIndex, _ttgIndex);

  _legend->SetLineColor(1);
  _legend->SetLineWidth(2);
  _legend->SetFillColor(0);
  if (!CheckSizesOfAllVectors()) {
    std::cout<<"wrong vector sizes in TSelectionPlots::SetHistorgams"<<std::endl;
    return 0;
  }
  return 1;
}

void TSelectionPlots::SetSourceIndex(int iHist)
{
  if (_isData[iHist]) _dataIndex=iHist;
  if (_isSigMC[iHist]) _sigmcIndex=iHist;
  if (_sourceName[iHist]=="Wg_to_taunu") _taunuIndex=iHist;
  if (_sourceName[iHist]=="Wjets_to_lnu") _wjetsIndex=iHist;
  if (_sourceName[iHist]=="Zg") _zgIndex=iHist;
  if (_sourceName[iHist]=="DYjets_to_ll") _dyjetsIndex=iHist;
  if (_sourceName[iHist]=="ttbarg") _ttgIndex=iHist;
  if (_sourceName[iHist]=="ttbarjets") _ttjetsIndex=iHist;
  if (_sourceName[iHist]=="multibosons") _multibosonsIndex=iHist;
}

TString TSelectionPlots::StrHistName(TString plotVar, int iHist)
{
  TString histName="hist"+plotVar;
  histName+=_sourceLabel[iHist];
  histName.ReplaceAll("#","");
  histName.ReplaceAll("rightarrow","_to_");
  histName.ReplaceAll("bar","");
  histName.ReplaceAll("{","");
  histName.ReplaceAll("}","");
  histName.ReplaceAll("+","");
  histName.ReplaceAll("-","");
  histName.ReplaceAll("gamma","g");
  return histName;
}

void TSelectionPlots::SetHistProperties(TH1F* hist, int iHist, bool isCutted)
{
  hist->SetLineColor(_colors[iHist]);
  hist->SetFillColor(_colors[iHist]);
  if (!isCutted) hist->SetFillStyle(3001);
  if (isCutted) hist->SetFillStyle(1001);
  hist->SetMarkerColor(_colors[iHist]);
  hist->SetStats(0);
  if (_isData[iHist]) {
    if (!isCutted) hist->SetMarkerStyle(20);
    if (isCutted) hist->SetMarkerStyle(26);
    hist->SetFillStyle(0);
    //if (isCutted) hist->SetLineColor(635);
    //if (isCutted) hist->SetMarkerColor(635);
    hist->SetOption("PE");
  }
}

void TSelectionPlots::HistSubtractions(int ind1, int ind2)
{
  if (ind1==-1 || ind2==-1) return;
  _hist[ind1]->Add(_hist[ind2],-1);
  _histCutted[ind1]->Add(_histCutted[ind2],-1);
}

void TSelectionPlots::ScaleHistogramsToData()
{
  int dataIndex = -1;
  int vectorLength = _hasHist.size();
  float sumWeightsData = 0;
  float sumWeightsMC = 0;
  for (int i=0; i<vectorLength; i++)
    {
      if (_hasHist[i]) {   
        if (_isData[i]) { 
          dataIndex = i;
          sumWeightsData = _hist[i]->GetSumOfWeights();
        }
        else sumWeightsMC+=_hist[i]->GetSumOfWeights();
      }
    }
  if (dataIndex==-1) return;
  for (int i=0; i<vectorLength; i++)
    {
      if (_hasHist[i]) {   
        if (_isData[i]) continue;
        _hist[i]->Scale(sumWeightsData/sumWeightsMC);
      }
    }
  std::cout<<"TSelectionPlots::ScaleHistogramsToData(): Scale = "<<sumWeightsData/sumWeightsMC<<std::endl;
}


void TSelectionPlots::DrawSpectrumDataVsMC(TString nameCanvas, TString nameForSave, bool isNoData)
{
  TCanvas* c = new TCanvas(nameCanvas,nameCanvas,600,720);
  THStack* mcHists = new THStack("mcHists","mcHists");
  THStack* mcHistsCutted = new THStack("mcHistsCutted","mcHistsCutted"); 
  if (_hist.size()==_hasHist.size() && _hist.size()==_isData.size());
  else {
    std::cout<<"ERROR detected in TSelectionPlots::DrawSpectrumDataVsMC: vector sizes mismatch: _file.size()="<<_file.size()<<", _hasHist.size()="<<_hasHist.size()<<", _tree.size()="<<_tree.size()<<", _colors.size()="<<_colors.size()<<", _isData.size()="<<_isData.size()<<", _isSigMC.size()="<<_isSigMC.size()<<", _sourceLabel.size()="<<_sourceLabel.size()<<", _hist.size()="<<_hist.size()<<std::endl;
    return;
  }
  float maxData=0;
  float maxMC=0;
  float maxDataMC=0;
  bool maxIsData=0;
  for (int i=_hasHist.size()-1; i>=0; i--){
    if (_hasHist[i] && !_isData[i]) { mcHists->Add(_hist[i]); mcHists->SetTitle(_hist[i]->GetTitle());}
    if (_hasHist[i] && !_isData[i]) mcHistsCutted->Add(_histCutted[i]);
    if (_hasHist[i] && _isData[i]) {     
      maxData = _hist[i]->GetMaximum();
    }
  }
  maxMC = mcHists->GetMaximum();
  if (maxData>=maxMC){ maxDataMC = maxData; maxIsData=1;}
  else maxDataMC = maxMC;
  c->Divide(1,2);
  TPad* pad1 = (TPad*)c->GetPad(1);
  TPad* pad2 = (TPad*)c->GetPad(2);
  pad1->SetPad(0,0.2,1.0,1.0);
  pad2->SetPad(0,0.0,1.0,0.18);
//  pad1->SetLeftMargin(0.18);
//  pad1->SetTopMargin(0.08);
//  pad1->SetRightMargin(0.07);
//  pad1->SetBottomMargin(0.01); // All X axis labels and titles are thus cut off
//  pad2->SetLeftMargin(0.18);
//  pad2->SetTopMargin(0.01);
//  pad2->SetRightMargin(0.07);
//  pad2->SetBottomMargin(0.45);

//  pad1->SetLogx();
//  pad1->SetLogy();
  pad1->cd();
  if (isNoData){
    mcHists->Draw();
    mcHistsCutted->Draw("same");
  }
  else if (maxIsData){
    if (_dataIndex!=-1) 
      _hist[_dataIndex]->GetYaxis()->SetRangeUser(0,1.1*maxDataMC);
    if (_dataIndex!=-1) _hist[_dataIndex]->Draw("AP");
    mcHists->Draw("same");
    mcHistsCutted->Draw("same");
    if (_dataIndex!=-1) _hist[_dataIndex]->Draw("EP same");
    if (_dataIndex!=-1) _histCutted[_dataIndex]->Draw("EP same");
  }
  else{
    mcHists->Draw();
    mcHistsCutted->Draw("same");
    if (_dataIndex!=-1) _hist[_dataIndex]->Draw("EP same");  
    if (_dataIndex!=-1) _histCutted[_dataIndex]->Draw("EP same");  
  }
  _legend->Draw("same");
  pad2->cd();
  c->SaveAs(nameForSave);
}

void TSelectionPlots::DrawSpectrumSigVsBkg(TString nameCanvas, TString nameForSave)
{
  TCanvas* c = new TCanvas(nameCanvas,nameCanvas);
  TString histName="hist_"+nameCanvas;
  THStack* bkgHists = new THStack(histName,histName);
  int sigMC=-1;
  for (unsigned int i=0; i<_hasHist.size(); i++){
    if (_hasHist[i] && !_isSigMC[i] && !_isData[i]) {
      bkgHists->Add(_hist[i]);
    }
    if (_hasHist[i] &&  _isSigMC[i] && !_isData[i]) {   
      _hist[i]->SetFillStyle(3144);
      sigMC = i;   
      //_hist[i]->Draw();
    }
  }
  bkgHists->Draw();
  if (sigMC!=-1) 
    _hist[sigMC]->Draw("same");
//  if (hasSigMC) bkgHists->Draw("same");
//  else bkgHists->Draw();
  _legend->Draw("same");
  c->SaveAs(nameForSave);
}

bool TSelectionPlots::CheckSizesOfAllVectors()
{
  if (_file.size()!=_nSources || _tree.size()!=_nSources || _hist.size()!=_nSources || 
      _sourceLabel.size()!=_nSources || _hasHist.size()!=_nSources || _isData.size()!=_nSources ||
      _isSigMC.size()!=_nSources || _colors.size()!=_nSources)
    {
      std::cout<<"ERROR detected in TSelectionPlots::CheckSizesOfAllVectors: sizes of some vectors are not equal to _nSources="<<_nSources<<std::endl;
      std::cout<<"_file.size()="<<_file.size()<<", _tree.size()="<<_tree.size()<<", _hist.size()="<<_hist.size()<<", _sourceLabel.size()="<<_sourceLabel.size()<<", _hasHist.size()="<<_hasHist.size()<<", _isData.size()="<<_isData.size()<<", _isSigMC.size()="<<_isSigMC.size()<<", _colors.size()="<<_colors.size()<<std::endl;
      return 0;
    }
  return 1;
}

void TSelectionPlots::PrintWeightedNumberOfEvents(TString cut)
{
  float nMCEvents=0;
  float nMCEventsErr=0;
  TCanvas* c = new TCanvas("cPrintWeightedNumberOfEvents","cPrintWeightedNamberOfEvents");
  for (unsigned int i=0; i<_nSources; i++){
    std::cout<<_sourceLabel[i]<<" ";
//    std::cout<<_tree[i]->GetEntries()<<" ";
//    std::cout<<_tree[i]->GetEntries(cut)<<" ";
    TH1F* h = new TH1F("h", "WMt", 100, _tree[i]->GetMinimum("WMt"), _tree[i]->GetMaximum("WMt"));
    _tree[i]->Draw("WMt>>h","1","goff");
    TH1F* hCut = new TH1F("hCut", "WMt", 100, _tree[i]->GetMinimum("WMt"), _tree[i]->GetMaximum("WMt"));
    _tree[i]->Draw("WMt>>hCut",cut,"goff");
    TH1F* hWeight = new TH1F("hWeight", "WMt", 100, _tree[i]->GetMinimum("WMt"), _tree[i]->GetMaximum("WMt"));
    _tree[i]->Draw("WMt>>hWeight","weight","goff");
    TH1F* hWeightCut = new TH1F("hWeightCut", "WMt", 100, _tree[i]->GetMinimum("WMt"), _tree[i]->GetMaximum("WMt"));
    _tree[i]->Draw("WMt>>hWeightCut","("+cut+")*weight","goff");
    TH1F* hWeightSquaredCut = new TH1F("hWeightSquaredCut", "WMt", 100, _tree[i]->GetMinimum("WMt"), _tree[i]->GetMaximum("WMt"));
    _tree[i]->Draw("WMt>>hWeightSquaredCut","("+cut+")*weight*weight","goff");
//    std::cout<<h->Integral()<<" ";
//    std::cout<<hCut->Integral()<<" ";
//    std::cout<<hWeight->Integral()<<" ";
    if (!_isData[i]) nMCEvents+=hWeightCut->Integral();
    std::cout<<hWeightCut->Integral()<<" +- ";
//    std::cout<<hWeightSquaredCut->Integral()<<" ";
    if (!_isData[i]) nMCEventsErr+=hWeightSquaredCut->Integral();
    std::cout<<sqrt(hWeightSquaredCut->Integral())<<" ";
    delete h;
    delete hCut;
    delete hWeight;
    delete hWeightCut;
    delete hWeightSquaredCut;
    std::cout<<std::endl;
  }
  nMCEventsErr=sqrt(nMCEventsErr);
  std::cout<<"TOTAL MC: ";
  std::cout<<nMCEvents<<" +- ";
  std::cout<<nMCEventsErr;
  std::cout<<std::endl;
}

