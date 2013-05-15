#include "../Include/TMuonCuts.h"
#include "../Include/TPhotonCuts.h"
#include <iostream>
#include "TTree.h"

void CheckCuts(TTree* tr)
{

  TEventTree evTr;
  evTr.Init(tr);
  evTr.SetMCSpecificAddresses();
  

  int nMuonsGen = 0;
  int nMuonsGenKinAcc = 0;
  int nMuonsRec = 0;
  int nMuonsRecKinAcc = 0;
  int nMuonsTrigPass = 0;
  int nMuonsId = 0;
  int nMuonsIso = 0;

  int nPhotonsGen = 0;
  int nPhotonsGenKinAcc = 0;
  int nPhotonsRec = 0;
  int nPhotonsRecKinAcc = 0;
  int nPhotonsId = 0;

  long nentries = tr->GetEntries();
  //nentries = 100000;

  TFile fOut("testGenHists.root","RECREATE");
  TTree* trOutMu = new TTree ("trGenMu", "trGenMu");
  TTree* trOutPho = new TTree ("trGenPho", "trGenPho");
  float genMuPt = 0;
  float genPhoEt = 0;
  float genMuEta = 0;
  float genPhoEta = 0;
  trOutMu->Branch("genMuPt",&genMuPt,"genMuPt/F");
  trOutMu->Branch("genMuEta",&genMuEta,"genMuEta/F");
  trOutPho->Branch("genPhoEt",&genPhoEt,"genPhoEt/F");
  trOutPho->Branch("genPhoEta",&genPhoEta,"genPhoEta/F");
  

  for (long entry=0; entry<nentries; entry++)
    {
      evTr.GetEntryNeededBranchesOnly(entry);
      evTr.GetEntryMCSpecific(entry);

      if (evTr.nMC==0) continue;

      for (int iMC=0; iMC<evTr.nMC; iMC++)
        {

          /////////////////////////////////////
          // muons check starts
          /////////////////////////////////////

          if (fabs(evTr.mcPID[iMC])==13){
            genMuPt=evTr.mcPt[iMC];
            genMuEta=evTr.mcEta[iMC];
            trOutMu->Fill();
            nMuonsGen++;
            TMuonCuts muonEmpty;
            if (muonEmpty.MuKinematics(evTr.mcPt[iMC],evTr.mcEta[iMC])){
              nMuonsGenKinAcc++;
              for (int imu=0; imu<evTr.nMu; imu++)
                {
                  if (evTr.mcIndex[iMC]==evTr.muGenIndex[imu]){
                     nMuonsRec++;
                     TMuonCuts muon (imu, evTr.muPt[imu], evTr.muPt, evTr.muEta[imu],  evTr.muEta, evTr.nMu, evTr.muNumberOfValidPixelHits[imu], evTr.muNumberOfValidTrkHits[imu],
evTr.muNumberOfValidTrkLayers[imu], evTr.muNumberOfValidMuonHits[imu], evTr.muStations[imu], evTr.HLT[evTr.HLTIndex[18]], evTr.HLT[evTr.HLTIndex[19]], evTr.muTrg[imu][0],  evTr.muTrg[imu][1], evTr.muChi2NDF[imu], evTr.muD0[imu], evTr.muDz[imu], evTr.muPFIsoR04_CH[imu],
evTr.muPFIsoR04_NH[imu], evTr.muPFIsoR04_Pho[imu], evTr.muPFIsoR04_PU[imu]);
                     if (muon.MuKinematics(evTr.muPt[imu],evTr.muEta[imu])){
                       nMuonsRecKinAcc++;
                       if (muon.MuTriggerMatch())
                         {
                            nMuonsTrigPass++;
                            if (muon.MuId()){
                              nMuonsId++;
                              if (muon.MuIsolation()){
                                nMuonsIso++;
                              }  //end of if (muon.MuIsolation())
                            } //end of if (muon.MuId())
                         } //end of if (muon.MuTriggerMatch())
                     }//end of  if (muon.MuKinematics()
                  } //end of if (mcIndex[iMC]==muGenIndex[imu])
                } //end of loop over muons
            } //end of if (muonEmpty.MuKinematics())
          } //end of if (fabs(mcPID[iMC])==13)

          /////////////////////////////////////
          // muons check ends
          /////////////////////////////////////

          /////////////////////////////////////
          // photons check starts
          /////////////////////////////////////

          if (fabs(evTr.mcPID[iMC])==22){
            genPhoEt=evTr.mcEt[iMC];
            genPhoEta=evTr.mcEta[iMC];
            trOutPho->Fill();
            nPhotonsGen++;
            TPhotonCuts photonEmpty;
            if (photonEmpty.PhoKinematics(evTr.mcEt[iMC],evTr.mcEta[iMC])){
              nPhotonsGenKinAcc++;
              for (int ipho=0; ipho<evTr.nPho; ipho++)
                {
                  if (evTr.mcIndex[iMC]==evTr.phoGenIndex[ipho]){
                     nPhotonsRec++;
                     TPhotonCuts photon(evTr.phoEleVeto[ipho], evTr.phoEt[ipho],
evTr.phoEta[ipho], evTr.phoSCEt[ipho], evTr.phoSCEta[ipho], evTr.phoHoverE12[ipho],  evTr.phoSigmaIEtaIEta[ipho], evTr.phoPFChIso[ipho], evTr.phoPFNeuIso[ipho],  evTr.phoPFPhoIso[ipho], evTr.rho2012);
                     if (photon.PhoKinematics(evTr.phoSCEt[ipho],evTr.muEta[ipho])){
                       nPhotonsRecKinAcc++;
                       if (photon.SimpleCutBasedPhotonID2012())
                         {
                            nPhotonsId++;
                         } //end of if (photon.SimpleCutBasedPhotonID2012())
                     }//end of  if (photon.PhoKinematics()
                  } //end of if (mcIndex[iMC]==phoGenIndex[ipho])
                } //end of loop over photons
            } //end of if (photonEmpty.PhoKinematics())
          } //end of if (fabs(mcPID[iMC])==22)

          /////////////////////////////////////
          // photons check ends
          /////////////////////////////////////


        } //end of loop over iMC
      
    } //end of loop over entry 

  trOutMu->Write();
  trOutPho->Write();

  std::cout<<"nMuonsGen = "<<nMuonsGen<<std::endl;
  std::cout<<"nMuonsGenKinAcc = "<<nMuonsGenKinAcc<<std::endl;
  std::cout<<"nMuonsRec = "<<nMuonsRec<<std::endl;
  std::cout<<"nMuonsRecKinAcc = "<<nMuonsRecKinAcc<<std::endl;
  std::cout<<"nMuonsTrigPass = "<<nMuonsTrigPass<<std::endl;
  std::cout<<"nMuonsId = "<<nMuonsId<<std::endl;
  std::cout<<"nMuonsIso = "<<nMuonsIso<<std::endl;

  std::cout<<"nPhotonsGen = "<<nPhotonsGen<<std::endl;
  std::cout<<"nPhotonsGenKinAcc = "<<nPhotonsGenKinAcc<<std::endl;
  std::cout<<"nPhotonsRec = "<<nPhotonsRec<<std::endl;
  std::cout<<"nPhotonsRecKinAcc = "<<nPhotonsRecKinAcc<<std::endl;
  std::cout<<"nPhotonsId = "<<nPhotonsId<<std::endl;

}
