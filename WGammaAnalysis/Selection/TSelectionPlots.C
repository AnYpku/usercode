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

  nSources_ = 0;
  for (int i=0; i<INPUT.nSources_; i++)
    {
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
      nSources_++;
      
      file_.push_back(new TFile(fileName,"READ"));
      hasHist_.push_back(file_.back()->IsOpen());

      if (!hasHist_.back())
        {
          std::cout<<"file "<<file_.back()->GetName()<<" can't be opened"<<std::endl;
          tree_.push_back(0);
          colors_.push_back(0);
          isData_.push_back(0);
          isSigMC_.push_back(0);
          sourceLabel_.push_back("");
          continue;
        }

     tree_.push_back((TTree*)file_.back()->Get("selectedEvents"));
     colors_.push_back(INPUT.allInputs_[i].color_);

     if (INPUT.allInputs_[i].sample_ == TConfiguration::DATA)
       isData_.push_back(1);
     else
       isData_.push_back(0);

     if (INPUT.allInputs_[i].sample_ == TConfiguration::SIGMC)
       isSigMC_.push_back(1);
     else
       isSigMC_.push_back(0);

     sourceLabel_.push_back(INPUT.allInputs_[i].sourceLatexLabel_);
    }

  return 1;
}

bool TSelectionPlots::SetHistograms(TString plotVar, int nBins, float* binLimits, TString cut)
{

  legend_ = new TLegend(0.7,0.7,0.95,0.95);
  for (unsigned int i=0; i<nSources_; i++)
    {

      TString histName="hist"+plotVar;
      histName+=i;
      if (!hasHist_[i]){
        hist_.push_back(new TH1F(histName, plotVar , nBins, binLimits));
        continue;
      }
      file_[i]->cd();
      hist_.push_back(new TH1F(histName, plotVar , nBins, binLimits));
      tree_[i]->Draw(plotVar+TString(">>") + histName,"("+cut+")*weight");
      hist_.back()->Print();
      hist_.back()->SetLineColor(colors_[i]);
      hist_.back()->SetFillColor(colors_[i]);
      hist_.back()->SetMarkerColor(colors_[i]);
      hist_.back()->SetStats(0);
     if (isData_[i]) {
       hist_.back()->SetMarkerStyle(20);
       hist_.back()->SetOption("PE");
     }

     legend_->AddEntry(hist_.back(), sourceLabel_[i] );
    }
    legend_->SetLineColor(1);
    legend_->SetLineWidth(2);
    legend_->SetFillColor(0);
  if (!CheckSizesOfAllVectors()) {
    std::cout<<"wrong vector sizes in TSelectionPlots::SetHistorgams"<<std::endl;
    return 0;
  }
  return 1;
}

void TSelectionPlots::ScaleHistogramsToData()
{
  int dataIndex = -1;
  int vectorLength = hasHist_.size();
  float sumWeightsData = 0;
  float sumWeightsMC = 0;
  for (int i=0; i<vectorLength; i++)
    {
      if (hasHist_[i]) {   
        if (isData_[i]) { 
          dataIndex = i;
          sumWeightsData = hist_[i]->GetSumOfWeights();
        }
        else sumWeightsMC+=hist_[i]->GetSumOfWeights();
      }
    }
  if (dataIndex==-1) return;
  for (int i=0; i<vectorLength; i++)
    {
      if (hasHist_[i]) {   
        if (isData_[i]) continue;
        hist_[i]->Scale(sumWeightsData/sumWeightsMC);
      }
    }
  std::cout<<"TSelectionPlots::ScaleHistogramsToData(): Scale = "<<sumWeightsData/sumWeightsMC<<std::endl;
}


