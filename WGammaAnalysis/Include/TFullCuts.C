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

   _isEvForCheck=0;
   if (leaf.event==6136849 || leaf.event==2559882 || leaf.event==479598 || leaf.event==3574696 || leaf.event==5208885) _isEvForCheck=1;
// 16135540 passed but rejected later
   if (_isEvForCheck) {std::cout<<std::endl; std::cout<<"event="<<leaf.event<<std::endl;}

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

   _passed.metFilters++;
  
   if (_isEvForCheck) std::cout<<"passed met filters"<<std::endl;

   _nLep=0;
   if (channel==_config.MUON) _nLep=leaf.nMu;
   else if (channel==_config.ELECTRON) _nLep=leaf.nEle;

   if (!TriggerCut(channel,vgamma)) { passed=_passed; return 0;}
   _passed.trigger++;
   if (_isEvForCheck) std::cout<<"passed trigger"<<std::endl;

   if ((leaf.IsVtxGood)==-1){ passed=_passed; return 0;}
   _passed.goodVertex++;
   if (_isEvForCheck) std::cout<<"passed good vertex"<<std::endl;

   if (leaf.nPho < 1){ passed=_passed;  return 0;}
   if (_nLep < 1) { passed=_passed;  return 0;}

   if (isVJets && IsOverlapVJetsVGamma(channel)) { passed=_passed;  return 0;}
      _passed.vgvjOverlap++;
   if (_isEvForCheck) std::cout<<"passed overlap removal"<<std::endl;

   int nGoodPhotons=0;
   int nGoodLeptonsLead=0;
   int nGoodLeptonsSublead=0;//between sublead and lead

  int icand=0;
  //check matching between lepton and photon
  bool hasOverlap=0;
  bool hasEventAfterPass=0;

  // find good photons
  nGoodPhotons=FindGoodPhotons(channel,vgamma);
  if (nGoodPhotons==0) {passed=_passed; return 0;} 
  _passed.photon++;
   if (_isEvForCheck) {
     std::cout<<"passed photon; nGoodPhotons="<<nGoodPhotons<<std::endl;
     std::cout<<"ipho good: ";
     for (int ipho=0; ipho<_leaf.nPho; ipho++) 
       if (_passedPhoton[ipho]) std::cout<<ipho<<", ";
     std::cout<<std::endl;
   }//end of (_isEvForCheck)

  // find good leptons
  _ilepLead=-1;
  _ilepSublead=-1;
  _ilepLead=FindGoodLeptonIndex(channel,vgamma,1);
  _ilepSublead=FindGoodLeptonIndex(channel,vgamma,0);
  if (_ilepLead==-1) {passed=_passed; return 0;} 
  if (vgamma==_config.Z_GAMMA && _ilepSublead==-1) {passed=_passed; return 0;} 
  _passed.lepton++;
   if (_isEvForCheck) {std::cout<<"passed lepton; _ilepLead="<<_ilepLead<<", _ilepSublead="<<_ilepSublead<<std::endl;}


  // second lepton veto for W_GAMMA
  if (vgamma==_config.W_GAMMA){
    if (_isEvForCheck)
      std::cout<<"passedLepton = "<<std::endl;
      if (channel==_config.MUON)
        if (_muon.HasMoreMuons(_leaf,_ilepLead)) {passed=_passed; return 0;}
      if (channel==_config.ELECTRON)
        if (_electron.HasMoreElectrons(_leaf,_ilepLead, _isEvForCheck)) {
          passed=_passed; return 0;
        }
  }//end of if (vgamma==W_GAMMA)
  _passed.secondLeptonVeto++;
   if (_isEvForCheck) std::cout<<"passed second lepton veto"<<std::endl;

  // check lep-lep invariant mass for Z_GAMMA case
  if (vgamma==_config.Z_GAMMA) 
    if (!MLeptonLeptonCut(channel)) {passed=_passed; return 0;}
  _passed.leptonInvMass++;
  if (_isEvForCheck) std::cout<<"passed lepton inv mass"<<std::endl;
 
  // check pho-lep cuts (dR and Zmass window for W_GAMMA ELECTRON)
  // and form candidate if passed
  bool pass = CheckDRandProceed(channel, vgamma, isVJets, icand);
  if (!pass) {passed=_passed; return 0;}
  if (_isEvForCheck) std::cout<<"passed dR"<<std::endl;

  nCands=icand;
  passed=_passed;
  if (nCands==0) return 0;
  if (_isEvForCheck) std::cout<<"I passed; nCands="<<nCands<<std::endl;

  return 1;
}//end of TFullCuts::VeryPreliminaryCut

