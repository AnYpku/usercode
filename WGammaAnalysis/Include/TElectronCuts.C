#include "TElectronCuts.h" //class of this package
#include "TEventTree.h"
#include <iostream>

TElectronCuts::TElectronCuts()
{
}

TElectronCuts::~TElectronCuts()
{
}

bool TElectronCuts::HasMoreElectrons(TEventTree::InputTreeLeaves& leaf, int iele, bool isEvForCheck)
{
  for (int iele1=0; iele1<leaf.nEle; iele1++){
    if (iele1==iele) continue;
    if ( PassedKinematics(_config.W_GAMMA,1,leaf.elePt->at(iele1),leaf.eleSCEta->at(iele1)) &&
         EleID2012(leaf,iele1,ELE_VETO,isEvForCheck)){
      if (isEvForCheck){
        std::cout<<"HasMoreLeptons, check "<<iele1<<": "<<"pt="<<leaf.elePt->at(iele1)<<", eleSCEta="<<leaf.eleSCEta->at(iele1);
        std::cout<<", PassedKin="<<PassedKinematics(_config.W_GAMMA,1,leaf.elePt->at(iele1),leaf.eleSCEta->at(iele1));
        std::cout<<", EleID2012(WP VETO)="<<EleID2012(leaf,iele1,ELE_VETO,isEvForCheck);
        std::cout<<std::endl;
      }
      return 1;
    }
  }
  return 0;
}// end of HasMoreElectrons

bool TElectronCuts::PassedKinematics(int vgamma, bool isLead, float pt, float eta)
{
  float elePtCut;
  if (vgamma==_config.W_GAMMA) elePtCut=_elePtCut;
  else if (vgamma==_config.Z_GAMMA) 
    if (isLead) elePtCut=20.;
    else elePtCut=20;
  if (!IsBarrel(eta) && !IsEndcap(eta)) return 0;
  if (!(pt>elePtCut)) return 0;
  return 1;
}// end of PassedKinematics

bool TElectronCuts::EleID2012(TEventTree::InputTreeLeaves& leaf, int iele, int wp, bool doEventCheck){
  
  int etaBin=0;
  if (IsBarrel(leaf.eleSCEta->at(iele))) etaBin=_config.BARREL;
  else if (IsEndcap(leaf.eleSCEta->at(iele))) etaBin=_config.ENDCAP;
  else return 0;
  if (doEventCheck) std::cout<<"passed eleSCEta"<<std::endl;
  if (!(fabs(leaf.eledEtaAtVtx->at(iele))<_dEtaIn_Cut[etaBin][wp])) return 0;
  if (doEventCheck) std::cout<<"passed eledEtaAtVtx"<<std::endl;
  if (!(fabs(leaf.eledPhiAtVtx->at(iele))<_dPhiIn_Cut[etaBin][wp])) return 0;
  if (doEventCheck) std::cout<<"passed eledPhiAtVtx"<<std::endl;
  if (!(leaf.eleSigmaIEtaIEta->at(iele)<_sigmaIEtaIEta_Cut[etaBin])) return 0;
  if (doEventCheck) std::cout<<"passed eleSigmaIEtaIEta"<<std::endl;
  if (doEventCheck){
     std::cout<<"leaf.eleHoverE12->at(iele)="<<leaf.eleHoverE12->at(iele)<<", _HoverE_Cut[etaBin][wp]="<<_HoverE_Cut[etaBin][wp]<<" (iele="<<iele<<", etaBin="<<etaBin<<", wp="<<wp<<")"<<std::endl;
  }
  if (!(leaf.eleHoverE12->at(iele)<_HoverE_Cut[etaBin][wp])) return 0;
  if (doEventCheck) std::cout<<"passed eleHoverE12"<<std::endl;
  if (!(fabs((leaf.eleD0Vtx->at(iele))[0])<_d0_vtx_Cut[wp])) return 0;
  if (doEventCheck) std::cout<<"passed eleD0Vtx"<<std::endl;
  if (!(fabs((leaf.eleDzVtx->at(iele))[0])<_dZ_vtx_Cut[wp])) return 0;
  if (doEventCheck) std::cout<<"passed eleDzVtx"<<std::endl;

  if (wp==ELE_VETO);
  else{
    if (leaf.eleEcalEn->at(iele)==0 || leaf.elePt->at(iele)==0) return 0;
    float oneOverE=1.0/leaf.eleEcalEn->at(iele);
    float oneOverp=1.0/leaf.elePt->at(iele);
    if (!(fabs(oneOverE-oneOverp)<_1ovE_1ovp_Cut)) return 0;
    if (doEventCheck) std::cout<<"passed oneOverE-oneOverp"<<std::endl;
    if (!(leaf.eleConvVtxFit->at(iele)<_vertex_fit_probability_Cut)) return 0;
    if (doEventCheck) std::cout<<"passed eleConvVtxFit"<<std::endl;
    if (!(leaf.eleMissHits->at(iele)<=_missing_hits_Cut[wp])) return 0;
    if (doEventCheck) std::cout<<"passed eleMissHits"<<std::endl;
  }

  if (leaf.elePt->at(iele)==0) return 0;
  float isoCorr = GetCorrectedIsolation(leaf, iele, doEventCheck)/leaf.elePt->at(iele);
  if (doEventCheck){
   std::cout<<"leaf.elePt->at(iele)="<<leaf.elePt->at(iele)<<std::endl;
   std::cout<<"isoCorr="<<isoCorr<<std::endl;
  }// end of doEvCheck
  if (etaBin==_config.ENDCAP && leaf.elePt->at(iele)<20) {
    if (!(isoCorr<_PFisoOvPT03_E_PTless20_Cut[wp])) return 0;
  } 
  else{
    if (!(isoCorr<_PFisoOvPT03_Cut[wp])) return 0;
  }   
  if (doEventCheck) std::cout<<"passed electron isolation"<<std::endl;

  return 1;
}// end of TElectronCuts::EleID2012

