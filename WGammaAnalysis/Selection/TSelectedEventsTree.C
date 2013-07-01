#include "TSelectedEventsTree.h" //this class
#include "TTree.h" //ROOT class
#include "TFile.h" //ROOT class
#include "TString.h" //ROOT class
#include <iostream> //standard C++ class

TSelectedEventsTree::TSelectedEventsTree()
{

}

TSelectedEventsTree::~TSelectedEventsTree()
{

}

void TSelectedEventsTree::SetOutputTree(TTree* tree)
{
  outTree_=tree;
  outTree_->Branch("leptonEta",&leEta_,"leptonEta/F");
  outTree_->Branch("leptonPhi",&lePhi_,"leptonPhi/F");
  outTree_->Branch("leptonPt",&lePt_,"leptonPt/F");
  outTree_->Branch("phoEta",&phoEta_,"phoEta/F");
  outTree_->Branch("phoPhi",&phoPhi_,"phoPhi/F");
  outTree_->Branch("phoEt",&phoEt_,"phoEt/F");
  outTree_->Branch("lePhoDeltaR",&lePhoDeltaR_,"lePhoDeltaR/F");
  outTree_->Branch("WMt",&WMt_,"WMt/F");
  outTree_->Branch("pfMETPhi",&pfMETPhi_,"pfMETPhi/F");
  outTree_->Branch("pfMET",&pfMET_,"pfMET/F");
  outTree_->Branch("run",&run_,"run/I");
  outTree_->Branch("inputFileNumber",&inputFileN_,"inputFileNumber/I");
  outTree_->Branch("weight",&weight_,"weight/F");
  outTree_->Branch("PUweight",&PUweight_,"PUweight/F");
  outTree_->Branch("PU",&PU_,"PU/F");
}

void TSelectedEventsTree::Fill()
{
  outTree_->Fill();
}

void TSelectedEventsTree::SetValues(float leEta, float lePhi, float lePt, 
                 float phoEta, float phoPhi, float phoEt,
                 float lePhoDeltaR,
                 float WMt,
                 float pfMET, float pfMETPhi,
                 int run,
                 int inputFileN,
                 float weight, float PUweight, float PU)
{
  leEta_=leEta;
  lePhi_=lePhi;
  lePt_=lePt;
  phoEta_=phoEta;
  phoPhi_=phoPhi;
  phoEt_=phoEt;
  lePhoDeltaR_=lePhoDeltaR;
  WMt_=WMt;
  pfMET_=pfMET;
  pfMETPhi_=pfMETPhi;
  run_=run;
  inputFileN_=inputFileN;
  weight_=weight;
  PUweight_=PUweight;
  PU_=PU;
}
