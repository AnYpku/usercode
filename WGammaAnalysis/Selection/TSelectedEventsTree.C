#include "TSelectedEventsTree.h" //this class
#include "../Include/TEventTree.h" //this package
#include "../Include/TPhotonCuts.h" //this package
#include "../Include/TMuonCuts.h" //this package
#include "../Include/TFullCuts.h" //this package
#include "../Include/TMetTools.h" //this package
#include "../Include/TMathTools.h"
#include "../Include/TScaleFactors.h"
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
    tree->Branch(TString("lepton")+stril+TString("SCEta"),&_lepSCEta[il],TString("lepton")+stril+TString("SCEta/F"));
    tree->Branch(TString("lepton")+stril+TString("SCPhi"),&_lepSCPhi[il],TString("lepton")+stril+TString("SCPhi/F"));
    tree->Branch(TString("lepton")+stril+TString("Pt"),&_lepPt[il],TString("lepton")+stril+TString("Pt/F"));
    tree->Branch(TString("lepton")+stril+TString("GenPID"),&_lepGenPID[il],TString("lepton")+stril+TString("GenPID/I"));
    tree->Branch(TString("lepton")+stril+TString("GenParentage"),&_lepGenParentage[il],TString("lepton")+stril+TString("GenParentage/I"));
    tree->Branch(TString("lepton")+stril+TString("GenMomPID"),&_lepGenMomPID[il],TString("lepton")+stril+TString("GenMomPID/I"));
    tree->Branch(TString("lepton")+stril+TString("GenGMomPID"),&_lepGenGMomPID[il],TString("lepton")+stril+TString("GenGMomPID/I"));
    tree->Branch(TString("lepton")+stril+TString("GenPt"),&_lepGenPt[il],TString("lepton")+stril+TString("GenPt/F"));
    tree->Branch(TString("lepton")+stril+TString("GenEta"),&_lepGenEta[il],TString("lepton")+stril+TString("GenEta/F"));
    tree->Branch(TString("lepton")+stril+TString("GenPhi"),&_lepGenPhi[il],TString("lepton")+stril+TString("GenPhi/F"));
    tree->Branch(TString("lepton")+stril+TString("Id2012"),&_lepId2012[il],TString("lepton")+stril+TString("Id2012/O"));
    tree->Branch(TString("lepton")+stril+TString("Id2011"),&_lepId2011[il],TString("lepton")+stril+TString("Id2011/O"));
    tree->Branch(TString("lepton")+stril+TString("Isolation2012"),&_lepIsolation2012[il],TString("lepton")+stril+TString("Isolation2012/F"));
    tree->Branch(TString("lepton")+stril+TString("Isolation2011"),&_lepIsolation2011[il],TString("lepton")+stril+TString("Isolation2011/F"));
    tree->Branch(TString("lepton")+stril+TString("SF"),&_lepSF[il],TString("lepton")+stril+TString("SF/F"));
//    tree->Branch(TString("lepton")+stril+TString("Trg"),&_lepTrg[il],TString("lepton")+stril+TString("Trg/I"));

//    tree->Branch(TString("trgMatch")+stril+TString("IsoMu24eta2p1"),&_trgMatchIsoMu24eta2p1[il],TString("trgMatch")+stril+TString("IsoMu24eta2p1/O"));
//    tree->Branch(TString("trgMatch")+stril+TString("IsoMu24"),&_trgMatchIsoMu24[il],TString("trgMatch")+stril+TString("IsoMu24/O"));
//    tree->Branch(TString("trgMatch")+stril+TString("Ele27WP80"),&_trgMatchEle27WP80[il],TString("trgMatch")+stril+TString("Ele27WP80/O"));
//    tree->Branch(TString("trgMatch")+stril+TString("Ele17Ele8"),&_trgMatchIsoMu24[il],TString("trgMatch")+stril+TString("Ele17Ele8/O"));
    tree->Branch(TString("lep")+stril+TString("TrgMatch"),&_lepTrgMatch[il],TString("lep")+stril+TString("TrgMatch/O"));


    tree->Branch(TString("lep")+stril+TString("PhoDeltaR"),&_lepPhoDeltaR[il],TString("lep")+stril+TString("PhoDeltaR/F"));
    tree->Branch(TString("Mpholep")+stril,&_Mpholep[il],TString("Mpholep")+stril+TString("/F"));
  }//end of loop over il (il=0 and transforms to 1 and 2)

  tree->Branch("hasMoreLeptons",&_hasMoreLeptons,"hasMoreLeptons/O");
