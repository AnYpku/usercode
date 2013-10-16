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
                   int leGenPID,
                   float phoEta, float phoPhi, float phoEt,
                   int phoGenPID, float phoGenEt,
                   float phoSigmaIEtaIEta,
                   float phoPFChIsoCorr,
                   float phoSCRChIsoCorr,
                   float lePhoDeltaR,
                   float WMt,
                   float pfMET, float pfMETPhi,
                   float rho2012,
                   int run,
                   int inputFileN,
                   float weight, float PUweight, float PU); 

  private:
    TTree *outTree_;
    float leEta_;
    float lePhi_;
    float lePt_;
    int leGenPID_;
    float phoEta_;
    float phoPhi_;
    float phoEt_;
    int phoGenPID_;
    float phoGenEt_;
    float phoSigmaIEtaIEta_;
    float phoPFChIsoCorr_;
    float phoSCRChIsoCorr_;
    float lePhoDeltaR_;
    float WMt_;
    float pfMET_;
    float pfMETPhi_;
    float rho2012_;
    int run_;
    int inputFileN_;
    float weight_;
    float PUweight_;
    float PU_;
};

#endif //#ifndef TSelectedEventsTree_h
