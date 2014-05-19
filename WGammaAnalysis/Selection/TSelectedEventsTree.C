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

void TSelectedEventsTree::SetOutputTree(TTree* tree)
{
  _outTree=tree;
  _outTree->Branch("event",&_event,"event/I");
  _outTree->Branch("iMCle",&_iMCle,"iMCle/I");
  _outTree->Branch("leptonEta",&_leEta,"leptonEta/F");
  _outTree->Branch("leptonPhi",&_lePhi,"leptonPhi/F");
  _outTree->Branch("leptonPt",&_lePt,"leptonPt/F");
  _outTree->Branch("leptonGenPID",&_leGenPID,"leptonGenPID/I");
  _outTree->Branch("leptonGenMomPID",&_leGenMomPID,"leptonGenMomPID/I");
  _outTree->Branch("leptonGenGMomPID",&_leGenGMomPID,"leptonGenGMomPID/I");
  _outTree->Branch("leptonId",&_leId,"leptonId/O");
  _outTree->Branch("leptonIsolation",&_leIsolation,"leptonIsolation/F");
  _outTree->Branch("leptonTrg",&_leTrg,"leptonTrg/I");
  _outTree->Branch("trgMatchIsoMu24eta2p1",&_trgMatchIsoMu24eta2p1,"trgMatchIsoMu24eta2p1/O");
  _outTree->Branch("trgMatchIsoMu24",&_trgMatchIsoMu24,"trgMatchIsoMu24/O");
  _outTree->Branch("hasMoreLeptons",&_hasMoreLeptons,"hasMoreLeptons/O");
  _outTree->Branch("HLT_IsoMu24_eta2p1_",&_HLT_IsoMu24_eta2p1_,"HLT_IsoMu24_eta2p1_/I");
  _outTree->Branch("HLT_IsoMu24_v",&_HLT_IsoMu24_v,"HLT_IsoMu24_v/I");
  _outTree->Branch("iMCpho",&_iMCpho,"iMCpho/I");
  _outTree->Branch("phoElectronVeto",&_phoEleVeto,"phoElectronVeto/I");
  _outTree->Branch("phoEta",&_phoEta,"phoEta/F");
  _outTree->Branch("phoPhi",&_phoPhi,"phoPhi/F");
  _outTree->Branch("phoEt",&_phoEt,"phoEt/F");
  _outTree->Branch("phoEtNoPhosphor",&_phoEtNoPhosphor,"phoEtNoPhosphor/F");
  _outTree->Branch("phoSCEta",&_phoSCEta,"phoSCEta/F");
  _outTree->Branch("phoSCPhi",&_phoSCPhi,"phoSCPhi/F");
  _outTree->Branch("phoSCEt",&_phoSCEt,"phoSCEt/F");
  _outTree->Branch("phoGenPID",&_phoGenPID,"phoGenPID/I");
  _outTree->Branch("phoGenMomPID",&_phoGenMomPID,"phoGenMomPID/I");
  _outTree->Branch("phoGenGMomPID",&_phoGenGMomPID,"phoGenGMomPID/I");
  _outTree->Branch("phoGenEt",&_phoGenEt,"phoGenEt/F");
  _outTree->Branch("phoHoverE12",&_phoHoverE12,"phoHoverE12/F");
  _outTree->Branch("phoSigmaIEtaIEta",&_phoSigmaIEtaIEta,"phoSigmaIEtaIEta/F");
  _outTree->Branch("phoPFChIsoCorr",&_phoPFChIsoCorr,"phoPFChIsoCorr/F");
  _outTree->Branch("phoSCRChIsoCorr",&_phoSCRChIsoCorr,"phoSCRChIsoCorr/F");
  _outTree->Branch("phoPFNeuIsoCorr",&_phoPFNeuIsoCorr,"phoPFNeuIsoCorr/F");
  _outTree->Branch("phoSCRNeuIsoCorr",&_phoSCRNeuIsoCorr,"phoSCRNeuIsoCorr/F");
  _outTree->Branch("phoPFPhoIsoCorr",&_phoPFPhoIsoCorr,"phoPFPhoIsoCorr/F");
  _outTree->Branch("phoSCRPhoIsoCorr",&_phoSCRPhoIsoCorr,"phoSCRPhoIsoCorr/F");
  _outTree->Branch("lePhoDeltaR",&_lePhoDeltaR,"lePhoDeltaR/F");
  _outTree->Branch("WMt",&_WMt,"WMt/F");
  _outTree->Branch("pfMET",&_pfMET,"pfMET/F");
  _outTree->Branch("pfMETPhi",&_pfMETPhi,"pfMETPhi/F");
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
    _leId=emptyMuon.MuId(treeLeaf.muChi2NDF->at(ile), 
            treeLeaf.muD0->at(ile), treeLeaf.muDz->at(ile), 
            treeLeaf.muNumberOfValidMuonHits->at(ile),
            treeLeaf.muNumberOfValidTrkHits->at(ile), 
            treeLeaf.muNumberOfValidPixelHits->at(ile),
            treeLeaf.muNumberOfValidTrkLayers->at(ile), 
            treeLeaf.muStations->at(ile));
    _leIsolation=emptyMuon.MuIsolation(treeLeaf.muPt->at(ile), 
                   treeLeaf.muPFIsoR04_NH->at(ile), 
                   treeLeaf.muPFIsoR04_Pho->at(ile), 
                   treeLeaf.muPFIsoR04_PU->at(ile),
                   treeLeaf.muPFIsoR04_CH->at(ile));
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
  _phoGenMomPID=0;
  _phoGenGMomPID=0;
  if (!treeLeaf.isData){
    for (int iMC=0; iMC<treeLeaf.nMC; iMC++){
      if(treeLeaf.mcIndex->at(iMC)==treeLeaf.phoGenIndex->at(ipho)){
        _phoGenPID=treeLeaf.mcPID->at(iMC);
        _phoGenMomPID=treeLeaf.mcMomPID->at(iMC);
        _phoGenGMomPID=treeLeaf.mcGMomPID->at(iMC);
        _phoGenEt=treeLeaf.mcEt->at(iMC);
        _iMCpho=iMC;
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
  _phoHoverE12=treeLeaf.phoHoverE12->at(ipho);
  _phoSigmaIEtaIEta=treeLeaf.phoSigmaIEtaIEta->at(ipho);

  //photon isolation corrections
  TPhotonCuts emptyPhoton;
  _phoPFChIsoCorr=emptyPhoton.GetPhoChIsoCorr(treeLeaf.phoPFChIso->at(ipho),treeLeaf.rho2012,treeLeaf.phoEta->at(ipho));
  _phoSCRChIsoCorr=emptyPhoton.GetPhoChIsoCorr(treeLeaf.phoSCRChIso->at(ipho),treeLeaf.rho2012,treeLeaf.phoEta->at(ipho));
  _phoPFNeuIsoCorr=emptyPhoton.GetPhoNeuIsoCorr(treeLeaf.phoPFNeuIso->at(ipho),treeLeaf.rho2012,treeLeaf.phoEta->at(ipho));
  _phoSCRNeuIsoCorr=emptyPhoton.GetPhoNeuIsoCorr(treeLeaf.phoSCRNeuIso->at(ipho),treeLeaf.rho2012,treeLeaf.phoEta->at(ipho));
  _phoPFPhoIsoCorr=emptyPhoton.GetPhoNeuIsoCorr(treeLeaf.phoPFPhoIso->at(ipho),treeLeaf.rho2012,treeLeaf.phoEta->at(ipho));
  _phoSCRPhoIsoCorr=emptyPhoton.GetPhoNeuIsoCorr(treeLeaf.phoSCRPhoIso->at(ipho),treeLeaf.rho2012,treeLeaf.phoEta->at(ipho));

  _lePhoDeltaR=lePhoDeltaR;

   //pfMET smearing
   if (!treeLeaf.isData) {
     TMetTools met(treeLeaf.event, treeLeaf.pfMET, treeLeaf.pfMETPhi,
              //inpTreeLeaf.nLowPtJet, inpTreeLeaf.jetLowPtRawPt,
              //inpTreeLeaf.jetLowPtRawEn, inpTreeLeaf.jetLowPtPt,  
              //inpTreeLeaf.jetLowPtEta, inpTreeLeaf.jetLowPtPhi,
              //inpTreeLeaf.jetLowPtGenJetPt, inpTreeLeaf.jetLowPtGenJetEta, 
              //inpTreeLeaf.jetLowPtGenJetPhi, 
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