//  tree->Branch("HLT_IsoMu24_eta2p1_",&_HLT_IsoMu24_eta2p1_,"HLT_IsoMu24_eta2p1_/I");
//  tree->Branch("HLT_IsoMu24_v",&_HLT_IsoMu24_v,"HLT_IsoMu24_v/I");
//  tree->Branch("HLT_Mu22_Mu8_v",&_HLT_Mu22_Mu8_v,"HLT_Mu22_Mu8_v/I");
//  tree->Branch("HLT_Ele27_WP80_v",&_HLT_Ele27_WP80_v,"HLT_Ele27_WP80_v/I");
//  tree->Branch("HLT_Ele17_Ele8_v",&_HLT_Ele17_Ele8_v,"HLT_Ele17_Ele8_v/I");
  tree->Branch("HLT",&_HLT,"HLT/I");
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
  tree->Branch("phoGenEta",&_phoGenEta,"phoGenEta/F");
  tree->Branch("phoGenPhi",&_phoGenPhi,"phoGenPhi/F");
  tree->Branch("phoHoverE12",&_phoHoverE12,"phoHoverE12/F");
  tree->Branch("phoHoverE",&_phoHoverE,"phoHoverE/F");
  tree->Branch("phoSigmaIEtaIEta",&_phoSigmaIEtaIEta,"phoSigmaIEtaIEta/F");

  tree->Branch("phoSF",&_phoSF,"phoSF/F");

  tree->Branch("pho_genEle_dRMin",&_pho_genEle_dRMin,"pho_genEle_dRMin/F");

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
  tree->Branch("Mpholeplep",&_Mpholeplep,"Mpholeplep/F");
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
    tree->SetBranchAddress(TString("lepton")+stril+TString("SCEta"),&_lepSCEta[il],&_b_lepSCEta[il]);
    tree->SetBranchAddress(TString("lepton")+stril+TString("SCPhi"),&_lepSCPhi[il],&_b_lepSCPhi[il]);
    tree->SetBranchAddress(TString("lepton")+stril+TString("Pt"),&_lepPt[il],&_b_lepPt[il]);
    tree->SetBranchAddress(TString("lepton")+stril+TString("GenPID"),&_lepGenPID[il],&_b_lepGenPID[il]);
    tree->SetBranchAddress(TString("lepton")+stril+TString("GenParentage"),&_lepGenParentage[il],&_b_lepGenParentage[il]);
    tree->SetBranchAddress(TString("lepton")+stril+TString("GenMomPID"),&_lepGenMomPID[il],&_b_lepGenMomPID[il]);
    tree->SetBranchAddress(TString("lepton")+stril+TString("GenGMomPID"),&_lepGenGMomPID[il],&_b_lepGenGMomPID[il]);
    tree->SetBranchAddress(TString("lepton")+stril+TString("GenPt"),&_lepGenPt[il],&_b_lepGenPt[il]);
    tree->SetBranchAddress(TString("lepton")+stril+TString("GenEta"),&_lepGenEta[il],&_b_lepGenEta[il]);
    tree->SetBranchAddress(TString("lepton")+stril+TString("GenPhi"),&_lepGenPhi[il],&_b_lepGenPhi[il]);
    tree->SetBranchAddress(TString("lepton")+stril+TString("Id2012"),&_lepId2012[il],&_b_lepId2012[il]);
    tree->SetBranchAddress(TString("lepton")+stril+TString("Id2011"),&_lepId2011[il],&_b_lepId2011[il]);
    tree->SetBranchAddress(TString("lepton")+stril+TString("Isolation2012"),&_lepIsolation2012[il],&_b_lepIsolation2012[il]);
    tree->SetBranchAddress(TString("lepton")+stril+TString("Isolation2011"),&_lepIsolation2011[il],&_b_lepIsolation2011[il]);
    tree->SetBranchAddress(TString("lepton")+stril+TString("SF"),&_lepSF[il],&_b_lepSF[il]);
