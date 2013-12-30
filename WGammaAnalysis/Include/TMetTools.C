#include "TMetTools.h"

#include "TLorentzVector.h"
#include "TRandom3.h"

#include <vector>
#include <iostream>

TMetTools::TMetTools()
{
}

TMetTools::TMetTools(int event, float recoPfMET, float recoPfMETPhi,
              int nLowPtJet,
              vector <float> *jetLowPtRawPt,
              vector <float> *jetLowPtRawEn,
              vector <float> *jetLowPtPt,
              vector <float> *jetLowPtEta,
              vector <float> *jetLowPtPhi,
              vector <float> *jetLowPtGenJetPt,
              vector <float> *jetLowPtGenJetEta,
              vector <float> *jetLowPtGenJetPhi,
              int nJet,
              vector <float> *jetRawPt,
              vector <float> *jetRawEn,
              vector <float> *jetPt,
              vector <float> *jetEta,
              vector <float> *jetPhi,
              vector <float> *jetGenJetPt,
              vector <float> *jetGenJetEta,
              vector <float> *jetGenJetPhi)
{
  event_=event;
  recoPfMET_=recoPfMET;
  recoPfMETPhi_=recoPfMETPhi;
  nLowPtJet_=nLowPtJet;
  for (int i=0; i<nLowPtJet_; i++) {
    jetLowPtRawPt_.push_back(jetLowPtRawPt->at(i));
    jetLowPtRawEn_.push_back(jetLowPtRawEn->at(i));
    jetLowPtPt_.push_back(jetLowPtPt->at(i));
    jetLowPtEta_.push_back(jetLowPtEta->at(i));
    jetLowPtPhi_.push_back(jetLowPtPhi->at(i));
    jetLowPtGenJetPt_.push_back(jetLowPtGenJetPt->at(i));
    jetLowPtGenJetEta_.push_back(jetLowPtGenJetEta->at(i));
    jetLowPtGenJetPhi_.push_back(jetLowPtGenJetPhi->at(i));    
  }
  nJet_=nJet;
  for (int i=0; i<nJet_; i++) {
    jetRawPt_.push_back(jetRawPt->at(i));
    jetRawEn_.push_back(jetRawEn->at(i));
    jetPt_.push_back(jetPt->at(i));
    jetEta_.push_back(jetEta->at(i));
    jetPhi_.push_back(jetPhi->at(i));
    jetGenJetPt_.push_back(jetGenJetPt->at(i));
    jetGenJetEta_.push_back(jetGenJetEta->at(i));
    jetGenJetPhi_.push_back(jetGenJetPhi->at(i));    
  }
}

TMetTools::TMetTools(int event, float recoPfMET, float recoPfMETPhi,
              int nJet,
              vector <float> *jetRawPt,
              vector <float> *jetRawEn,
              vector <float> *jetPt,
              vector <float> *jetEta,
              vector <float> *jetPhi,
              vector <float> *jetGenJetPt,
              vector <float> *jetGenJetEta,
              vector <float> *jetGenJetPhi)
{
  event_=event;
  recoPfMET_=recoPfMET;
  recoPfMETPhi_=recoPfMETPhi;
  nLowPtJet_=0;
  nJet_=nJet;
  for (int i=0; i<nJet_; i++) {
    jetRawPt_.push_back(jetRawPt->at(i));
    jetRawEn_.push_back(jetRawEn->at(i));
    jetPt_.push_back(jetPt->at(i));
    jetEta_.push_back(jetEta->at(i));
    jetPhi_.push_back(jetPhi->at(i));
    jetGenJetPt_.push_back(jetGenJetPt->at(i));
    jetGenJetEta_.push_back(jetGenJetEta->at(i));
    jetGenJetPhi_.push_back(jetGenJetPhi->at(i));   
  }
}

TMetTools::~TMetTools()
{
}

// MET Correction Step 1: MC smearing
void TMetTools::METSmearCorrection() {
  //TRandom3 genRan((unsigned int)event_);

  jetSumSmeared_.SetXYZT(0.,0.,0.,0.);
  jetSumRaw_.SetXYZT(0.,0.,0.,0.);
    
  // jets of 5<rawPt<15
  LoopOverJetsForSmearing(nLowPtJet_, jetLowPtRawPt_, jetLowPtRawEn_, jetLowPtPt_,  jetLowPtEta_, jetLowPtPhi_);

  // jets of rawPt>=15
  LoopOverJetsForSmearing(nJet_, jetRawPt_, jetRawEn_, jetPt_, jetEta_,  jetPhi_);

  double corrMetPx = recoPfMET_*(TMath::Cos(recoPfMETPhi_));
  double corrMetPy = recoPfMET_*(TMath::Sin(recoPfMETPhi_));
  TLorentzVector smearCorrectedMET;
  smearCorrectedMET.SetPxPyPzE(corrMetPx, corrMetPy, 0., sqrt(corrMetPx*corrMetPx+corrMetPy*corrMetPy));
  smearCorrectedMET = smearCorrectedMET + jetSumRaw_ - jetSumSmeared_;
  recoPfMET_    = smearCorrectedMET.Pt();
  recoPfMETPhi_ = smearCorrectedMET.Phi();

}