int TFullCuts::FindGoodPhotons(int channel, int vgamma)
{
  int nGoodPhotons=0;
  bool ifPassedPt=0;
  bool ifPassedEta=0;
  for (int ipho=0; ipho<_leaf.nPho; ipho++){ 
      _passedPhoton[ipho]=0;
      _photon.PassedKinematics(_leaf.phoEt->at(ipho),_leaf.phoSCEta->at(ipho),ifPassedPt,ifPassedEta);
      if (ifPassedPt) _passed.phoPt++; else continue;
      if (ifPassedEta) _passed.phoEta++; else continue;
      if (_isEvForCheck) std::cout<<"passed phoPt and phoEta"<<std::endl;
//      if (vgamma==_config.W_GAMMA && channel==_config.ELECTRON){
//        if (_isEvForCheck) std::cout<<"WGamma, Electron cuts:"<<std::endl;
//        if (_photon.HasMatchingGSFelectron(_leaf,ipho)) continue;
//        if (_isEvForCheck) std::cout<<"passed HasMatchingGSFelectron"<<std::endl;
//        if (_leaf.phohasPixelSeed->at(ipho)) continue;
//        if (_isEvForCheck) std::cout<<"passed phohasPixelSeed"<<std::endl;
//      }
//      we decided to remove pixel seed veto in order to be able to use 
//      centrally provided scale factors which 
//      were computed for the standard photon ID
//      e -> gamma data driven background estimation 
//      will be done for the electron channel of Wgamma
      _passedPhoton[ipho]=1;   
      nGoodPhotons++;
  } //end of loop over ipho
  return nGoodPhotons;
}// end of FindGoodPhotons

int TFullCuts::FindGoodLeptonIndex(int channel, int vgamma, bool isLead)
{
  int nGoodLeptons=0;
  bool ifPassedPt=0;
  bool ifPassedEta=0;
  for (int ilep=0; ilep<_nLep; ilep++){  
    if (!isLead && ilep==_ilepLead) continue;
//    _passedLepton[ilep]=0;
    if (channel==_config.MUON){
      bool passKin = _muon.PassedKinematics(vgamma, isLead, _leaf.muPt->at(ilep),_leaf.muEta->at(ilep),ifPassedPt,ifPassedEta);
      if (ifPassedPt) _passed.leptonPt++; else continue;
      if (_isEvForCheck) std::cout<<"passed lepton Pt"<<std::endl;
      if (ifPassedEta) _passed.leptonEta++; else continue;
      if (_isEvForCheck) std::cout<<"passed lepton Eta"<<std::endl;
      if (!passKin) continue;
      if (_isEvForCheck) std::cout<<"passed lepton kin"<<std::endl;
      if (!_muon.MuId(2012,_leaf,ilep)) continue;
      if (_isEvForCheck) std::cout<<"passed MuId"<<std::endl;
    }
    else if (channel==_config.ELECTRON){
      bool passKin = _electron.PassedKinematics(vgamma, isLead, _leaf.elePt->at(ilep),_leaf.eleSCEta->at(ilep));
      if (!passKin) continue;
      if (_isEvForCheck) std::cout<<"passed lepton kin"<<std::endl;
      if (!_electron.EleID2012(_leaf,ilep,_electron.ELE_MEDIUM,_isEvForCheck)) continue;
      if (_isEvForCheck) std::cout<<"passed ele ID MEDIUM"<<std::endl;
    }
    _passed.leptonId++;
    if (!TriggerMatch(channel,vgamma,ilep)) continue;
    if (_isEvForCheck) std::cout<<"passed trigger match"<<std::endl;
    _passed.leptonTriggerMatch++;
    return ilep;
  }//end of loop over ilep1
  return -1; 
}// end of FindGoodLeptons

