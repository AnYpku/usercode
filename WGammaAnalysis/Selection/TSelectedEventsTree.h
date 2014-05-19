#ifndef TSelectedEventsTree_h
#define TSelectedEventsTree_h

#include "TTree.h" //ROOT class
#include "TFile.h" //ROOT class
#include "TString.h" //ROOT class

#include "../Include/TEventTree.h" //class of this package
#include "../Include/PhosphorCorrectorFunctor.hh"
  //taken from http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/CPena/src/PHOSPHOR_Corr_v2/
  //currently in this package

class TSelectedEventsTree
{
  public:
    TSelectedEventsTree();
    virtual ~TSelectedEventsTree();

    void SetOutputTree(TTree* tree);
    void Fill();
    void SetValues(int channel, int sample, TEventTree::InputTreeLeaves treeLeaf, int ipho, int ile, float lePhoDeltaR, int inputFileN, float weight, float PUweight, float PU,zgamma::PhosphorCorrectionFunctor* photonCorrector); 

  private:
    TTree* _outTree;
    int _event;
    int _iMCle;
    float _leEta;
    float _lePhi;
    float _lePt;
    int _leGenPID;
    int _leGenMomPID;
    int _leGenGMomPID;
    bool _leId;
    float _leIsolation;
    int _leTrg;
    bool _trgMatchIsoMu24eta2p1;//0th bit of muTrg[imu]
    bool _trgMatchIsoMu24;//1th bit of muTrg[imu]
    bool _hasMoreLeptons;
    int _HLT_IsoMu24_eta2p1_;//HLT[HLTIndex[18]]
    int _HLT_IsoMu24_v;//HLT[HLTIndex[19]]
    int _iMCpho;
    int _phoEleVeto;
    float _phoEta;
    float _phoPhi;
    float _phoEt;
    float _phoEtNoPhosphor;
    float _phoSCEta;
    float _phoSCPhi;
    float _phoSCEt;
    int _phoGenPID;
    int _phoGenMomPID;
    int _phoGenGMomPID;
    float _phoGenEt;
    float _phoHoverE12;
    float _phoSigmaIEtaIEta;
    float _phoPFChIsoCorr;
    float _phoSCRChIsoCorr;
    float _phoPFNeuIsoCorr;
    float _phoSCRNeuIsoCorr;
    float _phoPFPhoIsoCorr;
    float _phoSCRPhoIsoCorr;
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