void TMetTools::LoopOverJetsForSmearing(int nJet, vector <float> jetRawPt, vector <float> jetRawEn, vector <float> jetPt, vector <float> jetEta, vector <float> jetPhi) {

  for (int iJet=0; iJet<nJet; iJet++) {
    //jetRawPtSmeared[iJet] = jetRawPt[iJet];
    //jetRawEnSmeared[iJet] = jetRawEn[iJet];

    // jet basic cuts
    if (jetRawPt[iJet]<10. || fabs(jetEta[iJet])>4.7) continue;

    double SmearC = GetMETJetSmearFactor(jetEta[iJet]);
    Double_t minDRgenJet(999.);
    int      genMatched(-999);
    int      typeMatched(0);

    // Search for gen jet, soft jets
    for (int iGenJet=0; iGenJet<nLowPtJet_; iGenJet++) {
      if (jetLowPtGenJetPt_[iGenJet]<0.) continue;

      Double_t dR_jg  = DeltaR(jetLowPtGenJetEta_[iGenJet],jetLowPtGenJetPhi_[iGenJet],jetEta[iJet],jetPhi[iJet]);
      double   expres = ErrEt(jetPt[iJet],jetEta[iJet]);

      if(dR_jg < minDRgenJet && (jetPt[iJet]-jetLowPtGenJetPt_[iGenJet])/jetPt[iJet] < 5. * expres) {
        typeMatched = 1;
        genMatched  = iGenJet;
        minDRgenJet = dR_jg;
      }
    } // loop over low Pt jets gen level values ends
    // Search for gen jet
    for (int iGenJet=0; iGenJet<nJet_; iGenJet++) {
      if (jetGenJetPt_[iGenJet]<0.) continue;
      
      Double_t dR_jg  = DeltaR(jetGenJetEta_[iGenJet],jetGenJetPhi_[iGenJet],jetEta[iJet],jetPhi[iJet]);
      double   expres = ErrEt(jetPt[iJet],jetEta[iJet]);

      if(dR_jg < minDRgenJet && (jetPt[iJet]-jetGenJetPt_[iGenJet])/jetPt[iJet] < 5. * expres) {
        typeMatched = 2;
        genMatched  = iGenJet;
        minDRgenJet = dR_jg;
      }
    } // loop over "normal" Pt jets gen level values ends

    if ((genMatched>-1) && (typeMatched==1) && (minDRgenJet > 0.1 + 0.3 * exp(-0.05*(jetLowPtGenJetPt_[genMatched]-10)))) genMatched = -999;
    if ((genMatched>-1) && (typeMatched==2) && (minDRgenJet > 0.1 + 0.3 * exp(-0.05*(jetGenJetPt_[genMatched]-10))))      genMatched = -999;

    double SmearRes = 0.;
    if(genMatched>-1 && typeMatched>0) {
      if (     typeMatched==1) SmearRes = (SmearC-1.) * (jetPt[iJet]-jetLowPtGenJetPt_[genMatched])/jetPt[iJet];
      else if (typeMatched==2) SmearRes = (SmearC-1.) * (jetPt[iJet]-jetGenJetPt_[genMatched])/jetPt[iJet];
    } else {
       double expres = ErrEt(jetRawPt[iJet], jetEta[iJet]);
       double relsmear = expres * sqrt(SmearC*SmearC-1);
       genRan_.SetSeed(event_+(Int_t)jetEta[iJet]*1000);
       SmearRes = genRan_.Gaus(0.,relsmear);
    }

    float rawPtSmeared = jetRawPt[iJet];
    float rawEnSmeared = jetRawEn[iJet];

    if(SmearRes>-1 && SmearRes < 2) {
      rawPtSmeared *= (1 + SmearRes);
      rawEnSmeared *= (1 + SmearRes);
    }

    TLorentzVector smearedJet;
    smearedJet.SetPtEtaPhiE(rawPtSmeared,jetEta[iJet],jetPhi[iJet],rawEnSmeared);

    TLorentzVector rawJet;
    rawJet.SetPtEtaPhiE(jetRawPt[iJet],jetEta[iJet],jetPhi[iJet],jetRawEn[iJet]);

    jetSumSmeared_ += smearedJet;
    jetSumRaw_     += rawJet;

  } //loop over nJets ends

}