//    tree->SetBranchAddress(TString("lepton")+stril+TString("Trg"),&_lepTrg[il],&_b_lepTrg[il]);
 //   tree->SetBranchAddress(TString("trgMatch")+stril+TString("IsoMu24eta2p1"),&_trgMatchIsoMu24eta2p1[il],&_b_trgMatchIsoMu24eta2p1[il]);
//    tree->SetBranchAddress(TString("trgMatch")+stril+TString("IsoMu24"),&_trgMatchIsoMu24[il],&_b_trgMatchIsoMu24[il]);
//    tree->SetBranchAddress(TString("trgMatch")+stril+TString("Mu22Mu8"),&_trgMatchMu22Mu8[il],&_b_trgMatchMu22Mu8[il]);
//    tree->SetBranchAddress(TString("trgMatch")+stril+TString("Ele27WP80"),&_trgMatchEle27WP80[il],&_b_trgMatchEle27WP80[il]);
//    tree->SetBranchAddress(TString("trgMatch")+stril+TString("Ele17Ele8"),&_trgMatchEle17Ele8[il],&_b_trgMatchEle17Ele8[il]);
tree->SetBranchAddress(TString("lep")+stril+TString("TrgMatch"),&_lepTrgMatch[il],&_b_lepTrgMatch[il]);
    tree->SetBranchAddress(TString("lep")+stril+TString("PhoDeltaR"),&_lepPhoDeltaR[il],&_b_lepPhoDeltaR[il]);
    tree->SetBranchAddress(TString("Mpholep")+stril,&_Mpholep[il],&_b_Mpholep[il]);
  }//end of loop over il (il=0 and transforms to 1 and 2)

  tree->SetBranchAddress("hasMoreLeptons",&_hasMoreLeptons,&_b_hasMoreLeptons);
//  tree->SetBranchAddress("HLT_IsoMu24_eta2p1_",&_HLT_IsoMu24_eta2p1_,&_b_HLT_IsoMu24_eta2p1_);
//  tree->SetBranchAddress("HLT_IsoMu24_v",&_HLT_IsoMu24_v,&_b_HLT_IsoMu24_v);
//  tree->SetBranchAddress("HLT_Mu22_Mu8_v",&_HLT_Mu22_Mu8_v,&_b_HLT_Mu22_Mu8_v);
//  tree->SetBranchAddress("HLT_Ele27_WP80_v",&_HLT_Ele27_WP80_v,&_b_HLT_Ele27_WP80_v);
//  tree->SetBranchAddress("HLT_Ele17_Ele8_v",&_HLT_Ele17_Ele8_v,&_b_HLT_Ele17_Ele8_v);
  tree->SetBranchAddress("HLT",&_HLT,&_b_HLT);
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
  tree->SetBranchAddress("phoGenEta",&_phoGenEta,&_b_phoGenEta);
  tree->SetBranchAddress("phoGenPhi",&_phoGenPhi,&_b_phoGenPhi);
  tree->SetBranchAddress("phoHoverE12",&_phoHoverE12,&_b_phoHoverE12);
  tree->SetBranchAddress("phoHoverE",&_phoHoverE,&_b_phoHoverE);
  tree->SetBranchAddress("phoSigmaIEtaIEta",&_phoSigmaIEtaIEta,&_b_phoSigmaIEtaIEta);

  tree->SetBranchAddress("phoSF",&_phoSF,&_b_phoSF);

  tree->SetBranchAddress("pho_genEle_dRMin",&_pho_genEle_dRMin,&_b_pho_genEle_dRMin);

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
  tree->SetBranchAddress("Mpholeplep",&_Mpholeplep,&_b_Mpholeplep);
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

