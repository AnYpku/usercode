#ifndef TEtoGamma_h
#define TEtoGamma_h

#include "../Configuration/TConfiguration.h"

#include "TH1F.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TTree.h"
#include "THStack.h"
#include "TLatex.h"
#include "TCut.h"

#include <vector>

class TEtoGamma
{
  public:

    TEtoGamma(TConfiguration::AnalysisParameters &anPars);
    virtual ~TEtoGamma();
    void ComputePlotSave();

  private:
    TFile* _fOut;

    // etaBin: BARREL, ENDCAP
    TH1F* _yieldDataEnr[2];// e->gamma enriched
    TH1F* _yieldDYjetsEnr[2];// e->gamma enriched
    TH1F* _hRatio[2];
    TH1F* _yieldDYjets[2];// nominally selected
    TH1F* _yieldDDEtoGamma[3];// data driven estimate

    TConfiguration _conf;
    TCut _cutAdd;
    TString _varKin;
    int _nKinBins;
    float _kinBinLims[100];

    void WriteToFile();
    void SetHists();
    void ComputeBkg();
    TTree* GetTree(TString strFileName);

};


#endif //TEtoGamma_h