bool TFullCuts::MLeptonLeptonCut(int channel)
{
  TLorentzVector vlep1, vlep2;
  if (channel==_config.MUON){
    vlep1.SetPtEtaPhiM(_leaf.muPt->at(_ilepLead),_leaf.muEta->at(_ilepLead),_leaf.muPhi->at(_ilepLead),0);
    vlep2.SetPtEtaPhiM(_leaf.muPt->at(_ilepSublead),_leaf.muEta->at(_ilepSublead),_leaf.muPhi->at(_ilepSublead),0);
  }
  else if (channel==_config.ELECTRON){
    vlep1.SetPtEtaPhiM(_leaf.elePt->at(_ilepLead),_leaf.eleEta->at(_ilepLead),_leaf.elePhi->at(_ilepLead),0);
    vlep2.SetPtEtaPhiM(_leaf.elePt->at(_ilepSublead),_leaf.eleEta->at(_ilepSublead),_leaf.elePhi->at(_ilepSublead),0);
  }
  if((vlep1 + vlep2).M() > 50) return 1;
  return 0;
}// end of MLeptonLeptonCut()

bool TFullCuts::ZMassWindowCut(int ipho, int iele)
{
  //only for W_GAMMA, ELECTRON channel
  TLorentzVector vele, vpho;
  vele.SetPtEtaPhiM(_leaf.elePt->at(iele),_leaf.eleEta->at(iele),_leaf.elePhi->at(iele),0);
  vpho.SetPtEtaPhiM(_leaf.phoEt->at(ipho),_leaf.phoEta->at(ipho),_leaf.phoPhi->at(ipho),0);
  float M=(vele + vpho).M(); 
  if (_isEvForCheck){
    std::cout<<"M(ele,pho)="<<M;
    std::cout<<", iele="<<iele<<", elePt="<<_leaf.elePt->at(iele)<<", eleEta="<<_leaf.eleEta->at(iele)<<", elePhi="<<_leaf.elePhi->at(iele);
    std::cout<<", ipho="<<ipho<<", phoPt="<<_leaf.phoEt->at(ipho)<<", phoEta="<<_leaf.phoEta->at(ipho)<<", phoPhi="<<_leaf.phoPhi->at(ipho);
    std::cout<<std::endl;
  }
  if (M>_ZmassLeft && M<_ZmassRight) return 0;
  return 1;
}// end of ZMassWindowCut

bool TFullCuts::TriggerCut(int channel, int vgamma)
{
   if (channel==_config.MUON && vgamma==_config.W_GAMMA){
     if (_leaf.HLTIndex[18]>=0 && _leaf.HLT[_leaf.HLTIndex[18]]) return 1; 
     //_HLT_IsoMu24_eta2p1_
     else  return 0;
   }// MUON, W_GAMMA

   if (channel==_config.ELECTRON && vgamma==_config.W_GAMMA){
     if (_leaf.HLTIndex[17]>=0 && _leaf.HLT[_leaf.HLTIndex[17]]) return 1;
     //_HLT_Ele27_WP80_v
     else return 0;
   }// ELECTRON, W_GAMMA

   if (channel==_config.MUON && vgamma==_config.Z_GAMMA){
     if (_leaf.HLTIndex[14]>=0 && _leaf.HLT[_leaf.HLTIndex[14]]) return 1;
     //_HLT_Mu17_Mu8_v
     else return 0;
   }// MUON, Z_GAMMA

   if (channel==_config.ELECTRON && vgamma==_config.Z_GAMMA){
     if (_leaf.HLTIndex[9]>=0 && _leaf.HLT[_leaf.HLTIndex[9]]) return 1;
     //_HLT_Ele17_Ele8_v, many other conditions
     else return 0;
   }// ELECTRON, Z_GAMMA

   return 0; 
}

