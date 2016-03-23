#ifndef FullChain_h
#define FullChain_h

#include "TCut.h"
#include "../Configuration/TAllInputSamples.h"
#include "../Configuration/TConfiguration.h"
#include <string>

class FullChain
{
  public:
    FullChain();
    virtual ~FullChain();

    void SetDefaultFullChainParameters(TConfiguration::AnalysisParameters& anPars, TString varKin="phoEt");
    void SetAnalysisKinParameters(TConfiguration::AnalysisParameters& anPars);

    void SetDiffKinFullChainParameters(TConfiguration::AnalysisParameters& anPars, TString varKin);

    //    void RunAnalysis(TConfiguration::AnalysisParameters& anPars);

  private:
    TConfiguration _config;
    TAllInputSamples* _INPUT;
    float* _phoPtBinLimits;

};


#endif //FullChain_h