void TSelectedEventsTree::SetValues(int channel, int sample, TEventTree::InputTreeLeaves& leaf, TFullCuts::Candidate cand, int inputFileN, float weight, float PUweight, float PU,zgamma::PhosphorCorrectionFunctor* photonCorrector)
{

  int ilMax=1;
  if (cand.ilep2<0) ilMax=0;//ilMax=0 - only 1 lepton
  else ilMax=1; //ilMax=1 - 2 leptons
  if (cand.ipho<0 || cand.ilep1<0){
    std::cout<<"ERROR in TSelectedEventsTree::SetValues: ipho="<<cand.ipho<<", ilep1="<<cand.ilep1<<std::endl;
    return;
  }

  _event=leaf.event;

  //lepton values
  if (channel==_config.MUON)
    SetMuonValues(leaf,cand,ilMax);
  else if (channel==_config.ELECTRON)
    SetElectronValues(leaf,cand,ilMax);

  SetPhotonValues(leaf,cand,channel,ilMax);
  SetPhotonIsoValues(leaf,cand,sample,channel);





  _lepPhoDeltaR[0]=cand.dRlep1pho;
  _lepPhoDeltaR[1]=cand.dRlep2pho;

   //pfMET smearing
   _pfMET_notSmeared=leaf.pfMET;
   _pfMETPhi_notSmeared=leaf.pfMETPhi;
   if (!leaf.isData) {
     TMetTools met(leaf.event, leaf.pfMET, leaf.pfMETPhi,
              //leaf.nLowPtJet, leaf.jetLowPtRawPt,
              //leaf.jetLowPtRawEn, leaf.jetLowPtPt,  
              //leaf.jetLowPtEta, leaf.jetLowPtPhi,
              //leaf.jetLowPtGenJetPt, leaf.jetLowPtGenJetEta, 
              //leaf.jetLowPtGenJetPhi, 
              leaf.nJet, 
              leaf.jetRawPt, leaf.jetRawEn, 
              leaf.jetPt, leaf.jetEta, 
              leaf.jetPhi, leaf.jetGenJetPt, 
              leaf.jetGenJetEta, leaf.jetGenJetPhi);
     met.METSmearCorrection();
     _pfMET = met.GetRecoPfMET();
     _pfMETPhi = met.GetRecoPfMETPhi();
 
   }//end of "if (!leaf.isData)"
   else{
     _pfMET=leaf.pfMET;
     _pfMETPhi=leaf.pfMETPhi;
   }

  _WMt = sqrt(2*_lepPt[0]*_pfMET*(1-cos(_lepPhi[0]-_pfMETPhi))); //makes sense for W_GAMMA only
  
  TLorentzVector vlep1, vlep2, vpho;
  vlep1.SetPtEtaPhiM(_lepPt[0],_lepEta[0],_lepPhi[0],0);
  vpho.SetPtEtaPhiM(_phoEt,_phoEta,_phoPhi,0);
  _Mpholep[0]=(vpho+vlep1).M();
  if (cand.ilep2<0) {_Mleplep=-1; _Mpholeplep=-1; _Mpholep[1]=-1;} //if W_GAMMA and no second lepton
  else{ //if Z_GAMMA   
    vlep2.SetPtEtaPhiM(_lepPt[1],_lepEta[1],_lepPhi[1],0);    
    _Mleplep=(vlep1 + vlep2).M();
    _Mpholeplep=(vpho+vlep1 + vlep2).M();
    _Mpholep[1]=(vpho+vlep2).M();
  }


  _rho2012=leaf.rho2012;
  _rho2011=leaf.rho2011;
  _run=leaf.run;
  _inputFileN=inputFileN;
  TScaleFactors sf;
  _phoSF=1.0; _lepSF[0]=1.0; _lepSF[1]=1.0;
  if (sample!=_config.DATA){
    _phoSF = sf.SF_MediumPhoID(_phoEt,_phoSCEta);
    if (channel==_config.ELECTRON) _phoSF*=sf.SF_PixelSeedVeto(_phoEt,_phoSCEta);
    if (channel==_config.MUON) _lepSF[0]=sf.SF_MuonIso(_lepPt[0],_lepEta[0])*sf.SF_MuonID(_lepPt[0],_lepEta[0]);
    if (channel==_config.ELECTRON) _lepSF[0]=sf.SF_TightEleID(_lepPt[0],_lepSCEta[0]);
    if (cand.ilep2>=0 && _Mleplep>0){// means Z_GAMMA
      if (channel==_config.MUON) _lepSF[1]=sf.SF_MuonIso(_lepPt[1],_lepEta[1])*sf.SF_MuonID(_lepPt[1],_lepEta[1]);
      if (channel==_config.ELECTRON) _lepSF[1]=sf.SF_TightEleID(_lepPt[1],_lepSCEta[1]);    
    } 
  }// end of if (sample==!_config.DATA)
  _weight=weight*_phoSF*_lepSF[0]*_lepSF[1];
  _PUweight=PUweight;
  _PU=PU;
  _nMC=leaf.nMC;
  _mcPID=leaf.mcPID;
  _mcMomPID=leaf.mcMomPID;
  _mcGMomPID=leaf.mcGMomPID;

}//end of SetValues

