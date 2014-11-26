#include "TFullCuts.h"
#include "TEventTree.h"
#include "TMathTools.h"
#include "TMetTools.h"
#include "TMuonCuts.h"
#include "TElectronCuts.h"
#include "TPhotonCuts.h"
#include "../Configuration/TInputSample.h"
  //this package

#include "../PHOSPHOR_CORRECTION/PhosphorCorrectorFunctor.hh"
  //taken from http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/CPena/src/PHOSPHOR_Corr_v2/
  //currently in this package

#include "TString.h"
#include "TCut.h"
//ROOT

#include <iostream>
#include <vector>

TFullCuts::TFullCuts()
{
}

TFullCuts::~TFullCuts()
{
}

bool TFullCuts::VeryPreliminaryCut(TEventTree::InputTreeLeaves& leaf,  
                    zgamma::PhosphorCorrectionFunctor* photonCorrector, 
                    int channel, int vgamma, bool isVJets,
                    int& nCands, Candidate* cands,
                    PassedLevels& passed)
{
//  std::cout<<"channel="<<_config.StrChannel(channel)<<std::endl;

  // This function is called from LoopOverEvents.
  // returns 1 if entry is accepted (has at least one candidate)
  // returns 0 otherwise (has no candidates)
   
   _leaf=leaf;
   _cands=cands;
   _passed=passed;

   if (!_leaf.metFilters[0]) return 0; //[0]CSC Beam Halo
   if (!_leaf.metFilters[1]) return 0; //[1]HBHE Noise
   if (!_leaf.metFilters[2]) return 0; //[2]HCAL laser
   if (!_leaf.metFilters[3]) return 0; //[3]ECAL dead cell
   if (!_leaf.metFilters[4]) return 0; //[4]Tracking failure
   if (!_leaf.metFilters[5]) return 0; //[5]Bad EE SC
//   if (!_leaf.metFilters[9]) return 0; //[9]log error too many clusters

   _passed.metFiltersPassed++;

   _kinPhoton=new bool[leaf.nPho];

   int nLe=0;
   if (channel==_config.MUON) nLe=leaf.nMu;
   else if (channel==_config.ELECTRON) nLe=leaf.nEle;
   if (channel==_config.MUON && vgamma==_config.W_GAMMA)
     if ((!leaf.HLT[leaf.HLTIndex[18]]) && (!leaf.HLT[leaf.HLTIndex[19]]))
     //_HLT_IsoMu24_eta2p1_, _HLT_IsoMu24_v
       { passed=_passed;  return 0;}
   if (channel==_config.ELECTRON && vgamma==_config.W_GAMMA);
   if (channel==_config.MUON && vgamma==_config.Z_GAMMA);
     if (!leaf.HLT[leaf.HLTIndex[21]])
     //_HLT_Mu22_Mu8_v
       { passed=_passed;  return 0;}
   if (channel==_config.ELECTRON && vgamma==_config.Z_GAMMA);

   _passed.triggerPassed++;

   if ((leaf.IsVtxGood)==-1){ passed=_passed; return 0;}
   _passed.goodVertexPassed++;
   if (leaf.nPho < 1){ passed=_passed;  return 0;}
   if (nLe < 1) { passed=_passed;  return 0;}

   if (isVJets && IsOverlapVJetsVGamma(channel)) { passed=_passed;  return 0;}
      _passed.vgvjOverlapPassed++;

   _kinLepton1=new bool[nLe];
   bool kinLepton1Exists=0;
   _kinLepton2=new bool[nLe];
   bool kinLepton2Exists=0;


 bool kinPhotonExists=0;
  int icand=0;
  //check matching between lepton and photon
  bool hasOverlap=0;
  bool hasEventAfterPass=0;
  bool ifPassedPt=0;
  bool ifPassedEta=0;
  for (int ipho=0; ipho<_leaf.nPho; ipho++){   

    for (int ilep1=0; ilep1<nLe; ilep1++){ 

      //double phoEtCorr = photonCorrector->GetCorrEtData(_leaf.phoR9->at(ipho), 2012, _leaf.phoEt->at(ipho), _leaf.phoEta->at(ipho));

      _kinPhoton[ipho]=_photon.PassedKinematics(_leaf.phoEt->at(ipho),_leaf.phoSCEta->at(ipho),ifPassedPt,ifPassedEta);
      if (ifPassedPt) _passed.phoPtPassed++; else continue;
      if (ifPassedEta) _passed.phoEtaPassed++; else continue;
   
      if (channel==_config.MUON){
        _kinLepton1[ilep1] = _muon.PassedKinematics(_leaf.muPt->at(ilep1),_leaf.muEta->at(ilep1),ifPassedPt,ifPassedEta);
        if (ifPassedPt) _passed.leptonPtPassed++; else continue;
        if (ifPassedEta) _passed.leptonEtaPassed++; else continue;
      }
      else if (channel==_config.ELECTRON);
      float dR1;
      if (channel==_config.MUON){
        dR1=_math.DeltaR(_leaf.muPhi->at(ilep1),_leaf.muEta->at(ilep1),_leaf.phoPhi->at(ipho),_leaf.phoEta->at(ipho));
      }
      else if (channel==_config.ELECTRON);
      if (vgamma==_config.W_GAMMA){
        CheckDRandProceed(_config.W_GAMMA, isVJets, icand,
                    hasEventAfterPass, hasOverlap,
                    dR1, -1, ipho, ilep1, -1);
        continue;
      }//end of if (vgamma==_config.W_GAMMA) 
      // for vgamma==_config.Z_GAMMA, loop over the second lepton
      for (int ilep2=0; ilep2<nLe; ilep2++){  
        if (ilep1==ilep2) continue;      
          TLorentzVector vlep1, vlep2;
          if (channel==_config.MUON){
            _kinLepton2[ilep2] = _muon.PassedKinematics(_leaf.muPt->at(ilep2),_leaf.muEta->at(ilep2),ifPassedPt,ifPassedEta);
            if (!_kinLepton2[ilep2]) continue;
            vlep1.SetPtEtaPhiM(_leaf.muPt->at(ilep1),_leaf.muEta->at(ilep1),_leaf.muPhi->at(ilep1),0);
            vlep2.SetPtEtaPhiM(_leaf.muPt->at(ilep2),_leaf.muEta->at(ilep2),_leaf.muPhi->at(ilep2),0);
          }
          else if (channel==_config.ELECTRON);
          if((vlep1 + vlep2).M() < 50) continue;

          float dR2;
          if (channel==_config.MUON){
            dR2=_math.DeltaR(_leaf.muPhi->at(ilep2),_leaf.muEta->at(ilep2),_leaf.phoPhi->at(ipho),_leaf.phoEta->at(ipho));
          }
          else if (channel==_config.ELECTRON);
          CheckDRandProceed(_config.Z_GAMMA, isVJets, icand,
                    hasEventAfterPass, hasOverlap,
                    dR1, dR2, ipho, ilep1, ilep2);
      } //end of loop over ilep2

    } //end of loop over ilep1
  } //end of loop over ipho 

  nCands=icand;
  passed=_passed;
  return (icand);
//  return 1;
}//end of TFullCuts::VeryPreliminaryCut

