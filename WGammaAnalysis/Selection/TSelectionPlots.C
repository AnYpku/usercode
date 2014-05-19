#include "TSelectionPlots.h"
  //this class
#include "../Configuration/TInputSample.h"
#include "../Configuration/TAllInputSamples.h"
#include "../Configuration/TConfiguration.h"
  //this package
#include <TString.h>
#include <TFile.h>
#include <TTree.h>
#include <TTree.h>
#include <TCanvas.h>
#include <THStack.h>
#include <TCut.h>
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

bool TSelectionPlots::GetTrees(int channel, string confFile, string strSources)
{

  stringstream ss(strSources);
  vector <TString> names;
  TString name;
  while (ss >> name) 
    names.push_back(name);
  int nNames = names.size();

  TAllInputSamples INPUT(channel, confFile);
  TConfiguration config;

  _nSources = 0;
  for (int i=0; i<INPUT.nSources_; i++)
    {
//      TString fileName = (TString)(config.GetSelectedName(config.VERY_PRELIMINARY, channel, INPUT.allInputs_[i].sample_,INPUT.allInputs_[i].sourceName_)) ;
      TString fileName = (TString)(config.GetSelectedName(config.FULLY, channel, INPUT.allInputs_[i].sample_,INPUT.allInputs_[i].sourceName_)) ;
      bool doThisSource=0;

      for (int j=0; j<nNames; j++)
        {
          if (names[j]==INPUT.allInputs_[i].sourceName_)
            doThisSource=1; 
        }
      if (strSources=="ALL")
        doThisSource=1;

      if (!doThisSource) continue;
      _nSources++;
      
      _file.push_back(new TFile(fileName,"READ"));
      _hasHist.push_back(_file.back()->IsOpen());

      if (!_hasHist.back())
        {
          std::cout<<"file "<<_file.back()->GetName()<<" can't be opened"<<std::endl;
          _tree.push_back(0);
          _colors.push_back(0);
          _isData.push_back(0);
          _isSigMC.push_back(0);
          _sourceLabel.push_back("");
          continue;
        }

     _tree.push_back((TTree*)_file.back()->Get("selectedEvents"));
     _colors.push_back(INPUT.allInputs_[i].color_);

     if (INPUT.allInputs_[i].sample_ == TConfiguration::DATA)
       _isData.push_back(1);
     else
       _isData.push_back(0);

     if (INPUT.allInputs_[i].sample_ == TConfiguration::SIGMC)
       _isSigMC.push_back(1);
     else
       _isSigMC.push_back(0);

     _sourceLabel.push_back(INPUT.allInputs_[i].sourceLatexLabel_);
     _sourceName.push_back(INPUT.allInputs_[i].sourceName_);
    }

  return 1;
}

