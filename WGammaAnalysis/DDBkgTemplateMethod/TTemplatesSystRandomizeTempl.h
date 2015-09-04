#ifndef TTemplatesSystRandomizeTempl_h
#define TTemplatesSystRandomizeTempl_h

#include "TTemplates.h"

#include "TGraphErrors.h"
#include "TGraph.h"
#include "TGraph2D.h"
//#include "TH2D.h"
#include "TVectorD.h"
#include "TLine.h"

class TTemplatesSystRandomizeTempl: public TTemplates
{
  public:

    static const int nRandPointsTrue=20;//20;
    static const int nRandPointsFake=100;//100;
//    struct TemplatesRandomizePars{
//      int nRandPoints;
//    };

    TTemplatesSystRandomizeTempl();
    TTemplatesSystRandomizeTempl(TemplatesPars pars);
    virtual ~TTemplatesSystRandomizeTempl();

    void RandomizeTempl(); 


  private:

//    TemplatesRandomizePars _randPars;

    void RandomizeFakeTemplates(int ikin, int ieta);
    void RandomizeTrueTemplates(int ikin, int ieta);

    void PlotOneKinOneEtaBin(int ikin, int ieta);
    void FitWithRandTemplatesKinEtaBin(int ikin, int ieta);
//    void PrintOutKinEtaBin(int ikin, int ieta);


    TH1F* _hYieldsTrue[nKinBinsMax][2];// _BARREL, _ENDCAP
    TH1F* _hYieldsFake[nKinBinsMax][2];// _BARREL, _ENDCAP

    int _nValuesTrue[nKinBinsMax][2];
    int _nValuesFake[nKinBinsMax][2];
    float _sumSquaresTrue[nKinBinsMax][2];
    float _sumSquaresFake[nKinBinsMax][2];
    float _rmsTrue[nKinBinsMax][2];
    float _rmsFake[nKinBinsMax][2];
    float _nTrueYieldsValRef[nKinBinsMax][2];
    float _nFakeYieldsValRef[nKinBinsMax][2];

};

#endif //TTemplatesSystRandomizeTempl