// MET Correction Step 1b: get MC smear factor
double TMetTools::GetMETJetSmearFactor(Float_t metJetEta) {
  double jetSmearFactor = 0.;

  if      (fabs(metJetEta)<=1.1) jetSmearFactor = 1.06177;
  else if (fabs(metJetEta)<=1.7) jetSmearFactor = 1.08352;
  else if (fabs(metJetEta)<=2.3) jetSmearFactor = 1.02911;
  else                           jetSmearFactor = 1.15288;

  //std::cout << "--> metCorr: smear factor = " << jetSmearFactor << std::endl;
  return jetSmearFactor;
}

// MET Correction Step 1c: get MC expected resolution
double TMetTools::ErrEt( double Et, double Eta) {
  double InvPerr2;

  double N(0), S(0), C(0), m(0);
  if(fabs(Eta) < 0.5 ) {
    N = 3.96859;
    S = 0.18348;
    C = 0.;
    m = 0.62627;
  } else if( fabs(Eta) < 1. ) {
    N = 3.55226;
    S = 0.24026;
    C = 0.;
    m = 0.52571;
  } else if( fabs(Eta) < 1.5 ) {
    N = 4.54826;
    S = 0.22652;
    C = 0.;
    m = 0.58963;
  } else if( fabs(Eta) < 2. ) {
    N = 4.62622;
    S = 0.23664;
    C = 0.;
    m = 0.48738;
  } else if( fabs(Eta) < 3. ) {
    N = 2.53324;
    S = 0.34306;
    C = 0.;
    m = 0.28662;
  } else if( fabs(Eta) < 5. ) {
    N = 2.95397;
    S = 0.11619;
    C = 0.;
    m = 0.96086;
  }

  // this is the absolute resolution (squared), not sigma(pt)/pt
  // so have to multiply by pt^2, thats why m+1 instead of m-1
  InvPerr2 =  (N * fabs(N) ) + (S * S) * pow(Et, m+1) + (C * C) * Et * Et ;

  return sqrt(InvPerr2)/Et;
} 

float TMetTools::DeltaR (float eta1, float phi1, float eta2, float phi2)
{
  float dphi;
  for (dphi=fabs(phi1-phi2); fabs(dphi)>=2*TMath::Pi(); dphi=dphi-2*TMath::Pi()) ;
  float dR=sqrt((eta1-eta2)*(eta1-eta2)+dphi*dphi);
  return dR;
}

float TMetTools::GetRecoPfMET()
{
  return recoPfMET_;
}

float TMetTools::GetRecoPfMETPhi()
{
  return recoPfMETPhi_;
}

/*
bool TMetTools::MetTagSelection( const TLorentzVector &g1, const TLorentzVector &g2, const vector<int>& jind ) {
  TLorentzVector gg = g1+g2;
  
  if( g1.Pt()/gg.M() < 45./120. ) return false;
  if( g2.Pt()/gg.M() < 30./120. ) return false;
  
  TLorentzVector met; 
  met.SetPtEtaPhiM( _minitree->mtree_corMet, 0, _minitree->mtree_corMetPhi = recoPfMETPhi, 0);  

  //-- met back-to-bakc with gg
  if( fabs( gg.DeltaPhi( met ) ) <= 2.1 ) return false;

  //-- no jet bak-to-back with the met
  TLorentzVector jet;
  float jetThreshold = 50;
  float jetHighestPt = -1;
  int lJet = -1;
  for( unsigned ijet = 0; ijet < jind.size(); ++ijet ) {
    int iJet = jind[ijet];
    if( jetPt[iJet] > jetThreshold ) {
     jet.SetPtEtaPhiE( jetPt[iJet], jetEta[iJet], jetPhi[iJet], jetEn[iJet] );
     if( jet.DeltaR( g1 ) < 0.5 || jet.DeltaR( g2 ) < 0.5 ) continue;
     if( jet.Pt() > jetHighestPt ) { jetHighestPt = jet.Pt(); lJet = iJet; }
    }
  }

  if( jetHighestPt > jetThreshold ) {
    jet.SetPtEtaPhiE( jetPt[lJet], jetEta[lJet], jetPhi[lJet], jetEn[lJet] );
    if( fabs(jet.DeltaPhi(met)) > 2.7 ) return false;
  }

  return true;
}
*/

