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

    static const int nRandPointsMax=10;
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

//    float _yieldsTrueVal[_nPointsMax][_nPointsMax];
//    float _yieldsTrueErr[_nPointsMax][_nPointsMax];
//    float _yieldsFakeVal[_nPointsMax][_nPointsMax];
//    float _yieldsFakeErr[_nPointsMax][_nPointsMax];
//    float _MCtruthKolmogorov[_nPointsMax][_nPointsMax];

//    TGraph2D* _grTrueVal[nKinBinsMax][2];
//    TGraph2D* _grFakeVal[nKinBinsMax][2];
//    TGraph2D* _grTrueErr[nKinBinsMax][2];
//    TGraph2D* _grFakeErr[nKinBinsMax][2];

//    TVectorD* _vecSidebandLower[nKinBinsMax][2];
//    TVectorD* _vecSidebandUpper[nKinBinsMax][2];
//    TVectorD* _vecTrueVal[nKinBinsMax][2];
//    TVectorD* _vecTrueErr[nKinBinsMax][2];
//    TVectorD* _vecFakeVal[nKinBinsMax][2];
//    TVectorD* _vecFakeErr[nKinBinsMax][2];
//    TVectorD* _vecMCtruthKolmogorov[nKinBinsMax][2];
//    TLine* _lineRefTrue[nKinBinsMax][2];
//    TLine* _lineRefFake[nKinBinsMax][2];
//    TGraph* _grEmptTrue[nKinBinsMax][2];
//    TGraph* _grEmptFake[nKinBinsMax][2];
};

#endif //TTemplatesSystRandomizeTempl