void TSelectedEventsTree::SetMuonValues(TEventTree::InputTreeLeaves& leaf, TFullCuts::Candidate cand, int ilMax)
{
    for (int il=0; il<=ilMax; il++){
      int ilep=cand.ilep1;
      if (il==0) ilep=cand.ilep1;
      if (il==1) ilep=cand.ilep2;
      _ilep[il]=ilep;
      _lepEta[il]=leaf.muEta->at(ilep);
      _lepPhi[il]=leaf.muPhi->at(ilep);
      _lepPt[il]=leaf.muPt->at(ilep);
//      _lepTrg[il]=leaf.muTrg->at(ilep);
      _iMClep[il]=-1;
      _lepGenPID[il]=0;
      _lepGenMomPID[il]=0;
      _lepGenGMomPID[il]=0;
      _lepId2012[il]=_muon.MuId(2012,leaf,ilep);
//      _lepId2011[il]=SetValuesMuId(leaf,2011,ilep);
//      _lepIsolation2012[il]=SetValuesMuIsolation(leaf,2012,ilep);
//      _lepIsolation2011[il]=SetValuesMuIsolation(leaf,2011,ilep);
//      _trgMatchIsoMu24eta2p1[il]=leaf.muTrg->at(ilep)%2;
//      _trgMatchIsoMu24[il]=(leaf.muTrg->at(ilep)/2)%2;
//      _trgMatchMu22Mu8[il]=(leaf.muTrg->at(ilep)/16)%2;

      // Single Mu: * muTriggerMatch1 "muonTriggerMatchHLTIsoMu24eta2p1" 
      // Double Mu: * muTriggerMatch5 "muonTriggerMatchHLTMu22TkMu8"
    }

    _hasMoreLeptons=_muon.HasMoreMuons(leaf,cand.ilep1);//only important for WGamma
//    _HLT_IsoMu24_eta2p1_=leaf.HLT[leaf.HLTIndex[18]];
//    _HLT_IsoMu24_v=leaf.HLT[leaf.HLTIndex[19]];
//    _HLT_Mu22_Mu8_v=leaf.HLT[leaf.HLTIndex[21]];
}//end of SetMuonValues

