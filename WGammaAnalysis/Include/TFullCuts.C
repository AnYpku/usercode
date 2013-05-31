#include "TFullCuts.h"
#include "TEventTree.h"
#include "TMetTools.h"
#include "TMuonCuts.h"
#include "TElectronCuts.h"
#include "TPhotonCuts.h"
#include "../Configuration/TInputSample.h"
  //this package

#include "PhosphorCorrectorFunctor.hh"
  //taken from http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/CPena/src/PHOSPHOR_Corr_v2/
  //currently in this package

TFullCuts::TFullCuts()
{
}

TFullCuts::~TFullCuts()
{
}

//bool TFullCuts::Cut(bool** goodLeptonPhotonPairs, 
//                    TEventTree::InputTreeLeaves &inpTreeLeaf,   
//                    int channel, bool isReleasedCutsMode,
//                    float* WMt, float** lePhoDeltaR,
//                    zgamma::PhosphorCorrectionFunctor* photonCorrector)
bool TFullCuts::Cut(bool** goodLeptonPhotonPairs, 
                    TEventTree::InputTreeLeaves &inpTreeLeaf,   
                    int channel, bool isReleasedCutsMode,
                    float* WMt, float** lePhoDeltaR,
                    zgamma::PhosphorCorrectionFunctor* photonCorrector)
{

  // This function is called from LoopOverEvents.
  // returns  1 if entry is accepted.
  // returns 0 otherwise.

//   nTotal_++;

   int nLe=0;
   if (channel==TInputSample::MUON) nLe=inpTreeLeaf.nMu;
   else if (channel==TInputSample::ELECTRON) nLe=inpTreeLeaf.nEle;

   if ((inpTreeLeaf.IsVtxGood)==-1) return 0;
   if (inpTreeLeaf.nPho < 1) return 0; 
   if (nLe < 1) return 0;
      //these variables are fields of TEventTree
   if (!inpTreeLeaf.metFilters[6]) return 0;
      //metFilters - is field of TEventTree
      //[6] - ecalLaserCorrFilter
      //necessary to remove spikes in the photon distributions 
      //for Jul13 rereco


   if (!inpTreeLeaf.isData) {
     //MET smearing
     TMetTools met(inpTreeLeaf.event, inpTreeLeaf.pfMET, inpTreeLeaf.pfMETPhi,
              inpTreeLeaf.nLowPtJet, inpTreeLeaf.jetLowPtRawPt,
              inpTreeLeaf.jetLowPtRawEn, inpTreeLeaf.jetLowPtPt,  
              inpTreeLeaf.jetLowPtEta, inpTreeLeaf.jetLowPtPhi,
              inpTreeLeaf.jetLowPtGenJetPt, inpTreeLeaf.jetLowPtGenJetEta, 
              inpTreeLeaf.jetLowPtGenJetPhi, inpTreeLeaf.nJet, 
              inpTreeLeaf.jetRawPt, inpTreeLeaf.jetRawEn, 
              inpTreeLeaf.jetPt, inpTreeLeaf.jetEta, 
              inpTreeLeaf.jetPhi, inpTreeLeaf.jetGenJetPt, 
              inpTreeLeaf.jetGenJetEta, inpTreeLeaf.jetGenJetPhi);
     met.METSmearCorrection();
     inpTreeLeaf.pfMET = met.GetRecoPfMET();
     inpTreeLeaf.pfMETPhi = met.GetRecoPfMETPhi();
  
   }

   bool goodLeptonPhotonPairsExist=0;
   for (int ipho=0; ipho<inpTreeLeaf.nPho; ipho++)
     for (int ile=0; ile<nLe; ile++) 
       goodLeptonPhotonPairs[ile][ipho]=0;   

   bool goodLepton[nLe];
   bool goodLeptonExists=0;
   bool thisLeptonPassed;

//   nBeforeLeptonLoop_++;

   for (int ile=0; ile<nLe; ile++) 
   //start of lepton loop
     { 

//        nLeptons_++; 

        thisLeptonPassed=0;        
        if (channel==TInputSample::MUON)
          {
            TMuonCuts muon(ile,inpTreeLeaf.muPt[ile],
               inpTreeLeaf.muPt,inpTreeLeaf.muEta[ile],inpTreeLeaf.muEta,
               inpTreeLeaf.nMu,inpTreeLeaf.muNumberOfValidPixelHits[ile],
               inpTreeLeaf.muNumberOfValidTrkHits[ile],
               inpTreeLeaf.muNumberOfValidTrkLayers[ile],
               inpTreeLeaf.muNumberOfValidMuonHits[ile],
               inpTreeLeaf.muStations[ile],  
               inpTreeLeaf.HLT[inpTreeLeaf.HLTIndex[18]],
               inpTreeLeaf.HLT[inpTreeLeaf.HLTIndex[19]],
               inpTreeLeaf.muTrg[ile][0],inpTreeLeaf.muTrg[ile][1],
               inpTreeLeaf.muChi2NDF[ile],
               inpTreeLeaf.muD0[ile],inpTreeLeaf.muDz[ile],
               inpTreeLeaf.muPFIsoR04_CH[ile],
               inpTreeLeaf.muPFIsoR04_NH[ile],
               inpTreeLeaf.muPFIsoR04_Pho[ile],
               inpTreeLeaf.muPFIsoR04_PU[ile]);
               //variables which are input for TMuonCuts constructor
               //are fields of TEventTree
             if (muon.Passed()) 
               {

//                 nLeptonsPassed_++;

                 if (!muon.MoreMuonsVeto()) return 0; 

//                 nMoreVetoPassed_++;

                 WMt[ile] = sqrt(2*inpTreeLeaf.muPt[ile]*inpTreeLeaf.pfMET*(1-cos(inpTreeLeaf.muPhi[ile]-inpTreeLeaf.pfMETPhi)));
                 if ( (WMt[ile]>WMtCut_ && !isReleasedCutsMode) || (isReleasedCutsMode))
                   {
//                     nWMtPassed_++;
                     thisLeptonPassed=1;
                   }    
               } 
          }
        else if (channel==TInputSample::ELECTRON)
          {
            //TElectronCuts functions are empty now;
            //they are included here for the future
            TElectronCuts electron(inpTreeLeaf.elePt[ile]);
            if (electron.Passed()) 
              {
                if (!electron.MoreElectronsVeto()) return 0;  
                WMt[ile] = sqrt(2*inpTreeLeaf.elePt[ile]*inpTreeLeaf.pfMET*(1-cos(inpTreeLeaf.elePhi[ile]-inpTreeLeaf.pfMETPhi)));
                if ((WMt[ile]>WMtCut_ && !isReleasedCutsMode) || (isReleasedCutsMode)) thisLeptonPassed=1;        
              } 
          }

        if (thisLeptonPassed)
          {
            if (goodLeptonExists) return 0;  
              //to remove events with more than one leptons in the event
            goodLeptonExists=1;
            goodLepton[ile]=1; 
          } 
        else goodLepton[ile]=0; 
      } //end of lepton loop 

   if (!goodLeptonExists) return 0;
     //skip loop over photons if no good leptons found

   bool goodPhoton[inpTreeLeaf.nPho];
   bool goodPhotonExists=0;
   for (int ipho=0; ipho<inpTreeLeaf.nPho; ipho++) 
   //start of photon loop
     {       
       
//        nPhotons_++;

        if (inpTreeLeaf.isData) 
           inpTreeLeaf.phoEt[ipho] = photonCorrector->GetCorrEtData(inpTreeLeaf.phoR9[ipho], 2012, inpTreeLeaf.phoEt[ipho], inpTreeLeaf.phoEta[ipho]);
          //Phosphor correction needs to be applied for the photon Et only, 
          //not for SC Et
        else
          {
            int phoMCIndex = -1;
            for (int iMC=0; iMC<inpTreeLeaf.nMC; iMC++){
              if (inpTreeLeaf.mcPID[iMC]==22) phoMCIndex = iMC;
            }
            if (phoMCIndex > -1) 
              inpTreeLeaf.phoEt[ipho] = photonCorrector->GetCorrEtMC(inpTreeLeaf.phoR9[ipho], 2012, inpTreeLeaf.phoEt[ipho], inpTreeLeaf.phoEta[ipho], inpTreeLeaf.mcE[phoMCIndex]);
          }

                
        TPhotonCuts photon(inpTreeLeaf.phoEleVeto[ipho],
                    inpTreeLeaf.phoEt[ipho],inpTreeLeaf.phoEta[ipho],
                    inpTreeLeaf.phoSCEt[ipho],inpTreeLeaf.phoSCEta[ipho],
                    inpTreeLeaf.phoHoverE12[ipho],
                    inpTreeLeaf.phoSigmaIEtaIEta[ipho],
                    inpTreeLeaf.phoPFChIso[ipho],inpTreeLeaf.phoPFNeuIso[ipho],
                    inpTreeLeaf.phoPFPhoIso[ipho],
                    inpTreeLeaf.rho2012);
          //variables which are input for TPhotonCuts constructor
          //are fields of TEventTree

	if (photon.Passed()) 
          {
     
//            nPhotonsPassed_++;

            goodPhotonExists=1;
            goodPhoton[ipho]=1;
          }
        else goodPhoton[ipho]=0;
      } //end of photon loop   

   if (!goodPhotonExists) return 0;
     //skip checking lepton-photon matching if no good photons found

    //check matching between muon and photon
    for (int ile=0; ile<nLe; ile++) 
      {
        for (int ipho=0; ipho<inpTreeLeaf.nPho; ipho++)
          {

            if (channel==TInputSample::MUON)
              lePhoDeltaR[ile][ipho]=DeltaR(inpTreeLeaf.muPhi[ile],inpTreeLeaf.muEta[ile],inpTreeLeaf.phoPhi[ipho],inpTreeLeaf.phoEta[ipho]);
            else if (channel==TInputSample::ELECTRON)
              lePhoDeltaR[ile][ipho]=DeltaR(inpTreeLeaf.elePhi[ile],inpTreeLeaf.eleEta[ile],inpTreeLeaf.phoPhi[ipho],inpTreeLeaf.phoEta[ipho]);

            if (goodPhoton[ipho] && goodLepton[ile] && 
                (lePhoDeltaR[ile][ipho]>lePhoDeltaRCut_ || isReleasedCutsMode))
              {

//                nPhoLepPassed_++;

                goodLeptonPhotonPairs[ile][ipho]=1;
                goodLeptonPhotonPairsExist=1;  
              }                
           }    
       }     

   return goodLeptonPhotonPairsExist;
}


float TFullCuts::DeltaR(float phi1, float eta1, float phi2, float eta2) 
{ 

  float dphi;
  for (dphi=fabs(phi1-phi2);
       fabs(dphi)>=2*TMath::Pi(); 
       dphi=dphi-2*TMath::Pi()) ;
  float dR=sqrt((eta1-eta2)*(eta1-eta2)+dphi*dphi);
  return dR;
}
