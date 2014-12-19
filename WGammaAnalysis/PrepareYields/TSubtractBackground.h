#ifndef TSubtractBackground_h
#define TSubtractBackground_h

#include "TPrepareYields.h"

class TSubtractBackground: public TPrepareYields
{
  public:
    TSubtractBackground();
    virtual ~TSubtractBackground();

    void SetYieldsDataDrivenTrue(TString name, TString label, int color, TString fileName, TString strYieldsName1D[3], TString strYieldsNameTot[3]);
    void SetYieldsDataDrivenFake(TString name, TString label, int color, TString fileName, TString strYieldsName1D[3], TString strYieldsNameTot[3]);

    void SubtractBackground();

    void PlotPrintSave();

    struct SubtractBackgroundPars{
//      float DDTrueGammaYieldTot[3];
//      float DDTrueGammaYieldTotErr[3];
//      TH1F* DDTrueGammaYields[3];
//      float DDFakeGammaYieldTot[3];
//      float DDFakeGammaYieldTotErr[3];
//      TH1F* DDFakeGammaYields[3];
    };
  
  private:
    SubtractBackgroundPars _sbPars;
    YieldsSource _sourceDDTrue;
    YieldsSource _sourceDDFake;
    YieldsSource _sourceBkgSubtrData;

    void SetYieldsDataDriven(YieldsSource& source, TString name, TString label, int color, TString fileName, TString strYieldsName1D[3], TString strYieldsNameTot[3]);
    void CompareFakeDDvsMC(int ieta);
//    void CompareTrueDDvsMC(int ieta);
//    void CompareBkgSubtrDATAvsSIGMC(int ieta);

    TCanvas* _canvFakeDDvsMC[3];
    TCanvas* _canvTrueDDvsMC[3];
    TCanvas* _canvBkgSubtrDATAvsSIGMC[3];
};

#endif //TSubtractBackground
