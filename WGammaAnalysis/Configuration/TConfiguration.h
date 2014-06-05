#ifndef TConfiguration_h
#define TConfiguration_h
 
#include "TString.h" 
#include "TColor.h" 
  //ROOT class
#include <vector>

class TConfiguration
{
  public:
    TConfiguration();
    virtual ~TConfiguration();

    enum {MUON, ELECTRON, BOTH};//channel
    enum {DATA, SIGMC, BKGMC};//sample
    enum {UNBLIND, BLIND_PRESCALE, BLIND_DECRACC};//blinding type
    enum {BARREL, ENDCAP, COMMON};//eta bin
    enum {TOTAL, ONEDI, TWODI};//csMode
    enum {VERY_PRELIMINARY,
          PRELIMINARY_FOR_MET_CUT,
          PRELIMINARY_FOR_TEMPLATE_METHOD,
          PRELIMINARY_FOR_UNFOLDING,
          FULLY};//selection stage

    void Print();

    TString GetOutputDirName(int channel);
    TString GetSampleName(int sample);
    TString GetEtaBinName(int etaBin);
    TString GetCsModeName(int csMode);
    TString GetBlindModeName(int sample, int blindMode);

    TString GetSelectedName(int selectionStage, int channel, int blindType, int sample, TString sourceName="", bool isDebugMode=0, bool isNoPuReweight=0);
    //strSelectionStage
    TString GetSpecialModeName(bool isDebugMode, bool isNoPuReweight);

    TString GetYieldsFileName(int channel);
    TString GetYieldsSelectedName(int csMode, int sample, TString sourceName="");
    TString GetYieldsSelectedSignalMCGenName(int csMode);
    TString GetYieldsDDTemplateBkgName(int csMode);
    TString GetYieldsSignalName(int csMode);

    TString GetDDTemplateBkgFileName(int channel);

    TString GetAccFileName(int channel);
    TString GetAccName(int csMode);
    TString GetEffFileName(int channel);
    TString GetEffName(int csMode);

//    TString GetUnfoldingFileName(int channel);
//    TString GetMatrUnfo1DName();
//    TString GetMatrMigr1DName();
//    TString GetYieldsRec1DName();
//    TString GetYieldsGen1DName();

    TString GetPhotonScaleFactorsFileNamePt15to20GeV();
    TString GetPhotonScaleFactorsFileNamePt20toInfGeV();
    TString GetPileupDataFileName();
    TString GetPhosphorConstantFileName();

    int GetNPhoPtBins();
    void GetPhoPtBinsLimits(float* lims);
    int FindPhoPtBinByPhoPt(float pt);

    float GetPhoPtMin();
    float GetPhoPtMax();
    float GetLePhoDeltaRMin();

    int GetBlindPrescale();
    int GetPhoPtBlindThreshold();

    int GetNPhoPtUnfBins(bool isOverflowUsed);
    void GetPhoPtUnfBinsLimits(float* lims,bool isOverflowUsed);
    int FindPhoPtUnfBinByPhoPt(float pt, bool isOverflowUsed);

};

/////////////////////////////////////////
//photon Pt binning
//and other analysis constants
static const int _nPhoPtBins = 9;
static const float _phoPtBinsLimits[_nPhoPtBins+1]={15.,20.,25.,30.,35.,40.,60.,80.,200.,600.};
//static const int _nPhoPtBins = 5;
//static const float _phoPtBinsLimits[_nPhoPtBins+1]={15.,20.,25.,30.,35.,40.};
static const float _phoPtMin=15.;
static const float _phoPtMax=2000.;
  //minimum value for total CS
static const float _lePhoDeltaRMin=0.7;
  //minimum value for total and differential cross section; 
  //cut value may be bigger or equal to this value
static const int _blindPrescale=20;//13;
static const float _phoPtBlindThreshold=40.;

/////////////////////////////////////////
//general locations
//
static const TString _outputDirMu="../WGammaOutput/MUON/";
static const TString _outputDirEle="../WGammaOutput/ELECTRON/";
//all other pathes are relative to one of these two

/////////////////////////////////////////
//selected files, directory and file names
//(these files will be output of the script Selection)
//names are listed in the constructor for now
TString _selectedNameBase[5];

//////////////////////////////////////////
//yields
//
static const TString _yieldsFileName="YieldsAndBackground/yields.root";
static const TString _yieldsSelectedName="yieldsSelected";
static const TString _yieldsDDTemplateBkgName="yieldsDDBkg";
static const TString _yieldsSignalName="yieldsSignal";
static const TString _yieldsSelectedSignalMCGenName="yieldsSelectedSignalMCGen";


//////////////////////////////////////////
//data driven background estimation
//
static const TString _DDTemplateBkgFileName="YieldsAndBackground/bkgDDTemplate.root";
//_yieldsDDTemplateBkgHist are also here

//////////////////////////////////////////
//acceptance and efficiency
//
static const TString _accFileName="Constants/Acc.root";
static const TString _accName="acc"; 
static const TString _effFileName="Constants/Eff.root";
static const TString _effName="eff"; 

//////////////////////////////////////////
//unfolding
//
static const TString unfoldingFileName_="Constants/Unfolding.root";
static const TString matrUnfo1DName_="matrUnfo1D";
static const TString matrMigr1DName_="matrMigr1D";
static const TString yieldsRec1DName_="yieldsRec1D";
static const TString yieldsGen1DName_="yieldsGen1D";

//////////////////////////////////////////
static const TString nameDebugMode_ = "_debugMode";
static const TString nameNoPuReweight_="_noPuReweight";

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