/*
void TMetTools::ApplyJEC4SoftJets() {
  int jecMode = _weight_manager->getCrossSection()->mode();
  /// ------------------------------- JEC --------------------------------------------------//
  /// step 1
  // Create the JetCorrectorParameter objects, the order does not matter.
  // YYYY is the first part of the txt files: usually the global tag from which they are retrieved
  JetCorrectorParameters *ResJetPar = new JetCorrectorParameters("./jecAK5PF/Summer12_V7_MC_L2L3Residual_AK5PF.txt");
  JetCorrectorParameters *L3JetPar  = new JetCorrectorParameters("./jecAK5PF/Summer12_V7_MC_L3Absolute_AK5PF.txt");
  JetCorrectorParameters *L2JetPar  = new JetCorrectorParameters("./jecAK5PF/Summer12_V7_MC_L2Relative_AK5PF.txt");
  JetCorrectorParameters *L1JetPar  = new JetCorrectorParameters("./jecAK5PF/Summer12_V7_MC_L1FastJet_AK5PF.txt");

  if (jecMode<0) { // ======== Offline DATA JEC Setup ======== //
    ResJetPar = new JetCorrectorParameters("./jecAK5PF/Summer12_V7_DATA_L2L3Residual_AK5PF.txt");
    L3JetPar  = new JetCorrectorParameters("./jecAK5PF/Summer12_V7_DATA_L3Absolute_AK5PF.txt");
    L2JetPar  = new JetCorrectorParameters("./jecAK5PF/Summer12_V7_DATA_L2Relative_AK5PF.txt");
    L1JetPar  = new JetCorrectorParameters("./jecAK5PF/Summer12_V7_DATA_L1FastJet_AK5PF.txt");
  }

  /// step 2
  //  Load the JetCorrectorParameter objects into a vector, IMPORTANT: THE ORDER MATTERS HERE !!!! 
  vector<JetCorrectorParameters> vPar;
  vPar.push_back(*L1JetPar);
  vPar.push_back(*L2JetPar);
  vPar.push_back(*L3JetPar);
  if (jecMode<0) vPar.push_back(*ResJetPar);

  /// step 3
  FactorizedJetCorrector *JetCorrector = new FactorizedJetCorrector(vPar);

  for( int i = 0 ; i < nLowPtJet; ++i ) {
    /// step 4
    JetCorrector->setJetEta(jetLowPtEta[i]);
    JetCorrector->setJetPt(jetLowPtRawPt[i]);
    JetCorrector->setJetA(jetLowPtArea[i]);
    JetCorrector->setRho(rho2012); 
    /// step 5
    double correction = JetCorrector->getCorrection();
    // vector<double> factors = JetCorrector->getSubCorrections();

    /// step 6
    jetLowPtPt[i] = jetLowPtRawPt[i] * correction;
    jetLowPtEn[i] = jetLowPtRawEn[i] * correction;
    /// --------------------------------------------------------------------------------------//
  }

  delete ResJetPar;
  delete L3JetPar;
  delete L2JetPar;
  delete L1JetPar;
  delete JetCorrector;
}

*/


/*
// MET Correction Step 2: phi shifting
void TMetTools::METPhiShiftCorrection() {
  double corr2METx = 0.;
  double corr2METy = 0.;
  if(_weight_manager->getCrossSection()->mode() < 0){ // <----- DATA
    corr2METx = recoPfMET*(TMath::Cos(recoPfMETPhi))-0.006239*recoPfMETsumEt+0.662;
    corr2METy = recoPfMET*(TMath::Sin(recoPfMETPhi))+0.004613*recoPfMETsumEt-0.673;    
  } else { // <----------- MC
    corr2METx = recoPfMET*(TMath::Cos(recoPfMETPhi))+0.00135*recoPfMETsumEt-0.021;
    corr2METy = recoPfMET*(TMath::Sin(recoPfMETPhi))+0.00371*recoPfMETsumEt-0.826;
  }
  TVector3 shiftMET(corr2METx,corr2METy,0.);

  recoPfMET    = shiftMET.Pt();
  recoPfMETPhi = shiftMET.Phi();

  //std::cout << "--> metCorr: Px = " << corr2METx << ", Py = " << corr2METy << "  " << shiftMET.Pt() << "  " << sqrt(corr2METx*corr2METx + corr2METy*corr2METy) << std::endl;
}
*/

