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
                   float phoEta, float phoPhi, float phoEt,
                   float lePhoDeltaR,
                   float WMt,
                   float pfMET, float pfMETPhi,
                   int run,
                   int inputFileN,
                   float weight); 

  private:
    TTree *outTree_;
    float leEta_;
    float lePhi_;
    float lePt_;
    float phoEta_;
    float phoPhi_;
    float phoEt_;
    float lePhoDeltaR_;
    float WMt_;
    float pfMET_;
    float pfMETPhi_;
    int run_;
    int inputFileN_;
    float weight_;
};

#endif //#ifndef TSelectedEventsTree_h
