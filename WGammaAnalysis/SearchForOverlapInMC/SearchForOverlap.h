#ifndef SearchForOverlap_h
#define SearchForOverlap_h

#include "../Include/TEventTree.h" 
#include "../Include/TPuReweight.h" 
#include "../Configuration/TAllInputSamples.h"
  //this package
#include "TTree.h" 
#include "TMath.h" 
  //ROOT package

class SearchForOverlap 
  {
     //TEventTree - class for input tree
     //TSelectedEventsTree - class for output tree and output root file
     public:
       SearchForOverlap(int channel, int cutType, bool isDebugMode=0, string analyzedSampleNames="Wg Wjets_to_lnu Zg DYjets_to_ll", string configFile="../Configuration/configForOverlapSearch.txt", bool isNoPuReweight=0);
       virtual ~SearchForOverlap();
       void    LoopOverInputFiles();   
       enum {CUT_PAR, CUT_SOME_KIN_NO_PAR, CUT_KIN_PAR, CUT_KIN_MLL_PAR};
       float DeltaR(float phi1, float phi2, float eta1, float eta2);

     private:
       void LoopOverTreeEvents(int bosonType);
       void LoopOverMCParticles(int bosonType);
       bool CheckIfTrueGammaMCparentage(int imc);
       void CheckIfFSR(int imc, bool& isZMuonFSR, bool& isWMuonFSR);
       void CheckIfTGC(int imc, bool& isZMuonTGC, bool& isWMuonTGC);
       void CheckIfISR(int imc, bool& isZMuonISR, bool& isWMuonISR);
       void FindHighestPtPhotonIndexWithGivenKin(int bosonType, float dRCut, int& ipho, int &ilep1, int &ilep2);
       void FindLeptonIndexes(int bosonType, int imc, int& ilep1, int& ilep2);
       void FindLeptonIndexesZ(int imc, int& ilep1, int& ilep2);
       void FindLeptonIndexesW(int imc, int& ilep1, int& ilep2);
       void ComputeKinematicVariables(int ipho, int ilep1, int ilep2);
       void FillHists(int imc, int ilep1, int ilep2);
       void PlotTwoHistograms(TString canvName, TH1F* hGamma, TH1F* hJets);
       void PlotHistsGammaJetsCompare(TString strZorW, int ind1, int ind2);
       void SetHists();
       void PlotHists();
       void PrintResults();
       void PrintIfSuspicious();

       TAllInputSamples* _INPUT;
       TEventTree _eventTree;
       TEventTree::InputTreeLeaves _treeLeaf;
       TConfiguration _config;

       int _channel;//ELECTRON, MUON
       int _sample; //DATA, SIGMC, BKGMC

       TFile* _fOut;
       
       bool _hasWGamma;
       bool _hasWJets;
       bool _hasZGamma;
       bool _hasZJets;
       int _idWGamma;
       int _idWJets;
       int _idZGamma;
       int _idZJets;
       bool _histsSet[10];
       int _cutType;

       bool _isDebugMode;
       bool _isNoPuReweight;
       int _sampleMode;
       vector <bool> _doAnalizeSample;

       float _lumiData;
       float _lumiWeight;
       float _totalWeight;
       long _nentries;
       int _nSomeScheme;

       int _iSource;
       long _entry;
       int _nthFakePhoton;

       float _dRLep1Pho;
       float _dRLep2Pho;
       float _dRLepLep;
       float _massTLepLep;
       float _massLepLep;

       TH1F* _histPhoPt[10];
       TH1F* _histPhoEta[10];
       TH1F* _histPhoPhi[10];
       TH1F* _histLep1Pt[10];
       TH1F* _histLep1Eta[10];
       TH1F* _histLep1Phi[10];
       TH1F* _histLep2Pt[10];
       TH1F* _histLep2Eta[10];
       TH1F* _histLep2Phi[10];

       TH1F* _histLep1PhodR[10];
       TH1F* _histLep2PhodR[10];
       TH1F* _histLepLepdR[10];
       TH1F* _histLepLepMassT[10];
       TH1F* _histLepLepMass[10];

       TH1F* _histMCparentage[10];

       TH1F* _histWeight[10];

       const static int _debugModeNEntries=100000;

       TPuReweight* _puWeight;
//       TString _selectedTreeFileName;

       float _nEvents;
       float _nEventsWithPhotons;
       float _nPhotons;
       float _nPhotonsPassedAcc;
       float _nPhotonsPassedTrueGammaCondition;

       enum {Z_BOSON=23, W_BOSON=24};
       enum {ANY_LEPTON=0, E_LEPTON=11, MU_LEPTON=13, MU_NEUTRINO=14, TAU_LEPTON=15};
       enum {ANY_QUARK,D_QUARK,U_QUARK,S_QUARK,C_QUARK,B_QUARK,T_QUARK,GLUON=21};

       float _nFSR[W_BOSON+1][TAU_LEPTON+1];    //(W,Z) -> (mu,e,tau) -> gamma      
       float _nTGC[W_BOSON+1]; //(<quark,gluon> -> W,Z -> gamma)
       float _nISR[W_BOSON+1][GLUON+1];   //(any,d,u,s,c,b,t,gluon) -> gamma, 
       float _nTrue; //


       int _nEventsWithGivenNOfPhotons[91];
       int _nEventsWithSomeScheme[91];

  };

#endif //#ifndef SearchForOverlap_h
