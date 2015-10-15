#ifndef TTemplatesSystZgNorm_h
#define TTemplatesSystZgNorm_h

#include "TTemplates.h"

#include "TGraphErrors.h"
#include "TGraph.h"
#include "TGraph2D.h"
//#include "TH2D.h"
#include "TVectorD.h"
#include "TLine.h"

class TTemplatesSystZgNorm: public TTemplates
{
  public:

    TTemplatesSystZgNorm();
    TTemplatesSystZgNorm(TemplatesPars pars);
    virtual ~TTemplatesSystZgNorm();

    void SystDueToZgNorm(); 


  private:

    void FitWithDifferentNorms(int ikin, int ieta);
    void ChangeTemplatesDiffNorm(TH1D* hTemp, TH1D* hLeak, int k);

    float _nTrueYieldsValDiff[nKinBinsMax][2][5];
      //[2]: _BARREL, _ENDCAP
      //[5]: [0]nominal, [1][2] Zg norm +-20%, [3][4] DYjets norm +-20% 

    TH1D* _hFakeDiff[nKinBinsMax][2][3];

    float _nTrueYieldsValRef[nKinBinsMax][2];
    float _nFakeYieldsValRef[nKinBinsMax][2];

};

// const static float csErrFromZgMeas[13]={0.20,0.066,0.069,0.079,0.097,0.088,0.122,0.165,0.20,0.20,0.20,0.165,0.174};

#endif //TTemplatesSystZgNorm
