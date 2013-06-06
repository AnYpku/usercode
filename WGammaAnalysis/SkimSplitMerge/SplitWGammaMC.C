#include "SplitWGammaMC.h"
#include <TString.h>
#include <TTree.h>
#include <TH1F.h>
#include <iostream>

SplitWGammaMC::SplitWGammaMC(TString* nameWGammaSample, TString nameDir, TString nameTree)
{
  nameDir_=nameDir;
  nameTree_=nameTree;

  nameWGammaSample_[0]=nameWGammaSample[0];

  for (int i=1; i<numberOfTrees; i++)
    {

      nameWGammaSample_[i]=nameWGammaSample[i];
      fileOut_[i] = new TFile(nameWGammaSample_[i],"recreate");
      fileOut_[i]->mkdir(nameDir_);
      fileOut_[i]->cd(nameDir_);
      outputTree_[i] = new TTree(nameTree_,nameTree_);
      TREE_.InitOutputTree(outputTree_[i]);
        //method of TOutputTree

    }
}

SplitWGammaMC::~SplitWGammaMC()
{
  TREE_.fChain = 0;
  for (int i=1; i<numberOfTrees; i++)
    {
      //delete outputTree_[i];
      //outputTree_[i]=0;
      //delete fileOut_[i];
      //fileOut_[i]=0;
    }
}

void SplitWGammaMC::LoopOverInputTree()
{
  TFile f(nameWGammaSample_[0],"READ");
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
  //nentries = 10;
  for (Long64_t entry=0; entry<nentries; entry++) 
    {
      if (entry < 0) break;

      TREE_.fChain->GetEntry(entry);
      bool hasW=0;
      for (int iMC=0; iMC<TREE_.treeLeaf.nMC; iMC++)
        {
          if (TREE_.treeLeaf.mcPID[iMC]==24 || TREE_.treeLeaf.mcPID[iMC]==-24)
            {
              for (int id=2; id<numberOfTrees; id++)
                if (TREE_.treeLeaf.mcDecayType[iMC]==id) outputTree_[id]->Fill();
              hasW=1;
            }
        }
      if (!hasW) outputTree_[1]->Fill();
    
    }//loop over entries ends

  //writing output trees to files
  for (int id=1; id<numberOfTrees; id++)
    {
      fileOut_[id]->cd();
      fileOut_[id]->cd(nameDir_);
      outputTree_[id]->Write(nameTree_);
      hEvents->Write();
      hPU->Write();
      hPUTrue->Write();
    }

}//SplitWGammaMC::LoopOverInputTree() ends