bool TFullCuts::TriggerMatch(int channel, int vgamma, int ilep)
{
   if (channel==_config.MUON && vgamma==_config.W_GAMMA){
     if (_leaf.HLTIndex[18]>=0 && _leaf.HLT[_leaf.HLTIndex[18]] && (_leaf.muTrg->at(ilep)%2) ) 
       return 1; 
     //_HLT_IsoMu24_eta2p1_
     //"muonTriggerMatchHLTIsoMu24eta2p1"
     else  return 0;
   }// MUON, W_GAMMA

   if (channel==_config.ELECTRON && vgamma==_config.W_GAMMA){
     if (_leaf.HLTIndex[17]>=0 && _leaf.HLT[_leaf.HLTIndex[17]] && (_leaf.eleTrg->at(ilep)%2) ) 
       return 1;
     //_HLT_Ele27_WP80_v
     //"electronTriggerMatchHLTEle27WP80"
     else return 0;
   }// ELECTRON, W_GAMMA

   if (channel==_config.MUON && vgamma==_config.Z_GAMMA){
     if (_leaf.HLTIndex[14]>=0 && _leaf.HLT[_leaf.HLTIndex[14]] && ((_leaf.muTrg->at(ilep)/4)%2) ) 
       return 1;
     //_HLT_Mu17_Mu8_v
     //"muonTriggerMatchHLTMu17Mu8"
     else return 0;
   }// MUON, Z_GAMMA

   if (channel==_config.ELECTRON && vgamma==_config.Z_GAMMA){
     if (_leaf.HLTIndex[9]>=0 && _leaf.HLT[_leaf.HLTIndex[9]]  && ((_leaf.eleTrg->at(ilep)/16)%2) ) 
       return 1;
     //"HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v"
     //"electronTriggerMatchEle17CaloIdTCaloIsoVLTrkIdVLTrkIsoVLEle8CaloIdTCaloIsoVLTrkIdVLTrkIsoVL"
     else return 0;
   }// ELECTRON, Z_GAMMA

   return 0; 
}

