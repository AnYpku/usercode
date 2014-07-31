#include "TSelectedEventsTree.h" //this class
#include "../Include/TEventTree.h" //this class
#include "../Include/TPhotonCuts.h" //this class
#include "../Include/TMuonCuts.h" //this class
#include "../Include/TMetTools.h" //this class
#include "../Include/PhosphorCorrectorFunctor.hh"
  //taken from http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/CPena/src/PHOSPHOR_Corr_v2/
  //currently in this package
#include "TTree.h" //ROOT class
#include "TFile.h" //ROOT class
#include "TString.h" //ROOT class
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
  tree->Branch("iMCle",&_iMCle,"iMCle/I");
  tree->Branch("leptonEta",&_leEta,"leptonEta/F");
  tree->Branch("leptonPhi",&_lePhi,"leptonPhi/F");
  tree->Branch("leptonPt",&_lePt,"leptonPt/F");
  tree->Branch("leptonGenPID",&_leGenPID,"leptonGenPID/I");
  tree->Branch("leptonGenParentage",&_leGenParentage,"leptonGenParentage/I");
  tree->Branch("leptonGenMomPID",&_leGenMomPID,"leptonGenMomPID/I");
  tree->Branch("leptonGenGMomPID",&_leGenGMomPID,"leptonGenGMomPID/I");
  tree->Branch("leptonId2012",&_leId2012,"leptonId2012/O");
  tree->Branch("leptonId2011",&_leId2011,"leptonId2011/O");
  tree->Branch("leptonIsolation2012",&_leIsolation2012,"leptonIsolation2012/F");
  tree->Branch("leptonIsolation2011",&_leIsolation2011,"leptonIsolation2011/F");
  tree->Branch("leptonTrg",&_leTrg,"leptonTrg/I");
  tree->Branch("trgMatchIsoMu24eta2p1",&_trgMatchIsoMu24eta2p1,"trgMatchIsoMu24eta2p1/O");
  tree->Branch("trgMatchIsoMu24",&_trgMatchIsoMu24,"trgMatchIsoMu24/O");
  tree->Branch("hasMoreLeptons",&_hasMoreLeptons,"hasMoreLeptons/O");
  tree->Branch("HLT_IsoMu24_eta2p1_",&_HLT_IsoMu24_eta2p1_,"HLT_IsoMu24_eta2p1_/I");
  tree->Branch("HLT_IsoMu24_v",&_HLT_IsoMu24_v,"HLT_IsoMu24_v/I");
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
  tree->Branch("phoSCRChIsoCorr",&_phoSCRChIsoCorr,"phoSCRChIsoCorr/F");
  tree->Branch("phoPFNeuIsoCorr",&_phoPFNeuIsoCorr,"phoPFNeuIsoCorr/F");
  tree->Branch("phoSCRNeuIsoCorr",&_phoSCRNeuIsoCorr,"phoSCRNeuIsoCorr/F");
  tree->Branch("phoPFPhoIsoCorr",&_phoPFPhoIsoCorr,"phoPFPhoIsoCorr/F");
  tree->Branch("phoSCRPhoIsoCorr",&_phoSCRPhoIsoCorr,"phoSCRPhoIsoCorr/F");
  tree->Branch("phoEcalIsoDR04Corr",&_phoEcalIsoDR04Corr,"phoEcalIsoDR04Corr/F");
  tree->Branch("phoHcalIsoDR04Corr",&_phoHcalIsoDR04Corr,"phoHcalIsoDR04Corr/F");
  tree->Branch("phoTrkIsoHollowDR04Corr",&_phoTrkIsoHollowDR04Corr,"phoTrkIsoHollowCorr/F");
  tree->Branch("phohasPixelSeed",&_phohasPixelSeed,"phohasPixelSeed/I");
  tree->Branch("lePhoDeltaR",&_lePhoDeltaR,"lePhoDeltaR/F");
  tree->Branch("WMt",&_WMt,"WMt/F");
  tree->Branch("pfMET",&_pfMET,"pfMET/F");
  tree->Branch("pfMETPhi",&_pfMETPhi,"pfMETPhi/F");
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
  tree->SetBranchAddress("iMCle",&_iMCle,&_b_iMCle);
  tree->SetBranchAddress("leptonEta",&_leEta,&_b_leEta);
  tree->SetBranchAddress("leptonPhi",&_lePhi,&_b_lePhi);
  tree->SetBranchAddress("leptonPt",&_lePt,&_b_lePt);
  tree->SetBranchAddress("leptonGenPID",&_leGenPID,&_b_leGenPID);
  tree->SetBranchAddress("leptonGenParentage",&_leGenParentage,&_b_leGenParentage);
  tree->SetBranchAddress("leptonGenMomPID",&_leGenMomPID,&_b_leGenMomPID);
  tree->SetBranchAddress("leptonGenGMomPID",&_leGenGMomPID,&_b_leGenGMomPID);
  tree->SetBranchAddress("leptonId2012",&_leId2012,&_b_leId2012);
  tree->SetBranchAddress("leptonId2011",&_leId2011,&_b_leId2011);
  tree->SetBranchAddress("leptonIsolation2012",&_leIsolation2012,&_b_leIsolation2012);
  tree->SetBranchAddress("leptonIsolation2011",&_leIsolation2011,&_b_leIsolation2011);
  tree->SetBranchAddress("leptonTrg",&_leTrg,&_b_leTrg);
  tree->SetBranchAddress("trgMatchIsoMu24eta2p1",&_trgMatchIsoMu24eta2p1,&_b_trgMatchIsoMu24eta2p1);
  tree->SetBranchAddress("trgMatchIsoMu24",&_trgMatchIsoMu24,&_b_trgMatchIsoMu24);
  tree->SetBranchAddress("hasMoreLeptons",&_hasMoreLeptons,&_b_hasMoreLeptons);
  tree->SetBranchAddress("HLT_IsoMu24_eta2p1_",&_HLT_IsoMu24_eta2p1_,&_b_HLT_IsoMu24_eta2p1_);
  tree->SetBranchAddress("HLT_IsoMu24_v",&_HLT_IsoMu24_v,&_b_HLT_IsoMu24_v);
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
  tree->SetBranchAddress("phoSCRChIsoCorr",&_phoSCRChIsoCorr,&_b_phoSCRChIsoCorr);
  tree->SetBranchAddress("phoPFNeuIsoCorr",&_phoPFNeuIsoCorr,&_b_phoPFNeuIsoCorr);
  tree->SetBranchAddress("phoSCRNeuIsoCorr",&_phoSCRNeuIsoCorr,&_b_phoSCRNeuIsoCorr);
  tree->SetBranchAddress("phoPFPhoIsoCorr",&_phoPFPhoIsoCorr,&_b_phoPFPhoIsoCorr);
  tree->SetBranchAddress("phoSCRPhoIsoCorr",&_phoSCRPhoIsoCorr,&_b_phoSCRPhoIsoCorr);
  tree->SetBranchAddress("phoEcalIsoDR04Corr",&_phoEcalIsoDR04Corr,&_b_phoEcalIsoDR04Corr);
  tree->SetBranchAddress("phoHcalIsoDR04Corr",&_phoHcalIsoDR04Corr,&_b_phoHcalIsoDR04Corr);
  tree->SetBranchAddress("phoTrkIsoHollowDR04Corr",&_phoTrkIsoHollowDR04Corr,&_b_phoTrkIsoHollowDR04Corr);
  tree->SetBranchAddress("phohasPixelSeed",&_phohasPixelSeed,&_b_phohasPixelSeed);
  tree->SetBranchAddress("lePhoDeltaR",&_lePhoDeltaR,&_b_lePhoDeltaR);
  tree->SetBranchAddress("WMt",&_WMt,&_b_WMt);
  tree->SetBranchAddress("pfMET",&_pfMET,&_b_pfMET);
  tree->SetBranchAddress("pfMETPhi",&_pfMETPhi,&_b_pfMETPhi);
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