bool TFullCuts::ZMassWindowCut(TEventTree::InputTreeLeaves& leaf, int ipho, int iele)
{
  //only for W_GAMMA, ELECTRON channel
  TLorentzVector vele, vpho;
  vele.SetPtEtaPhiM(leaf.elePt->at(iele),leaf.eleEta->at(iele),leaf.elePhi->at(iele),0);
  vpho.SetPtEtaPhiM(leaf.phoEt->at(ipho),leaf.phoEta->at(ipho),leaf.phoPhi->at(ipho),0);
  float M=(vele + vpho).M(); 
  if (M>_ZmassLeft && M<_ZmassRight) return 0;
  return 1;
}// end of ZMassWindowCut

void TFullCuts::CheckDRandProceed(int vgamma, bool isVJets, int& icand,
                    bool& hasEventAfterPass, bool& hasOverlap,
                    float dR1, float dR2, int ipho, int ilep1, int ilep2)
{

  bool passCond;
  if (vgamma==_config.W_GAMMA) passCond=_kinPhoton[ipho] && _kinLepton1[ilep1] && (dR1>_lePhoDeltaRCut);
  if (vgamma==_config.Z_GAMMA) passCond=_kinPhoton[ipho] && _kinLepton1[ilep1] && (dR1>_lePhoDeltaRCut) && _kinLepton2[ilep2] && (dR2>_lePhoDeltaRCut);
  if (!passCond) return;
  //all the rest - if (passCond)
  _passed.dRPassed++;
  if (!hasEventAfterPass) _passed.evAfterKinCuts++;
  hasEventAfterPass=1; 
  _cands[icand].ipho=ipho; 
  if (vgamma==_config.W_GAMMA || dR1<dR2){
    _cands[icand].ilep1=ilep1; _cands[icand].ilep2=ilep2;
    _cands[icand].dRlep1pho=dR1; _cands[icand].dRlep2pho=dR2;
  }
  else{
    _cands[icand].ilep1=ilep2; _cands[icand].ilep2=ilep1;
    _cands[icand].dRlep1pho=dR2; _cands[icand].dRlep2pho=dR1;
  }
  icand++;
}

