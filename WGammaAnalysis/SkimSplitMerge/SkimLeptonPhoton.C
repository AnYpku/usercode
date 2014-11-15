#include "SkimLeptonPhoton.h"
#include <TString.h>
#include <TTree.h>
#include <TH1F.h>
#include <iostream>

SkimLeptonPhoton::SkimLeptonPhoton(int channel, int vgamma, int sample, TString inputFileName, bool isDebugMode, TString outDir, TString nameDir, TString nameTree)
{

  _sample=sample;
  _channel=channel; 
  _vgamma=vgamma; 

  _isDebugMode=isDebugMode;

  _nameDir=nameDir;
  _nameTree=nameTree;

  _inputFileName=inputFileName;

  TString inpTemp = _inputFileName;
  while (inpTemp.Contains('/')){
    inpTemp.Remove(0,1);
  }
  inpTemp.ReplaceAll(".root","");

  TString skimPartOfName[2][2];
  skimPartOfName[_config.MUON][_config.W_GAMMA]="_MuPhoSkim";// muon + photon
  skimPartOfName[_config.MUON][_config.Z_GAMMA]="_MuMuPhoSkim";// two muons + photon
  skimPartOfName[_config.ELECTRON][_config.W_GAMMA]="_ElePhoSkim";// electron + photon
  skimPartOfName[_config.ELECTRON][_config.Z_GAMMA]="_EleElePhoSkim";// two electrons + photon

  for (int ich=_config.MUON; ich<=_config.ELECTRON; ich++){
    if (ich==_channel || _channel==_config.BOTH_CHANNELS);
    else continue;
    for (int ivg=_config.W_GAMMA; ivg<=_config.Z_GAMMA; ivg++){
      if (ivg==_vgamma || _vgamma==_config.V_GAMMA);
      else continue;
      _skimmedFileName[ich][ivg]=outDir+inpTemp+skimPartOfName[ich][ivg]+".root";
      _fileOut[ich][ivg] = new TFile(_skimmedFileName[ich][ivg],"recreate");
      _fileOut[ich][ivg]->mkdir(_nameDir);
      _fileOut[ich][ivg]->cd(_nameDir);
      _outputTree[ich][ivg] = new TTree(_nameTree,_nameTree);
      _TREE.InitOutputTree(_outputTree[ich][ivg]);
        //method of TInputOutputTree
      _hskim[ich][ivg] = new TH1F("hskim","hskim",2,0,2);      
    }//end of loop over ivg
  }//end of loop over ich
}

SkimLeptonPhoton::~SkimLeptonPhoton()
{

}

void SkimLeptonPhoton::LoopOverInputTree()
{
  TFile f(_inputFileName,"READ");
  std::cout<<"processing "<<_inputFileName<<std::endl;
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
  if (_isDebugMode) nentries=100;
  std::cout<<"nentries "<<nentries<<std::endl;

  for (Long64_t entry=0; entry<nentries; entry++) {

    if (entry < 0) break;
    if ((entry%1000000)==0) std::cout<<"entry="<<entry<<std::endl;
    _TREE.GetEntryNeededBranchesOnly(_channel,_sample,entry);
  
    if (_TREE.treeLeaf.nPho<1) continue;
    if (_TREE.treeLeaf.phoEt->at(0)<10) continue;


    if (_channel==_config.MUON || _channel==_config.BOTH_CHANNELS){
      if (_vgamma==_config.W_GAMMA || _vgamma==_config.V_GAMMA){
        if (_TREE.treeLeaf.nMu>=1)
          if (_TREE.treeLeaf.muPt->at(0)>20)
            _outputTree[_config.MUON][_config.W_GAMMA]->Fill();
      }// if MUON, W_GAMMA
      if (_vgamma==_config.Z_GAMMA || _vgamma==_config.V_GAMMA){
        if (_TREE.treeLeaf.nMu>=2)
          if (_TREE.treeLeaf.muPt->at(0)>20)
            _outputTree[_config.MUON][_config.Z_GAMMA]->Fill();
      }// if MUON, Z_GAMMA
    }// if MUON or BOTH
    if (_channel==_config.ELECTRON || _channel==_config.BOTH_CHANNELS){
      if (_vgamma==_config.W_GAMMA || _vgamma==_config.V_GAMMA){
        if (_TREE.treeLeaf.nEle>=1)
          if (_TREE.treeLeaf.elePt->at(0)>20)
            _outputTree[_config.ELECTRON][_config.W_GAMMA]->Fill();
      }// if ELECTRON, W_GAMMA
      if (_vgamma==_config.Z_GAMMA || _vgamma==_config.V_GAMMA){
        if (_TREE.treeLeaf.nEle>=2)
          if (_TREE.treeLeaf.elePt->at(0)>20)
            _outputTree[_config.ELECTRON][_config.Z_GAMMA]->Fill();
      }// if ELECTRON, Z_GAMMA
    }// if ELECTRON or BOTH 

  }//loop over entries ends

  //writing output trees to files
  for (int ich=_config.MUON; ich<=_config.ELECTRON; ich++){
    if (ich==_channel || _channel==_config.BOTH_CHANNELS);
    else continue;
    for (int ivg=_config.W_GAMMA; ivg<=_config.Z_GAMMA; ivg++){
      if (ivg==_vgamma || _vgamma==_config.V_GAMMA);
      else continue;
    _fileOut[ich][ivg]->cd();
    _fileOut[ich][ivg]->cd(_nameDir);
    _outputTree[ich][ivg]->Write(_nameTree);
    if (hashEvents)
      hEvents->Write();
    if (hashPU)
      hPU->Write();
    if (hashPUTrue)
      hPUTrue->Write();
    if (hashskim)
      _hskim[ich][ivg]->SetBinContent(1,hskim->GetBinContent(1));
    else 
      _hskim[ich][ivg]->SetBinContent(1,tree->GetEntries());

    }//end of loop over ivg
  }//end of loop over ich

  //close output files
  _TREE.fChain = 0;
  for (int ich=_config.MUON; ich<=_config.ELECTRON; ich++){
    if (ich==_channel || _channel==_config.BOTH_CHANNELS);
    else continue;
    for (int ivg=_config.W_GAMMA; ivg<=_config.Z_GAMMA; ivg++){
      if (ivg==_vgamma || _vgamma==_config.V_GAMMA);
      else continue;
      _fileOut[ich][ivg]->Close();
      std::cout<<"file "<<_fileOut[ich][ivg]->GetName()<<" closed"<<std::endl;
    }
  }

}//Skim::LoopOverInputTree() ends