/*
// MET Correction Step 3: Data scaling
void TMetTools::METScaleCorrection(Int_t iPho1, Int_t iPho2) {
  TLorentzVector jetSumScaled;  
  jetSumScaled.SetXYZT(0.,0.,0.,0.);
  TLorentzVector jetSumRaw;
  jetSumRaw.SetXYZT(0.,0.,0.,0.);

  // jets with 10<pt<20
  for (int j_ind=0; j_ind<nLowPtJet; j_ind++) {
    // jet basic cuts
    if (jetLowPtRawPt[j_ind]<10. || fabs(jetLowPtEta[j_ind])>4.7) continue;

    // remove selected photons
    float dR_jetg1 = deltaR(phoEta[iPho1], phoPhi[iPho1],jetLowPtEta[j_ind],jetLowPtPhi[j_ind]);
    float dR_jetg2 = deltaR(phoEta[iPho2], phoPhi[iPho2],jetLowPtEta[j_ind],jetLowPtPhi[j_ind]);
    if( dR_jetg1 < 0.5 || dR_jetg2 < 0.5 ) continue;

    double ScaleFactor = getMETJetScaleFactor(jetLowPtEta[j_ind]);
    double rawPtScaled = jetLowPtRawPt[j_ind] * ScaleFactor;
    double rawEnScaled = jetLowPtRawEn[j_ind] * ScaleFactor;

    TLorentzVector scaledJet;
    scaledJet.SetPtEtaPhiE(rawPtScaled,jetLowPtEta[j_ind],jetLowPtPhi[j_ind],rawEnScaled);
        
    TLorentzVector rawJet;
    rawJet.SetPtEtaPhiE(jetLowPtRawPt[j_ind],jetLowPtEta[j_ind],jetLowPtPhi[j_ind],jetLowPtRawEn[j_ind]);
        
    jetSumScaled += scaledJet;
    jetSumRaw    += rawJet;

  }

  // jets with pt>=20
  for (int j_ind=0; j_ind<nJet; j_ind++) {
    // jet basic cuts
    if (jetRawPt[j_ind]<10. || fabs(jetEta[j_ind])>4.7) continue;

    // remove selected photons
    float dR_jetg1 = deltaR(phoEta[iPho1], phoPhi[iPho1],jetEta[j_ind],jetPhi[j_ind]);
    float dR_jetg2 = deltaR(phoEta[iPho2], phoPhi[iPho2],jetEta[j_ind],jetPhi[j_ind]);
    if( dR_jetg1 < 0.5 || dR_jetg2 < 0.5 ) continue;

    double ScaleFactor = getMETJetScaleFactor(jetEta[j_ind]);
    double rawPtScaled = jetRawPt[j_ind] * ScaleFactor;
    double rawEnScaled = jetRawEn[j_ind] * ScaleFactor;

    TLorentzVector scaledJet;
    scaledJet.SetPtEtaPhiE(rawPtScaled,jetEta[j_ind],jetPhi[j_ind],rawEnScaled);

    TLorentzVector rawJet;
    rawJet.SetPtEtaPhiE(jetRawPt[j_ind],jetEta[j_ind],jetPhi[j_ind],jetRawEn[j_ind]);

    jetSumScaled += scaledJet;
    jetSumRaw    += rawJet;
  }

  double corrMetPx = recoPfMET*(TMath::Cos(recoPfMETPhi));
  double corrMetPy = recoPfMET*(TMath::Sin(recoPfMETPhi));
  TLorentzVector scaleCorrectedMET;
  scaleCorrectedMET.SetPxPyPzE(corrMetPx, corrMetPy, 0., sqrt(corrMetPx*corrMetPx+corrMetPy*corrMetPy));
  scaleCorrectedMET = scaleCorrectedMET + jetSumRaw - jetSumScaled;

  recoPfMET    = scaleCorrectedMET.Pt();
  recoPfMETPhi = scaleCorrectedMET.Phi();
}
*/
/*
// MET Correction Step 3b: get Data scale factor
double TMetTools::GetMETJetScaleFactor(Float_t metJetEta) {
  double jetScaleFactor = 1.;

  if(fabs(metJetEta)<1.5)    jetScaleFactor = 1.015;
  else if(fabs(metJetEta)<3) jetScaleFactor = 1.04;
  else                       jetScaleFactor = 1.15;

  //std::cout << "--> metCorr: scale factor = " << jetScaleFactor << std::endl;
  return jetScaleFactor;
}
*/
