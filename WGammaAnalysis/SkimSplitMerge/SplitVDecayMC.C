#include "SplitVDecayMC.h"
#include "../Configuration/TConfiguration.h"
#include <TString.h>
#include <TTree.h>
#include <TH1F.h>
#include <iostream>

SplitVDecayMC::SplitVDecayMC(TString nameInput, TString nameEle, TString nameMuo, TString nameTau, TString nameDir, TString nameTree, bool isDebugMode)
{

  _isDebugMode=isDebugMode;
  _nameDir=nameDir;
  _nameTree=nameTree;
  _nameInput=nameInput;
  _nameEle=nameEle;
  _nameMuo=nameMuo;
  _nameTau=nameTau;

  _fileOutEle = new TFile(_nameEle,"recreate");
  _fileOutEle->mkdir(_nameDir);
  _fileOutEle->cd(_nameDir);
  _outputTreeEle = new TTree(_nameTree,_nameTree);
  _TREE.InitOutputTree(_outputTreeEle);

  _fileOutMuo = new TFile(_nameMuo,"recreate");
  _fileOutMuo->mkdir(_nameDir);
  _fileOutMuo->cd(_nameDir);
  _outputTreeMuo = new TTree(_nameTree,_nameTree);
  _TREE.InitOutputTree(_outputTreeMuo);

  _fileOutTau = new TFile(_nameTau,"recreate");
  _fileOutTau->mkdir(_nameDir);
  _fileOutTau->cd(_nameDir);
  _outputTreeTau = new TTree(_nameTree,_nameTree);
  _TREE.InitOutputTree(_outputTreeTau);

}

SplitVDecayMC::~SplitVDecayMC()
{
  _TREE.fChain = 0;
}

void SplitVDecayMC::LoopOverInputTree()
{

  TFile f(_nameInput,"READ");
  f.cd(_nameDir);
  TTree* tree =(TTree*)gDirectory->Get(_nameTree);
  _TREE.Init(tree);
    //method of TInputTree
  TH1F* hEvents = (TH1F*)gDirectory->Get("hEvents");
  TH1F* hPU = (TH1F*)gDirectory->Get("hPU");
  TH1F* hPUTrue = (TH1F*)gDirectory->Get("hPUTrue");

  if (_TREE.fChain == 0) return;
        //field of TInputTree

  Long64_t nentries = _TREE.fChain->GetEntries();
  if (_isDebugMode) nentries = 100;
  TConfiguration config;
  for (Long64_t entry=0; entry<nentries; entry++) 
    {
      if (entry < 0) break;

     _TREE.GetEntryNeededBranchesOnly(config.BOTH,config.SIGMC,entry);

      bool hasWorZ=0;
      for (int iMC=0; iMC<_TREE.treeLeaf.nMC; iMC++)
        {
          if (_TREE.treeLeaf.mcPID->at(iMC)==24 ||   //W PDGID
              _TREE.treeLeaf.mcPID->at(iMC)==-24 ||  //W PDGID
              _TREE.treeLeaf.mcPID->at(iMC)==23)     //Z PDGID
            {
                if (_TREE.treeLeaf.mcDecayType->at(iMC)==ID_ELECTRON) 
                  _outputTreeEle->Fill();
                if (_TREE.treeLeaf.mcDecayType->at(iMC)==ID_MUON) 
                  _outputTreeMuo->Fill();
                if (_TREE.treeLeaf.mcDecayType->at(iMC)==ID_TAU) 
                  _outputTreeTau->Fill();

              hasWorZ=1;

            }

        }
   
    }//loop over entries ends

  //writing output trees to files

  _fileOutEle->cd();
  _fileOutEle->cd(_nameDir);
  _outputTreeEle->Write(_nameTree);
  hEvents->Write();
  hPU->Write();
  hPUTrue->Write();

  _fileOutMuo->cd();
  _fileOutMuo->cd(_nameDir);
  _outputTreeMuo->Write(_nameTree);
  hEvents->Write();
  hPU->Write();
  hPUTrue->Write();

  _fileOutTau->cd();
  _fileOutTau->cd(_nameDir);
  _outputTreeTau->Write(_nameTree);
  hEvents->Write();
  hPU->Write();
  hPUTrue->Write();


}//SplitVDecayMC::LoopOverInputTree() ends
