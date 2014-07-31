#include "SplitWGammaMC.h"
#include "../Configuration/TConfiguration.h"
#include <TString.h>
#include <TTree.h>
#include <TH1F.h>
#include <iostream>

SplitWGammaMC::SplitWGammaMC(TString nameWGammaInput, TString nameWGammaEle, TString nameWGammaMuo, TString nameWGammaTau, TString nameDir, TString nameTree, bool isDebugMode)
{

  _isDebugMode=isDebugMode;
  _nameDir=nameDir;
  _nameTree=nameTree;
  _nameWGammaInput=nameWGammaInput;
  _nameWGammaEle=nameWGammaEle;
  _nameWGammaMuo=nameWGammaMuo;
  _nameWGammaTau=nameWGammaTau;

  _fileOutEle = new TFile(_nameWGammaEle,"recreate");
  _fileOutEle->mkdir(_nameDir);
  _fileOutEle->cd(_nameDir);
  _outputTreeEle = new TTree(_nameTree,_nameTree);
  _TREE.InitOutputTree(_outputTreeEle);

  _fileOutMuo = new TFile(_nameWGammaMuo,"recreate");
  _fileOutMuo->mkdir(_nameDir);
  _fileOutMuo->cd(_nameDir);
  _outputTreeMuo = new TTree(_nameTree,_nameTree);
  _TREE.InitOutputTree(_outputTreeMuo);

  _fileOutTau = new TFile(_nameWGammaTau,"recreate");
  _fileOutTau->mkdir(_nameDir);
  _fileOutTau->cd(_nameDir);
  _outputTreeTau = new TTree(_nameTree,_nameTree);
  _TREE.InitOutputTree(_outputTreeTau);

}

SplitWGammaMC::~SplitWGammaMC()
{
  _TREE.fChain = 0;
}

void SplitWGammaMC::LoopOverInputTree()
{

  TFile f(_nameWGammaInput,"READ");
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

      bool hasW=0;
      for (int iMC=0; iMC<_TREE.treeLeaf.nMC; iMC++)
        {
          if (_TREE.treeLeaf.mcPID->at(iMC)==24 || _TREE.treeLeaf.mcPID->at(iMC)==-24)
            {
                if (_TREE.treeLeaf.mcDecayType->at(iMC)==ID_ELECTRON) 
                  _outputTreeEle->Fill();
                if (_TREE.treeLeaf.mcDecayType->at(iMC)==ID_MUON) 
                  _outputTreeMuo->Fill();
                if (_TREE.treeLeaf.mcDecayType->at(iMC)==ID_TAU) 
                  _outputTreeTau->Fill();

              hasW=1;

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


}//SplitWGammaMC::LoopOverInputTree() ends
