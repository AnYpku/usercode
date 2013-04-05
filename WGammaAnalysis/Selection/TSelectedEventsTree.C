#include "TSelectedEventsTree.h" //this class
#include "TTree.h" //ROOT class
#include "TFile.h" //ROOT class
#include "TString.h" //ROOT class
#include <iostream> //standard C++ class

TSelectedEventsTree::TSelectedEventsTree(TString selectedTreeFileName)
{
  if (selectedTreeFileName==0)
    {
      outTree_=0;
      std::cout<<"TSelectedEventsTree: no outputFile specified"<<std::endl;
    }
  else
    TSelectedEventsTree::SetOutputTreeFile();
}

TSelectedEventsTree::~TSelectedEventsTree()
{
  if (outTree_) delete (outTree_);
  outTree_=0;
}

void TSelectedEventsTree::SetOutputTreeFile()
{
  outTree_ = new TTree("selectedEvents","selected Events");
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
}

void TSelectedEventsTree::Fill()
{
  outTree_->Fill();
}

void TSelectedEventsTree::SaveTreeToFile()
{
  outFile_.Open(selectedTreeFileName_,"recreate");
  outTree_->Write();
  outFile_.Close();
}

void TSelectedEventsTree::SetValues(float leEta, float lePhi, float lePt, 
                 float phoEta, float phoPhi, float phoEt,
                 float lePhoDeltaR,
                 float WMt,
                 float pfMET, float pfMETPhi,
                 int run,
                 int inputFileN)
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
}
