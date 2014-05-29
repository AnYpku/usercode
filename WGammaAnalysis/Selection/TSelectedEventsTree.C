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
  tree->Branch("event",&_event,"event/I");
  tree->Branch("iMCle",&_iMCle,"iMCle/I");
  tree->Branch("leptonEta",&_leEta,"leptonEta/F");
  tree->Branch("leptonPhi",&_lePhi,"leptonPhi/F");
  tree->Branch("leptonPt",&_lePt,"leptonPt/F");
  tree->Branch("leptonGenPID",&_leGenPID,"leptonGenPID/I");
  tree->Branch("leptonGenMomPID",&_leGenMomPID,"leptonGenMomPID/I");
  tree->Branch("leptonGenGMomPID",&_leGenGMomPID,"leptonGenGMomPID/I");
  tree->Branch("leptonId",&_leId,"leptonId/O");
  tree->Branch("leptonIsolation",&_leIsolation,"leptonIsolation/F");
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
  tree->Branch("phoGenMomPID",&_phoGenMomPID,"phoGenMomPID/I");
  tree->Branch("phoGenGMomPID",&_phoGenGMomPID,"phoGenGMomPID/I");
  tree->Branch("phoGenEt",&_phoGenEt,"phoGenEt/F");
  tree->Branch("phoHoverE12",&_phoHoverE12,"phoHoverE12/F");
  tree->Branch("phoSigmaIEtaIEta",&_phoSigmaIEtaIEta,"phoSigmaIEtaIEta/F");
  tree->Branch("phoPFChIsoCorr",&_phoPFChIsoCorr,"phoPFChIsoCorr/F");
  tree->Branch("phoSCRChIsoCorr",&_phoSCRChIsoCorr,"phoSCRChIsoCorr/F");
  tree->Branch("phoPFNeuIsoCorr",&_phoPFNeuIsoCorr,"phoPFNeuIsoCorr/F");
  tree->Branch("phoSCRNeuIsoCorr",&_phoSCRNeuIsoCorr,"phoSCRNeuIsoCorr/F");
  tree->Branch("phoPFPhoIsoCorr",&_phoPFPhoIsoCorr,"phoPFPhoIsoCorr/F");
  tree->Branch("phoSCRPhoIsoCorr",&_phoSCRPhoIsoCorr,"phoSCRPhoIsoCorr/F");
  tree->Branch("lePhoDeltaR",&_lePhoDeltaR,"lePhoDeltaR/F");
  tree->Branch("WMt",&_WMt,"WMt/F");
  tree->Branch("pfMET",&_pfMET,"pfMET/F");
  tree->Branch("pfMETPhi",&_pfMETPhi,"pfMETPhi/F");
  tree->Branch("rho2012",&_rho2012,"rho2012/F");
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

//void TSelectedEventsTree::Fill(TTree* tree)
//{
//  tree->Fill();
//}

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
