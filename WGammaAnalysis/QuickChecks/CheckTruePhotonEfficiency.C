#include "../Include/TEventTree.h"
#include "../Include/TPhotonCuts.h"
#include "../Configuration/TConfiguration.h"
#include "TTree.h"
#include "TFile.h"
#include <iostream>
#include <iomanip>

void CheckTruePhotonEfficiency(int year, int wp, TString isoBase)
{
  TEventTree eventTree;
  TFile f("/mnt/hadoop/user/uscms01/pnfs/unl.edu/data4/cms/store/user/eavdeeva2/WGammaMCsummer12New/job_summer12_Wg_munu.root");
  f.IsOpen(); 
  f.cd("ggNtuplizer");
  TTree* tree = (TTree*)gDirectory->Get("EventTree");
  eventTree.Init(tree);
  TConfiguration config;
  long nentries = tree->GetEntries();
  enum {COM, BAR, END, GAP};
  long nTotal=0;
  long nPhotons[4]={0,0,0,0};
  long nPassedSigmaIEtaIEta[4]={0,0,0,0};
  long nPassedHoverE[4]={0,0,0,0};
  long nPassedPhoEleVeto[4]={0,0,0,0};
  long nPassedPhoHasPixelSeed[4]={0,0,0,0};
  long nPassedIsoChTrk[4]={0,0,0,0};
  long nPassedIsoNeuHcal[4]={0,0,0,0};
  long nPassedIsoPhoEcal[4]={0,0,0,0};
//  nentries = 1000;
  TPhotonCuts photon;
  for (long ientry=0; ientry<nentries; ientry++){
    eventTree.GetEntryNeededBranchesOnly(config.MUON,config.SIGMC,ientry);
    for (int imc=0; imc<eventTree.treeLeaf.nMC; imc++){
      if (eventTree.treeLeaf.mcPID->at(imc)==22 && 
          eventTree.treeLeaf.mcMomPID->at(imc)>=-24 && eventTree.treeLeaf.mcMomPID->at(imc)<=24);
      else continue;
      for (int ipho=0; ipho<eventTree.treeLeaf.nPho; ipho++){
        if (eventTree.treeLeaf.phoGenIndex->at(ipho)==eventTree.treeLeaf.mcIndex->at(imc));
        else continue;
        int etaBin;
        float eta = eventTree.treeLeaf.phoEta->at(ipho);
        if (photon.IsBarrel(eta)) etaBin=BAR;
        else if (photon.IsEndcap(eta)) etaBin=END;
        else etaBin=GAP;
        nTotal++;
        if (etaBin==BAR || etaBin==END) {nPhotons[COM]++; nPhotons[etaBin]++;}
        else continue;
        float sigietaieta = eventTree.treeLeaf.phoSigmaIEtaIEta->at(ipho);
        if ((etaBin==BAR && sigietaieta<photon.GetPhoSigmaIEtaIEtaCutB(year,wp))|| 
            (etaBin==END && sigietaieta<photon.GetPhoSigmaIEtaIEtaCutE(year,wp))  )
          {nPassedSigmaIEtaIEta[COM]++; nPassedSigmaIEtaIEta[etaBin]++;}
        else continue; 
        if ((year==2011 && eventTree.treeLeaf.phoHoverE->at(ipho)<0.05) || 
            (year==2012 && eventTree.treeLeaf.phoHoverE12->at(ipho)<0.05)  )
          {nPassedHoverE[COM]++; nPassedHoverE[etaBin]++;}
        else continue;
        if (eventTree.treeLeaf.phoEleVeto->at(ipho)==0)
          {nPassedPhoEleVeto[COM]++; nPassedPhoEleVeto[etaBin]++;}
        else continue;
        if (year==2011 && eventTree.treeLeaf.phohasPixelSeed->at(ipho)==0)
          {nPassedPhoHasPixelSeed[COM]++; nPassedPhoHasPixelSeed[etaBin]++;}
        else continue;

        float phoEt = eventTree.treeLeaf.phoSCEt->at(ipho); 
        float rho; 
        if (year==2011) rho=eventTree.treeLeaf.rho2011;
        if (year==2012) rho=eventTree.treeLeaf.rho2012;

        float isoChTrk; 
        if (year==2011) isoChTrk = photon.GetPhoTrkIsoHollowDR04Corr(eventTree.treeLeaf.phoTrkIsoHollowDR04->at(ipho), rho, eta);
        if (year==2012) isoChTrk = photon.GetPhoChIsoCorr(eventTree.treeLeaf.phoPFChIso->at(ipho),rho,eta);
        if ((etaBin==BAR && isoChTrk<photon.GetOneIsolationCutB(year,wp,photon.ISO_CHorTRK,phoEt)) ||
            (etaBin==END && isoChTrk<photon.GetOneIsolationCutE(year,wp,photon.ISO_CHorTRK,phoEt))  )
          {nPassedIsoChTrk[COM]++; nPassedIsoChTrk[etaBin]++;}
        else continue;

        float isoNeuHcal; 
        if (year==2011) isoNeuHcal = photon.GetPhoHcalIsoDR04Corr(eventTree.treeLeaf.phoHcalIsoDR04->at(ipho), rho, eta);
        if (year==2012) isoNeuHcal = photon.GetPhoNeuIsoCorr(eventTree.treeLeaf.phoPFNeuIso->at(ipho),rho,eta);
        if ((etaBin==BAR && isoNeuHcal<photon.GetOneIsolationCutB(year,wp,photon.ISO_NEUorHCAL,phoEt)) ||
            (etaBin==END && isoNeuHcal<photon.GetOneIsolationCutE(year,wp,photon.ISO_NEUorHCAL,phoEt))  )
          {nPassedIsoNeuHcal[COM]++; nPassedIsoNeuHcal[etaBin]++;}
        else continue;

        float isoPhoEcal; 
        if (year==2011) isoPhoEcal = photon.GetPhoEcalIsoDR04Corr(eventTree.treeLeaf.phoEcalIsoDR04->at(ipho), rho, eta);
        if (year==2012) isoPhoEcal = photon.GetPhoPhoIsoCorr(eventTree.treeLeaf.phoPFPhoIso->at(ipho),rho,eta);
        if ((etaBin==BAR && isoPhoEcal<photon.GetOneIsolationCutB(year,wp,photon.ISO_PHOorECAL,phoEt)) ||
            (etaBin==END && isoPhoEcal<photon.GetOneIsolationCutE(year,wp,photon.ISO_PHOorECAL,phoEt))  )
          {nPassedIsoPhoEcal[COM]++; nPassedIsoPhoEcal[etaBin]++;}
        else continue;
      }//end of loop over ipho
    }//end of loop over imc
  }//end of loop over ientry
  std::cout<<"cut level;      COMMON;  BARREL;  ENDCAP;  GAP"<<std::endl;
  std::cout<<"nPhoTot = "<<nTotal<<std::endl;
  std::cout<<"n B or E = ";
  for (int i=0; i<4; i++)  std::cout<<nPhotons[i]<<";  ";
  std::cout<<std::endl;
  std::cout<<"nP sietaieta = ";
  for (int i=0; i<3; i++)  std::cout<<nPassedSigmaIEtaIEta[i]<<" ("<<std::setprecision(2)<<1.0*nPassedSigmaIEtaIEta[i]/nPhotons[i]<<");  ";
  std::cout<<std::endl;
  std::cout<<"nP H/E = ";
  for (int i=0; i<3; i++)  std::cout<<nPassedHoverE[i]<<" ("<<std::setprecision(2)<<1.0*nPassedHoverE[i]/nPassedSigmaIEtaIEta[i]<<");  ";
  std::cout<<std::endl;
  std::cout<<"nP phoEleVeto = ";
  for (int i=0; i<3; i++)  std::cout<<nPassedPhoEleVeto[i]<<" ("<<std::setprecision(2)<<1.0*nPassedPhoEleVeto[i]/nPassedHoverE[i]<<");  ";
  std::cout<<std::endl;
  std::cout<<"nP phohasPixelSeed = ";
  for (int i=0; i<3; i++)  std::cout<<nPassedPhoHasPixelSeed[i]<<" ("<<std::setprecision(2)<<1.0*nPassedPhoHasPixelSeed[i]/nPassedPhoEleVeto[i]<<");  ";
  std::cout<<std::endl;
  std::cout<<"nP iso ch/trk = ";
  for (int i=0; i<3; i++)  std::cout<<nPassedIsoChTrk[i]<<" ("<<std::setprecision(2)<<1.0*nPassedIsoChTrk[i]/nPassedPhoHasPixelSeed[i]<<");  ";
  std::cout<<std::endl;
  std::cout<<"nP iso neu/hcal = ";
  for (int i=0; i<3; i++)  std::cout<<nPassedIsoNeuHcal[i]<<" ("<<std::setprecision(2)<<1.0*nPassedIsoNeuHcal[i]/nPassedIsoChTrk[i]<<");  ";
  std::cout<<std::endl;
  std::cout<<"nP iso pho/ecal = ";
  for (int i=0; i<3; i++)  std::cout<<nPassedIsoPhoEcal[i]<<" ("<<std::setprecision(2)<<1.0*nPassedIsoPhoEcal[i]/nPassedIsoNeuHcal[i]<<");  ";
  std::cout<<std::endl;
  std::cout<<"efficiency = ";
  for (int i=0; i<3; i++)  std::cout<<" ("<<std::setprecision(2)<<1.0*nPassedIsoPhoEcal[i]/nPhotons[i]<<");  ";
  std::cout<<std::endl;
 
}