bool TFullCuts::CheckDRandProceed(int channel, int vgamma, bool isVJets, int& icand)
{

  for (int ipho=0; ipho<_leaf.nPho; ipho++){
      float dR1=-1, dR2=-1;
      if (_isEvForCheck) {
        std::cout<<"ipho good: ";
        for (int ipho=0; ipho<_leaf.nPho; ipho++) 
          if (_passedPhoton[ipho]) std::cout<<ipho<<", ";
        std::cout<<std::endl;
      }//end of (_isEvForCheck)
      if (!_passedPhoton[ipho]) continue;
      if (_isEvForCheck)
        std::cout<<"passed ipho="<<ipho<<std::endl;
      if (vgamma==_config.W_GAMMA && channel==_config.ELECTRON)
        {  if (!ZMassWindowCut(ipho,_ilepLead)) continue; }
      _passed.zMassWindow++;
      if (_isEvForCheck)
        std::cout<<"passed zMassWindow"<<std::endl;

      float lepPhi, lepEta;
      if (channel==_config.MUON)
        { lepPhi=_leaf.muPhi->at(_ilepLead); lepEta=_leaf.muEta->at(_ilepLead); }
      if (channel==_config.ELECTRON)
        { lepPhi=_leaf.elePhi->at(_ilepLead); lepEta=_leaf.eleEta->at(_ilepLead); }

      dR1=_math.DeltaR(lepPhi,lepEta,_leaf.phoPhi->at(ipho),_leaf.phoEta->at(ipho));
      if (_isEvForCheck)
        std::cout<<"ipho="<<ipho<<", ilep="<<_ilepLead<<", dR1="<<dR1<<std::endl;
      if (dR1<_lePhoDeltaRPreCut) continue;
      if (vgamma==_config.W_GAMMA && dR1<_lePhoDeltaRCut) continue;
      else if (vgamma==_config.Z_GAMMA){ // if Z_GAMMA and first dR already found
        if (channel==_config.MUON)
          { lepPhi=_leaf.muPhi->at(_ilepSublead); lepEta=_leaf.muEta->at(_ilepSublead); }
        if (channel==_config.ELECTRON)
          { lepPhi=_leaf.elePhi->at(_ilepSublead); lepEta=_leaf.eleEta->at(_ilepSublead); }
        dR2=_math.DeltaR(lepPhi,lepEta,_leaf.phoPhi->at(ipho),_leaf.phoEta->at(ipho));
        if (dR2<_lePhoDeltaRPreCut) continue;
      }
      _passed.dR++;
      if (_isEvForCheck)
        std::cout<<"passed dR"<<std::endl;

    _cands[icand].ipho=ipho; 
    if (vgamma==_config.W_GAMMA || dR1<dR2){
      _cands[icand].ilep1=_ilepLead; _cands[icand].ilep2=_ilepSublead;
      _cands[icand].dRlep1pho=dR1; _cands[icand].dRlep2pho=dR2;
    }
    else{
      _cands[icand].ilep1=_ilepSublead; _cands[icand].ilep2=_ilepLead;
      _cands[icand].dRlep1pho=dR2; _cands[icand].dRlep2pho=dR1;
    }

    icand++;
  }//end of loop over ipho
  return icand;
}// end of CheckDRandProceed

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

float TFullCuts::GetWMtCut(int year, int channel)
{
  if (year==2012) return _WMtCut2012[channel];
  else if (year==2011) return _WMtCut2011;
  return _WMtCut2012[channel];
}

TCut TFullCuts::RangeMoreLeptonsVeto(){
  return "!hasMoreLeptons";
}

