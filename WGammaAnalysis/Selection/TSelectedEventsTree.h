#ifndef TSelectedEventsTree_h
#define TSelectedEventsTree_h

#include "TTree.h" //ROOT class
#include "TFile.h" //ROOT class
#include "TString.h" //ROOT class

class TSelectedEventsTree
{
  public:
    TSelectedEventsTree();
    virtual ~TSelectedEventsTree();

    void SetOutputTree(TTree* tree);
    void Fill();
    void SetValues(float leEta, float lePhi, float lePt,
                   int leGenPID, int leGenMomPID, int leGenGMomPID,
                   float phoEta, float phoPhi, float phoEt,
                   int phoGenPID, int phoGenMomPID, int phoGenGMomPID,
                   float phoGenEt,
                   float phoSigmaIEtaIEta,
                   float phoPFChIsoCorr,
                   float phoSCRChIsoCorr,
                   float lePhoDeltaR,
                   float WMt,
                   float pfMET, float pfMETPhi,
                   float rho2012,
                   int run,
                   int inputFileN,
                   float weight, float PUweight, float PU,
                   int nMC, vector <int> *mcPID,
                   vector <int> *mcMomPID, vector <int> *mcGMomPID); 

  private:
    TTree* _outTree;
    float _leEta;
    float _lePhi;
    float _lePt;
    int _leGenPID;
    int _leGenMomPID;
    int _leGenGMomPID;
    float _phoEta;
    float _phoPhi;
    float _phoEt;
    int _phoGenPID;
    int _phoGenMomPID;
    int _phoGenGMomPID;
    float _phoGenEt;
    float _phoSigmaIEtaIEta;
    float _phoPFChIsoCorr;
    float _phoSCRChIsoCorr;
    float _lePhoDeltaR;
    float _WMt;
    float _pfMET;
    float _pfMETPhi;
    float _rho2012;
    int _run;
    int _inputFileN;
    float _weight;
    float _PUweight;
    float _PU;
    int _nMC;
    vector <int>* _mcPID;
    vector <int>* _mcMomPID;
    vector <int>* _mcGMomPID;
};

#endif //#ifndef TSelectedEventsTree_h
