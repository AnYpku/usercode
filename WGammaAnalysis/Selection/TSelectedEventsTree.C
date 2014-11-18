#include "TSelectedEventsTree.h" //this class
#include "../Include/TEventTree.h" //this package
#include "../Include/TPhotonCuts.h" //this package
#include "../Include/TMuonCuts.h" //this package
#include "../Include/TFullCuts.h" //this package
#include "../Include/TMetTools.h" //this package
#include "../PHOSPHOR_CORRECTION/PhosphorCorrectorFunctor.hh"
#include "TTree.h" //ROOT class
#include "TFile.h" //ROOT class
#include "TString.h" //ROOT class
#include "TLorentzVector.h" //ROOT class
#include <iostream> //standard C++ class
#include <vector> //standard C++ class

TSelectedEventsTree::TSelectedEventsTree()
{

}

TSelectedEventsTree::~TSelectedEventsTree()
{

}

void TSelectedEventsTree::SetAsOutputTree(TTree* tree)
{
  tree->Branch("event",&_event,"event/I");

  for (int il=0; il<=1; il++){
    TString stril="";
    stril+=il+1;// 1 and 2
    tree->Branch(TString("ilep")+stril,&_ilep[il],TString("ilep")+stril+TString("/I"));
    tree->Branch(TString("iMClep")+stril,&_iMClep[il],TString("iMClep")+stril+TString("/I"));
    tree->Branch(TString("lepton")+stril+TString("Eta"),&_lepEta[il],TString("lepton")+stril+TString("Eta/F"));
    tree->Branch(TString("lepton")+stril+TString("Phi"),&_lepPhi[il],TString("lepton")+stril+TString("Phi/F"));
    tree->Branch(TString("lepton")+stril+TString("Pt"),&_lepPt[il],TString("lepton")+stril+TString("Pt/F"));
    tree->Branch(TString("lepton")+stril+TString("GenPID"),&_lepGenPID[il],TString("lepton")+stril+TString("GenPID/I"));
    tree->Branch(TString("lepton")+stril+TString("GenParentage"),&_lepGenParentage[il],TString("lepton")+stril+TString("GenParentage/I"));
    tree->Branch(TString("lepton")+stril+TString("GenMomPID"),&_lepGenMomPID[il],TString("lepton")+stril+TString("GenMomPID/I"));
    tree->Branch(TString("lepton")+stril+TString("GenGMomPID"),&_lepGenGMomPID[il],TString("lepton")+stril+TString("GenGMomPID/I"));
    tree->Branch(TString("lepton")+stril+TString("Id2012"),&_lepId2012[il],TString("lepton")+stril+TString("Id2012/O"));
    tree->Branch(TString("lepton")+stril+TString("Id2011"),&_lepId2011[il],TString("lepton")+stril+TString("Id2011/O"));
    tree->Branch(TString("lepton")+stril+TString("Isolation2012"),&_lepIsolation2012[il],TString("lepton")+stril+TString("Isolation2012/F"));
    tree->Branch(TString("lepton")+stril+TString("Isolation2011"),&_lepIsolation2011[il],TString("lepton")+stril+TString("Isolation2011/F"));
    tree->Branch(TString("lepton")+stril+TString("Trg"),&_lepTrg[il],TString("lepton")+stril+TString("Trg/I"));
    tree->Branch(TString("trgMatch")+stril+TString("IsoMu24eta2p1"),&_trgMatchIsoMu24eta2p1[il],TString("trgMatch")+stril+TString("IsoMu24eta2p1/O"));
    tree->Branch(TString("trgMatch")+stril+TString("IsoMu24"),&_trgMatchIsoMu24[il],TString("trgMatch")+stril+TString("IsoMu24/O"));
    tree->Branch(TString("trgMatch")+stril+TString("Mu22Mu8"),&_trgMatchIsoMu24[il],TString("trgMatch")+stril+TString("Mu22Mu8/O"));
    tree->Branch(TString("lep")+stril+TString("PhoDeltaR"),&_lepPhoDeltaR[il],TString("lep")+stril+TString("PhoDeltaR/F"));
  }//end of loop over il (il=0 and transforms to 1 and 2)

  tree->Branch("hasMoreLeptons",&_hasMoreLeptons,"hasMoreLeptons/O");
  tree->Branch("HLT_IsoMu24_eta2p1_",&_HLT_IsoMu24_eta2p1_,"HLT_IsoMu24_eta2p1_/I");
  tree->Branch("HLT_IsoMu24_v",&_HLT_IsoMu24_v,"HLT_IsoMu24_v/I");
  tree->Branch("HLT_Mu22_Mu8_v",&_HLT_Mu22_Mu8_v,"HLT_Mu22_Mu8_v/I");
  tree->Branch("ipho",&_ipho,"ipho/I");
  tree->Branch("iMCpho",&_iMCpho,"iMCpho/I");
  tree->Branch("phoElectronVeto",&_phoEleVeto,"phoElectronVeto/I");
  tree->Branch("phoEta",&_phoEta,"phoEta/F");
  tree->Branch("phoPhi",&_phoPhi,"phoPhi/F");
  tree->Branch("phoEt",&_phoEt,"phoEt/F");
  tree->Branch("phoEtNoPhosphor",&_phoEtNoPhosphor,"phoEtNoPhosphor/F");
  tree->Branch("phoSCEta",&_phoSCEta,"phoSCEta/F");
  tree->Branch("phoSCPhi",&_phoSCPhi,"phoSCPhi/F");
  tree->Branch("phoSCEt",&_phoSCEt,"phoSCEt/F");
  tree->Branch("phoGenPID",&_phoGenPID,"phoGenPID/I");
  tree->Branch("phoGenParentage",&_phoGenParentage,"phoGenParentage/I");
  tree->Branch("phoGenMomPID",&_phoGenMomPID,"phoGenMomPID/I");
  tree->Branch("phoGenGMomPID",&_phoGenGMomPID,"phoGenGMomPID/I");
  tree->Branch("phoGenEt",&_phoGenEt,"phoGenEt/F");
  tree->Branch("phoHoverE12",&_phoHoverE12,"phoHoverE12/F");
  tree->Branch("phoHoverE",&_phoHoverE,"phoHoverE/F");
  tree->Branch("phoSigmaIEtaIEta",&_phoSigmaIEtaIEta,"phoSigmaIEtaIEta/F");

  tree->Branch("phoPFChIsoCorr",&_phoPFChIsoCorr,"phoPFChIsoCorr/F");
  tree->Branch("phoPFNeuIsoCorr",&_phoPFNeuIsoCorr,"phoPFNeuIsoCorr/F");
  tree->Branch("phoPFPhoIsoCorr",&_phoPFPhoIsoCorr,"phoPFPhoIsoCorr/F");

  tree->Branch("phoSCRChIsoCorr",&_phoSCRChIsoCorr,"phoSCRChIsoCorr/F");
  tree->Branch("phoSCRNeuIsoCorr",&_phoSCRNeuIsoCorr,"phoSCRNeuIsoCorr/F");
  tree->Branch("phoSCRPhoIsoCorr",&_phoSCRPhoIsoCorr,"phoSCRPhoIsoCorr/F");

  tree->Branch("phoSCRChIso04Corr",&_phoSCRChIso04Corr,"phoSCRChIso04Corr/F");
  tree->Branch("phoSCRNeuIso04Corr",&_phoSCRNeuIso04Corr,"phoSCRNeuIso04Corr/F");
  tree->Branch("phoSCRPhoIso04Corr",&_phoSCRPhoIso04Corr,"phoSCRPhoIso04Corr/F");

  tree->Branch("phoRandConeChIsoCorr",&_phoRandConeChIsoCorr,"phoRandConeChIsoCorr/F");
  tree->Branch("phoRandConeNeuIsoCorr",&_phoRandConeNeuIsoCorr,"phoRandConeNeuIsoCorr/F");
  tree->Branch("phoRandConePhoIsoCorr",&_phoRandConePhoIsoCorr,"phoRandConePhoIsoCorr/F");


  tree->Branch("phoRandConeChIso04Corr",&_phoRandConeChIso04Corr,"phoRandConeChIso04Corr/F");
  tree->Branch("phoRandConeNeuIso04Corr",&_phoRandConeNeuIso04Corr,"phoRandConeNeuIso04Corr/F");
  tree->Branch("phoRandConePhoIso04Corr",&_phoRandConePhoIso04Corr,"phoRandConePhoIso04Corr/F");

  tree->Branch("phoEcalIsoDR04Corr",&_phoEcalIsoDR04Corr,"phoEcalIsoDR04Corr/F");
  tree->Branch("phoHcalIsoDR04Corr",&_phoHcalIsoDR04Corr,"phoHcalIsoDR04Corr/F");
  tree->Branch("phoTrkIsoHollowDR04Corr",&_phoTrkIsoHollowDR04Corr,"phoTrkIsoHollowCorr/F");
  tree->Branch("phohasPixelSeed",&_phohasPixelSeed,"phohasPixelSeed/I");
  tree->Branch("WMt",&_WMt,"WMt/F");
  tree->Branch("Mleplep",&_Mleplep,"Mleplep/F");
  tree->Branch("pfMET",&_pfMET,"pfMET/F");
  tree->Branch("pfMETPhi",&_pfMETPhi,"pfMETPhi/F");
  tree->Branch("pfMET_notSmeared",&_pfMET_notSmeared,"pfMET_notSmeared/F");
  tree->Branch("pfMETPhi_notSmeared",&_pfMETPhi_notSmeared,"pfMETPhi_notSmeared/F");
  tree->Branch("rho2012",&_rho2012,"rho2012/F");
  tree->Branch("rho2011",&_rho2011,"rho2011/F");
  tree->Branch("run",&_run,"run/I");
  tree->Branch("inputFileNumber",&_inputFileN,"inputFileNumber/I");
  tree->Branch("weight",&_weight,"weight/F");
  tree->Branch("PUweight",&_PUweight,"PUweight/F");
  tree->Branch("PU",&_PU,"PU/F");
  tree->Branch("nMC",&_nMC,"nMC/I");
  tree->Branch("mcPID","vector<int>", &_mcPID);
  tree->Branch("mcMomPID","vector<int>", &_mcMomPID);
  tree->Branch("mcGMomPID","vector<int>", &_mcGMomPID);
}

