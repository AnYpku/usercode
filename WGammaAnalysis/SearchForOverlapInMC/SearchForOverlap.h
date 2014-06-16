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
       SearchForOverlap(int channel, bool isDebugMode=0, string analyzedSampleNames="Wg Wjets_to_lnu Zg DYjets_to_ll", string configFile="../Configuration/configForOverlapSearch.txt", bool isNoPuReweight=0);
       virtual ~SearchForOverlap();
       void    LoopOverInputFiles();
       void    LoopOverTreeEvents();

       void PlotTwoHistograms(TString canvName, TH1F* hGamma, TH1F* hJets);

     private:

       TAllInputSamples* _INPUT;
       TEventTree _eventTree;
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

       bool _isDebugMode;
       bool _isNoPuReweight;
       int _sampleMode;
       vector <bool> _doAnalizeSample;

       float _lumiData;
       float _lumiWeight;
       float _totalWeight;
       long _nentries;
       int _iSource;

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
       TH1F* _histLepLepMass[10];

       TH1F* _histWeight[10];

       const static int _debugModeNEntries=10000;

       TPuReweight* _puWeight;
//       TString _selectedTreeFileName;

       float _nEvents;
       float _nEventsWithPhotons;
       float _nPhotons;

       enum {Z_BOSON=23, W_BOSON=24};
       enum {ANY_LEPTON=0, E_LEPTON=11, MU_LEPTON=13, TAU_LEPTON=15};
       enum {ANY_QUARK,D_QUARK,U_QUARK,S_QUARK,C_QUARK,B_QUARK,T_QUARK,GLUON=21};

       float _nFSR[W_BOSON+1][TAU_LEPTON+1];    //(W,Z) -> (mu,e,tau) -> gamma
       
       float _nTGC[W_BOSON+1]; //(<quark,gluon> -> W,Z -> gamma)

       float _nISR[W_BOSON+1][GLUON+1];   //(any,d,u,s,c,b,t,gluon) -> gamma, 

       int _nEventsWithGivenNOfPhotons[91];
       int _nEventsWithSomeScheme[91];

  };

#endif //#ifndef SearchForOverlap_h
