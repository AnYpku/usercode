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

#include "TString.h"
#include "TCut.h"
//ROOT

TFullCuts::TFullCuts()
{
}

TFullCuts::~TFullCuts()
{
}

bool TFullCuts::VeryPreliminaryCut(bool** goodLeptonPhotonPairs, 
                    TEventTree::InputTreeLeaves inpTreeLeaf,   
                    int channel, float** lePhoDeltaR, PassedLevels& passed)
{

  // This function is called from LoopOverEvents.
  // returns 1 if entry is accepted.
  // returns 0 otherwise.

   bool kinPhoton[inpTreeLeaf.nPho];

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

   bool goodLeptonPhotonPairsExist=0;

   for (int ipho=0; ipho<inpTreeLeaf.nPho; ipho++)
     for (int ile=0; ile<nLe; ile++) 
       goodLeptonPhotonPairs[ile][ipho]=0;   

   bool kinLepton[nLe];
   bool kinLeptonExists=0;
   for (int ile=0; ile<nLe; ile++){        
     if (channel==TConfiguration::MUON){
       TMuonCuts emptyMuon;
       if (inpTreeLeaf.muPt->at(ile)>26)  passed.leptonPtPassed++;
       if (fabs(inpTreeLeaf.muEta->at(ile))<2.1)    passed.leptonEtaPassed++;
       if (emptyMuon.PassedKinematics(inpTreeLeaf.muPt->at(ile),inpTreeLeaf.muEta->at(ile))){ 
         kinLeptonExists=1;
         kinLepton[ile]=1; 
       }
       else
         kinLepton[ile]=0;
     }
     else if (channel==TConfiguration::ELECTRON){
       //TElectronCuts functions are empty now;
       //they are included here for the future
       TElectronCuts electron(inpTreeLeaf.elePt->at(ile));
//            if (electron.Passed()) 
//              {
//                if (!electron.MoreElectronsVeto()) return 0;  
//                kinLeptonPassed=1;        
//              } 
     }
   } //end of loop over ile

//   if (!kinLeptonExists) return 0;
     //skip loop over photons if no leptons passed kinematics


 bool kinPhotonExists=0;
  for (int ipho=0; ipho<inpTreeLeaf.nPho; ipho++){       
    TPhotonCuts emptyPhoton;
    if (inpTreeLeaf.phoSCEt->at(ipho)>15) passed.phoPtPassed++;
    if (emptyPhoton.IsBarrel(inpTreeLeaf.phoSCEta->at(ipho)))
      passed.phoBarrelPassed++;
    if (emptyPhoton.IsEndcap(inpTreeLeaf.phoSCEta->at(ipho)))
        passed.phoEndcapPassed++;
    if (emptyPhoton.PassedKinematics(inpTreeLeaf.phoSCEt->at(ipho),inpTreeLeaf.phoSCEta->at(ipho))){
      kinPhotonExists=1;
      kinPhoton[ipho]=1;
    }
    else kinPhoton[ipho]=0;
  } //end of loop over ipho 

  if (!kinLeptonExists) return 0;
  if (!kinPhotonExists) return 0;
     //skip checking lepton-photon matching if no good photons found

    //check matching between lepton and photon
  for (int ile=0; ile<nLe; ile++) {
    for (int ipho=0; ipho<inpTreeLeaf.nPho; ipho++){
      if (channel==TConfiguration::MUON)
        lePhoDeltaR[ile][ipho]=DeltaR(inpTreeLeaf.muPhi->at(ile),inpTreeLeaf.muEta->at(ile),inpTreeLeaf.phoPhi->at(ipho),inpTreeLeaf.phoEta->at(ipho));
      else if (channel==TConfiguration::ELECTRON)
        lePhoDeltaR[ile][ipho]=0; //DeltaR(inpTreeLeaf.elePhi[ile],inpTreeLeaf.eleEta[ile],inpTreeLeaf.phoPhi[ipho],inpTreeLeaf.phoEta[ipho]);

      if (kinPhoton[ipho] && kinLepton[ile] && 
          (lePhoDeltaR[ile][ipho]>_lePhoDeltaRCut)){
        passed.dRPassed++;
        goodLeptonPhotonPairs[ile][ipho]=1;
        goodLeptonPhotonPairsExist=1;  
      }                
    }//end of loop over ipho    
  }//end of loop over ile     

  return goodLeptonPhotonPairsExist;
}//end of TFullCuts::Cut


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
  return _WMtCut;
}

TCut TFullCuts::RangeMoreLeptonsVeto(){
  return "!hasMoreLeptons";
}

TCut TFullCuts::RangeMetRelatedCut()
{
  TString cutStr="WMt>";
  cutStr+=_WMtCut;
  TCut cut(cutStr);
  return cut;
}

TCut TFullCuts::RangePhoEt()
{
  TString cutStr="phoEt>";
  cutStr+=_config.GetPhoPtMin();
  TCut cut(cutStr);
  return cut;
}

TCut TFullCuts::RangeForMetCut(int channel, int phoWP, TString phoIsoBase){
  TPhotonCuts emptyPhoton;
  TCut cutPhoton=emptyPhoton.RangePhoton(phoWP, phoIsoBase, 0, 0);
  //phoIsoBase = "PF" or "SCR"
  TCut cutLepton;
  if (channel==TConfiguration::MUON){
    TMuonCuts emptyMuon;
    cutLepton=emptyMuon.RangeMuon();
  }
  else if (channel==TConfiguration::ELECTRON);
  TCut cut = cutPhoton && cutLepton && RangeMoreLeptonsVeto();
  return cut;
}

TCut TFullCuts::RangeForTemplateMethodCut(int channel, int phoWP, TString phoIsoBase){
  TPhotonCuts emptyPhoton;
  TCut cutPhoton=emptyPhoton.RangePhoton(phoWP, phoIsoBase, 0, 0);
  //no cuts on sigmaIEtaIEta and chIso
  TCut cutLepton;
  if (channel==TConfiguration::MUON){
    TMuonCuts emptyMuon;
    cutLepton=emptyMuon.RangeMuon();
  }
  else if (channel==TConfiguration::ELECTRON);
  TCut cut = cutPhoton && cutLepton && RangeMoreLeptonsVeto() && RangeMetRelatedCut();
  return cut;
}

TCut TFullCuts::RangeFullCut(int channel, int phoWP, TString phoIsoBase)
{
  TPhotonCuts emptyPhoton;
  TCut cutPhoton=emptyPhoton.RangePhoton(phoWP, phoIsoBase);
  //phoIsoBase = "PF" or "SCR"
  TCut cutLepton;
  if (channel==TConfiguration::MUON){
    TMuonCuts emptyMuon;
    cutLepton=emptyMuon.RangeMuon();
  }
  else if (channel==TConfiguration::ELECTRON);
  TCut cut = cutPhoton && cutLepton && RangeMoreLeptonsVeto() && RangeMetRelatedCut();
  return cut;
}
