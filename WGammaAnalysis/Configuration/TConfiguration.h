#ifndef TConfiguration_h
#define TConfiguration_h
 
#include "TString.h" 
#include "TColor.h" 
  //ROOT class

class TConfiguration
{
  public:
    TConfiguration();
    virtual ~TConfiguration();

    enum {MUON, ELECTRON};
    enum {DATA, SIGMC, BKGMC};
    enum {BARREL, ENDCAP, COMMON};
    enum {TOTAL, ONEDI, TWODI};

    void Print();

    TString GetOutputDirName(int channel);
    TString GetSampleName(int sample);
    TString GetEtaBinName(int etaBin);

    TString GetSelectedPreliminaryName(int channel, int sample, TString sourceName="");
    TString GetSelectedFullyName(int channel, int sample, TString sourceName="");

    TString GetYieldsFileName(int channel);
    TString GetYieldsSelectedHistName(int sample, int etaBin, TString sourceName="");
    TString GetYieldsSelectedSignalMCGenHistName();
    TString GetYieldsDDTemplateBkgHistName(int etaBin);
    TString GetYieldsSignalName(int etaBin);

    TString GetFractionsDDTemplateBkgFileName(int channel);
    TString GetFractionsDDTemplateBkgHistName(int etaBin);
    TString GetTemplatePicNameBase(int ptBin, int etaBin);

    TString GetAccEffFileName(int channel);
    TString GetAcc1DName();
    TString GetEff1DName();
    TString GetAccErr1DName();
    TString GetEffErr1DName();
    TString GetAccTotalName();
    TString GetEffTotalName();
    TString GetAccErrTotalName();
    TString GetEffErrTotalName();

    TString GetUnfoldingFileName(int channel);
    TString GetMatrUnfo1DName();
    TString GetMatrMigr1DName();

    TString GetNameDebugMode();

    TString GetPhotonScaleFactorsFileNamePt15to20GeV();
    TString GetPhotonScaleFactorsFileNamePt20toInfGeV();
    TString GetPileupDataFileName();
    TString GetPhosphorConstantFileName();

    int GetNPhoPtBins();
    vector <float> GetPhoPtBinsLimits();
    int FindPhoPtBinByPhoPt(float pt);
    float GetPhoPtMin();
    float GetLePhoDeltaRMin();

    int GetNPhoPtUnfBins(bool isOverflowUsed);
    vector <float> GetPhoPtUnfBinsLimits(bool isOverflowUsed);
    int FindPhoPtUnfBinByPhoPt(float pt, bool isOverflowUsed);

};

/////////////////////////////////////////
//photon Pt binning
static const int nPhoPtBins_ = 9;
static const float phoPtBinsLimits_[nPhoPtBins_+1]={15.,20.,25.,30.,35.,40.,60.,80.,200.,600.};


static const float phoPtOverflowBinLimit_=1000.;
static const float phoPtMin_=15.;
  //minimum value for total CS
static const float lePhoDeltaRMin_=0.7;
  //minimum value for total and differential cross section; 
  //cut value may be bigger or equal to this value

/////////////////////////////////////////
//general locations
//
static const TString outputDirMu_="../WGammaOutput/MUON/";
static const TString outputDirEle_="../WGammaOutput/ELECTRON/";
//all other pathes are relative to one of these two

/////////////////////////////////////////
//selected files, directory and file names
//(these files will be output of the script Selection)
static const TString selectedPreliminaryEventsNameBase_="PreliminarySelected/selected";
static const TString selectedFullyEventsNameBase_="FullySelected/selected";


//////////////////////////////////////////
//yields
//
static const TString yieldsFileName_="YieldsAndBackground/yields.root";
static const TString yieldsSelectedHistName_="yieldsSelected";
static const TString yieldsDDTemplateBkgHist_="yieldsDDBkg";
static const TString yieldsSignal_="yieldsSignal";
static const TString yieldsSelectedSignalMCGenHistName_="yieldsSelectedSignalMCGen";


//////////////////////////////////////////
//data driven background estimation
//
static const TString fractionsDDTemplateBkgFile_="YieldsAndBackground/fractionsDDTemplateBkg.root";
static const TString fractionsDDTemplateBkgHist_="fractionsBkg";
static const TString templatePicNameBase_="templateFit_";

//////////////////////////////////////////
//acceptance and efficiency
//
static const TString acceffFile_="Constants/AccEff.root";
static const TString acc1DName_="acc1D"; 
static const TString eff1DName_="eff1D"; 
static const TString accErr1DName_="accErr1D"; 
static const TString effErr1DName_="effErr1D";
static const TString accTotalName_="accTotal"; 
static const TString effTotalName_="effTotal"; 
static const TString accErrTotalName_="accErrTotal"; 
static const TString effErrTotalName_="effErrTotal";

//////////////////////////////////////////
//unfolding
//
static const TString unfoldingFileName_="Constants/Unfolding.root";
static const TString matrUnfo1DName_="matrUnfo1D";
static const TString matrMigr1DName_="matrMigr1D";

//////////////////////////////////////////
static const TString nameDebugMode_ = "_debugMode";

//////////////////////////////////////////
//certrified constants
//
static const TString certifiedConstantsDir_="../CertifiedConstants/";
static const TString photonScaleFactorsFileNamePt15to20GeV_="PhotonScaleFactors/ScaleFactors_Pt15to20GeV_2012.root";
static const TString photonScaleFactorsFileNamePt20toInfGeV_="PhotonScaleFactors/ScaleFactors_Pt20toInfGeV_2012.root";
static const TString pileupDataFileName_ = "Pileup/TruePU_69300_2012Full.root";
// phosphor corrections file
static const TString phosphorConstantsFile_ = "PHOSPHOR_NUMBERS_EXPFIT_ERRORS.txt";

#endif //#ifndef TConfiguration_h
