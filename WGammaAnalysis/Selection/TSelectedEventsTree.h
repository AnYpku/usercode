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

    void SetAsOutputTree(TTree* tree);
    void SetAsInputTree(TTree* tree);
    void SetValues(int channel, int sample, TEventTree::InputTreeLeaves treeLeaf, int ipho, int ilep1, int ilep2, float lep1PhoDeltaR, float lep2PhoDeltaR, int inputFileN, float weight, float PUweight, float PU,zgamma::PhosphorCorrectionFunctor* photonCorrector); 
    bool SetValuesMuId(TEventTree::InputTreeLeaves treeLeaf, int year, int ile);
    float SetValuesMuIsolation(TEventTree::InputTreeLeaves treeLeaf, int year, int ile);


  private:
    int _event;
    int _iMClep[2];
    float _lepEta[2];
    float _lepPhi[2];
    float _lepPt[2];
    int _lepGenPID[2];
    int _lepGenParentage[2];
    int _lepGenMomPID[2];
    int _lepGenGMomPID[2];
    bool _lepId2012[2];
    bool _lepId2011[2];
    float _lepIsolation2012[2];
    float _lepIsolation2011[2];
    int _lepTrg[2];
    int _lepType[2];
    bool _trgMatchIsoMu24eta2p1[2];//0th bit of muTrg[imu]
    bool _trgMatchIsoMu24[2];//1th bit of muTrg[imu]
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
    int _phoGenParentage;
    int _phoGenMomPID;
    int _phoGenGMomPID;
    float _phoGenEt;
    float _phoHoverE;//for 2011 cuts
    float _phoHoverE12;
    float _phoSigmaIEtaIEta;
    float _phoPFChIsoCorr;
    float _phoSCRChIsoCorr;
    float _phoPFNeuIsoCorr;
    float _phoSCRNeuIsoCorr;
    float _phoPFPhoIsoCorr;
    float _phoSCRPhoIsoCorr;
    float _phoEcalIsoDR04Corr;//for 2011 cuts
    float _phoHcalIsoDR04Corr;//for 2011 cuts
    float _phoTrkIsoHollowDR04Corr;//for 2011 cuts
    int _phohasPixelSeed;//for 2011 cuts
    float _lepPhoDeltaR[2];
    float _WMt;
    float _pfMET;
    float _pfMETPhi;
    float _rho2011;//for 2011 cuts
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

    TBranch* _b_event;
    TBranch* _b_iMClep[2];
    TBranch* _b_lepEta[2];
    TBranch* _b_lepPhi[2];
    TBranch* _b_lepPt[2];
    TBranch* _b_lepGenPID[2];
    TBranch* _b_lepGenParentage[2];
    TBranch* _b_lepGenMomPID[2];
    TBranch* _b_lepGenGMomPID[2];
    TBranch* _b_lepId2012[2];
    TBranch* _b_lepId2011[2];
    TBranch* _b_lepIsolation2012[2];
    TBranch* _b_lepIsolation2011[2];
    TBranch* _b_lepTrg[2];
    TBranch* _b_lepType[2];
    TBranch* _b_trgMatchIsoMu24eta2p1[2];//0th bit of muTrg[imu]
    TBranch* _b_trgMatchIsoMu24[2];//1th bit of muTrg[imu]
    TBranch* _b_hasMoreLeptons;
    TBranch* _b_HLT_IsoMu24_eta2p1_;//HLT[HLTIndex[18]]
    TBranch* _b_HLT_IsoMu24_v;//HLT[HLTIndex[19]]
    TBranch* _b_iMCpho;
    TBranch* _b_phoEleVeto;
    TBranch* _b_phoEta;
    TBranch* _b_phoPhi;
    TBranch* _b_phoEt;
    TBranch* _b_phoEtNoPhosphor;
    TBranch* _b_phoSCEta;
    TBranch* _b_phoSCPhi;
    TBranch* _b_phoSCEt;
    TBranch* _b_phoGenPID;
    TBranch* _b_phoGenParentage;
    TBranch* _b_phoGenMomPID;
    TBranch* _b_phoGenGMomPID;
    TBranch* _b_phoGenEt;
    TBranch* _b_phoHoverE;//for 2011 cuts
    TBranch* _b_phoHoverE12;
    TBranch* _b_phoSigmaIEtaIEta;
    TBranch* _b_phoPFChIsoCorr;
    TBranch* _b_phoSCRChIsoCorr;
    TBranch* _b_phoPFNeuIsoCorr;
    TBranch* _b_phoSCRNeuIsoCorr;
    TBranch* _b_phoPFPhoIsoCorr;
    TBranch* _b_phoSCRPhoIsoCorr;
    TBranch* _b_phoEcalIsoDR04Corr;//for 2011 cuts
    TBranch* _b_phoHcalIsoDR04Corr;//for 2011 cuts
    TBranch* _b_phoTrkIsoHollowDR04Corr;//for 2011 cuts
    TBranch* _b_phohasPixelSeed;//for 2011 cuts
    TBranch* _b_lepPhoDeltaR[2];
    TBranch* _b_WMt;
    TBranch* _b_pfMET;
    TBranch* _b_pfMETPhi;
    TBranch* _b_rho2011;//for 2011 cuts
    TBranch* _b_rho2012;
    TBranch* _b_run;
    TBranch* _b_inputFileN;
    TBranch* _b_weight; 
    TBranch* _b_PUweight;
    TBranch* _b_PU;
    TBranch* _b_nMC;
    TBranch* _b_mcPID;
    TBranch* _b_mcMomPID;
    TBranch* _b_mcGMomPID;
};

#endif //#ifndef TSelectedEventsTree_h
