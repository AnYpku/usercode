#include "SkimLeptonPhoton.h"
#include <TString.h>
#include <TTree.h>
#include <TH1F.h>
#include <iostream>

SkimLeptonPhoton::SkimLeptonPhoton(int channel, int sample, TString inputFileName, bool doOverlapRemoval, bool isDebugMode, TString outDir, TString nameDir, TString nameTree)
{

  _sample=sample;
  _channel=channel;  

  _doOverlapRemoval=doOverlapRemoval;
  _isDebugMode=isDebugMode;

  _nameDir=nameDir;
  _nameTree=nameTree;

  _inputFileName=inputFileName;

  TString inpTemp = _inputFileName;
  while (inpTemp.Contains('/')){
    inpTemp.Remove(0,1);
  }
  inpTemp.ReplaceAll(".root","");

  if (channel == _config.MUON || channel == _config.BOTH){
    _skimmedFileNameMu=outDir+inpTemp+"_MuPhoSkim.root";
    _fileOutMu = new TFile(_skimmedFileNameMu,"recreate");
    _fileOutMu->mkdir(_nameDir);
    _fileOutMu->cd(_nameDir);
    _outputTreeMu = new TTree(_nameTree,_nameTree);
    _TREE.InitOutputTree(_outputTreeMu);
      //method of TInputOutputTree
    _hskimMu = new TH1F("hskim","hskim",2,0,2);
  }

  if (channel == _config.ELECTRON || channel == _config.BOTH){
    _skimmedFileNameEle=outDir+inpTemp+"_ElePhoSkim.root"; 
    _fileOutEle = new TFile(_skimmedFileNameEle,"recreate");
    _fileOutEle->mkdir(_nameDir);
    _fileOutEle->cd(_nameDir);
    _outputTreeEle = new TTree(_nameTree,_nameTree);
    _TREE.InitOutputTree(_outputTreeEle);
      //method of TInputOutputTree
    _hskimEle = new TH1F("hskim","hskim",2,0,2);
  }

}

SkimLeptonPhoton::~SkimLeptonPhoton()
{
  _TREE.fChain = 0;
  if (_channel==_config.MUON || _channel==_config.BOTH) _fileOutMu->Close();
  if (_channel==_config.ELECTRON || _channel==_config.BOTH) _fileOutEle->Close();
}

void SkimLeptonPhoton::LoopOverInputTree()
{
  TFile f(_inputFileName,"READ");
  f.cd(_nameDir);
  TTree* tree =(TTree*)gDirectory->Get(_nameTree);
  bool hashEvents = gDirectory->GetListOfKeys()->Contains("hEvents");
  bool hashPU = gDirectory->GetListOfKeys()->Contains("hPU");
  bool hashPUTrue = gDirectory->GetListOfKeys()->Contains("hPUTrue");
  bool hashskim = gDirectory->GetListOfKeys()->Contains("hskim");  

  _TREE.Init(tree);
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

  if (_TREE.fChain == 0) return;
        //field of TInputTree

  Long64_t nentries = _TREE.fChain->GetEntries();
 // nentries = 100;
  int nMuPassed=0;
  int nElePassed=0;

  for (Long64_t entry=0; entry<nentries; entry++) 
    {
      if (entry < 0) break;

      _TREE.GetEntryNeededBranchesOnly(_channel,_sample,entry);
      
      if (_TREE.treeLeaf.nPho<1) continue;
      bool phoExists=0;
      for (int ipho=0; ipho<_TREE.treeLeaf.nPho; ipho++){
        if (_TREE.treeLeaf.phoEt->at(ipho)>10) phoExists=1;
      }
      if (!phoExists) continue;
      if (_TREE.treeLeaf.nMu>0 && (_channel==_config.MUON || _channel==_config.BOTH))
        {
          nMuPassed++; 
          _outputTreeMu->Fill();
        }
      if (_TREE.treeLeaf.nEle>0 && (_channel==_config.ELECTRON || _channel==_config.BOTH))
        {
          nElePassed++;
          _outputTreeEle->Fill();
        } 
  
    }//loop over entries ends

  //writing output trees to files
  if (_channel==_config.MUON || _channel==_config.BOTH){
    _fileOutMu->cd();
    _fileOutMu->cd(_nameDir);
    _outputTreeMu->Write(_nameTree);
    if (hashEvents)
      hEvents->Write();
    if (hashPU)
      hPU->Write();
    if (hashPUTrue)
      hPUTrue->Write();
    if (hashskim)
      _hskimMu->SetBinContent(1,hskim->GetBinContent(1));
    else 
      _hskimMu->SetBinContent(1,tree->GetEntries());
    _hskimMu->SetBinContent(2,nMuPassed); 
    _hskimMu->Write();
  }

  if (_channel==_config.ELECTRON || _channel==_config.BOTH){
    _fileOutEle->cd();
    _fileOutEle->cd(_nameDir);
    _outputTreeEle->Write(_nameTree);
    //if (hashEvents)
    //  hEvents->Write();
    if (hashPU)
      hPU->Write();
    //if (hashPUTrue)
    //  hPUTrue->Write();
    if (hashskim)
      _hskimEle->SetBinContent(1,hskim->GetBinContent(1));
    else 
      _hskimEle->SetBinContent(1,tree->GetEntries());
    _hskimEle->SetBinContent(2,nElePassed); 
    _hskimEle->Write();
  }
}//Skim::LoopOverInputTree() ends
