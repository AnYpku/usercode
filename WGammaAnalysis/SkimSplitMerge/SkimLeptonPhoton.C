#include "SkimLeptonPhoton.h"
#include <TString.h>
#include <TTree.h>
#include <TH1F.h>
#include <iostream>

SkimLeptonPhoton::SkimLeptonPhoton(TString inputFileName, TString nameDir, TString nameTree, TString outDir, int channel)
{

  channel_=channel;  

  nameDir_=nameDir;
  nameTree_=nameTree;

  inputFileName_=inputFileName;

  TString inpTemp = inputFileName_;
  while (inpTemp.Contains('/')){
    inpTemp.Remove(0,1);
  }
  inpTemp.ReplaceAll(".root","");

  if (channel == MUON || channel == BOTH){
    skimmedFileNameMu_=outDir+inpTemp+"_MuPhoSkim.root";
    fileOutMu_ = new TFile(skimmedFileNameMu_,"recreate");
    fileOutMu_->mkdir(nameDir_);
    fileOutMu_->cd(nameDir_);
    outputTreeMu_ = new TTree(nameTree_,nameTree_);
    TREE_.InitOutputTree(outputTreeMu_);
      //method of TInputOutputTree
    hskimMu_ = new TH1F("hskim","hskim",2,0,2);
  }

  if (channel == ELECTRON || channel == BOTH){
    skimmedFileNameEle_=outDir+inpTemp+"_ElePhoSkim.root"; 
    fileOutEle_ = new TFile(skimmedFileNameEle_,"recreate");
    fileOutEle_->mkdir(nameDir_);
    fileOutEle_->cd(nameDir_);
    outputTreeEle_ = new TTree(nameTree_,nameTree_);
    TREE_.InitOutputTree(outputTreeEle_);
      //method of TInputOutputTree
    hskimEle_ = new TH1F("hskim","hskim",2,0,2);
  }

}

SkimLeptonPhoton::~SkimLeptonPhoton()
{
  TREE_.fChain = 0;
  if (channel_==MUON || channel_==BOTH) fileOutMu_->Close();
  if (channel_==ELECTRON || channel_==BOTH) fileOutEle_->Close();
}

void SkimLeptonPhoton::LoopOverInputTree()
{
  TFile f(inputFileName_,"READ");
  f.cd(nameDir_);
  TTree* tree =(TTree*)gDirectory->Get(nameTree_);
  bool hashEvents = gDirectory->GetListOfKeys()->Contains("hEvents");
  bool hashPU = gDirectory->GetListOfKeys()->Contains("hPU");
  bool hashPUTrue = gDirectory->GetListOfKeys()->Contains("hPUTrue");
  bool hashskim = gDirectory->GetListOfKeys()->Contains("hskim");  

  TREE_.Init(tree);
    //method of TEventTree
  TH1F* hEvents;
  TH1F* hPU;
  TH1F* hPUTrue;
  TH1F* hskim;

  if (hashEvents)
    hEvents = (TH1F*)gDirectory->Get("hEvents");
  if (hashPU)
    hPU = (TH1F*)gDirectory->Get("hPU");
  if (hashPUTrue)
    hPUTrue = (TH1F*)gDirectory->Get("hPUTrue");
  if (hashskim)
    hskim = (TH1F*)gDirectory->Get("hskim");

  if (TREE_.fChain == 0) return;
        //field of TInputTree

  Long64_t nentries = TREE_.fChain->GetEntries();
  //nentries = 10;
  int nMuPassed=0;
  int nElePassed=0;

  for (Long64_t entry=0; entry<nentries; entry++) 
    {
      if (entry < 0) break;

      TREE_.fChain->GetEntry(entry);
      if (TREE_.treeLeaf.nPho>0 && TREE_.treeLeaf.nMu>0 && (channel_==MUON || channel_==BOTH))
        {
          nMuPassed++; 
          outputTreeMu_->Fill();
        }
      if (TREE_.treeLeaf.nPho>0 && TREE_.treeLeaf.nEle>0 && (channel_==ELECTRON || channel_==BOTH))
        {
          nElePassed++;
          outputTreeEle_->Fill();
        } 
  
    }//loop over entries ends

  //writing output trees to files
  if (channel_==MUON || channel_==BOTH){
    fileOutMu_->cd();
    fileOutMu_->cd(nameDir_);
    outputTreeMu_->Write(nameTree_);
    if (hashEvents)
      hEvents->Write();
    if (hashPU)
      hPU->Write();
    if (hashPUTrue)
      hPUTrue->Write();
    if (hashskim)
      hskimMu_->SetBinContent(1,hskim->GetBinContent(1));
    else 
      hskimMu_->SetBinContent(1,tree->GetEntries());
    hskimMu_->SetBinContent(2,nMuPassed); 
    hskimMu_->Write();
  }

  if (channel_==ELECTRON || channel_==BOTH){
    fileOutEle_->cd();
    fileOutEle_->cd(nameDir_);
    outputTreeEle_->Write(nameTree_);
    //if (hashEvents)
    //  hEvents->Write();
    if (hashPU)
      hPU->Write();
    //if (hashPUTrue)
    //  hPUTrue->Write();
    if (hashskim)
      hskimEle_->SetBinContent(1,hskim->GetBinContent(1));
    else 
      hskimEle_->SetBinContent(1,tree->GetEntries());
    hskimEle_->SetBinContent(2,nElePassed); 
    hskimEle_->Write();
  }
}//Skim::LoopOverInputTree() ends