void TSelectedEventsTree::SetElectronValues(TEventTree::InputTreeLeaves& leaf, TFullCuts::Candidate cand, int ilMax)
{
    for (int il=0; il<=ilMax; il++){
      int ilep=cand.ilep1;
      if (il==0) ilep=cand.ilep1;
      if (il==1) ilep=cand.ilep2;
      _ilep[il]=ilep;
      _lepEta[il]=leaf.eleEta->at(ilep);
      _lepPhi[il]=leaf.elePhi->at(ilep);
      _lepSCEta[il]=leaf.eleSCEta->at(ilep);
      _lepSCPhi[il]=leaf.eleSCPhi->at(ilep);
      _lepPt[il]=leaf.elePt->at(ilep);
//      _lepTrg[il]=leaf.eleTrg->at(ilep);
      _iMClep[il]=-1;
      _lepGenPID[il]=0;
      _lepGenMomPID[il]=0;
      _lepGenGMomPID[il]=0;
      _lepId2012[il]=_electron.EleID2012(leaf,ilep,_electron.ELE_MEDIUM);
//      _lepId2011[il]=SetValuesMuId(leaf,2011,ilep);
//      _lepIsolation2012[il]=SetValuesMuIsolation(leaf,2012,ilep);
//      _lepIsolation2011[il]=SetValuesMuIsolation(leaf,2011,ilep);
//      _trgMatchEle27WP80[il]=leaf.eleTrg->at(ilep)%2;
//      _trgMatchEle17Ele8[il]=(leaf.eleTrg->at(ilep)/4)%2;

      // Single Ele: const *eleTriggerMatch1 "electronTriggerMatchHLTEle27WP80"; 
      // Possibly Double Ele: *eleTriggerMatch3
      //   "electronTriggerMatchHLTEle17CaloIdLCaloIsoVLEle8CaloIdLCaloIsoVL"));
      // Possibly Double Ele: *eleTriggerMatch8
      //   "electronTriggerMatchHLTEle17TightIdLooseIsoEle8TightIdLooseIsoTrackIsoDoubleFilter")); 
      //       matched to Ele17 and Ele8
    }



    _hasMoreLeptons=_electron.HasMoreElectrons(leaf,cand.ilep1,0);//only important for WGamma
//    _HLT_Ele27_WP80_v=leaf.HLT[leaf.HLTIndex[17]];
//    _HLT_Ele17_Ele8_v=leaf.HLT[leaf.HLTIndex[22]];
}//end of SetElectronValues

