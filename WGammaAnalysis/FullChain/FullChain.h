#ifndef FullChain_h
#define FullChain_h

#include "../Configuration/TAllInputSamples.h"
#include "../Configuration/TConfiguration.h"

class FullChain
{
  public:
    FullChain();
    virtual ~FullChain();

    struct FullChainParameters{
      int channel;
      int phoWP;
      TString strPhoIsoBase;
      bool noPreSelection;
      bool noExtraSelection;
      bool noDDBkgComputation;
      bool noPrepareYields;
      bool noCalcAccAndEff;
    };

    void SetDefaultFullChainParameters(FullChainParameters& anPars);

    void RunAnalysis(FullChainParameters anPars);

  private:
//    TConfiguration _config;
//    TAllInputSamples* _INPUT;
//    float* _phoPtBinLimits;
};


#endif //FullChain_h
