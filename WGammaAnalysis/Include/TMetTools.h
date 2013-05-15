#ifndef TMetTools_h
#define TMetTools_h

#include "../Selection/TEventTree.h"
#include "TLorentzVector.h"
#include "TRandom3.h"

//#include "xAna_allAna.h"
//#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"



class TMetTools
{
  public:
    TMetTools();
    TMetTools(int event, float recoPfMET, float recoPfMETPhi,
              int nLowPtJet,
              float* jetLowPtRawPt,
              float* jetLowPtRawEn,
              float* jetLowPtPt,
              float* jetLowPtEta,
              float* jetLowPtPhi,
              float* jetLowPtGenJetPt,
              float* jetLowPtGenJetEta,
              float* jetLowPtGenJetPhi,
              int nJet,
              float* jetRawPt,
              float* jetRawEn,
              float* jetPt,
              float* jetEta,
              float* jetPhi,
              float* jetGenJetPt,
              float* jetGenJetEta,
              float* jetGenJetPhi);
    virtual ~TMetTools();

    void METSmearCorrection();
    void LoopOverJetsForSmearing(int nJet, float* jetRawPt, float* jetRawEn, float* jetPt, float* jetEta, float* jetPhi);
    double GetMETJetSmearFactor(float metJetEta);
    double ErrEt( double Et, double Eta);
    float DeltaR (float eta1, float phi1, float eta2, float phi2);

    float GetRecoPfMET();
    float GetRecoPfMETPhi();

    //bool MetTagSelection( const TLorentzVector &g1, const TLorentzVector &g2, const vector<int>& jind );
    //void ApplyJEC4SoftJets();
    //void METPhiShiftCorrection();
    //void METScaleCorrection(Int_t iPho1, Int_t iPho2);
    //double GetMETJetScaleFactor(Float_t metJetEta);

  private:
    int event_;
    float recoPfMET_;
    float recoPfMETPhi_;
    int nLowPtJet_;
    float jetLowPtRawPt_[TEventTree::kMaxnLowPtJet];
    float jetLowPtRawEn_[TEventTree::kMaxnLowPtJet];
    float jetLowPtPt_[TEventTree::kMaxnLowPtJet];
    float jetLowPtEta_[TEventTree::kMaxnLowPtJet];
    float jetLowPtPhi_[TEventTree::kMaxnLowPtJet];
    float jetLowPtGenJetPt_[TEventTree::kMaxnLowPtJet];
    float jetLowPtGenJetEta_[TEventTree::kMaxnLowPtJet];
    float jetLowPtGenJetPhi_[TEventTree::kMaxnLowPtJet];
    int nJet_;
    float jetRawPt_[TEventTree::kMaxnJet];
    float jetRawEn_[TEventTree::kMaxnJet];
    float jetPt_[TEventTree::kMaxnJet];
    float jetEta_[TEventTree::kMaxnJet];
    float jetPhi_[TEventTree::kMaxnJet];
    float jetGenJetPt_[TEventTree::kMaxnJet];
    float jetGenJetEta_[TEventTree::kMaxnJet];
    float jetGenJetPhi_[TEventTree::kMaxnJet];


    TLorentzVector jetSumSmeared_;
    TLorentzVector jetSumRaw_;
    TRandom3 genRan_;



};



#endif