void TSelectedEventsTree::SetPhotonValues(TEventTree::InputTreeLeaves& leaf, TFullCuts::Candidate cand, int channel, int ilMax)
{
  //photon values
  _ipho=cand.ipho;
  _iMCpho=-1;
  _phoGenPID=0;
  _phoGenParentage=0;
  _phoGenMomPID=0;
  _phoGenGMomPID=0;
  TFullCuts fullCut;
  if (!leaf.isData){
    float dRmin=1000;
    for (int iMC=0; iMC<leaf.nMC; iMC++){
      if (leaf.mcPID->at(iMC)==22){
        float dR = _math.DeltaR(leaf.phoPhi->at(_ipho),leaf.phoEta->at(_ipho),leaf.mcPhi->at(iMC),leaf.mcEta->at(iMC));
        if (dR<dRmin) dRmin=dR;
      }
    }
    for (int iMC=0; iMC<leaf.nMC; iMC++){
      if(leaf.mcIndex->at(iMC)==leaf.phoGenIndex->at(cand.ipho)){
        _phoGenPID=leaf.mcPID->at(iMC);
        _phoGenParentage=leaf.mcParentage->at(iMC);
        if (_phoGenParentage==0){
          std::cout<<std::endl;
          std::cout<<"_phoGenParentage==0"<<std::endl;
          std::cout<<"event="<<leaf.event<<", ipho="<<cand.ipho<<", imc="<<iMC<<std::endl;
          std::cout<<"mcIndex="<<leaf.mcIndex->at(iMC)<<", phoGenIndex="<<leaf.phoGenIndex->at(cand.ipho)<<std::endl;
          std::cout<<"mcPID="<<leaf.mcPID->at(iMC)<<", mcMomPID="<<leaf.mcMomPID->at(iMC)<<std::endl;
          std::cout<<"dR(gen,reco)="<<_math.DeltaR(leaf.phoPhi->at(_ipho),leaf.phoEta->at(_ipho),leaf.mcPhi->at(iMC),leaf.mcEta->at(iMC))<<", dRmin="<<dRmin<<std::endl;
          std::cout<<std::endl;
        }
        _phoGenMomPID=leaf.mcMomPID->at(iMC);
        _phoGenGMomPID=leaf.mcGMomPID->at(iMC);
        _phoGenEt=leaf.mcEt->at(iMC);
        _phoGenEta=leaf.mcEta->at(iMC);
        _phoGenPhi=leaf.mcPhi->at(iMC);
        _iMCpho=iMC;
      }
      for (int il=0; il<=ilMax; il++){
        int ilep=cand.ilep1;
        if (il==0) ilep=cand.ilep1;
        if (il==1) ilep=cand.ilep2;
        if( (channel==TConfiguration::MUON && 
           leaf.mcIndex->at(iMC)==leaf.muGenIndex->at(ilep)) ||
          (channel==TConfiguration::ELECTRON && 
           leaf.mcIndex->at(iMC)==leaf.eleGenIndex->at(ilep)) ){
          _lepGenPID[il]=leaf.mcPID->at(iMC);
          _lepGenParentage[il]=leaf.mcParentage->at(iMC);
          _lepGenMomPID[il]=leaf.mcMomPID->at(iMC);
          _lepGenGMomPID[il]=leaf.mcGMomPID->at(iMC);
          _lepGenPt[il]=leaf.mcPt->at(iMC);
          _lepGenEta[il]=leaf.mcEta->at(iMC);
          _lepGenPhi[il]=leaf.mcPhi->at(iMC);
          _iMClep[il]=iMC;
        }
       }
    }//end of loop over iMC

    float dRMin=1000;
    for (int iMC=0; iMC<leaf.nMC; iMC++){
      if (leaf.mcPID->at(iMC)!=11 && leaf.mcPID->at(iMC)!=-11) continue; 
      if (leaf.mcMomPID->at(iMC)!=23) continue; 
      float dR = _math.DeltaR(leaf.phoPhi->at(_ipho),leaf.phoEta->at(_ipho),leaf.mcPhi->at(iMC),leaf.mcEta->at(iMC));
      if (dR<dRMin) dRMin=dR;
    }//end of loop over iMC
    _pho_genEle_dRMin=dRMin;

  }//end of if (!leaf.isData)

  _phoEleVeto=leaf.phoEleVeto->at(cand.ipho);
  _phoEta=leaf.phoEta->at(cand.ipho);
  _phoPhi=leaf.phoPhi->at(cand.ipho);
  _phoEtNoPhosphor=leaf.phoEt->at(cand.ipho);
  _phoEt=leaf.phoEt->at(cand.ipho);//no phosphor

//  if (leaf.isData)
//       _phoEt = photonCorrector->GetCorrEtData(leaf.phoR9->at(cand.ipho), 2012, leaf.phoEt->at(cand.ipho), leaf.phoEta->at(cand.ipho));
       //Phosphor correction needs to be applied for the photon Et only, 
       //not for SC Et
//  else if (_iMCpho >= 0) 
//    _phoEt = photonCorrector->GetCorrEtMC(leaf.phoR9->at(cand.ipho), 2012, leaf.phoEt->at(cand.ipho), leaf.phoEta->at(cand.ipho), leaf.mcE->at(_iMCpho));

  _phoSCEta=leaf.phoSCEta->at(cand.ipho);
  _phoSCPhi=leaf.phoSCPhi->at(cand.ipho);
  _phoSCEt=leaf.phoSCEt->at(cand.ipho);
  _phoHoverE=leaf.phoHoverE->at(cand.ipho);
  _phoHoverE12=leaf.phoHoverE12->at(cand.ipho);
  _phoSigmaIEtaIEta=leaf.phoSigmaIEtaIEta->at(cand.ipho);
  _phohasPixelSeed=leaf.phohasPixelSeed->at(cand.ipho);
}//end of SetPhotonValues

