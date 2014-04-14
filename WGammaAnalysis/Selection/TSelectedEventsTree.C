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
  _outTree=tree;
  _outTree->Branch("leptonEta",&_leEta,"leptonEta/F");
  _outTree->Branch("leptonPhi",&_lePhi,"leptonPhi/F");
  _outTree->Branch("leptonPt",&_lePt,"leptonPt/F");
  _outTree->Branch("leptonGenPID",&_leGenPID,"leptonGenPID/I");
  _outTree->Branch("leptonGenMomPID",&_leGenMomPID,"leptonGenMomPID/I");
  _outTree->Branch("leptonGenGMomPID",&_leGenGMomPID,"leptonGenGMomPID/I");
  _outTree->Branch("phoEta",&_phoEta,"phoEta/F");
  _outTree->Branch("phoPhi",&_phoPhi,"phoPhi/F");
  _outTree->Branch("phoEt",&_phoEt,"phoEt/F");
  _outTree->Branch("phoGenPID",&_phoGenPID,"phoGenPID/I");
  _outTree->Branch("phoGenMomPID",&_phoGenMomPID,"phoGenMomPID/I");
  _outTree->Branch("phoGenGMomPID",&_phoGenGMomPID,"phoGenGMomPID/I");
  _outTree->Branch("phoGenEt",&_phoGenEt,"phoGenEt/F");
  _outTree->Branch("phoSigmaIEtaIEta",&_phoSigmaIEtaIEta,"phoSigmaIEtaIEta/F");
  _outTree->Branch("phoPFChIsoCorr",&_phoPFChIsoCorr,"phoPFChIsoCorr/F");
  _outTree->Branch("phoSCRChIsoCorr",&_phoSCRChIsoCorr,"phoSCRChIsoCorr/F");
  _outTree->Branch("lePhoDeltaR",&_lePhoDeltaR,"lePhoDeltaR/F");
  _outTree->Branch("WMt",&_WMt,"WMt/F");
  _outTree->Branch("pfMETPhi",&_pfMETPhi,"pfMETPhi/F");
  _outTree->Branch("pfMET",&_pfMET,"pfMET/F");
  _outTree->Branch("rho2012",&_rho2012,"rho2012/F");
  _outTree->Branch("run",&_run,"run/I");
  _outTree->Branch("inputFileNumber",&_inputFileN,"inputFileNumber/I");
  _outTree->Branch("weight",&_weight,"weight/F");
  _outTree->Branch("PUweight",&_PUweight,"PUweight/F");
  _outTree->Branch("PU",&_PU,"PU/F");
  _outTree->Branch("nMC",&_nMC,"nMC/I");
  _outTree->Branch("mcPID","vector<int>", &_mcPID);
  _outTree->Branch("mcMomPID","vector<int>", &_mcMomPID);
  _outTree->Branch("mcGMomPID","vector<int>", &_mcGMomPID);
}

void TSelectedEventsTree::Fill()
{
  _outTree->Fill();
}

void TSelectedEventsTree::SetValues(float leEta, float lePhi, float lePt,
                   int leGenPID, int leGenMomPID, int leGenGMomPID,
                   float phoEta, float phoPhi, float phoEt,
                   int phoGenPID, int phoGenMomPID, int phoGenGMomPID,
                   float phoGenEt,
                   float phoSigmaIEtaIEta,
                   float phoPFChIsoCorr,
                   float phoSCRChIsoCorr,
                   float lePhoDeltaR,
                   float WMt,
                   float pfMET, float pfMETPhi,
                   float rho2012,
                   int run,
                   int inputFileN,
                   float weight, float PUweight, float PU,
                   int nMC, vector <int> *mcPID,
                   vector <int> *mcMomPID, vector <int> *mcGMomPID)
{
  _leEta=leEta;
  _lePhi=lePhi;
  _lePt=lePt;
  _leGenPID=leGenPID;
  _leGenMomPID=leGenMomPID;
  _leGenGMomPID=leGenGMomPID;
  _phoEta=phoEta;
  _phoPhi=phoPhi;
  _phoEt=phoEt;
  _phoGenPID=phoGenPID;
  _phoGenMomPID=phoGenMomPID;
  _phoGenGMomPID=phoGenGMomPID;
  _phoGenEt=phoGenEt;
  _phoSigmaIEtaIEta=phoSigmaIEtaIEta;
  _phoPFChIsoCorr=phoPFChIsoCorr;
  _phoSCRChIsoCorr=phoSCRChIsoCorr;
  _lePhoDeltaR=lePhoDeltaR;
  _WMt=WMt;
  _pfMET=pfMET;
  _pfMETPhi=pfMETPhi;
  _rho2012=rho2012;
  _run=run;
  _inputFileN=inputFileN;
  _weight=weight;
  _PUweight=PUweight;
  _PU=PU;
  _nMC=nMC;
  _mcPID=mcPID;
  _mcMomPID=mcMomPID;
  _mcGMomPID=mcGMomPID;
}
