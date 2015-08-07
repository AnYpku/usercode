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

    enum {_DATA_EtoGAMMA_ENR, _ZJETS_EtoGAMMA_ENR, _ZJETS_NOM_ELE, _ZJETS_NOM_MUO, _WJETS_NOM_ELE, _WJETS_NOM_MUO};

    struct Yield{
      int num;
      TH1F* hist[2];
      TString fName;
      TTree* tr;
//      float cont;
//      float err;
    };
    Yield _yield[6];

    TString FName(int inum);
    TString HName(int inum, int ieta);
    void SetYields(int inum);

    // etaBin: BARREL, ENDCAP
    TH1F* _yieldDDEtoGamma[3];// data driven estimate

    TConfiguration _conf;
    TCut _cutAdd;
    TString _varKin;
    int _nKinBins;
    float _kinBinLims[100];

    void WriteToFile();
    void ComputeBkg();
    TTree* GetTree(TString strFileName);

};


#endif //TEtoGamma_h