float TElectronCuts::GetCorrectedIsolation(TEventTree::InputTreeLeaves& leaf, int iele, bool doEventCheck){
 float EA = GetEffectiveArea(leaf.eleSCEta->at(iele));
 float phoANDhad = leaf.elePFPhoIso03->at(iele)+leaf.elePFNeuIso03->at(iele)-leaf.rho2012*EA;
 if (phoANDhad<0) phoANDhad=0;
 if (doEventCheck){
   std::cout<<"TElectronCuts::GetCorrectedIsolation:"<<std::endl;
   std::cout<<"leaf.elePFChIso03->at(iele)="<<leaf.elePFChIso03->at(iele)<<std::endl;
   std::cout<<"leaf.elePFPhoIso03->at(iele)="<<leaf.elePFPhoIso03->at(iele)<<std::endl;
   std::cout<<"leaf.elePFNeuIso03->at(iele)="<<leaf.elePFNeuIso03->at(iele)<<std::endl;
   std::cout<<"leaf.eleSCEta->at(iele)="<<leaf.eleSCEta->at(iele)<<", EA="<<EA<<std::endl;
   std::cout<<"leaf.rho2012="<<leaf.rho2012<<std::endl;
   std::cout<<"iso="<<leaf.elePFChIso03->at(iele)+phoANDhad<<std::endl;
 }// end of doEvCheck
 return (leaf.elePFChIso03->at(iele)+ phoANDhad);
}

float TElectronCuts::GetEffectiveArea(float eta){
  //https://github.com/cms-analysis/EgammaAnalysis-ElectronTools/blob/master/interface/ElectronEffectiveArea.h#L114
  double EffectiveArea=0.0;
  // effective area for isolation
  if (fabs(eta) >= 0.0 && fabs(eta) < 1.0 ) EffectiveArea = 0.130;
  if (fabs(eta) >= 1.0 && fabs(eta) < 1.479 ) EffectiveArea = 0.137;
  if (fabs(eta) >= 1.479 && fabs(eta) < 2.0 ) EffectiveArea = 0.067;
  if (fabs(eta) >= 2.0 && fabs(eta) < 2.2 ) EffectiveArea = 0.089;
  if (fabs(eta) >= 2.2 && fabs(eta) < 2.3 ) EffectiveArea = 0.107;
  if (fabs(eta) >= 2.3 && fabs(eta) < 2.4 ) EffectiveArea = 0.110;
  if (fabs(eta) >= 2.4) EffectiveArea = 0.138;
  return EffectiveArea;
}// end of TElectronCuts::GetEffectiveArea

bool TElectronCuts::IsBarrel(float eta){
  if (fabs(eta)<1.4442) return 1;
  return 0;
}

bool TElectronCuts::IsEndcap(float eta){
  if (fabs(eta)>1.566 && fabs(eta)<2.5) return 1;
  return 0;
}
