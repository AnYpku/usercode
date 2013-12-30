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
//                    int channel, 
//                    float* WMt, float** lePhoDeltaR,
//                    zgamma::PhosphorCorrectionFunctor* photonCorrector)
bool TFullCuts::Cut(bool** goodLeptonPhotonPairs, 
                    TEventTree::InputTreeLeaves &inpTreeLeaf,   
                    int channel, 
                    float* WMt, float** lePhoDeltaR,
                    zgamma::PhosphorCorrectionFunctor* photonCorrector, bool doSigmaIEtaIEtaCut, bool doPhoChIsoCut)
{

  // This function is called from LoopOverEvents.
  // returns 1 if entry is accepted.
  // returns 0 otherwise.

//   nTotal_++;

   int nLe=0;
   if (channel==TConfiguration::MUON) nLe=inpTreeLeaf.nMu;
   else if (channel==TConfiguration::ELECTRON) nLe=inpTreeLeaf.nEle;

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
              //inpTreeLeaf.nLowPtJet, inpTreeLeaf.jetLowPtRawPt,
              //inpTreeLeaf.jetLowPtRawEn, inpTreeLeaf.jetLowPtPt,  
              //inpTreeLeaf.jetLowPtEta, inpTreeLeaf.jetLowPtPhi,
              //inpTreeLeaf.jetLowPtGenJetPt, inpTreeLeaf.jetLowPtGenJetEta, 
              //inpTreeLeaf.jetLowPtGenJetPhi, 
              inpTreeLeaf.nJet, 
              inpTreeLeaf.jetRawPt, inpTreeLeaf.jetRawEn, 
              inpTreeLeaf.jetPt, inpTreeLeaf.jetEta, 
              inpTreeLeaf.jetPhi, inpTreeLeaf.jetGenJetPt, 
              inpTreeLeaf.jetGenJetEta, inpTreeLeaf.jetGenJetPhi);
     met.METSmearCorrection();
     inpTreeLeaf.pfMET = met.GetRecoPfMET();
     inpTreeLeaf.pfMETPhi = met.GetRecoPfMETPhi();
 
   }//end of "if (!inpTreeLeaf.isData)"

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
        if (channel==TConfiguration::MUON)
          {
            TMuonCuts muon(ile,inpTreeLeaf.muPt->at(ile),
               inpTreeLeaf.muPt,inpTreeLeaf.muEta->at(ile),inpTreeLeaf.muEta,
               inpTreeLeaf.nMu,inpTreeLeaf.muNumberOfValidPixelHits->at(ile),
               inpTreeLeaf.muNumberOfValidTrkHits->at(ile),
               inpTreeLeaf.muNumberOfValidTrkLayers->at(ile),
               inpTreeLeaf.muNumberOfValidMuonHits->at(ile),
               inpTreeLeaf.muStations->at(ile),  
               inpTreeLeaf.HLT[inpTreeLeaf.HLTIndex[18]],
               inpTreeLeaf.HLT[inpTreeLeaf.HLTIndex[19]],
               inpTreeLeaf.muTrg->at(ile),
               inpTreeLeaf.muChi2NDF->at(ile),
               inpTreeLeaf.muD0->at(ile),inpTreeLeaf.muDz->at(ile),
               inpTreeLeaf.muPFIsoR04_CH->at(ile),
               inpTreeLeaf.muPFIsoR04_NH->at(ile),
               inpTreeLeaf.muPFIsoR04_Pho->at(ile),
               inpTreeLeaf.muPFIsoR04_PU->at(ile));
               //variables which are input for TMuonCuts constructor
               //are fields of TEventTree
             if (muon.Passed()) 
               {

//                 nLeptonsPassed_++;

                 if (!muon.MoreMuonsVeto()) return 0; 

//                 nMoreVetoPassed_++;

                 WMt[ile] = sqrt(2*inpTreeLeaf.muPt->at(ile)*inpTreeLeaf.pfMET*(1-cos(inpTreeLeaf.muPhi->at(ile)-inpTreeLeaf.pfMETPhi)));
                 if ( WMt[ile]>WMtCut_ )
                   {
//                     nWMtPassed_++;
                     thisLeptonPassed=1;
                   }    
               } 
          }
        else if (channel==TConfiguration::ELECTRON)
          {
            //TElectronCuts functions are empty now;
            //they are included here for the future
            TElectronCuts electron(inpTreeLeaf.elePt->at(ile));
            if (electron.Passed()) 
              {
                if (!electron.MoreElectronsVeto()) return 0;  
                WMt[ile] = 0; 
//sqrt(2*inpTreeLeaf.elePt->at(ile)*inpTreeLeaf.pfMET*(1-cos(inpTreeLeaf.elePhi->at(ile)-inpTreeLeaf.pfMETPhi)));
                if (WMt[ile]>WMtCut_) thisLeptonPassed=1;        
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
   if (!PhotonsOnlyCuts(goodPhoton,inpTreeLeaf,photonCorrector, doSigmaIEtaIEtaCut,doPhoChIsoCut)) return 0;
     //skip checking lepton-photon matching if no good photons found

    //check matching between muon and photon
    for (int ile=0; ile<nLe; ile++) 
      {
        for (int ipho=0; ipho<inpTreeLeaf.nPho; ipho++)
          {

            if (channel==TConfiguration::MUON)
              lePhoDeltaR[ile][ipho]=DeltaR(inpTreeLeaf.muPhi->at(ile),inpTreeLeaf.muEta->at(ile),inpTreeLeaf.phoPhi->at(ipho),inpTreeLeaf.phoEta->at(ipho));
            else if (channel==TConfiguration::ELECTRON)
              lePhoDeltaR[ile][ipho]=0; //DeltaR(inpTreeLeaf.elePhi[ile],inpTreeLeaf.eleEta[ile],inpTreeLeaf.phoPhi[ipho],inpTreeLeaf.phoEta[ipho]);

            if (goodPhoton[ipho] && goodLepton[ile] && 
                (lePhoDeltaR[ile][ipho]>lePhoDeltaRCut_))
              {

//                nPhoLepPassed_++;

                goodLeptonPhotonPairs[ile][ipho]=1;
                goodLeptonPhotonPairsExist=1;  
              }                
           }    
       }     

   return goodLeptonPhotonPairsExist;
}//end of TFullCuts::Cut

bool TFullCuts::PhotonsOnlyCuts(bool* goodPhoton,
               TEventTree::InputTreeLeaves &inpTreeLeaf,   
               zgamma::PhosphorCorrectionFunctor* photonCorrector, bool doSigmaIEtaIEtaCut,  bool doPhoChIsoCut){
   bool goodPhotonExists=0;

   for (int ipho=0; ipho<inpTreeLeaf.nPho; ipho++) 
   //start of photon loop
     {       
       
//        nPhotons_++;
        if (inpTreeLeaf.isData) 
           inpTreeLeaf.phoEt->at(ipho) = photonCorrector->GetCorrEtData(inpTreeLeaf.phoR9->at(ipho), 2012, inpTreeLeaf.phoEt->at(ipho), inpTreeLeaf.phoEta->at(ipho));
          //Phosphor correction needs to be applied for the photon Et only, 
          //not for SC Et
        else
          {
            int phoMCIndex = -1;
            for (int iMC=0; iMC<inpTreeLeaf.nMC; iMC++){
              if (inpTreeLeaf.mcPID->at(iMC)==22) phoMCIndex = iMC;
            }
            if (phoMCIndex > -1) 
              inpTreeLeaf.phoEt->at(ipho) = photonCorrector->GetCorrEtMC(inpTreeLeaf.phoR9->at(ipho), 2012, inpTreeLeaf.phoEt->at(ipho), inpTreeLeaf.phoEta->at(ipho), inpTreeLeaf.mcE->at(phoMCIndex));
          }

                
        TPhotonCuts photon(inpTreeLeaf.phoEleVeto->at(ipho),
                    inpTreeLeaf.phoEt->at(ipho),inpTreeLeaf.phoEta->at(ipho),
                    inpTreeLeaf.phoSCEt->at(ipho),inpTreeLeaf.phoSCEta->at(ipho),
                    inpTreeLeaf.phoHoverE12->at(ipho),
                    inpTreeLeaf.phoSigmaIEtaIEta->at(ipho),
                    inpTreeLeaf.phoPFChIso->at(ipho),inpTreeLeaf.phoPFNeuIso->at(ipho),
                    inpTreeLeaf.phoPFPhoIso->at(ipho),
                    inpTreeLeaf.rho2012);
          //variables which are input for TPhotonCuts constructor
          //are fields of TEventTree


	if (photon.Passed(doSigmaIEtaIEtaCut,doPhoChIsoCut)) 
          {
     
//            nPhotonsPassed_++;

            goodPhotonExists=1;
            goodPhoton[ipho]=1;
          }
        else goodPhoton[ipho]=0;
      } //end of photon loop   

  return goodPhotonExists;
}//end of TFullCuts::PhotonOnlyCuts

float TFullCuts::DeltaR(float phi1, float eta1, float phi2, float eta2) 
{ 

  float dphi;
  for (dphi=fabs(phi1-phi2);
       fabs(dphi)>=2*TMath::Pi(); 
       dphi=dphi-2*TMath::Pi()) ;
  float dR=sqrt((eta1-eta2)*(eta1-eta2)+dphi*dphi);
  return dR;
}

float TFullCuts::GetWMtCut()
{
  return WMtCut_;
}

TCut TFullCuts::RangeMetRelatedCut()
{
  TCut cut("1");
  return cut;
}

TCut TFullCuts::RangeExtraCut()
{
  TPhotonCuts emptyPhoton;
  TCut cut = emptyPhoton.RangePhoChIso() && emptyPhoton.RangeSigmaIEtaIEta() && RangeMetRelatedCut();
  return cut;
}