void TSelectedEventsTree::SetAsInputTree(TTree* tree)
{

  tree->SetBranchAddress("event",&_event,&_b_event); 

  for (int il=0; il<=1; il++){
    TString stril="";
    stril+=il+1;// 1 and 2
    tree->SetBranchAddress(TString("ilep")+stril,&_ilep[il],&_b_ilep[il]);
    tree->SetBranchAddress(TString("iMClep")+stril,&_iMClep[il],&_b_iMClep[il]);
    tree->SetBranchAddress(TString("lepton")+stril+TString("Eta"),&_lepEta[il],&_b_lepEta[il]);
    tree->SetBranchAddress(TString("lepton")+stril+TString("Phi"),&_lepPhi[il],&_b_lepPhi[il]);
    tree->SetBranchAddress(TString("lepton")+stril+TString("Pt"),&_lepPt[il],&_b_lepPt[il]);
    tree->SetBranchAddress(TString("lepton")+stril+TString("GenPID"),&_lepGenPID[il],&_b_lepGenPID[il]);
    tree->SetBranchAddress(TString("lepton")+stril+TString("GenParentage"),&_lepGenParentage[il],&_b_lepGenParentage[il]);
    tree->SetBranchAddress(TString("lepton")+stril+TString("GenMomPID"),&_lepGenMomPID[il],&_b_lepGenMomPID[il]);
    tree->SetBranchAddress(TString("lepton")+stril+TString("GenGMomPID"),&_lepGenGMomPID[il],&_b_lepGenGMomPID[il]);
    tree->SetBranchAddress(TString("lepton")+stril+TString("Id2012"),&_lepId2012[il],&_b_lepId2012[il]);
    tree->SetBranchAddress(TString("lepton")+stril+TString("Id2011"),&_lepId2011[il],&_b_lepId2011[il]);
    tree->SetBranchAddress(TString("lepton")+stril+TString("Isolation2012"),&_lepIsolation2012[il],&_b_lepIsolation2012[il]);
    tree->SetBranchAddress(TString("lepton")+stril+TString("Isolation2011"),&_lepIsolation2011[il],&_b_lepIsolation2011[il]);
    tree->SetBranchAddress(TString("lepton")+stril+TString("Trg"),&_lepTrg[il],&_b_lepTrg[il]);
    tree->SetBranchAddress(TString("trgMatch")+stril+TString("IsoMu24eta2p1"),&_trgMatchIsoMu24eta2p1[il],&_b_trgMatchIsoMu24eta2p1[il]);
    tree->SetBranchAddress(TString("trgMatch")+stril+TString("IsoMu24"),&_trgMatchIsoMu24[il],&_b_trgMatchIsoMu24[il]);
    tree->SetBranchAddress(TString("trgMatch")+stril+TString("Mu22Mu8"),&_trgMatchMu22Mu8[il],&_b_trgMatchMu22Mu8[il]);
    tree->SetBranchAddress(TString("lep")+stril+TString("PhoDeltaR"),&_lepPhoDeltaR[il],&_b_lepPhoDeltaR[il]);
  }//end of loop over il (il=0 and transforms to 1 and 2)

  tree->SetBranchAddress("hasMoreLeptons",&_hasMoreLeptons,&_b_hasMoreLeptons);
  tree->SetBranchAddress("HLT_IsoMu24_eta2p1_",&_HLT_IsoMu24_eta2p1_,&_b_HLT_IsoMu24_eta2p1_);
  tree->SetBranchAddress("HLT_IsoMu24_v",&_HLT_IsoMu24_v,&_b_HLT_IsoMu24_v);
  tree->SetBranchAddress("HLT_Mu22_Mu8_v",&_HLT_Mu22_Mu8_v,&_b_HLT_Mu22_Mu8_v);
  tree->SetBranchAddress("ipho",&_ipho,&_b_ipho);
  tree->SetBranchAddress("iMCpho",&_iMCpho,&_b_iMCpho);
  tree->SetBranchAddress("phoElectronVeto",&_phoEleVeto,&_b_phoEleVeto);
  tree->SetBranchAddress("phoEta",&_phoEta,&_b_phoEta);
  tree->SetBranchAddress("phoPhi",&_phoPhi,&_b_phoPhi);
  tree->SetBranchAddress("phoEt",&_phoEt,&_b_phoEt);
  tree->SetBranchAddress("phoEtNoPhosphor",&_phoEtNoPhosphor,&_b_phoEtNoPhosphor);
  tree->SetBranchAddress("phoSCEta",&_phoSCEta,&_b_phoSCEta);
  tree->SetBranchAddress("phoSCPhi",&_phoSCPhi,&_b_phoSCPhi);
  tree->SetBranchAddress("phoSCEt",&_phoSCEt,&_b_phoSCEt);
  tree->SetBranchAddress("phoGenPID",&_phoGenPID,&_b_phoGenPID);
  tree->SetBranchAddress("phoGenParentage",&_phoGenParentage,&_b_phoGenParentage);
  tree->SetBranchAddress("phoGenMomPID",&_phoGenMomPID,&_b_phoGenMomPID);
  tree->SetBranchAddress("phoGenGMomPID",&_phoGenGMomPID,&_b_phoGenGMomPID);
  tree->SetBranchAddress("phoGenEt",&_phoGenEt,&_b_phoGenEt);
  tree->SetBranchAddress("phoHoverE12",&_phoHoverE12,&_b_phoHoverE12);
  tree->SetBranchAddress("phoHoverE",&_phoHoverE,&_b_phoHoverE);
  tree->SetBranchAddress("phoSigmaIEtaIEta",&_phoSigmaIEtaIEta,&_b_phoSigmaIEtaIEta);

  tree->SetBranchAddress("phoPFChIsoCorr",&_phoPFChIsoCorr,&_b_phoPFChIsoCorr);
  tree->SetBranchAddress("phoPFNeuIsoCorr",&_phoPFNeuIsoCorr,&_b_phoPFNeuIsoCorr);
  tree->SetBranchAddress("phoPFPhoIsoCorr",&_phoPFPhoIsoCorr,&_b_phoPFPhoIsoCorr);

  tree->SetBranchAddress("phoSCRChIsoCorr",&_phoSCRChIsoCorr,&_b_phoSCRChIsoCorr);
  tree->SetBranchAddress("phoSCRNeuIsoCorr",&_phoSCRNeuIsoCorr,&_b_phoSCRNeuIsoCorr);
  tree->SetBranchAddress("phoSCRPhoIsoCorr",&_phoSCRPhoIsoCorr,&_b_phoSCRPhoIsoCorr);

  tree->SetBranchAddress("phoSCRChIso04Corr",&_phoSCRChIso04Corr,&_b_phoSCRChIso04Corr);
  tree->SetBranchAddress("phoSCRNeuIso04Corr",&_phoSCRNeuIso04Corr,&_b_phoSCRNeuIso04Corr);
  tree->SetBranchAddress("phoSCRPhoIso04Corr",&_phoSCRPhoIso04Corr,&_b_phoSCRPhoIso04Corr);

  tree->SetBranchAddress("phoRandConeChIsoCorr",&_phoRandConeChIsoCorr,&_b_phoRandConeChIsoCorr);
  tree->SetBranchAddress("phoRandConeNeuIsoCorr",&_phoRandConeNeuIsoCorr,&_b_phoRandConeNeuIsoCorr);
  tree->SetBranchAddress("phoRandConePhoIsoCorr",&_phoRandConePhoIsoCorr,&_b_phoRandConePhoIsoCorr);

  tree->SetBranchAddress("phoRandConeChIso04Corr",&_phoRandConeChIso04Corr,&_b_phoRandConeChIso04Corr);
  tree->SetBranchAddress("phoRandConeNeuIso04Corr",&_phoRandConeNeuIso04Corr,&_b_phoRandConeNeuIso04Corr);
  tree->SetBranchAddress("phoRandConePhoIso04Corr",&_phoRandConePhoIso04Corr,&_b_phoRandConePhoIso04Corr);

  tree->SetBranchAddress("phoEcalIsoDR04Corr",&_phoEcalIsoDR04Corr,&_b_phoEcalIsoDR04Corr);
  tree->SetBranchAddress("phoHcalIsoDR04Corr",&_phoHcalIsoDR04Corr,&_b_phoHcalIsoDR04Corr);
  tree->SetBranchAddress("phoTrkIsoHollowDR04Corr",&_phoTrkIsoHollowDR04Corr,&_b_phoTrkIsoHollowDR04Corr);
  tree->SetBranchAddress("phohasPixelSeed",&_phohasPixelSeed,&_b_phohasPixelSeed);
  tree->SetBranchAddress("WMt",&_WMt,&_b_WMt);
  tree->SetBranchAddress("Mleplep",&_Mleplep,&_b_Mleplep);
  tree->SetBranchAddress("pfMET",&_pfMET,&_b_pfMET);
  tree->SetBranchAddress("pfMETPhi",&_pfMETPhi,&_b_pfMETPhi);
  tree->SetBranchAddress("pfMET_notSmeared",&_pfMET_notSmeared,&_b_pfMET_notSmeared);
  tree->SetBranchAddress("pfMETPhi_notSmeared",&_pfMETPhi_notSmeared,&_b_pfMETPhi_notSmeared);
  tree->SetBranchAddress("rho2012",&_rho2012,&_b_rho2012);
  tree->SetBranchAddress("rho2011",&_rho2011,&_b_rho2011);
  tree->SetBranchAddress("run",&_run,&_b_run);
  tree->SetBranchAddress("inputFileNumber",&_inputFileN,&_b_inputFileN);
  tree->SetBranchAddress("weight",&_weight,&_b_weight);
  tree->SetBranchAddress("PUweight",&_PUweight,&_b_PUweight);
  tree->SetBranchAddress("PU",&_PU,&_b_PU);
  tree->SetBranchAddress("nMC",&_nMC,&_b_nMC);

  _mcPID=0;
  _mcMomPID=0;
  _mcGMomPID=0;
  tree->SetBranchAddress("mcPID",&_mcPID,&_b_mcPID);
  tree->SetBranchAddress("mcMomPID",&_mcMomPID,&_b_mcMomPID);
  tree->SetBranchAddress("mcGMomPID",&_mcGMomPID,&_b_mcGMomPID);

}