void TSelectionPlots::DrawSpectrumDataVsMC(TString nameCanvas,TString nameForSave, bool isNoData)
{

  TCanvas* c = new TCanvas(nameCanvas,nameCanvas);
  THStack* mcHists = new THStack("mcHists","mcHists");
  int dataIndex = -1;
  
  if (hist_.size()==hasHist_.size() && hist_.size()==isData_.size());
  else {
   std::cout<<"ERROR detected in TSelectionPlots::DrawSpectrumDataVsMC: vector sizes mismatch: hist_.size()="<<hist_.size()<<", hasHist_.size()"<<hasHist_.size()<<", isData_.size()="<<isData_.size()<<std::cout;
   return;
  }

  float maxData=0;
  float maxMC=0;
  float maxDataMC=0;
  for (int i=hasHist_.size()-1; i>=0; i--)
    {
      if (hasHist_[i] && !isData_[i]) mcHists->Add(hist_[i]);
      if (hasHist_[i] && isData_[i]) {   
        dataIndex = i;   
        maxData = hist_[i]->GetMaximum();
      }
    }
  maxMC = mcHists->GetMaximum();
  if (maxData>=maxMC) maxDataMC = maxData;
  else maxDataMC = maxMC;


  if (isNoData || dataIndex==0)
    mcHists->Draw();
  else {
    hist_[dataIndex]->GetYaxis()->SetRangeUser(0,1.1*maxDataMC);
    hist_[dataIndex]->Draw();
    mcHists->Draw("same");
    hist_[dataIndex]->Draw("EP same");
  }

  legend_->Draw("same");
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
 
  for (unsigned int i=0; i<hasHist_.size(); i++)
    {
      if (hasHist_[i] && !isSigMC_[i] && !isData_[i]) {
        bkgHists->Add(hist_[i]);
      }
      if (hasHist_[i] &&  isSigMC_[i] && !isData_[i]) {   
        hist_[i]->SetFillStyle(3144);
        sigMC = i;   
        //hist_[i]->Draw();
      }
    }

  bkgHists->Draw();
  if (sigMC!=-1) 
    hist_[sigMC]->Draw("same");

//  if (hasSigMC) bkgHists->Draw("same");
//  else bkgHists->Draw();

  legend_->Draw("same");
  c->SaveAs(nameForSave);

}

bool TSelectionPlots::CheckSizesOfAllVectors()
{
  if (file_.size()!=nSources_ || tree_.size()!=nSources_ || hist_.size()!=nSources_ || 
      sourceLabel_.size()!=nSources_ || hasHist_.size()!=nSources_ || isData_.size()!=nSources_ ||
      isSigMC_.size()!=nSources_ || colors_.size()!=nSources_)
    {
      std::cout<<"ERROR detected in TSelectionPlots::CheckSizesOfAllVectors: sizes of some vectors are not equal to nSources_="<<nSources_<<std::endl;
      std::cout<<"file_.size()="<<file_.size()<<", tree_.size()="<<tree_.size()<<", hist_.size()="<<hist_.size()<<", sourceLabel_.size()="<<sourceLabel_.size()<<", hasHist_.size()="<<hasHist_.size()<<", isData_.size()="<<isData_.size()<<", isSigMC_.size()="<<isSigMC_.size()<<", colors_.size()="<<colors_.size()<<std::endl;
      return 0;
    }
  return 1;
}

void TSelectionPlots::PrintWeightedNumberOfEvents(TString cut)
{

  float nMCEvents=0;
  float nMCEventsErr=0;
  for (unsigned int i=0; i<nSources_; i++){
    std::cout<<sourceLabel_[i]<<" ";
//    std::cout<<tree_[i]->GetEntries()<<" ";
//    std::cout<<tree_[i]->GetEntries(cut)<<" ";

    TH1F* h = new TH1F("h", "WMt", 100, tree_[i]->GetMinimum("WMt"), tree_[i]->GetMaximum("WMt"));
    tree_[i]->Draw("WMt>>h","1","goff");

    TH1F* hCut = new TH1F("hCut", "WMt", 100, tree_[i]->GetMinimum("WMt"), tree_[i]->GetMaximum("WMt"));
    tree_[i]->Draw("WMt>>hCut",cut,"goff");

    TH1F* hWeight = new TH1F("hWeight", "WMt", 100, tree_[i]->GetMinimum("WMt"), tree_[i]->GetMaximum("WMt"));
    tree_[i]->Draw("WMt>>hWeight","weight","goff");

    TH1F* hWeightCut = new TH1F("hWeightCut", "WMt", 100, tree_[i]->GetMinimum("WMt"), tree_[i]->GetMaximum("WMt"));
    tree_[i]->Draw("WMt>>hWeightCut","("+cut+")*weight","goff");

    TH1F* hWeightSquaredCut = new TH1F("hWeightSquaredCut", "WMt", 100, tree_[i]->GetMinimum("WMt"), tree_[i]->GetMaximum("WMt"));
    tree_[i]->Draw("WMt>>hWeightSquaredCut","("+cut+")*weight*weight","goff");


//    std::cout<<h->Integral()<<" ";
//    std::cout<<hCut->Integral()<<" ";
//    std::cout<<hWeight->Integral()<<" ";
    if (!isData_[i]) nMCEvents+=hWeightCut->Integral();
    std::cout<<hWeightCut->Integral()<<" +- ";
//    std::cout<<hWeightSquaredCut->Integral()<<" ";
    if (!isData_[i]) nMCEventsErr+=hWeightSquaredCut->Integral();
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

