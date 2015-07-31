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
    void SetYieldsDataDrivenEtoGamma(TString name, TString label, int color, TString fileName, TString strYieldsName1D[3], TString strYieldsNameTot[3]);
    void Increase_nDDsources();

    void SubtractBackground();

    void PlotPrintSave();


  
  private:

    int _nDDsources;
    YieldsSource _sourceDDTrue[_nHistsMax];
    YieldsSource _sourceDDEtoGamma;
    YieldsSource _sourceDDFake[_nHistsMax];
    YieldsSource _sourceBkgSubtrData[_nHistsMax];

    void SetYieldsDataDriven(YieldsSource& source, TString name, TString label, int color, TString fileName, TString strYieldsName1D[3], TString strYieldsNameTot[3]);
    void CompareFakeDDvsMC(int ieta);
    void CompareTrueDDvsMC(int ieta);
    void CompareDDvsMC(int ieta, TString strDD, int bkgType, YieldsSource sourceDD[_nHistsMax], TCanvas* canv);
    void CompareDATAvsDDsum(int ieta);
    void CompareDATAvsBKGplusSIGMC(int ieta);
    void CompareBkgSubtrDATAvsSIGMC(int ieta);

    TCanvas* _canvFakeDDvsMC[3];
    TCanvas* _canvTrueDDvsMC[3];
    TCanvas* _canvBkgSubtrDATAvsSIGMC[3];
    TCanvas* _canvDATAvsBKGplusSIGMC[3];
    TCanvas* _canvDATAvsDDsum[3];
};

#endif //TSubtractBackground