void TSelectedEventsTree::SetValues(int channel, int sample, TEventTree::InputTreeLeaves treeLeaf, TFullCuts::Candidate cand, int inputFileN, float weight, float PUweight, float PU,zgamma::PhosphorCorrectionFunctor* photonCorrector)
{

  if (cand.ipho<0 || cand.ilep1<0){
    std::cout<<"ERROR in TSelectedEventsTree::SetValues: ipho="<<cand.ipho<<", ilep1="<<cand.ilep1<<std::endl;
    return;
  }
  int ilMax=1;
  if (cand.ilep2<0) ilMax=0;//ilMax=0 - only 1 lepton
  else ilMax=1; //ilMax=1 - 2 leptons

  _event=treeLeaf.event;

  //lepton values
  if (channel==TConfiguration::MUON){

    for (int il=0; il<=ilMax; il++){
      int ilep=cand.ilep1;
      if (il==0) ilep=cand.ilep1;
      if (il==1) ilep=cand.ilep2;
      _ilep[il]=ilep;
      _lepEta[il]=treeLeaf.muEta->at(ilep);
      _lepPhi[il]=treeLeaf.muPhi->at(ilep);
      _lepPt[il]=treeLeaf.muPt->at(ilep);
      _lepTrg[il]=treeLeaf.muTrg->at(ilep);
      _iMClep[il]=-1;
      _lepGenPID[il]=0;
      _lepGenMomPID[il]=0;
      _lepGenGMomPID[il]=0;
      _lepId2012[il]=SetValuesMuId(treeLeaf,2012,ilep);
      _lepId2011[il]=SetValuesMuId(treeLeaf,2011,ilep);
      _lepIsolation2012[il]=SetValuesMuIsolation(treeLeaf,2012,ilep);
      _lepIsolation2011[il]=SetValuesMuIsolation(treeLeaf,2011,ilep);
      _trgMatchIsoMu24eta2p1[il]=treeLeaf.muTrg->at(ilep)%2;
      _trgMatchIsoMu24[il]=(treeLeaf.muTrg->at(ilep)/2)%2;
      _trgMatchMu22Mu8[il]=(treeLeaf.muTrg->at(ilep)/32)%2;
    }

    TMuonCuts emptyMuon;
    _hasMoreLeptons=emptyMuon.HasMoreMuons(treeLeaf.nMu, cand.ilep1, 
                      treeLeaf.muPt, treeLeaf.muEta);//only important for WGamma
    _HLT_IsoMu24_eta2p1_=treeLeaf.HLT[treeLeaf.HLTIndex[18]];
    _HLT_IsoMu24_v=treeLeaf.HLT[treeLeaf.HLTIndex[19]];
    _HLT_Mu22_Mu8_v=treeLeaf.HLT[treeLeaf.HLTIndex[21]];
  }
  else if (channel==TConfiguration::ELECTRON);

  //photon values
  _ipho=cand.ipho;
  _iMCpho=-1;
  _phoGenPID=0;
  _phoGenParentage=0;
  _phoGenMomPID=0;
  _phoGenGMomPID=0;
  TFullCuts fullCut;
  if (!treeLeaf.isData){
    float dRmin=1000;
    for (int iMC=0; iMC<treeLeaf.nMC; iMC++){
      if (treeLeaf.mcPID->at(iMC)==22){
        float dR = fullCut.DeltaR(treeLeaf.phoPhi->at(_ipho),treeLeaf.phoEta->at(_ipho),treeLeaf.mcPhi->at(iMC),treeLeaf.mcEta->at(iMC));
        if (dR<dRmin) dRmin=dR;
      }
    }
    for (int iMC=0; iMC<treeLeaf.nMC; iMC++){
      if(treeLeaf.mcIndex->at(iMC)==treeLeaf.phoGenIndex->at(cand.ipho)){
        _phoGenPID=treeLeaf.mcPID->at(iMC);
        _phoGenParentage=treeLeaf.mcParentage->at(iMC);
        if (_phoGenParentage==0){
          std::cout<<std::endl;
          std::cout<<"_phoGenParentage==0"<<std::endl;
          std::cout<<"event="<<treeLeaf.event<<", ipho="<<cand.ipho<<", imc="<<iMC<<std::endl;
          std::cout<<"mcIndex="<<treeLeaf.mcIndex->at(iMC)<<", phoGenIndex="<<treeLeaf.phoGenIndex->at(cand.ipho)<<std::endl;
          std::cout<<"mcPID="<<treeLeaf.mcPID->at(iMC)<<", mcMomPID="<<treeLeaf.mcMomPID->at(iMC)<<std::endl;
          std::cout<<"dR(gen,reco)="<<fullCut.DeltaR(treeLeaf.phoPhi->at(_ipho),treeLeaf.phoEta->at(_ipho),treeLeaf.mcPhi->at(iMC),treeLeaf.mcEta->at(iMC))<<", dRmin="<<dRmin<<std::endl;
          std::cout<<std::endl;
        }
        _phoGenMomPID=treeLeaf.mcMomPID->at(iMC);
        _phoGenGMomPID=treeLeaf.mcGMomPID->at(iMC);
        _phoGenEt=treeLeaf.mcEt->at(iMC);
        _iMCpho=iMC;
      }
      for (int il=0; il<=ilMax; il++){
        int ilep=cand.ilep1;
        if (il==0) ilep=cand.ilep1;
        if (il==1) ilep=cand.ilep2;
        if( (channel==TConfiguration::MUON && 
           treeLeaf.mcIndex->at(iMC)==treeLeaf.muGenIndex->at(ilep)) ||
          (channel==TConfiguration::ELECTRON && 
           treeLeaf.mcIndex->at(iMC)==treeLeaf.eleGenIndex->at(ilep)) ){
          _lepGenPID[il]=treeLeaf.mcPID->at(iMC);
          _lepGenParentage[il]=treeLeaf.mcParentage->at(iMC);
          _lepGenMomPID[il]=treeLeaf.mcMomPID->at(iMC);
          _lepGenGMomPID[il]=treeLeaf.mcGMomPID->at(iMC);
          _iMClep[il]=iMC;
        }
       }
    }//end of loop over iMC
  }//end of if (!treeLeaf.isData)

  _phoEleVeto=treeLeaf.phoEleVeto->at(cand.ipho);
  _phoEta=treeLeaf.phoEta->at(cand.ipho);
  _phoPhi=treeLeaf.phoPhi->at(cand.ipho);
  _phoEtNoPhosphor=treeLeaf.phoEt->at(cand.ipho);
  _phoEt=treeLeaf.phoEt->at(cand.ipho);//no phosphor

//  if (treeLeaf.isData)
//       _phoEt = photonCorrector->GetCorrEtData(treeLeaf.phoR9->at(cand.ipho), 2012, treeLeaf.phoEt->at(cand.ipho), treeLeaf.phoEta->at(cand.ipho));
       //Phosphor correction needs to be applied for the photon Et only, 
       //not for SC Et
//  else if (_iMCpho >= 0) 
//    _phoEt = photonCorrector->GetCorrEtMC(treeLeaf.phoR9->at(cand.ipho), 2012, treeLeaf.phoEt->at(cand.ipho), treeLeaf.phoEta->at(cand.ipho), treeLeaf.mcE->at(_iMCpho));

  _phoSCEta=treeLeaf.phoSCEta->at(cand.ipho);
  _phoSCPhi=treeLeaf.phoSCPhi->at(cand.ipho);
  _phoSCEt=treeLeaf.phoSCEt->at(cand.ipho);
  _phoHoverE=treeLeaf.phoHoverE->at(cand.ipho);
  _phoHoverE12=treeLeaf.phoHoverE12->at(cand.ipho);
  _phoSigmaIEtaIEta=treeLeaf.phoSigmaIEtaIEta->at(cand.ipho);
  _phohasPixelSeed=treeLeaf.phohasPixelSeed->at(cand.ipho);

  TPhotonCuts emptyPhoton;
  //photon isolation corrections 2012
//  std::cout<<"event="<<_event<<", ipho="<<cand.ipho<<std::endl;

//  std::cout<<"phoPFChIso Correction starts"<<std::endl;
//  std::cout<<"iso="<<treeLeaf.phoPFChIso->at(cand.ipho)<<", rho2012="<<treeLeaf.rho2012<<", eta="<<treeLeaf.phoSCEta->at(cand.ipho)<<std::endl;
  _phoPFChIsoCorr=emptyPhoton.GetPhoChIsoCorr(treeLeaf.phoPFChIso->at(cand.ipho),treeLeaf.rho2012,treeLeaf.phoSCEta->at(cand.ipho));
//  std::cout<<"isoCorr="<<_phoPFChIsoCorr<<std::endl;
//  std::cout<<"phoPFChIso Correction ends"<<std::endl;

//  std::cout<<"phoPFNeuIso Correction starts"<<std::endl;
//  std::cout<<"iso="<<treeLeaf.phoPFNeuIso->at(cand.ipho)<<", rho2012="<<treeLeaf.rho2012<<", eta="<<treeLeaf.phoSCEta->at(cand.ipho)<<std::endl;
  _phoPFNeuIsoCorr=emptyPhoton.GetPhoNeuIsoCorr(treeLeaf.phoPFNeuIso->at(cand.ipho),treeLeaf.rho2012,treeLeaf.phoSCEta->at(cand.ipho));
//  std::cout<<"isoCorr="<<_phoPFNeuIsoCorr<<std::endl;
//  std::cout<<"phoPFNeuIso Correction ends"<<std::endl;

//  std::cout<<"phoPFPhoIso Correction starts"<<std::endl;
//  std::cout<<"iso="<<treeLeaf.phoPFPhoIso->at(cand.ipho)<<", rho2012="<<treeLeaf.rho2012<<", eta="<<treeLeaf.phoSCEta->at(cand.ipho)<<std::endl;
  _phoPFPhoIsoCorr=emptyPhoton.GetPhoPhoIsoCorr(treeLeaf.phoPFPhoIso->at(cand.ipho),treeLeaf.rho2012,treeLeaf.phoSCEta->at(cand.ipho));
//  std::cout<<"isoCorr="<<_phoPFPhoIsoCorr<<std::endl;
//  std::cout<<"phoPFPhoIso Correction ends"<<std::endl;

  _phoSCRChIsoCorr=emptyPhoton.GetPhoChIsoCorr(treeLeaf.phoSCRChIso->at(cand.ipho),treeLeaf.rho2012,treeLeaf.phoSCEta->at(cand.ipho));
  _phoSCRNeuIsoCorr=emptyPhoton.GetPhoNeuIsoCorr(treeLeaf.phoSCRNeuIso->at(cand.ipho),treeLeaf.rho2012,treeLeaf.phoSCEta->at(cand.ipho));
  _phoSCRPhoIsoCorr=emptyPhoton.GetPhoPhoIsoCorr(treeLeaf.phoSCRPhoIso->at(cand.ipho),treeLeaf.rho2012,treeLeaf.phoSCEta->at(cand.ipho));

  _phoSCRChIso04Corr=emptyPhoton.GetPhoChIsoCorr(treeLeaf.phoSCRChIso04->at(cand.ipho),treeLeaf.rho2012,treeLeaf.phoSCEta->at(cand.ipho));
  _phoSCRNeuIso04Corr=emptyPhoton.GetPhoNeuIsoCorr(treeLeaf.phoSCRNeuIso04->at(cand.ipho),treeLeaf.rho2012,treeLeaf.phoSCEta->at(cand.ipho));
  _phoSCRPhoIso04Corr=emptyPhoton.GetPhoPhoIsoCorr(treeLeaf.phoSCRPhoIso04->at(cand.ipho),treeLeaf.rho2012,treeLeaf.phoSCEta->at(cand.ipho));

  if (sample==_config.DATA) {
    _phoRandConeChIsoCorr=emptyPhoton.GetPhoChIsoCorr(treeLeaf.phoRandConeChIso->at(cand.ipho),treeLeaf.rho2012,treeLeaf.phoSCEta->at(cand.ipho));
    _phoRandConeNeuIsoCorr=emptyPhoton.GetPhoNeuIsoCorr(treeLeaf.phoRandConeNeuIso->at(cand.ipho),treeLeaf.rho2012,treeLeaf.phoSCEta->at(cand.ipho));
    _phoRandConePhoIsoCorr=emptyPhoton.GetPhoPhoIsoCorr(treeLeaf.phoRandConePhoIso->at(cand.ipho),treeLeaf.rho2012,treeLeaf.phoSCEta->at(cand.ipho));

    _phoRandConeChIso04Corr=emptyPhoton.GetPhoChIsoCorr(treeLeaf.phoRandConeChIso04->at(cand.ipho),treeLeaf.rho2012,treeLeaf.phoSCEta->at(cand.ipho));
    _phoRandConeNeuIso04Corr=emptyPhoton.GetPhoNeuIsoCorr(treeLeaf.phoRandConeNeuIso04->at(cand.ipho),treeLeaf.rho2012,treeLeaf.phoSCEta->at(cand.ipho));
    _phoRandConePhoIso04Corr=emptyPhoton.GetPhoPhoIsoCorr(treeLeaf.phoRandConePhoIso04->at(cand.ipho),treeLeaf.rho2012,treeLeaf.phoSCEta->at(cand.ipho));
  }
  //photon isolation corrections 2011
  _phoEcalIsoDR04Corr=emptyPhoton.GetPhoEcalIsoDR04Corr(treeLeaf.phoEcalIsoDR04->at(cand.ipho),treeLeaf.rho2011,treeLeaf.phoSCEta->at(cand.ipho));
  _phoHcalIsoDR04Corr=emptyPhoton.GetPhoHcalIsoDR04Corr(treeLeaf.phoHcalIsoDR04->at(cand.ipho),treeLeaf.rho2011,treeLeaf.phoSCEta->at(cand.ipho));
  _phoTrkIsoHollowDR04Corr=emptyPhoton.GetPhoTrkIsoHollowDR04Corr(treeLeaf.phoTrkIsoHollowDR04->at(cand.ipho),treeLeaf.rho2011,treeLeaf.phoSCEta->at(cand.ipho));

  _lepPhoDeltaR[0]=cand.dRlep1pho;
  _lepPhoDeltaR[1]=cand.dRlep2pho;

   //pfMET smearing
   _pfMET_notSmeared=treeLeaf.pfMET;
   _pfMETPhi_notSmeared=treeLeaf.pfMETPhi;
   if (!treeLeaf.isData) {
     TMetTools met(treeLeaf.event, treeLeaf.pfMET, treeLeaf.pfMETPhi,
              //treeLeaf.nLowPtJet, treeLeaf.jetLowPtRawPt,
              //treeLeaf.jetLowPtRawEn, treeLeaf.jetLowPtPt,  
              //treeLeaf.jetLowPtEta, treeLeaf.jetLowPtPhi,
              //treeLeaf.jetLowPtGenJetPt, treeLeaf.jetLowPtGenJetEta, 
              //treeLeaf.jetLowPtGenJetPhi, 
              treeLeaf.nJet, 
              treeLeaf.jetRawPt, treeLeaf.jetRawEn, 
              treeLeaf.jetPt, treeLeaf.jetEta, 
              treeLeaf.jetPhi, treeLeaf.jetGenJetPt, 
              treeLeaf.jetGenJetEta, treeLeaf.jetGenJetPhi);
     met.METSmearCorrection();
     _pfMET = met.GetRecoPfMET();
     _pfMETPhi = met.GetRecoPfMETPhi();
 
   }//end of "if (!treeLeaf.isData)"
   else{
     _pfMET=treeLeaf.pfMET;
     _pfMETPhi=treeLeaf.pfMETPhi;
   }

  _WMt = sqrt(2*_lepPt[0]*_pfMET*(1-cos(_lepPhi[0]-_pfMETPhi))); //makes sense for W_GAMMA only
  
  if (cand.ilep2<0) _Mleplep=-1; //if W_GAMMA and no second lepton
  else{ //if Z_GAMMA
    TLorentzVector vlep1, vlep2;
    vlep1.SetPtEtaPhiM(_lepPt[0],_lepEta[0],_lepPhi[0],0);
    vlep2.SetPtEtaPhiM(_lepPt[1],_lepEta[1],_lepPhi[1],0);
    _Mleplep=(vlep1 + vlep2).M();
  }

  _rho2012=treeLeaf.rho2012;
  _rho2011=treeLeaf.rho2011;
  _run=treeLeaf.run;
  _inputFileN=inputFileN;
  _weight=weight;
  _PUweight=PUweight;
  _PU=PU;
  _nMC=treeLeaf.nMC;
  _mcPID=treeLeaf.mcPID;
  _mcMomPID=treeLeaf.mcMomPID;
  _mcGMomPID=treeLeaf.mcGMomPID;

}//end of SetValues