bool TSelectionPlots::SetHistograms(TString plotVar, int nBins, float* binLimits, TCut cut)
{

  _legend = new TLegend(0.7,0.7,0.95,0.95);
  for (unsigned int i=0; i<_hasHist.size(); i++)
    {
      TString histName="hist"+plotVar;
      histName+=_sourceLabel[i];
      if (_isData[i]) _dataIndex=i;
      if (_isSigMC[i]) _sigmcIndex=i;
      if (_sourceName[i]=="Wg_to_taunu") _taunuIndex=i;
      if (_sourceName[i]=="Wjets_to_lnu") _wjetsIndex=i;
      if (_sourceName[i]=="Zg") _zgIndex=i;
      if (_sourceName[i]=="DYjets_to_ll") _dyjetsIndex=i;
      if (_sourceName[i]=="ttbarg") _ttgIndex=i;
      if (_sourceName[i]=="ttbarjets") _ttjetsIndex=i;
      histName.ReplaceAll("#","_");
      histName.ReplaceAll("rightarrow","_to_");
      histName.ReplaceAll("bar","");
      histName.ReplaceAll("{","");
      histName.ReplaceAll("}","");
      histName.ReplaceAll("+","");
      histName.ReplaceAll("gamma","g");
      if (!_hasHist[i]){
        _hist.push_back(new TH1F(histName, plotVar , nBins, binLimits));
        continue;
      }
      _file[i]->cd();
      _hist.push_back(new TH1F(histName, plotVar , nBins, binLimits));
      _tree[i]->Draw(plotVar+TString(">>") + histName,cut);
      _hist.back()->Print();
      _hist.back()->SetLineColor(_colors[i]);
      _hist.back()->SetFillColor(_colors[i]);
      _hist.back()->SetMarkerColor(_colors[i]);
      _hist.back()->SetStats(0);
     if (_isData[i]) {
       _hist.back()->SetMarkerStyle(20);
       _hist.back()->SetOption("PE");
     }

     _legend->AddEntry(_hist.back(), _sourceLabel[i] );
    }
    //subtracting smth+gamma from smth+jets
    std::cout<<"indeces: wjets="<<_wjetsIndex<<", Wg_to_taunu="<<_taunuIndex<<", sigmc="<<_sigmcIndex<<", dyjets="<<_dyjetsIndex<<", Zg="<<_zgIndex<<", ttjets="<<_ttjetsIndex<<", ttg="<<_ttgIndex<<std::endl;

    if (_wjetsIndex!=-1 && _taunuIndex!=-1){
      _hist[_wjetsIndex]->Add(_hist[_taunuIndex],-1);
      _hist[_wjetsIndex]->Print();
    }
    if (_wjetsIndex!=-1 && _sigmcIndex!=-1){
      _hist[_wjetsIndex]->Add(_hist[_sigmcIndex],-1);
      _hist[_wjetsIndex]->Print();
    }
    if (_dyjetsIndex!=-1 && _zgIndex!=-1){
      _hist[_dyjetsIndex]->Add(_hist[_zgIndex],-1);
      _hist[_dyjetsIndex]->Print();
    }
    if (_ttjetsIndex!=-1 && _ttgIndex!=-1){
      _hist[_ttjetsIndex]->Add(_hist[_ttgIndex],-1);
      _hist[_ttjetsIndex]->Print();
    }

    _legend->SetLineColor(1);
    _legend->SetLineWidth(2);
    _legend->SetFillColor(0);
  if (!CheckSizesOfAllVectors()) {
    std::cout<<"wrong vector sizes in TSelectionPlots::SetHistorgams"<<std::endl;
    return 0;
  }
  return 1;
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


void TSelectionPlots::DrawSpectrumDataVsMC(TString nameCanvas,TString nameForSave, bool isNoData)
{

  TCanvas* c = new TCanvas(nameCanvas,nameCanvas);
  THStack* mcHists = new THStack("mcHists","mcHists");
  int dataIndex = -1;
  
  if (_hist.size()==_hasHist.size() && _hist.size()==_isData.size());
  else {
   std::cout<<"ERROR detected in TSelectionPlots::DrawSpectrumDataVsMC: vector sizes mismatch: _file.size()="<<_file.size()<<", _hasHist.size()="<<_hasHist.size()<<", _tree.size()="<<_tree.size()<<", _colors.size()="<<_colors.size()<<", _isData.size()="<<_isData.size()<<", _isSigMC.size()="<<_isSigMC.size()<<", _sourceLabel.size()="<<_sourceLabel.size()<<", _hist.size()="<<_hist.size()<<std::endl;
   return;
  }

  float maxData=0;
  float maxMC=0;
  float maxDataMC=0;
  bool maxIsData=0;
  for (int i=_hasHist.size()-1; i>=0; i--)
    {
      if (_hasHist[i] && !_isData[i]) mcHists->Add(_hist[i]);
      if (_hasHist[i] && _isData[i]) {   
        dataIndex = i;   
        maxData = _hist[i]->GetMaximum();
      }
    }
  maxMC = mcHists->GetMaximum();
  if (maxData>=maxMC){ maxDataMC = maxData; maxIsData=1;}
  else maxDataMC = maxMC;


  if (isNoData)
    mcHists->Draw();
  else if (maxIsData){
    _hist[dataIndex]->GetYaxis()->SetRangeUser(0,1.1*maxDataMC);
    _hist[dataIndex]->Draw("AP");
    mcHists->Draw("same");
    _hist[dataIndex]->Draw("EP same");
  }
  else{
    mcHists->Draw();
    _hist[dataIndex]->Draw("EP same");    
  }

  _legend->Draw("same");
  c->SaveAs(nameForSave);


/*

  TH1F* ratioHist=(TH1F*)hMassv[0]->Clone("ratioHist");
  for(UInt_t isam=(hasData) ? 1:0; isam<samplev.size(); isam++)
    ratioHist->Add(hMassv[isam],-1);
  ratioHist->Divide(ratioHist,hMassv[0],1.0,1.0);



  if (actualBinning) canvName="massBinning-2";
  else canvName="massPeak-2";
  TCanvas *c2 = new TCanvas(canvName,canvName,800,800); 
  c2->Divide(1,2,0,0);
  TPad* pad1 = (TPad*)c2->GetPad(1);
  TPad* pad2 = (TPad*)c2->GetPad(2);
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

  pad1->SetLogx();
  pad1->SetLogy();
  pad1->cd();

  mcHists->Draw("hist");
  hMassv[0]->Draw("pe,same");

  pad2->SetLogx();
  pad2->cd();
  ratioHist->Draw();

  SaveCanvas(c2, canvName);
  if (histoFile) c2->Write();
*/
}

void TSelectionPlots::DrawSpectrumSigVsBkg(TString nameCanvas,TString nameForSave)
{

  TCanvas* c = new TCanvas(nameCanvas,nameCanvas);
  TString histName="hist_"+nameCanvas;
  THStack* bkgHists = new THStack(histName,histName);
  int sigMC=-1;
 
  for (unsigned int i=0; i<_hasHist.size(); i++)
    {
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