bool TFullCuts::IsOverlapVJetsVGamma(int channel)
{
  bool isOverlap=0;
  float drmin=1000;

  for (int imc1 = 0; imc1 < _leaf.nMC; ++imc1){
    if( _leaf.mcPID->at(imc1) != 22 ) continue;
    if( (_leaf.mcParentage->at(imc1) & 4) == 4) continue;
    if( _leaf.mcPt->at(imc1) < 10 ) continue;
    for (int imc2 = 0; imc2 < _leaf.nMC; ++imc2){
          if( channel==_config.MUON)
            if (fabs(_leaf.mcPID->at(imc2)) != 13) continue;
          if( channel==_config.ELECTRON)
            if (fabs(_leaf.mcPID->at(imc2)) != 11) continue;
          float dr = _math.DeltaR(_leaf.mcPhi->at(imc1),_leaf.mcEta->at(imc1),_leaf.mcPhi->at(imc2),_leaf.mcEta->at(imc2));
          if (dr<drmin) drmin=dr;
    }//end of loop over imc2
  }//end of loop over imc1
  if (drmin>0.4 && drmin<1000) isOverlap=1;

  return isOverlap;
}

//float TFullCuts::DeltaR(float phi1, float eta1, float phi2, float eta2) 
//{ 
//  float deta=fabs(eta1-eta2);
//  float dphi=fabs(phi1-phi2);
//  for (dphi=fabs(phi1-phi2); dphi>=2*TMath::Pi(); dphi=dphi-2*TMath::Pi());
//  float dR=sqrt(deta*deta+dphi*dphi);
//  return dR;
//}

float TFullCuts::GetWMtCut(int year)
{
  if (year==2012) return _WMtCut2012;
  else if (year==2011) return _WMtCut2011;
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
//  TString cutStr="phoEt>";
//  cutStr+=_config.GetPhoPtMin();
//  TCut cut(cutStr);
//  return cut;
  return "1";
}

TCut TFullCuts::RangeExtraLeptonPt2011()
{
  TString cutStr="lepton1Pt>35";
  TCut cut(cutStr);
  return cut;
}

TCut TFullCuts::RangeForMetCut(int year, int channel, int vgamma, int phoWP){
  TCut cut;
  TPhotonCuts emptyPhoton;
  TCut cutPhoton=emptyPhoton.RangePhoton(year, phoWP, 0);
  TCut cutLepton;
  if (channel==_config.MUON){
    TMuonCuts emptyMuon;
    cutLepton=emptyMuon.RangeMuon(year,vgamma,1);//1st muon
    if (vgamma==_config.Z_GAMMA)
      cutLepton = cutLepton && emptyMuon.RangeMuon(year,vgamma,2);//2nd muon
  }
  else if (channel==_config.ELECTRON);
  cut = cutPhoton && cutLepton && RangePhoEt();
  if (vgamma==_config.W_GAMMA) 
    cut = cut && RangeMoreLeptonsVeto();
  if (year==2011) 
    cut = cut && RangeExtraLeptonPt2011();
  return cut;
}

TCut TFullCuts::RangeForTemplateMethodCut(int year, int channel, int vgamma, int phoWP){
  TPhotonCuts emptyPhoton;
  TCut cutPhoton=emptyPhoton.RangePhoton(year, phoWP, 0, 0);
  //no cuts on sigmaIEtaIEta and chIso
  TCut cutLepton;
  if (channel==_config.MUON){
    TMuonCuts emptyMuon;
    cutLepton=emptyMuon.RangeMuon(year,vgamma,1);//1st muon
    if (vgamma==_config.Z_GAMMA)
      cutLepton = cutLepton && emptyMuon.RangeMuon(year,vgamma,2);//2nd muon
  }
  else if (channel==_config.ELECTRON);
  TCut cut = cutPhoton && cutLepton && RangePhoEt();
  if (vgamma==_config.W_GAMMA) 
    cut = cut && RangeMoreLeptonsVeto() && RangeMetRelatedCut(year);
  if (year==2011) 
    cut = cut && RangeExtraLeptonPt2011();
  return cut;
}

TCut TFullCuts::RangeFullCut(int year, int channel, int vgamma, int phoWP)
{
  TPhotonCuts emptyPhoton;
  TCut cutPhoton=emptyPhoton.RangePhoton(year, phoWP);
  TCut cutLepton;
  if (channel==_config.MUON){
    TMuonCuts emptyMuon;
    cutLepton=emptyMuon.RangeMuon(year,vgamma,1);//1st muon
    if (vgamma==_config.Z_GAMMA)
      cutLepton = cutLepton && emptyMuon.RangeMuon(year,vgamma,2);//2nd muon
  }
  else if (channel==_config.ELECTRON);
  TCut cut = cutPhoton && cutLepton && RangePhoEt();
  if (vgamma==_config.W_GAMMA) 
    cut = cut && RangeMoreLeptonsVeto() && RangeMetRelatedCut(year);
  if (year==2011) 
    cut = cut && RangeExtraLeptonPt2011();
  return cut;
}