TCut TFullCuts::RangeMetRelatedCut(int year, int channel)
{
//  TString cutStr="WMt>";
  TString cutStr="sqrt(2*lepton1Pt*pfMET_notSmeared*(1-cos(lepton1Phi-pfMETPhi_notSmeared)))>";
//_WMt = sqrt(2*_lepPt[0]*_pfMET*(1-cos(_lepPhi[0]-_pfMETPhi)));
  if (year==2012) cutStr+=_WMtCut2012[channel];
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

TCut TFullCuts::RangeBlinding(int blind)
{
  TCut cut="1";
  if (blind==_config.UNBLIND) return cut;
  TString strCut=" (event % ";
  strCut+=_config.GetBlindPrescale();
  strCut+=")==0";
  TCut cutPRESCALE(strCut);
  if (blind==_config.BLIND_PRESCALE){
    // (event % N) == 0
    cut=cutPRESCALE;
  }
  if (blind==_config.BLIND_COMBINED){
    // pt<=PTc || (pt>PTc && (event % N) == 0)
    TString strLOWPT="phoEt<=";
    strLOWPT+=_config.GetPhoPtBlindThreshold();
    TCut cutLOWPT(strLOWPT);
    TString strHIGHPT="phoEt>";
    strHIGHPT+=_config.GetPhoPtBlindThreshold();
    TCut cutHIGHPT(strHIGHPT);
    cut = cutLOWPT || (cutHIGHPT && cutPRESCALE);
  }
  return cut;
}// end of RangeBlinding(int blind)

TCut TFullCuts::RangeExtraLeptonPt2011()
{
  TString cutStr="lepton1Pt>35";
  TCut cut(cutStr);
  return cut;
}// end of RangeExtraLeptonPt2011

TCut TFullCuts::RangeDeltaR(int vgamma)
{
  TCut cut="lep1PhoDeltaR>0.7";
  TCut cut2="lep2PhoDeltaR>0.7";
  if (vgamma==_config.Z_GAMMA) cut = (cut && cut2);
  return cut;
}

TCut TFullCuts::RangeForTemplateMethodCut(int year, int channel, int vgamma, int blind, int phoWP){
  //all cuts except phoSigmaIEtaIEta and phoChIso
  TCut cutPhoton=_photon.RangePhoton(year, phoWP, 0, 0);
  TCut cut = cutPhoton && RangeDeltaR(vgamma); 
  if (vgamma==_config.W_GAMMA) {
    cut = cut && RangeMetRelatedCut(year,channel);
  }
  if (year==2011) 
    cut = cut && RangeExtraLeptonPt2011();
  if (blind!=_config.UNBLIND)
    cut = cut && RangeBlinding(blind);
  return cut;
}// end of RangeForTemplateMethodCut

TCut TFullCuts::RangeFullCut(int year, int channel, int vgamma, int blind, int phoWP){
  //all cuts 
  TCut cutPhoton=_photon.RangePhoton(year, phoWP);
  TCut cut = cutPhoton && RangeDeltaR(vgamma); 
  if (vgamma==_config.W_GAMMA) {
    cut = cut && RangeMetRelatedCut(year,channel);
  }
  if (year==2011) 
    cut = cut && RangeExtraLeptonPt2011();
  if (blind!=_config.UNBLIND)
    cut = cut && RangeBlinding(blind);
  return cut;
}//end of RangeFullCut

TCut TFullCuts::RangeFsrCut(int channel)
{
  TCut cut;
//  cut = "Mpholeplep<101 && Mpholeplep>81 && (lep1PhoDeltaR<0.8 || lep2PhoDeltaR<0.8)";
  cut = "Mpholeplep<101 && Mpholeplep>81 && lep1PhoDeltaR>0.4";
  cut = cut && _photon.RangePhoton(2012, _photon.WP_LOOSE, 0, 0);// 0 - no sigmaIEtaIEta cut
  return cut;
}

void TFullCuts::SetPassedToZeros(PassedLevels& p)
{
    p.metFilters=0;
    p.trigger=0;
    p.goodVertex=0;
    p.vgvjOverlap=0;
    p.phoPt=0;
    p.phoEta=0;
    p.photon=0;
    p.leptonPt=0;
    p.leptonEta=0;
    p.leptonId=0;
    p.leptonTriggerMatch=0;
    p.lepton=0;
    p.secondLeptonVeto=0;
    p.leptonInvMass=0;
    p.zMassWindow=0;
    p.dR=0;
}// end of SetPassedToZeros

void TFullCuts::Print(PassedLevels& p)
{
    std::cout<<std::endl;
    std::cout<<"metFilters="<<p.metFilters<<"; ";
    std::cout<<"trigger="<<p.trigger<<"; ";
    std::cout<<"goodVertex="<<p.goodVertex<<"; ";
    std::cout<<"vgvjOverlap="<<p.vgvjOverlap<<std::endl;

    std::cout<<"phoPt="<<p.phoPt<<"; ";
    std::cout<<"phoEta="<<p.phoEta<<"; ";
    std::cout<<"photon="<<p.photon<<std::endl;

    std::cout<<"leptonPt="<<p.leptonPt<<"; ";
    std::cout<<"leptonEta="<<p.leptonEta<<"; ";
    std::cout<<"leptonId="<<p.leptonId<<"; ";
    std::cout<<"leptonTriggerMatch="<<p.leptonTriggerMatch<<std::endl;

    std::cout<<"lepton="<<p.lepton<<"; ";
    std::cout<<"secondLeptonVeto="<<p.secondLeptonVeto<<"; ";
    std::cout<<"leptonInvMass="<<p.leptonInvMass<<std::endl;

    std::cout<<"zMassWindow="<<p.zMassWindow<<", ";
    std::cout<<"dR="<<p.dR<<std::endl;
    std::cout<<std::endl;
}// end of SetPassedToZeros
