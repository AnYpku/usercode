#ifndef TTemplatesRandConeSyst_h
#define TTemplatesRandConeSyst_h

#include "TTemplatesRandCone.h"

#include "TGraphErrors.h"
#include "TGraph.h"
#include "TLine.h"

class TTemplatesRandConeSyst: public TTemplatesRandCone
{
  public:

    TTemplatesRandConeSyst();
    TTemplatesRandConeSyst(TemplatesRandConePars pars);
    virtual ~TTemplatesRandConeSyst();

    void SidebandVariation();   

  private:


    void SidebandVariationOneKinBin(int ikin); 
    void PlotOneKinBin(int ikin);
    void VarySidebandKinEtaBin(int ikin, int ieta);
    void PrepareGraphsKinEtaBin(int ikin, int ieta);
    void CheckMinAndMax(float val, float err, float& min, float& max);
    void PrintOutKinEtaBin(int ikin, int ieta);
    void SetPlottingStyles(TGraphErrors* gr, TLine* line);


    const static int _nPointsMax=50;//larger or equal to the largest out of _nPoints[2]
    float _sidebandRef[2];
    float _sidebandsVal[_nPointsMax];
    float _sidebandsErr[_nPointsMax];
    float _yieldsTrueVal[_nPointsMax];
    float _yieldsTrueErr[_nPointsMax];
    float _yieldsFakeVal[_nPointsMax];
    float _yieldsFakeErr[_nPointsMax];

    TGraphErrors* _grTrue[nKinBinsMax][2];
    TGraphErrors* _grFake[nKinBinsMax][2];
    TLine* _lineRefTrue[nKinBinsMax][2];
    TLine* _lineRefFake[nKinBinsMax][2];
    TGraph* _grEmptTrue[nKinBinsMax][2];
    TGraph* _grEmptFake[nKinBinsMax][2];
};


    const static int _nPoints[2]={15,19};// _BARREL and _ENDCAP
    const static float _minSideband[2]={0.006,0.020};
    const static float _unit[2]={0.001,0.002};

#endif //TTemplatesRandConeSyst
