#include "TFullCuts.h"
#include "TEventTree.h"
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

bool TFullCuts::VeryPreliminaryCut(TEventTree::InputTreeLeaves& inpTreeLeaf,  
                    zgamma::PhosphorCorrectionFunctor* photonCorrector, 
                    int channel, int vgamma, bool isVJets,
                    int& nCands, Candidate* cands,
                    PassedLevels& passed)
{
//  std::cout<<"channel="<<_config.StrChannel(channel)<<std::endl;

  // This function is called from LoopOverEvents.
  // returns 1 if entry is accepted (has at least one candidate)
  // returns 0 otherwise (has no candidates)
   
   _inpTreeLeaf=inpTreeLeaf;
   _cands=cands;
   _passed=passed;

//  std::cout<<"vgamma="<<_config.StrVgType(vgamma)<<std::endl;

   _kinPhoton=new bool[inpTreeLeaf.nPho];

   int nLe=0;
   if (channel==TConfiguration::MUON) nLe=inpTreeLeaf.nMu;
   else if (channel==TConfiguration::ELECTRON) nLe=inpTreeLeaf.nEle;
   if (channel==TConfiguration::MUON && vgamma==TConfiguration::W_GAMMA)
     if ((!inpTreeLeaf.HLT[inpTreeLeaf.HLTIndex[18]]) && (!inpTreeLeaf.HLT[inpTreeLeaf.HLTIndex[19]]))
     //_HLT_IsoMu24_eta2p1_, _HLT_IsoMu24_v
       { passed=_passed;  return 0;}
   if (channel==TConfiguration::ELECTRON && vgamma==TConfiguration::W_GAMMA);
   if (channel==TConfiguration::MUON && vgamma==TConfiguration::Z_GAMMA);
     if (!inpTreeLeaf.HLT[inpTreeLeaf.HLTIndex[21]])
     //_HLT_Mu22_Mu8_v
       { passed=_passed;  return 0;}
   if (channel==TConfiguration::ELECTRON && vgamma==TConfiguration::Z_GAMMA);

   _passed.triggerPassed++;

   if ((inpTreeLeaf.IsVtxGood)==-1){ passed=_passed; return 0;}
   _passed.goodVertexPassed++;
   if (inpTreeLeaf.nPho < 1){ passed=_passed;  return 0;}
   if (nLe < 1) { passed=_passed;  return 0;}

      //these variables are fields of TEventTree
//   if (!inpTreeLeaf.metFilters[6]) return 0;
// we don't need it because we're running Jan22 rereco
      //metFilters - is field of TEventTree
      //[6] - ecalLaserCorrFilter
      //necessary to remove spikes in the photon distributions 
      //for Jul13 rereco
   if (isVJets && IsOverlapVJetsVGamma(channel)) { passed=_passed;  return 0;}
      _passed.vgvjOverlapPassed++;

   _kinLepton1=new bool[nLe];
   bool kinLepton1Exists=0;
   _kinLepton2=new bool[nLe];
   bool kinLepton2Exists=0;

   TMuonCuts emptyMuon;


 bool kinPhotonExists=0;
  int icand=0;
  //check matching between lepton and photon
  bool hasOverlap=0;
  bool hasEventAfterPass=0;
  bool ifPassedPt=0;
  bool ifPassedEta=0;
  for (int ipho=0; ipho<_inpTreeLeaf.nPho; ipho++){   

    for (int ilep1=0; ilep1<nLe; ilep1++){ 

      //double phoEtCorr = photonCorrector->GetCorrEtData(_inpTreeLeaf.phoR9->at(ipho), 2012, _inpTreeLeaf.phoEt->at(ipho), _inpTreeLeaf.phoEta->at(ipho));

      _kinPhoton[ipho]=_emptyPhoton.PassedKinematics(_inpTreeLeaf.phoEt->at(ipho),_inpTreeLeaf.phoSCEta->at(ipho),ifPassedPt,ifPassedEta);
      if (ifPassedPt) _passed.phoPtPassed++; else continue;
      if (ifPassedEta) _passed.phoEtaPassed++; else continue;
   
      if (channel==TConfiguration::MUON){
        _kinLepton1[ilep1] = _emptyMuon.PassedKinematics(_inpTreeLeaf.muPt->at(ilep1),_inpTreeLeaf.muEta->at(ilep1),ifPassedPt,ifPassedEta);
        if (ifPassedPt) _passed.leptonPtPassed++; else continue;
        if (ifPassedEta) _passed.leptonEtaPassed++; else continue;
      }
      else if (channel==TConfiguration::ELECTRON);
      float dR1;
      if (channel==TConfiguration::MUON){
        dR1=DeltaR(_inpTreeLeaf.muPhi->at(ilep1),_inpTreeLeaf.muEta->at(ilep1),_inpTreeLeaf.phoPhi->at(ipho),_inpTreeLeaf.phoEta->at(ipho));
      }
      else if (channel==TConfiguration::ELECTRON);
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
          if (channel==TConfiguration::MUON){
            _kinLepton2[ilep2] = _emptyMuon.PassedKinematics(_inpTreeLeaf.muPt->at(ilep2),_inpTreeLeaf.muEta->at(ilep2),ifPassedPt,ifPassedEta);
            if (!_kinLepton2[ilep2]) continue;
            vlep1.SetPtEtaPhiM(_inpTreeLeaf.muPt->at(ilep1),_inpTreeLeaf.muEta->at(ilep1),_inpTreeLeaf.muPhi->at(ilep1),0);
            vlep2.SetPtEtaPhiM(_inpTreeLeaf.muPt->at(ilep2),_inpTreeLeaf.muEta->at(ilep2),_inpTreeLeaf.muPhi->at(ilep2),0);
          }
          else if (channel==TConfiguration::ELECTRON);
          if((vlep1 + vlep2).M() < 50) continue;

          float dR2;
          if (channel==TConfiguration::MUON){
            dR2=DeltaR(_inpTreeLeaf.muPhi->at(ilep2),_inpTreeLeaf.muEta->at(ilep2),_inpTreeLeaf.phoPhi->at(ipho),_inpTreeLeaf.phoEta->at(ipho));
          }
          else if (channel==TConfiguration::ELECTRON);
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

  for (int imc1 = 0; imc1 < _inpTreeLeaf.nMC; ++imc1){
    if( _inpTreeLeaf.mcPID->at(imc1) != 22 ) continue;
    if( (_inpTreeLeaf.mcParentage->at(imc1) & 4) == 4) continue;
    if( _inpTreeLeaf.mcPt->at(imc1) < 10 ) continue;
    for (int imc2 = 0; imc2 < _inpTreeLeaf.nMC; ++imc2){
          if( channel==_config.MUON)
            if (fabs(_inpTreeLeaf.mcPID->at(imc2)) != 13) continue;
          if( channel==_config.ELECTRON)
            if (fabs(_inpTreeLeaf.mcPID->at(imc2)) != 11) continue;
          float dr = DeltaR(_inpTreeLeaf.mcPhi->at(imc1),_inpTreeLeaf.mcEta->at(imc1),_inpTreeLeaf.mcPhi->at(imc2),_inpTreeLeaf.mcEta->at(imc2));
          if (dr<drmin) drmin=dr;
    }//end of loop over imc2
  }//end of loop over imc1
  if (drmin>0.4 && drmin<1000) isOverlap=1;

  return isOverlap;
}

float TFullCuts::DeltaR(float phi1, float eta1, float phi2, float eta2) 
{ 

  float deta=fabs(eta1-eta2);
  float dphi=fabs(phi1-phi2);
  for (dphi=fabs(phi1-phi2); dphi>=2*TMath::Pi(); dphi=dphi-2*TMath::Pi());
  float dR=sqrt(deta*deta+dphi*dphi);
  return dR;
}

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
  if (channel==TConfiguration::MUON){
    TMuonCuts emptyMuon;
    cutLepton=emptyMuon.RangeMuon(year,vgamma,1);//1st muon
    if (vgamma==_config.Z_GAMMA)
      cutLepton = cutLepton && emptyMuon.RangeMuon(year,vgamma,2);//2nd muon
  }
  else if (channel==TConfiguration::ELECTRON);
  cut = cutPhoton && cutLepton && RangePhoEt();
  if (vgamma==_config.W_GAMMA) 
    cut = cut && RangeMoreLeptonsVeto();
  if (year==2011) 
    cut = cut && RangeExtraLeptonPt2011();
  return cut;
}

TCut TFullCuts::RangeForTemplateMethodCut(int year, int channel, int vgamma, int phoWP){
  TPhotonCuts emptyPhoton;
  TCut cutPhoton=emptyPhoton.RangePhoton(year, phoWP, 1, 0, 0);
  //no cuts on sigmaIEtaIEta and chIso
  TCut cutLepton;
  if (channel==TConfiguration::MUON){
    TMuonCuts emptyMuon;
    cutLepton=emptyMuon.RangeMuon(year,vgamma,1);//1st muon
    if (vgamma==_config.Z_GAMMA)
      cutLepton = cutLepton && emptyMuon.RangeMuon(year,vgamma,2);//2nd muon
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
    cutLepton=emptyMuon.RangeMuon(year,vgamma,1);//1st muon
    if (vgamma==_config.Z_GAMMA)
      cutLepton = cutLepton && emptyMuon.RangeMuon(year,vgamma,2);//2nd muon
  }
  else if (channel==TConfiguration::ELECTRON);
  TCut cut = cutPhoton && cutLepton && RangePhoEt();
  if (vgamma==_config.W_GAMMA) 
    cut = cut && RangeMoreLeptonsVeto() && RangeMetRelatedCut(year);
  if (year==2011) 
    cut = cut && RangeExtraLeptonPt2011();
  return cut;
}