void TSelectedEventsTree::SetValues(int channel, int sample, TEventTree::InputTreeLeaves treeLeaf, int ipho, int ile, float lePhoDeltaR, int inputFileN, float weight, float PUweight, float PU,zgamma::PhosphorCorrectionFunctor* photonCorrector)
{

  _event=treeLeaf.event;

  //lepton values
  if (channel==TConfiguration::MUON){
    _leEta=treeLeaf.muEta->at(ile);
    _lePhi=treeLeaf.muPhi->at(ile);
    _lePt=treeLeaf.muPt->at(ile);
    _leTrg=treeLeaf.muTrg->at(ile);
    _iMCle=-1;
    _leGenPID=0;
    _leGenMomPID=0;
    _leGenGMomPID=0;
    TMuonCuts emptyMuon;
    _leId2012=emptyMuon.MuId(2012,treeLeaf.muChi2NDF->at(ile), 
            treeLeaf.muD0->at(ile), treeLeaf.muDz->at(ile), 
            treeLeaf.muNumberOfValidMuonHits->at(ile),
            treeLeaf.muNumberOfValidTrkHits->at(ile), 
            treeLeaf.muNumberOfValidPixelHits->at(ile),
            treeLeaf.muNumberOfValidTrkLayers->at(ile), 
            treeLeaf.muStations->at(ile),
            treeLeaf.muType->at(ile));
    _leId2011=emptyMuon.MuId(2011,treeLeaf.muChi2NDF->at(ile), 
            treeLeaf.muD0->at(ile), treeLeaf.muDz->at(ile), 
            treeLeaf.muNumberOfValidMuonHits->at(ile),
            treeLeaf.muNumberOfValidTrkHits->at(ile), 
            treeLeaf.muNumberOfValidPixelHits->at(ile),
            treeLeaf.muNumberOfValidTrkLayers->at(ile), 
            treeLeaf.muStations->at(ile),
            treeLeaf.muType->at(ile));
    _leIsolation2012=emptyMuon.MuIsolation2012(treeLeaf.muPt->at(ile), 
                   treeLeaf.muPFIsoR04_NH->at(ile), 
                   treeLeaf.muPFIsoR04_Pho->at(ile), 
                   treeLeaf.muPFIsoR04_PU->at(ile),
                   treeLeaf.muPFIsoR04_CH->at(ile));
    _leIsolation2011=emptyMuon.MuIsolation2011(treeLeaf.muPt->at(ile), 
                   treeLeaf.muIsoTrk->at(ile), 
                   treeLeaf.muIsoEcal->at(ile), 
                   treeLeaf.muIsoHcal->at(ile),
                   treeLeaf.rho2011);
    _hasMoreLeptons=emptyMuon.HasMoreMuons(treeLeaf.nMu, ile, 
                      treeLeaf.muPt, treeLeaf.muEta);
    _HLT_IsoMu24_eta2p1_=treeLeaf.HLT[treeLeaf.HLTIndex[18]];
    _HLT_IsoMu24_v=treeLeaf.HLT[treeLeaf.HLTIndex[19]];
    _trgMatchIsoMu24eta2p1=treeLeaf.muTrg->at(ile)%2;
    _trgMatchIsoMu24=(treeLeaf.muTrg->at(ile)/2)%2;
  }
  else if (channel==TConfiguration::ELECTRON);

  //photon values
  _iMCpho=-1;
  _phoGenPID=0;
  _phoGenParentage=0;
  _phoGenMomPID=0;
  _phoGenGMomPID=0;
  if (!treeLeaf.isData){
    for (int iMC=0; iMC<treeLeaf.nMC; iMC++){
      if(treeLeaf.mcIndex->at(iMC)==treeLeaf.phoGenIndex->at(ipho)){
        _phoGenPID=treeLeaf.mcPID->at(iMC);
        _phoGenParentage=treeLeaf.mcParentage->at(iMC);
        _phoGenMomPID=treeLeaf.mcMomPID->at(iMC);
        _phoGenGMomPID=treeLeaf.mcGMomPID->at(iMC);
        _phoGenEt=treeLeaf.mcEt->at(iMC);
        _iMCpho=iMC;
      }
      if( (channel==TConfiguration::MUON && 
           treeLeaf.mcIndex->at(iMC)==treeLeaf.muGenIndex->at(ile)) ||
          (channel==TConfiguration::ELECTRON && 
           treeLeaf.mcIndex->at(iMC)==treeLeaf.eleGenIndex->at(ile)) ){
        _leGenPID=treeLeaf.mcPID->at(iMC);
        _leGenParentage=treeLeaf.mcParentage->at(iMC);
        _leGenMomPID=treeLeaf.mcMomPID->at(iMC);
        _leGenGMomPID=treeLeaf.mcGMomPID->at(iMC);
        _iMCle=iMC;
       }
    }//end of loop over iMC
  }//end of if (!treeLeaf.isData)

  _phoEleVeto=treeLeaf.phoEleVeto->at(ipho);
  _phoEta=treeLeaf.phoEta->at(ipho);
  _phoPhi=treeLeaf.phoPhi->at(ipho);
  _phoEtNoPhosphor=treeLeaf.phoEt->at(ipho);

  if (treeLeaf.isData)
       _phoEt = photonCorrector->GetCorrEtData(treeLeaf.phoR9->at(ipho), 2012, treeLeaf.phoEt->at(ipho), treeLeaf.phoEta->at(ipho));
       //Phosphor correction needs to be applied for the photon Et only, 
       //not for SC Et
  else if (_iMCpho >= 0) 
    _phoEt = photonCorrector->GetCorrEtMC(treeLeaf.phoR9->at(ipho), 2012, treeLeaf.phoEt->at(ipho), treeLeaf.phoEta->at(ipho), treeLeaf.mcE->at(_iMCpho));

  _phoSCEta=treeLeaf.phoSCEta->at(ipho);
  _phoSCPhi=treeLeaf.phoSCPhi->at(ipho);
  _phoSCEt=treeLeaf.phoSCEt->at(ipho);
  _phoHoverE=treeLeaf.phoHoverE->at(ipho);
  _phoHoverE12=treeLeaf.phoHoverE12->at(ipho);
  _phoSigmaIEtaIEta=treeLeaf.phoSigmaIEtaIEta->at(ipho);
  _phohasPixelSeed=treeLeaf.phohasPixelSeed->at(ipho);

  TPhotonCuts emptyPhoton;
  //photon isolation corrections 2012
  _phoPFChIsoCorr=emptyPhoton.GetPhoChIsoCorr(treeLeaf.phoPFChIso->at(ipho),treeLeaf.rho2012,treeLeaf.phoEta->at(ipho));
  _phoSCRChIsoCorr=emptyPhoton.GetPhoChIsoCorr(treeLeaf.phoSCRChIso->at(ipho),treeLeaf.rho2012,treeLeaf.phoEta->at(ipho));
  _phoPFNeuIsoCorr=emptyPhoton.GetPhoNeuIsoCorr(treeLeaf.phoPFNeuIso->at(ipho),treeLeaf.rho2012,treeLeaf.phoEta->at(ipho));
  _phoSCRNeuIsoCorr=emptyPhoton.GetPhoNeuIsoCorr(treeLeaf.phoSCRNeuIso->at(ipho),treeLeaf.rho2012,treeLeaf.phoEta->at(ipho));
  _phoPFPhoIsoCorr=emptyPhoton.GetPhoPhoIsoCorr(treeLeaf.phoPFPhoIso->at(ipho),treeLeaf.rho2012,treeLeaf.phoEta->at(ipho));
  _phoSCRPhoIsoCorr=emptyPhoton.GetPhoPhoIsoCorr(treeLeaf.phoSCRPhoIso->at(ipho),treeLeaf.rho2012,treeLeaf.phoEta->at(ipho));
  //photon isolation corrections 2011
  _phoEcalIsoDR04Corr=emptyPhoton.GetPhoEcalIsoDR04Corr(treeLeaf.phoEcalIsoDR04->at(ipho),treeLeaf.rho2011,treeLeaf.phoEta->at(ipho));
  _phoHcalIsoDR04Corr=emptyPhoton.GetPhoHcalIsoDR04Corr(treeLeaf.phoHcalIsoDR04->at(ipho),treeLeaf.rho2011,treeLeaf.phoEta->at(ipho));
  _phoTrkIsoHollowDR04Corr=emptyPhoton.GetPhoTrkIsoHollowDR04Corr(treeLeaf.phoTrkIsoHollowDR04->at(ipho),treeLeaf.rho2011,treeLeaf.phoEta->at(ipho));

  _lePhoDeltaR=lePhoDeltaR;

   //pfMET smearing
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
 
   }//end of "if (!inpTreeLeaf.isData)"
   else{
     _pfMET=treeLeaf.pfMET;
     _pfMETPhi=treeLeaf.pfMETPhi;
   }

  _WMt = sqrt(2*_lePt*_pfMET*(1-cos(_lePhi-_pfMETPhi)));

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
}
