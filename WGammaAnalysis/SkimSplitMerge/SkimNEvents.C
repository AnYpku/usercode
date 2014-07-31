#include "SkimNEvents.h"
#include <TString.h>
#include <TTree.h>
#include <TH1F.h>
#include <iostream>

SkimNEvents::SkimNEvents(long N, int channel, int sample, TString inputFileName, TString nameDir, TString nameTree, TString outDir,bool isDebugMode)
{

  _sample=sample;
  _channel=channel;  
  _isDebugMode=isDebugMode;

  _nameDir=nameDir;
  _nameTree=nameTree;

  _inputFileName=inputFileName;

  TString inpTemp = _inputFileName;
  while (inpTemp.Contains('/')){
    inpTemp.Remove(0,1);
  }
  inpTemp.ReplaceAll(".root","");

  _N=N;
  _skimmedFileName=outDir+inpTemp;
  _skimmedFileName+="_";
  _skimmedFileName+=N;
  _skimmedFileName+="Skim.root";
  _fileOut = new TFile(_skimmedFileName,"recreate");
  _fileOut->mkdir(_nameDir);
  _fileOut->cd(_nameDir);
  _outputTree = new TTree(_nameTree,_nameTree);
  _TREE.InitOutputTree(_outputTree);
    //method of TInputOutputTree
  _hskim = new TH1F("hskim","hskim",2,0,2);
  

}

SkimNEvents::~SkimNEvents()
{
  _TREE.fChain = 0;
  _fileOut->Close();
}

void SkimNEvents::LoopOverInputTree()
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

  int nentries = _TREE.fChain->GetEntries();
  if (_N<nentries) nentries = _N;

  for (int entry=0; entry<nentries; entry++) 
    {
      if (entry < 0) break;
      _TREE.GetEntryNeededBranchesOnly(_channel,_sample,entry);   
      _outputTree->Fill();
    }//loop over entries ends

  //writing output trees to files
  _fileOut->cd();
  _fileOut->cd(_nameDir);
  _outputTree->Write(_nameTree);
  if (hashEvents)
    hEvents->Write();
  if (hashPU)
    hPU->Write();
  if (hashPUTrue)
    hPUTrue->Write();
  if (hashskim)
    _hskim->Write();
}//Skim::LoopOverInputTree() ends
