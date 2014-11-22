#include "TPrepareYields.h"
//#include "TSubtractBackground.h"

void AuxPrepareYieldsLovedeep()
{
  TPrepareYields prep;
  TPrepareYields::PrepareYieldsPars pars;

  pars.varKin="pho_pt";
  pars.nKinBins=9;
  float lims[10]={15,20,25,30,35,40,55,75,95,500};
  for (int il=0; il<10; il++)
    pars.kinBinLims[il]=lims[il];
  pars.blindFraction=0.05;
  pars.varWeight="evt_weight";
  pars.cutBarrel="pho_eta>-1.4442 && pho_eta<1.4442";
  pars.cutEndcap="(pho_eta>-2.5 && pho_eta<-1.566)||(pho_eta<2.5 && pho_eta>1.566)";
  pars.doLogX=1;
  pars.doLogY=1;
  pars.varKinLabel="Pt_#gamma";
  pars.strFileOut="fOut.root";

  prep.SetPars(pars);

  //bool SetOneYieldSource(int sourceType, TString name, TString label, int color, TString fileName, TString treeName);

  prep.SetOneYieldSource(prep.DATA, "data", "data", 1, "../WGammaOutput/ELECTRON/Hist_job_electron_2012_data.root", "miniTree");

  prep.SetOneYieldSource(prep.SIGMC, "Wg", "W#gamma", 634, "../WGammaOutput/ELECTRON/Hist_job_summer12_Wg_skimmed_wg.root", "miniTree");
  
  prep.SetOneYieldSource(prep.BKGMC, "Wjets", "W+jets#rightarrowl#nu+jets", 433, "../WGammaOutput/ELECTRON/Hist_job_summer12_Wjets_skimmed_wg.root", "miniTree");
  
  prep.SetOneYieldSource(prep.BKGMC, "DYjets", "DY+jets#rightarrowl#bar{l}", 418, "../WGammaOutput/ELECTRON/Hist_job_summer12_DYJetsToLL_skimmed_wg.root", "miniTree");
  
  prep.SetOneYieldSource(prep.BKGMC, "Zg", "Z#gamma#rightarrowl#bar{l}", 879, "../WGammaOutput/ELECTRON/Hist_job_summer12_Zg_skimmed_wg.root", "miniTree");

  prep.SetOneYieldSource(prep.BKGMC, "ttjets", "t#bar{t}+jets", 631, "../WGammaOutput/ELECTRON/Hist_job_summer12_ttjets.root", "miniTree");

  prep.PlotPrintSave();

  // Other sources I plot in muon channel:
  // Wg_to_taunu,     W#gamma#rightarrow#tau#nu#gamma,      401
  // WWg,      WW#gamma,       884   
  
  
}

//void AuxSubtractBackground(int channel, int vgamma, int blind, TString varKin, int nKinBins, float* kinBinLims, bool noDDBkgComputation=0)
//{
//}

