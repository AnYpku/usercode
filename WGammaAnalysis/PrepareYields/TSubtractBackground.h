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


  
  private:

    YieldsSource _sourceDDTrue;
    YieldsSource _sourceDDFake;
    YieldsSource _sourceBkgSubtrData;

    void SetYieldsDataDriven(YieldsSource& source, TString name, TString label, int color, TString fileName, TString strYieldsName1D[3], TString strYieldsNameTot[3]);
    void CompareFakeDDvsMC(int ieta);
    void CompareTrueDDvsMC(int ieta);
    void CompareDDvsMC(int ieta, TString strDD, int bkgType, YieldsSource& sourceDD, TCanvas* canv);
    void CompareDATAvsDDsum(int ieta);
//    void CompareBkgSubtrDATAvsSIGMC(int ieta);

    TCanvas* _canvFakeDDvsMC[3];
    TCanvas* _canvTrueDDvsMC[3];
    TCanvas* _canvBkgSubtrDATAvsSIGMC[3];
    TCanvas* _canvDATAvsDDsum[3];
};

#endif //TSubtractBackground
