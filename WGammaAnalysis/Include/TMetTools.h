#ifndef TMetTools_h
#define TMetTools_h

#include "TEventTree.h"
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
              vector <float> *jetGenJetPhi);
    TMetTools(int event, float recoPfMET, float recoPfMETPhi,
              int nJet,
              vector <float> *jetRawPt,
              vector <float> *jetRawEn,
              vector <float> *jetPt,
              vector <float> *jetEta,
              vector <float> *jetPhi,
              vector <float> *jetGenJetPt,
              vector <float> *jetGenJetEta,
              vector <float> *jetGenJetPhi);
    virtual ~TMetTools();

    void METSmearCorrection();
    void LoopOverJetsForSmearing(int nJet, vector <float> jetRawPt, vector <float> jetRawEn, vector <float> jetPt, vector <float> jetEta, vector <float> jetPhi);
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
    vector <float> jetLowPtRawPt_;
    vector <float> jetLowPtRawEn_;
    vector <float> jetLowPtPt_;
    vector <float> jetLowPtEta_;
    vector <float> jetLowPtPhi_;
    vector <float> jetLowPtGenJetPt_;
    vector <float> jetLowPtGenJetEta_;
    vector <float> jetLowPtGenJetPhi_;
    int nJet_;
    vector <float> jetRawPt_;
    vector <float> jetRawEn_;
    vector <float> jetPt_;
    vector <float> jetEta_;
    vector <float> jetPhi_;
    vector <float> jetGenJetPt_;
    vector <float> jetGenJetEta_;
    vector <float> jetGenJetPhi_;


    TLorentzVector jetSumSmeared_;
    TLorentzVector jetSumRaw_;
    TRandom3 genRan_;



};



#endif