void TSelectedEventsTree::SetPhotonIsoValues(TEventTree::InputTreeLeaves& leaf, TFullCuts::Candidate cand, int sample, int channel)
{
  //photon isolation corrections 2012
//  std::cout<<"event="<<_event<<", ipho="<<cand.ipho<<std::endl;

    SetThreeIsolations(leaf, cand, _phoPFChIsoCorr, _phoPFNeuIsoCorr, _phoPFPhoIsoCorr, leaf.phoPFChIso->at(cand.ipho), leaf.phoPFNeuIso->at(cand.ipho), leaf.phoPFPhoIso->at(cand.ipho));

    SetThreeIsolations(leaf, cand, _phoSCRChIsoCorr, _phoSCRNeuIsoCorr, _phoSCRPhoIsoCorr, leaf.phoSCRChIso->at(cand.ipho), leaf.phoSCRNeuIso->at(cand.ipho), leaf.phoSCRPhoIso->at(cand.ipho));

    SetThreeIsolations(leaf, cand, _phoSCRChIso04Corr, _phoSCRNeuIso04Corr, _phoSCRPhoIso04Corr, leaf.phoSCRChIso04->at(cand.ipho), leaf.phoSCRNeuIso04->at(cand.ipho), leaf.phoSCRPhoIso04->at(cand.ipho));

    SetThreeIsolations(leaf, cand, _phoRandConeChIsoCorr, _phoRandConeNeuIsoCorr, _phoRandConePhoIsoCorr, leaf.phoRandConeChIso->at(cand.ipho), leaf.phoRandConeNeuIso->at(cand.ipho), leaf.phoRandConePhoIso->at(cand.ipho));

    SetThreeIsolations(leaf, cand, _phoRandConeChIso04Corr, _phoRandConeNeuIso04Corr, _phoRandConePhoIso04Corr, leaf.phoRandConeChIso04->at(cand.ipho), leaf.phoRandConeNeuIso04->at(cand.ipho), leaf.phoRandConePhoIso04->at(cand.ipho));

  //photon isolation corrections 2011
  _phoEcalIsoDR04Corr=_photon.GetPhoEcalIsoDR04Corr(leaf.phoEcalIsoDR04->at(cand.ipho),leaf.rho2011,leaf.phoSCEta->at(cand.ipho));
  _phoHcalIsoDR04Corr=_photon.GetPhoHcalIsoDR04Corr(leaf.phoHcalIsoDR04->at(cand.ipho),leaf.rho2011,leaf.phoSCEta->at(cand.ipho));
  _phoTrkIsoHollowDR04Corr=_photon.GetPhoTrkIsoHollowDR04Corr(leaf.phoTrkIsoHollowDR04->at(cand.ipho),leaf.rho2011,leaf.phoSCEta->at(cand.ipho));
}//end of SetPhotonIsoValues

void TSelectedEventsTree::SetThreeIsolations(TEventTree::InputTreeLeaves& leaf, TFullCuts::Candidate cand, float &chCorr, float &neuCorr, float &phoCorr, float chOrig, float neuOrig, float phoOrig)
{ 
    chCorr=_photon.GetPhoChIsoCorr(chOrig,leaf.rho2012,leaf.phoSCEta->at(cand.ipho));
    neuCorr=_photon.GetPhoNeuIsoCorr(neuOrig,leaf.rho2012,leaf.phoSCEta->at(cand.ipho));
    phoCorr=_photon.GetPhoPhoIsoCorr(phoOrig,leaf.rho2012,leaf.phoSCEta->at(cand.ipho));
}