bool TSelectedEventsTree::SetValuesMuId(TEventTree::InputTreeLeaves treeLeaf, int year, int ile)
{
  TMuonCuts muon;
  return muon.MuId(year,treeLeaf.muChi2NDF->at(ile), 
//            treeLeaf.muD0->at(ile), treeLeaf.muDz->at(ile), 
            treeLeaf.muInnerD0GV->at(ile), treeLeaf.muInnerDzGV->at(ile), 
            treeLeaf.muNumberOfValidMuonHits->at(ile),
            treeLeaf.muNumberOfValidTrkHits->at(ile), 
            treeLeaf.muNumberOfValidPixelHits->at(ile),
            treeLeaf.muNumberOfValidTrkLayers->at(ile), 
            treeLeaf.muStations->at(ile),
            treeLeaf.muType->at(ile));
}

float TSelectedEventsTree::SetValuesMuIsolation(TEventTree::InputTreeLeaves treeLeaf, int year, int ile)
{
  TMuonCuts muon;
  if (year==2012)
    return muon.MuIsolation2012(treeLeaf.muPt->at(ile), 
                   treeLeaf.muPFIsoR04_NH->at(ile), 
                   treeLeaf.muPFIsoR04_Pho->at(ile), 
                   treeLeaf.muPFIsoR04_PU->at(ile),
                   treeLeaf.muPFIsoR04_CH->at(ile));
  else if (year==2011)
    return muon.MuIsolation2011(treeLeaf.muPt->at(ile), 
                   treeLeaf.muIsoTrk->at(ile), 
                   treeLeaf.muIsoEcal->at(ile), 
                   treeLeaf.muIsoHcal->at(ile),
                   treeLeaf.rho2011);
  return -1;
}
