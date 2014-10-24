#include "TElectronCuts.h" //class of this package
#include "TEventTree.h"

TElectronCuts::TElectronCuts()
{
}

TElectronCuts::~TElectronCuts()
{
}

bool TElectronCuts::HasMoreElectrons(int nEle, int iele, vector <float> *elePt, vector <float> *eleEta)
{
  return true;
}

bool TElectronCuts::EleID2012(TEventTree::InputTreeLeaves& leaf, int iele){
  float EffectiveArea = GetEffectiveArea((*leaf.eleSCEta)[iele]);
  if((*leaf.elePt)[iele] < 10) return false;
  bool pass = true;
  if (fabs((*leaf.eleSCEta)[iele]) < 1.4442) {
    if (fabs((*leaf.eledEtaAtVtx)[iele]) > 0.007) pass = false;
    if (fabs((*leaf.eledPhiAtVtx)[iele]) > 0.15) pass = false;
    if ((*leaf.eleSigmaIEtaIEta)[iele] > 0.01) pass = false;
    if ((*leaf.eleHoverE)[iele] > 0.12) pass = false;
    if (fabs((*leaf.eleD0Vtx)[iele][0]) > 0.02) pass = false;
    if (fabs((*leaf.eleDzVtx)[iele][0]) > 0.2) pass = false;
    if (fabs(1./(*leaf.eleEcalEn)[iele] - 1./(*leaf.elePin)[iele]) > 0.05) pass = false;
    if ((*leaf.eleConvVtxFit)[iele] == 1) pass = false;
    if ((*leaf.eleMissHits)[iele] > 1) pass = false;
    if ( ((*leaf.elePFChIso03)[iele] + TMath::Max((Double_t) 0.,(Double_t) (*leaf.elePFPhoIso03)[iele] + (*leaf.elePFNeuIso03)[iele] - EffectiveArea*leaf.rho2012))/(*leaf.elePt)[iele] > 0.15) pass = false;
  } else {
    if (fabs((*leaf.eledEtaAtVtx)[iele]) > 0.009) pass = false;
    if (fabs((*leaf.eledPhiAtVtx)[iele]) > 0.10) pass = false;
    if ((*leaf.eleSigmaIEtaIEta)[iele] > 0.03) pass = false;
    if ((*leaf.eleHoverE)[iele] > 0.1) pass = false;
    if (fabs((*leaf.eleD0Vtx)[iele][0]) > 0.02) pass = false;
    if (fabs((*leaf.eleDzVtx)[iele][0]) > 0.2) pass = false;
    if (fabs(1./(*leaf.eleEcalEn)[iele] - 1./(*leaf.elePin)[iele]) > 0.05) pass = false;
    if ((*leaf.eleConvVtxFit)[iele] == 1) pass = false;
    if ((*leaf.eleMissHits)[iele] > 1) pass = false;
    if ((*leaf.elePt)[iele] < 20){
      if ( ((*leaf.elePFChIso03)[iele] + TMath::Max((Double_t) 0., (Double_t) (*leaf.elePFPhoIso03)[iele] +   (*leaf.elePFNeuIso03)[iele] - EffectiveArea*leaf.rho2012))/(*leaf.elePt)[iele] > 0.10) pass = false;
    } else {
      if ( ((*leaf.elePFChIso03)[iele] + TMath::Max((Double_t) 0., (Double_t) (*leaf.elePFPhoIso03)[iele] +   (*leaf.elePFNeuIso03)[iele] - EffectiveArea*leaf.rho2012))/(*leaf.elePt)[iele] > 0.15) pass = false;
    }
  }
  return pass;
}

float TElectronCuts::GetEffectiveArea(float eta){
  float EffectiveArea = 0;
  if (fabs(eta) >= 0.0 && fabs(eta) < 1.0 ) EffectiveArea = 0.013 + 0.122;
  if (fabs(eta) >= 1.0 && fabs(eta) < 1.479 ) EffectiveArea = 0.021 + 0.147 ;
  if (fabs(eta) >= 1.479 && fabs(eta) < 2.0 ) EffectiveArea = 0.013 + 0.055;
  if (fabs(eta) >= 2.0 && fabs(eta) < 2.2 ) EffectiveArea = 0.010 + 0.106;
  if (fabs(eta) >= 2.2 && fabs(eta) < 2.3 ) EffectiveArea = 0.024 + 0.138;
  if (fabs(eta) >= 2.3 && fabs(eta) < 2.4 ) EffectiveArea = 0.020 + 0.221;
  if (fabs(eta) >= 2.4 ) EffectiveArea = 0.019 + 0.211;
  return EffectiveArea;
}
