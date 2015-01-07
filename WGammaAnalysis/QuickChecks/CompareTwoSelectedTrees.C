#include "TFile.h"
#include "TTree.h"
#include "TBranch.h" 
#include <iostream>

void CompareTwoSelectedTrees()
{
  TFile* f1 = new TFile("../../../usercode_2014_11_24_PresentationOnSidebandVariation/WGammaAnalysis/WGammaOutput/MUON/FullySelected/selected_WGamma_blindPRESCALE_DATA.root");

  TFile* f2 = new TFile("../WGammaOutput/MUON_WGamma/FullySelected/selected_WGamma_blindPRESCALE_DATA.root");

  TTree* tr1 = (TTree*)f1->Get("selectedEvents");

  TTree* tr2 = (TTree*)f2->Get("selectedEvents");

  int ev1, ev2, ipho1, ipho2, ilep11, ilep12;
  float phoPFPhoIso1, phoPFPhoIso2;
  TBranch *b_ev1, *b_ev2, *b_ipho1, *b_ipho2, *b_ilep11, *b_ilep12, *b_phoPFPhoIso1, *b_phoPFPhoIso2;

  tr1->SetBranchAddress("event", &ev1, &b_ev1);
  tr1->SetBranchAddress("ipho", &ipho1, &b_ipho1);
  tr1->SetBranchAddress("ilep1", &ilep11, &b_ilep11);
  tr1->SetBranchAddress("phoPFPhoIsoCorr", &phoPFPhoIso1, &b_phoPFPhoIso1);
  tr2->SetBranchAddress("event", &ev2, &b_ev2);
  tr2->SetBranchAddress("ipho", &ipho2, &b_ipho2);
  tr2->SetBranchAddress("ilep1", &ilep12, &b_ilep12);
  tr2->SetBranchAddress("phoPFPhoIsoCorr", &phoPFPhoIso2, &b_phoPFPhoIso2);

  long nentries1 = tr1->GetEntries();
  long nentries2 = tr2->GetEntries();

  std::cout<<"Found in tree 1 only:"<<std::endl;
  std::cout<<f1->GetName()<<std::endl;
  for (long ient1=0; ient1<nentries1; ient1++){
    b_ev1->GetEntry(ient1);
    b_ipho1->GetEntry(ient1);
    b_ilep11->GetEntry(ient1);
    b_phoPFPhoIso1->GetEntry(ient1);
    bool evMatched=0;
    for (long ient2=0; ient2<nentries2; ient2++){
      b_ev2->GetEntry(ient2);
      b_ipho2->GetEntry(ient2);
      b_ilep12->GetEntry(ient2);
      b_phoPFPhoIso2->GetEntry(ient2);
      if (ev1==ev2 && ipho1==ipho2 && ilep11==ilep12) {
        evMatched=1;break; 
      }
    }//end of loop over ient2
    if (evMatched==0){
      std::cout<<"ev="<<ev1<<", ipho="<<ipho1<<", ilep1="<<ilep11<<std::endl;
    }
  }//end of loop over ient1
}
