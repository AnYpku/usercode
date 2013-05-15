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
  //root classes
#include <iostream>
#include <string>
  //standard C++

TSelectionPlots::TSelectionPlots()
{
}

TSelectionPlots::~TSelectionPlots()
{
}

void TSelectionPlots::GetTrees(int channel, string confFile)
{

  TAllInputSamples INPUT(channel, confFile);

  nSources_ = INPUT.nSources_;
  for (int i=0; i<nSources_; i++)
    {

      TString fileName = (TString)(INPUT.allInputs_[i].fileSelected_).c_str();
      
      file_.push_back(new TFile(fileName,"READ"));
      hasHist_.push_back(file_.back()->IsOpen());

      if (!hasHist_.back())
        {
          std::cout<<"file "<<file_.back()->GetName()<<" can't be opened"<<std::endl;
          continue;
        }

     tree_.push_back((TTree*)file_.back()->Get("selectedEvents"));
     colors_.push_back(INPUT.allInputs_[i].color_);

     if (INPUT.allInputs_[i].sample_ == TInputSample::DATA)
       isData_.push_back(1);
     else
       isData_.push_back(0);

     if (INPUT.allInputs_[i].sample_ == TInputSample::SIGMC)
       isSigMC_.push_back(1);
     else
       isSigMC_.push_back(0);

     sourceLabel_.push_back(INPUT.allInputs_[i].sourceLatexLabel_);
    }
}

void TSelectionPlots::SetHistograms(string plotVar, int nBins, float* binLimits)
{

  legend_ = new TLegend(0.5,0.7,0.95,0.95);
  for (int i=0; i<nSources_; i++)
    {
      if (!hasHist_[i]) continue;

      TString histName="hist";
      histName+=i;
      file_[i]->cd();
      hist_.push_back(new TH1F(histName, plotVar.c_str(), nBins, binLimits));
      tree_[i]->Draw(plotVar+TString(">>") + histName,"weight");
      hist_.back()->Print();
      hist_.back()->SetLineColor(colors_[i]);
      hist_.back()->SetFillColor(colors_[i]);
      hist_.back()->SetMarkerColor(colors_[i]);
      hist_.back()->SetStats(0);
     if (isData_[i]) {
       hist_.back()->SetMarkerStyle(20);
       hist_.back()->SetOption("PE");
     }

     legend_->AddEntry(hist_.back(), sourceLabel_[i].c_str());
    }
    legend_->SetLineColor(1);
    legend_->SetLineWidth(2);
    legend_->SetFillColor(0);
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


void TSelectionPlots::DrawSpectrumDataVsMC()
{

  TCanvas* c = new TCanvas("c","c");
  THStack* mcHists = new THStack("mcHists","mcHists");
  int dataIndex = -1;
  
  if (hist_.size()==hasHist_.size() && hist_.size()==isData_.size());
  else {
   std::cout<<"ERROR detected in TSelectionPlots::DrawSpectrumDataVsMC: vector sizes mismatch: hist_.size()="<<hist_.size()<<", hasHist_.size()"<<hasHist_.size()<<", isData_.size()="<<isData_.size()<<std::cout;
   return;
  }

  float maxData;
  float maxMC;
  float maxDataMC;
  for (int i=0; i<hasHist_.size(); i++)
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


  if (dataIndex!=-1) {
    hist_[dataIndex]->GetYaxis()->SetRange(0,maxDataMC);
    hist_[dataIndex]->Draw();
    mcHists->Draw("same");
    hist_[dataIndex]->Draw("EP same");
  }
  else mcHists->Draw();

  legend_->Draw("same");
  c->SaveAs("drawSpectrumDataVsMC.png");

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

void TSelectionPlots::DrawSpectrumSigVsBkg()
{

  TCanvas* c = new TCanvas("c1","c1");
  THStack* bkgHists = new THStack("bkgHists","bkgHists");
  bool hasSigMC=0;
  
  for (int i=0; i<hasHist_.size(); i++)
    {
      if (hasHist_[i] && !isSigMC_[i] && !isData_[i]) {
        hist_[i]->SetFillStyle(3144);
        bkgHists->Add(hist_[i]);
      }
      if (hasHist_[i] &&  isSigMC_[i] && !isData_[i]) {   
        hasSigMC = 1;   
        hist_[i]->Draw();
      }
    }

  if (hasSigMC) bkgHists->Draw("same");
  else bkgHists->Draw();

  legend_->Draw("same");
  c->SaveAs("drawSpectrumSigVsBkg.png");
}
