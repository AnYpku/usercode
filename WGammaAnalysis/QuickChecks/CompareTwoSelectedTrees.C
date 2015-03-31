#include "TFile.h"
#include "TTree.h"
#include "TBranch.h" 
#include <iostream>

void CompareTwoSelectedTrees()
{
  TFile* f1 = new TFile("../WGammaOutput/ELECTRON_WGamma/FullySelected/selected_WGammaSIGMC.root");

//  TFile* f2 = new TFile("../WGammaOutput/Lovedeep/OnlyWenuUsingMCDecayType_miniTree_job_summer12_Wg_skimmed_wg_vetoEIDonly.root");

  TTree* tr1 = (TTree*)f1->Get("selectedEvents");

//  TTree* tr2 = (TTree*)f2->Get("miniTree");
  TFile* f2 = new TFile("fOut.root","recreate");
  TTree* tr2 = new TTree("tr2","tr2");
  tr2->ReadFile("../WGammaOutput/Lovedeep/OnlyWenu.txt","run2/L:event2/L:WMt2/F");

  int ev1;
  Long64_t ev2;
  int ipho1, ipho2, ilep11, ilep12;
  float phoEt1, phoEt2, phoSCEta1, WMt1, WMt2, pfMET_NS1, pfMETPhi_NS1, lepPt1, lepPhi1;
  TBranch *b_ev1, *b_ev2, *b_ipho1, *b_ipho2, *b_ilep11, *b_ilep12, *b_phoEt1, *b_phoEt2, *b_phoSCEta1, *b_lepPt1, *b_lepPhi1;
  TBranch *b_WMt1, *b_WMt2, *b_pfMET_NS1, *b_pfMETPhi_NS1;

  tr1->SetBranchAddress("event", &ev1, &b_ev1);
  tr1->SetBranchAddress("ipho", &ipho1, &b_ipho1);
  tr1->SetBranchAddress("ilep1", &ilep11, &b_ilep11);
  tr1->SetBranchAddress("phoEt",&phoEt1, &b_phoEt1);
  tr1->SetBranchAddress("phoSCEta",&phoSCEta1, &b_phoSCEta1);
  tr1->SetBranchAddress("WMt",&WMt1, &b_WMt1);
  tr1->SetBranchAddress("pfMET_notSmeared",&pfMET_NS1,&b_pfMET_NS1);
  tr1->SetBranchAddress("pfMETPhi_notSmeared",&pfMETPhi_NS1,&b_pfMETPhi_NS1);
  tr1->SetBranchAddress("lepton1Pt",&lepPt1,&b_lepPt1);
  tr1->SetBranchAddress("lepton1Phi",&lepPhi1,&b_lepPhi1);

  tr2->SetBranchAddress("event2", &ev2, &b_ev2);
  tr2->SetBranchAddress("WMt2", &WMt2, &b_WMt2);
//  tr2->SetBranchAddress("iPho1", &ipho2, &b_ipho2);
//  tr2->SetBranchAddress("ilep1", &ilep12, &b_ilep12);
//  tr2->SetBranchAddress("pho_pt",&phoEt2, &b_phoEt2);

  long nentries1 = tr1->GetEntries("phoEt>15 && phoSCEta>-1.4442 && phoSCEta<1.4442");
  long nentries2 = tr2->GetEntries();

  std::cout<<"nentries1="<<nentries1<<", nentries2="<<nentries2<<std::endl;
//  nentries1=1500;

  nentries1 = tr1->GetEntries();

  std::cout<<"Found in tree 1 only:"<<std::endl;
  std::cout<<f1->GetName()<<std::endl;
  for (long ient1=0; ient1<nentries1; ient1++){
    b_phoEt1->GetEntry(ient1);
    if (phoEt1<15) continue;
    b_phoSCEta1->GetEntry(ient1);
    if (!(phoSCEta1>-1.4442 && phoSCEta1<1.4442)) continue;

    b_ev1->GetEntry(ient1);
//    b_ipho1->GetEntry(ient1);
//    b_ilep11->GetEntry(ient1);
    long ev1_Ref=ev1;
    bool evMatched=0;
    for (long ient2=0; ient2<nentries2; ient2++){
      b_ev2->GetEntry(ient2);
//      b_ipho2->GetEntry(ient2);
//      b_ilep12->GetEntry(ient2);
      if (ev1_Ref==ev2) {
        evMatched=1;break; 
      }

    }//end of loop over ient2
    if (evMatched==0){
      std::cout<<"event=="<<ev1_Ref<<std::endl;
    }
  }//end of loop over ient1

  nentries1 = tr1->GetEntries("phoEt>15 && phoSCEta>-1.4442 && phoSCEta<1.4442");
  nentries2 = tr2->GetEntries();

  std::cout<<"nentries1="<<nentries1<<", nentries2="<<nentries2<<std::endl;

  nentries1 = tr1->GetEntries();
//  nentries2=2140;//start from 2137 - all events
  //2136 - 0 ev

  std::cout<<"Found in tree 2 only:"<<std::endl;
  std::cout<<f2->GetName()<<std::endl;
  for (long ient2=0; ient2<nentries2; ient2++){
    b_ev2->GetEntry(ient2);
//    b_ipho2->GetEntry(ient2);
//    b_ilep12->GetEntry(ient2);
    long ev2_Ref=ev2;
    bool evMatched=0;
    for (long ient1=0; ient1<nentries1; ient1++){

      b_phoEt1->GetEntry(ient1);
      if (phoEt1<15) continue;
      b_phoSCEta1->GetEntry(ient1);
      if (!(phoSCEta1>-1.4442 && phoSCEta1<1.4442)) continue;
      b_ev1->GetEntry(ient1);
//      if (ient2>2136 && (ient1%500)==0){
//        std::cout<<"ient1="<<ient1<<", ient2="<<ient2<<", ev1="<<ev1<<", ev2_Ref="<<ev2_Ref<<std::endl;
//      }


//      b_ipho1->GetEntry(ient1);
//      b_ilep11->GetEntry(ient1);
      if (ev2_Ref==ev1) {
        evMatched=1;break; 
      }

    }//end of loop over ient1
    if (evMatched==0){
      std::cout<<"event=="<<ev2_Ref<<std::endl;
    }
  }//end of loop over ient2
}
