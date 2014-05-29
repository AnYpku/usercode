#ifndef FullChain_h
#define FullChain_h

#include "../Configuration/TAllInputSamples.h"
#include "../Configuration/TConfiguration.h"
#include <string>

class FullChain
{
  public:
    FullChain();
    virtual ~FullChain();

    struct FullChainParameters{
      int channel;
      int blind;
      TString varKin;
      int nKinBins;
      float* kinBinLims;
      int sampleMode;
      string analyzedSamples;
      string configfile;
      bool isMetCutOptimization;
      bool isNoPuReweight;
      bool isDebugMode;
      int phoWP;
      TString strPhoIsoBase;

      //for template method:
      TString strDDbkgVarFit;
      TString strDDbkgVarSideband;
      int* nBinsLeftB;
      float* maxVarFitB;
      int* nBinsLeftE;
      float* maxVarFitE;
      int nBinsLeftBTot;
      float maxVarFitBTot;
      int nBinsLeftETot;
      float maxVarFitETot;
      //end for template method;

      bool noPreSelection;
      bool noExtraSelection;
      bool noDDBkgComputation;
      bool noPrepareYields;
      bool noCalcAccAndEff;
      bool noCalcCrossSection;
    };

    void SetDefaultFullChainParameters(FullChainParameters& anPars, TString varKin="phoEt");
    void SetDiffKinFullChainParameters(FullChainParameters& anPars, TString varKin);
    void SetAnalysisKinParameters(FullChainParameters& anPars);

    void RunAnalysis(FullChainParameters anPars);

  private:
    TConfiguration _config;
//    TAllInputSamples* _INPUT;
//    float* _phoPtBinLimits;
};


#endif //FullChain_h
