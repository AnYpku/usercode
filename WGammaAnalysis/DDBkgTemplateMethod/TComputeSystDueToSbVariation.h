#ifndef TComputeSystDueToSbVariation_h
#define TComputeSystDueToSbVariation_h

#include "TVectorD.h"
#include "TFile.h"
#include "TH1F.h"

#include "../Configuration/TConfiguration.h"

class TComputeSystDueToSbVariation
{
  public:
    const static int nBinsMax=50;
    TComputeSystDueToSbVariation(TConfiguration::AnalysisParameters anPars);
    virtual ~TComputeSystDueToSbVariation();
    //ComputeSystDueToSbVariationPars 
    bool ComputeSyst();
    enum {METH1, METH2};
    bool ComputeSystOneBin(int meth, int ieta, int ikin);
    bool GetHistsAndVectors();
    bool PrintLatexTableMeth1();
    bool FillVecUi(vector <float> &vecUi, float& Ui_min, float& Ui_max, int meth, int ikin, int ieta, int N_Sb_Data, int N_Sb_MC, float KTthresh);

  private:
    TFile* _fOut;
    TFile* _fMCtruth;
    TH1F* _hMCtruth[2];//Barrel and Endcap
    TFile* _fSbs;
    TH1F* _hSbLselected[2];
    TH1F* _hSbUselected[2];
    TFile* _fSbVarMC;
    TFile* _fSbVarData;
    TVectorD* _vecSbL_MC[nBinsMax][2];//Barrel and Endcap
    TVectorD* _vecSbU_MC[nBinsMax][2];//Barrel and Endcap
    TVectorD* _vecFitRes_MC[nBinsMax][2];//Barrel and Endcap
    TVectorD* _vecKolmogorov_MC[nBinsMax][2];//Barrel and Endcap
    TVectorD* _vecSbL_Data[nBinsMax][2];//Barrel and Endcap
    TVectorD* _vecSbU_Data[nBinsMax][2];//Barrel and Endcap
    TVectorD* _vecFitRes_Data[nBinsMax][2];//Barrel and Endcap
    TConfiguration _config;
    TConfiguration::AnalysisParameters _anPars;
    TString StrLabelEta(int ieta);
    TString StrLabelKin(int ikin);

    float _f_d[nBinsMax][3];//Barrel, Endcap, Sum
    float _f_MC[nBinsMax][3];//Barrel, Endcap, Sum
    float _f_MCtruth[nBinsMax][3];//Barrel, Endcap, Sum
    float _U0[nBinsMax][2];
    float _meanMeth1[nBinsMax][2];
    float _RMSMeth1[nBinsMax][2];
    float _systMeth1[nBinsMax][3];//Barrel, Endcap, Sum
    float _systRelMeth1[nBinsMax][3];//Barrel, Endcap, Sum
};// end of class TComputeSystDueToSbVariation

#endif //TComputeSystDueToSbVariation_h
