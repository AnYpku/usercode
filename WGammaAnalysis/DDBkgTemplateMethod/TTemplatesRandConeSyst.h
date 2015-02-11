#ifndef TTemplatesRandConeSyst_h
#define TTemplatesRandConeSyst_h

#include "TTemplatesRandCone.h"

#include "TGraphErrors.h"
#include "TGraph.h"
#include "TGraph2D.h"
#include "TH2D.h"
#include "TLine.h"

class TTemplatesRandConeSyst: public TTemplatesRandCone
{
  public:

    struct TemplatesSidebandVariationPars{
      int nPointsLower[2];// _BARREL and _ENDCAP
      int nPointsUpper[2];// _BARREL and _ENDCAP
      float lowerSidebandCutFrom[2];
      float lowerSidebandCutTo[2];
      float upperSidebandCutFrom[2];
     float upperSidebandCutTo[2];
    };

    TTemplatesRandConeSyst();
    TTemplatesRandConeSyst(TemplatesRandConePars pars, TemplatesSidebandVariationPars variationPars);
    virtual ~TTemplatesRandConeSyst();

    void SidebandVariation(); 


  private:

    TemplatesSidebandVariationPars _variationPars;

    void SidebandVariationOneKinBin(int ikin); 
    void PlotOneKinBin(int ikin);
    void VarySidebandKinEtaBin(int ikin, int ieta);
    void PrepareGraphsKinEtaBin(int ikin, int ieta);
    void CheckMinAndMax(float val, float err, float& min, float& max);
    void PrintOutKinEtaBin(int ikin, int ieta);
    void SetPlottingStyles(TGraphErrors* gr, TLine* line);


    const static int _nPointsMax=50;//larger or equal to the largest out of _nPoints[2]
//    float _sidebandRef[2];
    float _sidebandLowerVal[_nPointsMax];
    float _sidebandLowerErr[_nPointsMax];
    float _sidebandUpperVal[_nPointsMax];
    float _sidebandUpperErr[_nPointsMax];
    float _yieldsTrueVal[_nPointsMax][_nPointsMax];
    float _yieldsTrueErr[_nPointsMax][_nPointsMax];
    float _yieldsFakeVal[_nPointsMax][_nPointsMax];
    float _yieldsFakeErr[_nPointsMax][_nPointsMax];

    TH2D* _histTrueVal[nKinBinsMax][2];
    TGraph2D* _grTrueVal[nKinBinsMax][2];
    TGraph2D* _grFakeVal[nKinBinsMax][2];
    TGraph2D* _grTrueErr[nKinBinsMax][2];
    TGraph2D* _grFakeErr[nKinBinsMax][2];
//    TLine* _lineRefTrue[nKinBinsMax][2];
//    TLine* _lineRefFake[nKinBinsMax][2];
//    TGraph* _grEmptTrue[nKinBinsMax][2];
//    TGraph* _grEmptFake[nKinBinsMax][2];
};

#endif //TTemplatesRandConeSyst
