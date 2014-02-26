#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include <iostream>
#include <vector>

void CheckGenLevelProcess()
{

  TFile f("/mnt/hadoop/user/uscms01/pnfs/unl.edu/data4/cms/store/user/eavdeeva2/WGammaMCsummer12New/job_summer12_Wg_munu.root");
  f.cd("ggNtuplizer");
  TTree* tr = (TTree*)gDirectory->Get("EventTree");

  Int_t           nMC;
  vector <int>    *mcPID; 
  mcPID=0;
  vector <int>    *mcMomPID; 
  mcMomPID=0;
  vector <int>    *mcDecayType; 
  mcDecayType=0;

  TBranch        *b_nMC;   //!
  TBranch        *b_mcPID;   //!
  TBranch        *b_mcMomPID;   //!
  TBranch        *b_mcDecayType;   //!


  tr->SetBranchAddress("nMC", &nMC, &b_nMC);
  tr->SetBranchAddress("mcPID", &mcPID, &b_mcPID);
  tr->SetBranchAddress("mcMomPID", &mcMomPID, &b_mcMomPID);
  tr->SetBranchAddress("mcDecayType", &mcDecayType, &b_mcDecayType);

  int nEvents = 0;
  int nWPlus = 0;
  int nWMinus = 0;
  int nPhotons = 0;
  int nMuPlus = 0;
  int nMuMinus = 0;
  int muonDecayType = 0;

  int nWMax=10;
  int WsPlusMinus[nWMax+1][nWMax+1];
    //WsPlusMinus[0][0] - number of events with 0 W+ and 0 W- 
  int nMuDecayTypes[nWMax+1];
  for (int i=0; i<=nWMax; i++){
    nMuDecayTypes[i]=0;
    for (int j=0; j<=nWMax; j++)
      WsPlusMinus[i][j]=0;
  }

  long nEntries = tr->GetEntries();

  for (long entry=0; entry<nEntries; entry++)
    {

      b_nMC->GetEntry(entry); 
      b_mcPID->GetEntry(entry);
      b_mcMomPID->GetEntry(entry);
      b_mcDecayType->GetEntry(entry);

      nEvents++;

      if (nMC==0){
        std::cout<<"no particles were generated in the event "<<entry<<std::endl; 
        continue;
      }

      int WPlusesHere=0;
      int WMinusesHere=0;
      int muonDecaysHere=0;
      for (int iMC=0; iMC<nMC; iMC++)
        {

          if (mcPID->at(iMC)==24) { nWPlus++; WPlusesHere++;}
          if (mcPID->at(iMC)==-24){ nWMinus++; WMinusesHere++;}
          if ((mcPID->at(iMC)==24)||(mcPID->at(iMC)==-24))
            if (mcDecayType->at(iMC)==3) 
              {muonDecayType++; muonDecaysHere++;}

          if ((mcMomPID->at(iMC)==24) || (mcMomPID->at(iMC)==-24)){
            if (mcPID->at(iMC)==22) nPhotons++;
            if (mcPID->at(iMC)==13) nMuMinus++;
            if (mcPID->at(iMC)==-13)nMuPlus++;
          }
   
        }//loop over iMC
      if (WPlusesHere>nWMax || WMinusesHere>nWMax){
        std::cout<<"in event "<<entry<<" there are "<<WPlusesHere<<" W+s and "<<WMinusesHere<<" W-s "<<std::endl;
      }
      else WsPlusMinus[WPlusesHere][WMinusesHere]++;
      if (muonDecaysHere>nWMax);
      else nMuDecayTypes[muonDecaysHere]++;

    } //loop over entry  

  std::cout<<"nEvents="<<nEvents<<", nWPlus="<<nWPlus<<", nWMinus="<<nWMinus<<", nMuPlus="<<nMuPlus<<", nMuMinus="<<nMuMinus<<",  nPhotons="<<nPhotons<<", muonDecays="<<muonDecayType<<std::endl;

  for (int i=0; i<=nWMax; i++)
    for (int j=0; j<=nWMax; j++){
      if (WsPlusMinus[i][j]!=0)
        std::cout<<"# of events with "<<i<<" W+s and "<<j<<" W-s:"<<WsPlusMinus[i][j]<<std::endl;
    }
  for (int i=0; i<=nWMax; i++){
    if (nMuDecayTypes[i]!=0)
        std::cout<<"# of events with "<<i<<" mu decay type = "<<nMuDecayTypes[i]<<std::endl;
  }

}
