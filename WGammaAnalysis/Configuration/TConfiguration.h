#ifndef TConfiguration_h
#define TConfiguration_h

#include "TString.h" //ROOT class

class TConfiguration
{
  public:
    TConfiguration(int channel, int sample);
    virtual ~TConfiguration();
    void SetChannelAndSample(int channel, int sample);
    void SetInputFileNames();
    void SetSelectedFileName();
    TString GetPhosphorConstantFileName();

    enum {MUON, ELECTRON};
    enum {DATA, SIGMC, BKGMC};

    int nInputFiles;
    static const int nInputFilesMax_=30;
      //increase this number if for one of the cases 
      //you have more than 30 input files
    TString inputFileNames[nInputFilesMax_];
    TString selectedFileName;

  private:
    int channel_;
      //select channel - MUON or ELECTRON
    int sample_;
      //select on what you would like to run the analysis:
      //DATA, SIGMC or BKGMC
};

static const int nDataMuFiles_ = 8;
static const TString inputDataMuFileNames_[nDataMuFiles_]=
{
"/mnt/hadoop/user/uscms01/pnfs/unl.edu/data4/cms/store/user/ikrav/WGamma/MuoGamDR03Skim/V05-03-07-04_maybe/job_1muon_2012a_Jul13rereco_skim.root",//Jul 13
"/mnt/hadoop/user/uscms01/pnfs/unl.edu/data4/cms/store/user/ikrav/WGamma/MuoGamDR03Skim/V05-03-07-06/job_muon_2012b_Jul13rereco_skim.root",//Jul 13
"/mnt/hadoop/user/uscms01/pnfs/unl.edu/data4/cms/store/user/ikrav/WGamma/MuoGamDR03Skim/V05-03-07-04_maybe/job_1muon_2012a_Aug6rereco_skim.root",//Aug 6
"/mnt/hadoop/user/uscms01/pnfs/unl.edu/data4/cms/store/user/ikrav/WGamma/MuoGamDR03Skim/V05-03-07-06/job_muon_2012c_Aug24rereco_skim.root",//Aug 24
"/mnt/hadoop/user/uscms01/pnfs/unl.edu/data4/cms/store/user/ikrav/WGamma/MuoGamDR03Skim/V05-03-07-06/job_muon_2012c_Dec11rereco_skim.root",//Dec 11
"/mnt/hadoop/user/uscms01/pnfs/unl.edu/data4/cms/store/user/ikrav/WGamma/MuoGamDR03Skim/V05-03-07-06/job_muon_2012c_PRv2_skim.root",//PR
"/mnt/hadoop/user/uscms01/pnfs/unl.edu/data4/cms/store/user/ikrav/WGamma/MuoGamDR03Skim/V05-03-07-06/job_muon_2012c_PRv21_skim.root",//PR
"/mnt/hadoop/user/uscms01/pnfs/unl.edu/data4/cms/store/user/ikrav/WGamma/MuoGamDR03Skim/V05-03-07-04_maybe/job_1muon_2012d_PRv1_skim.root"//PR
};
static const int nDataEleFiles_ = 0;
static const TString inputDataEleFileNames_[nDataEleFiles_];
static const int nSigMCMuFiles_ = 1;
static const TString inputSigMCMuFileNames_[nSigMCMuFiles_]=
{"/home/hep/eavdeeva2/WGammaAnalysis/job_summer12_Wg.root"};
static const int nSigMCEleFiles_ = 0;
static const TString inputSigMCEleFileNames_[nSigMCEleFiles_];
static const int nBkgMCMuFiles_ = 0;
static const TString inputBkgMCMuFileNames_[nBkgMCMuFiles_];
static const int nBkgMCEleFiles_ = 0;
static const TString inputBkgMCEleFileNames_[nBkgMCEleFiles_];

static const TString selectedEventsDir_="../WGammaOutput/";
static const TString selectedEventsNameBase_="selected";

static const TString phosphorConstantsFile_ = "../CertifiedConstants/PHOSPHOR_NUMBERS_EXPFIT_ERRORS.txt";

#endif //#ifndef TConfiguration_h
