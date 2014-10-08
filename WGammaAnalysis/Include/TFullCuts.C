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

#include <vector>

TFullCuts::TFullCuts()
{
}

TFullCuts::~TFullCuts()
{
}

bool TFullCuts::VeryPreliminaryCut(TEventTree::InputTreeLeaves& inpTreeLeaf,   
                    int channel, int vgamma, bool isVJets,
                    int& nCands, Candidate* cands,
                    PassedLevels& passed)
{

  // This function is called from LoopOverEvents.
  // returns 1 if entry is accepted (has at least one candidate)
  // returns 0 otherwise (has no candidates)

   bool kinPhoton[inpTreeLeaf.nPho];

   int nLe=0;
   if (channel==TConfiguration::MUON) nLe=inpTreeLeaf.nMu;
   else if (channel==TConfiguration::ELECTRON) nLe=inpTreeLeaf.nEle;

   if ((inpTreeLeaf.IsVtxGood)==-1) return 0;
   if (inpTreeLeaf.nPho < 1) return 0; 
   if (nLe < 1) return 0;
      //these variables are fields of TEventTree
//   if (!inpTreeLeaf.metFilters[6]) return 0;
// we don't need it because we're running Jan22 rereco
      //metFilters - is field of TEventTree
      //[6] - ecalLaserCorrFilter
      //necessary to remove spikes in the photon distributions 
      //for Jul13 rereco

   bool kinLepton1[nLe];
   bool kinLepton1Exists=0;
   bool kinLepton2[nLe];
   bool kinLepton2Exists=0;

   TMuonCuts emptyMuon;
   for (int ilep1=0; ilep1<nLe; ilep1++){        
     if (channel==TConfiguration::MUON){
       kinLepton1[ilep1] = CheckMuon(ilep1, inpTreeLeaf, emptyMuon, passed);
     }
     else if (channel==TConfiguration::ELECTRON);
     if (kinLepton1[ilep1]) kinLepton1Exists=1;
     for (int ilep2=0; ilep2<nLe; ilep2++){  
       if (ilep1==ilep2) continue;      
       if (channel==TConfiguration::MUON){
         kinLepton2[ilep2] = CheckMuon(ilep2, inpTreeLeaf, emptyMuon, passed);
       }
       else if (channel==TConfiguration::ELECTRON);
       if (kinLepton2[ilep2]) kinLepton2Exists=1;
     } //end of loop over ilep2
   } //end of loop over ilep1

 bool kinPhotonExists=0;
  for (int ipho=0; ipho<inpTreeLeaf.nPho; ipho++){   
    if (isVJets && IsOverlapVJetsVGamma(ipho, inpTreeLeaf)) continue;   
    TPhotonCuts emptyPhoton;
//    if (inpTreeLeaf.phoSCEt->at(ipho)>15) passed.phoPtPassed++;
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

  if (!kinPhotonExists) return 0;
  if (!kinLepton1Exists) return 0;
  if (!kinLepton2Exists && vgamma==_config.Z_GAMMA) return 0;
     //skip checking lepton-photon matching if no good photons found

  int icand=0;
  //check matching between lepton and photon
  for (int ipho=0; ipho<inpTreeLeaf.nPho; ipho++){  
    for (int ilep1=0; ilep1<nLe; ilep1++) {
      float dR1;
      if (channel==TConfiguration::MUON){
        dR1=DeltaR(inpTreeLeaf.muPhi->at(ilep1),inpTreeLeaf.muEta->at(ilep1),inpTreeLeaf.phoPhi->at(ipho),inpTreeLeaf.phoEta->at(ipho));
      }
      else if (channel==TConfiguration::ELECTRON);
      if (vgamma==_config.W_GAMMA){
        if (kinPhoton[ipho] && kinLepton1[ilep1] && 
             (dR1>_lePhoDeltaRCut)){
          passed.dRPassed++;
          cands[icand].ipho=ipho; 
          cands[icand].ilep1=ilep1; cands[icand].ilep2=-1;
          cands[icand].dRlep1pho=dR1; cands[icand].dRlep2pho=-1;
          icand++; 
        }
      }//end of if (vgamma==_config.W_GAMMA)    
 
      else if (vgamma==_config.Z_GAMMA){
        for (int ilep2=0; ilep2<nLe; ilep2++) {
          if (ilep1==ilep2) continue;
          float dR2;

          if (channel==TConfiguration::MUON){
            dR2=DeltaR(inpTreeLeaf.muPhi->at(ilep2),inpTreeLeaf.muEta->at(ilep2),inpTreeLeaf.phoPhi->at(ipho),inpTreeLeaf.phoEta->at(ipho));
          }
          else if (channel==TConfiguration::ELECTRON);
          if (kinPhoton[ipho] && kinLepton1[ilep1] && kinLepton2[ilep2] && 
             (dR1>_lePhoDeltaRCut) && (dR2>_lePhoDeltaRCut)){
            passed.dRPassed++;
            cands[icand].ipho=ipho; 
            if (dR1<dR2){
              cands[icand].ilep1=ilep1; cands[icand].ilep2=ilep2;
              cands[icand].dRlep1pho=dR1; cands[icand].dRlep2pho=dR2;
            }
            else{
              cands[icand].ilep1=ilep2; cands[icand].ilep2=ilep1;
              cands[icand].dRlep1pho=dR2; cands[icand].dRlep2pho=dR1;
            }
            icand++;
          }//end of if (kinPhoton[ipho] && kinLepton1[ilep1] ...
        }//end of loop over ilep2        
      }//end of  else if (vgamma==_config.Z_GAMMA) 

    }//end of loop over ilep1    
  }//end of loop over ipho   
  nCands=icand;
  return (icand);

}//end of TFullCuts::VeryPreliminaryCut

bool TFullCuts::CheckMuon(int ilep, TEventTree::InputTreeLeaves& inpTreeLeaf, TMuonCuts& emptyMuon, PassedLevels& passed)
{
   if (inpTreeLeaf.muPt->at(ilep)>26)  passed.leptonPtPassed++;
   if (fabs(inpTreeLeaf.muEta->at(ilep))<2.1)    passed.leptonEtaPassed++;
   if (emptyMuon.PassedKinematics(inpTreeLeaf.muPt->at(ilep),inpTreeLeaf.muEta->at(ilep))){ 
     return 1;
   }
   else return 0;
   return 0;
}

bool TFullCuts::IsOverlapVJetsVGamma(int ipho, TEventTree::InputTreeLeaves& inpTreeLeaf)
{
  // from Sachiko's code (Zgg, 
  // https://github.com/sachikot/Zgg/blob/master/analyze.cc#L317-343)
  bool pho_matched = false;
  float dr_min = 1000.;
  float dr;
  for (int genPho = 0; genPho < inpTreeLeaf.nMC; ++genPho){
    if( (*inpTreeLeaf.mcPID)[genPho] != 22 ) continue;
    if( ((*inpTreeLeaf.mcParentage)[genPho]& 4) == 4) continue;
    if( (*inpTreeLeaf.mcPt)[genPho] < 10 ) continue;
    // CALCULATE DR
    float deta = (*inpTreeLeaf.phoEta)[ipho] - (*inpTreeLeaf.mcEta)[genPho];
    float dphi = acos(cos((*inpTreeLeaf.phoPhi)[ipho] - (*inpTreeLeaf.mcPhi)[genPho]));
    dr = sqrt(deta*deta + dphi*dphi);
    if(dr < dr_min) dr_min = dr;
    if(dr < 0.2) {
      pho_matched = true;
      break;
    }
  } // loop over for gen-photon
//  deltaR->Fill(dr_min);
//  if(pho_matched) continue; // yes! I removed overlapping reco-photon!!
  if(pho_matched) return 1;
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

float TFullCuts::GetWMtCut(int year)
{
  if (year==2012) return _WMtCut2012;
  else if (year==2012) return _WMtCut2011;
  return _WMtCut2012;
}

TCut TFullCuts::RangeMoreLeptonsVeto(){
  return "!hasMoreLeptons";
}

TCut TFullCuts::RangeMetRelatedCut(int year)
{
  TString cutStr="WMt>";
  if (year==2012) cutStr+=_WMtCut2012;
  else if (year==2011) cutStr+=_WMtCut2011;
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

TCut TFullCuts::RangeExtraLeptonPt2011()
{
  TString cutStr="lepton1Pt>35";
  TCut cut(cutStr);
  return cut;
}

TCut TFullCuts::RangeForMetCut(int year, int channel, int vgamma, int phoWP){
  TPhotonCuts emptyPhoton;
  TCut cutPhoton=emptyPhoton.RangePhoton(year, phoWP, 1, 0, 0);
  TCut cutLepton;
  if (channel==TConfiguration::MUON){
    TMuonCuts emptyMuon;
    cutLepton=emptyMuon.RangeMuon(year,1);//1st muon
    if (vgamma==_config.Z_GAMMA)
      cutLepton = cutLepton && emptyMuon.RangeMuon(year,2);//2nd muon
  }
  else if (channel==TConfiguration::ELECTRON);
  TCut cut = cutPhoton && cutLepton && RangePhoEt();
  if (vgamma==_config.W_GAMMA) cut = cut && RangeMoreLeptonsVeto();
  if (year==2011) cut = cut && RangeExtraLeptonPt2011();
  return cut;
}

TCut TFullCuts::RangeForTemplateMethodCut(int year, int channel, int vgamma, int phoWP){
  TPhotonCuts emptyPhoton;
  TCut cutPhoton=emptyPhoton.RangePhoton(year, phoWP, 1, 0, 0);
  //no cuts on sigmaIEtaIEta and chIso
  TCut cutLepton;
  if (channel==TConfiguration::MUON){
    TMuonCuts emptyMuon;
    cutLepton=emptyMuon.RangeMuon(year,1);//1st muon
    if (vgamma==_config.Z_GAMMA)
      cutLepton = cutLepton && emptyMuon.RangeMuon(year,2);//2nd muon
  }
  else if (channel==TConfiguration::ELECTRON);
  TCut cut = cutPhoton && cutLepton && RangePhoEt();
  if (vgamma==_config.W_GAMMA) 
    cut = cut && RangeMoreLeptonsVeto() && RangeMetRelatedCut(year);
  if (year==2011) 
    cut = cut && RangeExtraLeptonPt2011();
  return cut;
}

TCut TFullCuts::RangeFullCut(int year, int channel, int vgamma, int phoWP, bool noPhoPFChIsoCut)
{
  TPhotonCuts emptyPhoton;
  TCut cutPhoton=emptyPhoton.RangePhoton(year, phoWP, noPhoPFChIsoCut);
  TCut cutLepton;
  if (channel==TConfiguration::MUON){
    TMuonCuts emptyMuon;
    cutLepton=emptyMuon.RangeMuon(year,1);//1st muon
    if (vgamma==_config.Z_GAMMA)
      cutLepton = cutLepton && emptyMuon.RangeMuon(year,2);//2nd muon
  }
  else if (channel==TConfiguration::ELECTRON);
  TCut cut = cutPhoton && cutLepton && RangePhoEt();
  if (vgamma==_config.W_GAMMA) 
    cut = cut && RangeMoreLeptonsVeto() && RangeMetRelatedCut(year);
  if (year==2011) 
    cut = cut && RangeExtraLeptonPt2011();
  return cut;
}
