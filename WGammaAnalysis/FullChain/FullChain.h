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
      int year;
      int channel;
      int vgamma;
      int blind;
      TString varKin;
      int nKinBins;
      float* kinBinLims;
      int sampleMode;
      string analyzedSamples;
      string configfile;
      bool noAdjustBinning;
      bool isMetCutOptimization;
      bool doSystTemplateStat;
      bool isNoPuReweight;
      bool isDebugMode;
      int phoWP;

      //for template method:
      TString strDDbkgVarFit;
      TString strDDbkgVarSideband;
      //end for template method;
 
      bool noPhoPFChIsoCut;

      bool noPreSelection;
      bool noExtraSelection;
      bool noDDBkgComputation;
      bool noPrepareYields;
      bool noCalcAccAndEff;
      bool noCalcCrossSection;
    };

    void SetDefaultFullChainParameters(FullChainParameters& anPars, TString varKin="phoEt");
    void SetAnalysisKinParameters(FullChainParameters& anPars);

    void SetDiffKinFullChainParameters(FullChainParameters& anPars, TString varKin);

    void RunAnalysis(FullChainParameters anPars);

  private:
    TConfiguration _config;
    TAllInputSamples* _INPUT;
    float* _phoPtBinLimits;

};


#endif //FullChain_h
