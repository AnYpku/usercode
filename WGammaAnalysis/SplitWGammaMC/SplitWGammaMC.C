#include "SplitWGammaMC.h"
#include <TString.h>
#include <TTree.h>
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
      InitOutputTree(outputTree_[i]);

    }
}

SplitWGammaMC::~SplitWGammaMC()
{
  fChain = 0;
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
  Init(tree);

  if (fChain == 0) return;

  Long64_t nentries = fChain->GetEntries();
  //nentries = 10;
  for (Long64_t entry=0; entry<nentries; entry++) {
    if (entry < 0) break;

    fChain->GetEntry(entry);
    int splitterID[nMC];
    for (int iMC=0; iMC<nMC; iMC++)
      {
        if (mcPID[iMC]!=24) splitterID[iMC]=-1;
        else if (mcDecayType[iMC]==1) splitterID[iMC]=1;//Wgg
        else if (mcDecayType[iMC]==2) splitterID[iMC]=2;//Wg_enu
        else if (mcDecayType[iMC]==3) splitterID[iMC]=3;//Wg_munu
        else if (mcDecayType[iMC]==4) splitterID[iMC]=4;//Wg_taunu
        else splitterID[iMC]=-1;
      }

    int nMCTemp[numberOfTrees];
    float mcPIDTemp[numberOfTrees][nMC];   //[nMC]
    float mcVtxTemp[numberOfTrees][nMC][3];   //[nMC]
    float mcPtTemp[numberOfTrees][nMC];   //[nMC]
    float mcMassTemp[numberOfTrees][nMC];   //[nMC]
    float mcEtaTemp[numberOfTrees][nMC];   //[nMC]
    float mcPhiTemp[numberOfTrees][nMC];   //[nMC]
    float mcETemp[numberOfTrees][nMC];   //[nMC]
    float mcEtTemp[numberOfTrees][nMC];   //[nMC]
    int mcGMomPIDTemp[numberOfTrees][nMC];   //[nMC]
    int mcMomPIDTemp[numberOfTrees][nMC];   //[nMC]
    float mcMomPtTemp[numberOfTrees][nMC];   //[nMC]
    float mcMomMassTemp[numberOfTrees][nMC];   //[nMC]
    float mcMomEtaTemp[numberOfTrees][nMC];   //[nMC]
    float mcMomPhiTemp[numberOfTrees][nMC];   //[nMC]
    int mcIndexTemp[numberOfTrees][nMC];   //[nMC]
    int mcDecayTypeTemp[numberOfTrees][nMC];   //[nMC]

    for (int id=1; id<numberOfTrees; id++)
      {
        nMCTemp[id]=0;
        int iMCTemp=0;
        for (int iMC=0; iMC<nMC; iMC++)
          {
            if (splitterID[iMC]==id)
              {
                nMCTemp[id]++;
                mcPIDTemp[id][iMCTemp]=mcPID[iMC];
                for (int j=0; j<3; j++)
                  mcVtxTemp[id][iMCTemp][j]=mcVtx[iMC][j];   
                mcPtTemp[id][iMCTemp]=mcPt[iMC];   
                mcMassTemp[id][iMCTemp]=mcMass[iMC];   
                mcEtaTemp[id][iMCTemp]=mcEta[iMC];   
                mcPhiTemp[id][iMCTemp]=mcPhi[iMC];   
                mcETemp[id][iMCTemp]=mcE[iMC];   
                mcEtTemp[id][iMCTemp]=mcEt[iMC];   
                mcGMomPIDTemp[id][iMCTemp]=mcGMomPID[iMC];   
                mcMomPIDTemp[id][iMCTemp]=mcMomPID[iMC];   
                mcMomPtTemp[id][iMCTemp]=mcMomPt[iMC];   
                mcMomMassTemp[id][iMCTemp]=mcMomMass[iMC];   
                mcMomEtaTemp[id][iMCTemp]=mcMomEta[iMC];   
                mcMomPhiTemp[id][iMCTemp]=mcMomPhi[iMC];   
                mcIndexTemp[id][iMCTemp]=mcIndex[iMC];   
                mcDecayTypeTemp[id][iMCTemp]=mcDecayType[iMC];
                iMCTemp++;
              }
          }//loop over iMC ends
      }//loop over id ends

    for (int id=1; id<numberOfTrees; id++)
      {
        nMC=nMCTemp[id];
        for (int iMC=0; iMC<nMC; iMC++)
          {
             mcPID[iMC]=mcPIDTemp[id][iMC];
             for (int j=0; j<3; j++)
               mcVtx[iMC][j]=mcVtxTemp[id][iMC][j];   
             mcPt[iMC]=mcPtTemp[id][iMC];   
             mcMass[iMC]=mcMassTemp[id][iMC];   
             mcEta[iMC]=mcEtaTemp[id][iMC];   
             mcPhi[iMC]=mcPhiTemp[id][iMC];   
             mcE[iMC]=mcETemp[id][iMC];   
             mcEt[iMC]=mcEtTemp[id][iMC];   
             mcGMomPID[iMC]=mcGMomPIDTemp[id][iMC];   
             mcMomPID[iMC]=mcMomPIDTemp[id][iMC];   
             mcMomPt[iMC]=mcMomPtTemp[id][iMC];   
             mcMomMass[iMC]=mcMomMassTemp[id][iMC];   
             mcMomEta[iMC]=mcMomEtaTemp[id][iMC];   
             mcMomPhi[iMC]=mcMomPhiTemp[id][iMC];   
             mcIndex[iMC]=mcIndexTemp[id][iMC];   
             mcDecayType[iMC]=mcDecayTypeTemp[id][iMC];
          }//loop over iMC ends
        if (nMC>0) 
          {
             outputTree_[id]->Fill();
          }
      }//loop over id ends
    
  }//loop over entries ends

  //writing output trees to files
  for (int id=1; id<numberOfTrees; id++)
    {
      fileOut_[id]->cd();
      fileOut_[id]->cd(nameDir_);
      outputTree_[id]->Write(nameTree_);
    }

}//SplitWGammaMC::LoopOverInputTree() ends
