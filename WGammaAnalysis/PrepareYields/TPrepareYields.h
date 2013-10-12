#ifndef TPrepareYields_h
#define TPrepareYields_h

#include "../Configuration/TConfiguration.h"
#include "../Configuration/TAllInputSamples.h"

#include "TH1F.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TLegend.h"

#include <vector>

class TPrepareYields
{
  public:
    TPrepareYields(int channel);
    virtual ~TPrepareYields();
    void PrepareYields();
    void SetYields();
    void SetYieldsOneSource(int iSource);
    void SetYieldsDDBkgTemplate();
    void SubtractBackground();
    void CompareBkgDDandMC();
    void CompareYieldsDATAandMC();
    void StoreYields();

  private:
    TConfiguration config_;
    int channel_;
    TAllInputSamples* INPUT_;

    TFile* fOut_;

    TH1F* dataYields_;
    TH1F* dataYieldsB_;
    TH1F* dataYieldsE_;
    
    TH1F* sigMCYields_;
    TH1F* sigMCYieldsB_;
    TH1F* sigMCYieldsE_;

    vector <TH1F*> vecBkgMCYields_;
    vector <TH1F*> vecBkgMCYieldsB_;
    vector <TH1F*> vecBkgMCYieldsE_;

    TH1F* bkgDDYields_;
    TH1F* bkgDDYieldsB_;
    TH1F* bkgDDYieldsE_;

    TH1F* signalYields_;
    TH1F* signalYieldsB_;
    TH1F* signalYieldsE_;

    TLegend* legend_;

    TCanvas* canvDDvsMC_;
    

};


#endif //TPrepareYields_h
