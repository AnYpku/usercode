#ifndef CalcAccAndEff_h
#define CalcAccAndEff_h

#include "../Configuration/TConfiguration.h"
#include "../Configuration/TAllInputSamples.h"
#include "../Include/TEventTree.h"
#include "../Include/TPuReweight.h"
#include "../Include/TMathTools.h"
#include "../Include/TPhotonCuts.h"
#include "../Include/TMuonCuts.h"
#include "../Include/TElectronCuts.h"
#include "../Include/TFullCuts.h"
  //this package
#include "../PHOSPHOR_CORRECTION/PhosphorCorrectorFunctor.hh"
  //from git
#include "TTree.h" 
#include "TMath.h" 
#include "TLorentzVector.h"
  //ROOT package

class CalcAccAndEff
  {
     public:
       CalcAccAndEff (int channel, int vgamma, bool isDebugMode=0, string configfile="../Configuration/config.txt");

       virtual ~CalcAccAndEff();
       void    LoopOverInputFiles();

     private:

       struct PassedPhaseSpaceLevels{
         long imcPho;
         long imcLep1;
         long imcLep2;
         long dR;
         long phoEta;
         long lep1Eta;
         long lep1Pt;
         long dR2;
         long lep2Eta;
         long lep2Pt;
         long Mll;
         long phoIgen;
         long phoPt;
       };

       PassedPhaseSpaceLevels _passed;

       void    LoopOverTreeEvents(); 
       void    ComputeNumerator();
       void    ComputeDenominator();
       int     FindMCleptons(TEventTree::InputTreeLeaves &leaf, int &imcLep1, int &imcLep2);
       void    FillLeptonLorentzVectors(TEventTree::InputTreeLeaves &leaf, int &imcLep1, int &imcLep2);
       int     FindMCphoton(TEventTree::InputTreeLeaves &leaf, int &imcPho, int imcLep1, int imcLep2);

       bool    PassedPhaseSpaceCut(TEventTree::InputTreeLeaves &leaf, int imcPho, int imcLep1, int imcLep2);
       bool    IsFSR(TEventTree::InputTreeLeaves &leaf, int imcPho, int lepID, int bosonID);
       bool    IsTGC(TEventTree::InputTreeLeaves &leaf, int imcPho, int bosonID);
       bool    IsISR(TEventTree::InputTreeLeaves &leaf, int imcPho, int bosonID);
       void    ComputeAccTimesEff();

       void SetNullPassedLevels();
       void PrintPassedLevels();

       void    PrintAccAndEffSummary();
       void    PlotAndSaveOutput();
       bool    CheckMaxNumbersInTree();

       TAllInputSamples* _INPUT;
       TEventTree _eventTree;

       bool _isDebugMode;
       int _channel;
       int _vgamma;
       float _lumiWeight;
       long _Nentries;
       float _csWholeMCfile;

       TFile* _fOut;

       //Acc times Eff
       float  _accXeffTot;
       float  _accXeffTotErr;
       TH1F* _HaccXeff1D;

       // numerator used to compute Acc x Eff
       // comes as output of PrepareYields (signal MC yields in phoGenEt bins)
       float _numTot;
       float _numTotErr;
       TH1F* _Hnumerator1D;

       // denominator used to compute Acc x Eff
       float _denTot;
       float _denTotErr;
       TH1F* _Hdenominator1D;

       // same as denominator but without lumiWeight
       float _denNoWeightTot;
       float _denNoWeightTotErr;
       TH1F* _HdenominatorNoWeight1D;

       // is computed from denominator without lumiWeight
       float _csTheoryTot;
       float _csTheoryTotErr;
       TH1F* _HcsTheory1D;

       TFullCuts _fullCuts;
       TMathTools _math;

       const static int _nPosPatts=10;
       int _mcPatternNeg;
       int _mcPatternPos[_nPosPatts];
       TString _strPattern[_nPosPatts];

       const static int _debugModeNEntries=100000;
       TConfiguration _config;

       TLorentzVector _lorentzPhoton;
       TLorentzVector _lorentzLepton1;
       TLorentzVector _lorentzLepton2;
       bool _arr_imcPhoIsDressing[100];
  };

#endif 
  //#ifndef 
