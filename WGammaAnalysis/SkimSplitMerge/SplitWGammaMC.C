#include "SplitWGammaMC.h"
#include "../Configuration/TConfiguration.h"
#include <TString.h>
#include <TTree.h>
#include <TH1F.h>
#include <iostream>

SplitWGammaMC::SplitWGammaMC(TString nameWGammaInput, TString nameWGammaEle, TString nameWGammaMuo, TString nameWGammaTau, TString nameDir, TString nameTree)
{

  nameDir_=nameDir;
  nameTree_=nameTree;
  nameWGammaInput_=nameWGammaInput;
  nameWGammaEle_=nameWGammaEle;
  nameWGammaMuo_=nameWGammaMuo;
  nameWGammaTau_=nameWGammaTau;

  fileOutEle_ = new TFile(nameWGammaEle_,"recreate");
  fileOutEle_->mkdir(nameDir_);
  fileOutEle_->cd(nameDir_);
  outputTreeEle_ = new TTree(nameTree_,nameTree_);
  TREE_.InitOutputTree(outputTreeEle_);

  fileOutMuo_ = new TFile(nameWGammaMuo_,"recreate");
  fileOutMuo_->mkdir(nameDir_);
  fileOutMuo_->cd(nameDir_);
  outputTreeMuo_ = new TTree(nameTree_,nameTree_);
  TREE_.InitOutputTree(outputTreeMuo_);

  fileOutTau_ = new TFile(nameWGammaTau_,"recreate");
  fileOutTau_->mkdir(nameDir_);
  fileOutTau_->cd(nameDir_);
  outputTreeTau_ = new TTree(nameTree_,nameTree_);
  TREE_.InitOutputTree(outputTreeTau_);

}

SplitWGammaMC::~SplitWGammaMC()
{
  TREE_.fChain = 0;
}

void SplitWGammaMC::LoopOverInputTree()
{

  TFile f(nameWGammaInput_,"READ");
  f.cd(nameDir_);
  TTree* tree =(TTree*)gDirectory->Get(nameTree_);
  TREE_.Init(tree);
    //method of TInputTree
  TH1F* hEvents = (TH1F*)gDirectory->Get("hEvents");
  TH1F* hPU = (TH1F*)gDirectory->Get("hPU");
  TH1F* hPUTrue = (TH1F*)gDirectory->Get("hPUTrue");

  if (TREE_.fChain == 0) return;
        //field of TInputTree

  Long64_t nentries = TREE_.fChain->GetEntries();
//  nentries = 100;
  TConfiguration config;
  for (Long64_t entry=0; entry<nentries; entry++) 
    {
      if (entry < 0) break;

     TREE_.GetEntryNeededBranchesOnly(config.BOTH,config.SIGMC,entry);

      bool hasW=0;
      for (int iMC=0; iMC<TREE_.treeLeaf.nMC; iMC++)
        {
          if (TREE_.treeLeaf.mcPID->at(iMC)==24 || TREE_.treeLeaf.mcPID->at(iMC)==-24)
            {
                if (TREE_.treeLeaf.mcDecayType->at(iMC)==ID_ELECTRON) 
                  outputTreeEle_->Fill();
                if (TREE_.treeLeaf.mcDecayType->at(iMC)==ID_MUON) 
                  outputTreeMuo_->Fill();
                if (TREE_.treeLeaf.mcDecayType->at(iMC)==ID_TAU) 
                  outputTreeTau_->Fill();

              hasW=1;

            }

        }
   
    }//loop over entries ends

  //writing output trees to files

  fileOutEle_->cd();
  fileOutEle_->cd(nameDir_);
  outputTreeEle_->Write(nameTree_);
  hEvents->Write();
  hPU->Write();
  hPUTrue->Write();

  fileOutMuo_->cd();
  fileOutMuo_->cd(nameDir_);
  outputTreeMuo_->Write(nameTree_);
  hEvents->Write();
  hPU->Write();
  hPUTrue->Write();

  fileOutTau_->cd();
  fileOutTau_->cd(nameDir_);
  outputTreeTau_->Write(nameTree_);
  hEvents->Write();
  hPU->Write();
  hPUTrue->Write();


}//SplitWGammaMC::LoopOverInputTree() ends
