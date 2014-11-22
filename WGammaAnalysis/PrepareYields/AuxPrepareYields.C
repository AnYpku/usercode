#include "../Configuration/TConfiguration.h"
#include "../Include/TPhotonCuts.h"

#include "TPrepareYields.h"
//#include "TSubtractBackground.h"

void AuxPrepareYields(int channel, int vgamma, int blind, TString varKin, int nKinBins, float* kinBinLims)
{

  TConfiguration config;
  TPhotonCuts photon;

  TPrepareYields prep;
  TPrepareYields::PrepareYieldsPars pars;

  pars.varKin=varKin;
  pars.nKinBins=nKinBins;
  for (int il=0; il<nKinBins+1; il++)
    pars.kinBinLims[il]=kinBinLims[il];
  if (blind==config.UNBLIND) pars.blindFraction=1;
  if (blind==config.BLIND_PRESCALE) 1./config.GetBlindPrescale();
  pars.varWeight="weight";
  pars.cutBarrel=photon.RangeBarrel();
  pars.cutEndcap=photon.RangeEndcap();
  pars.doLogX=0;  pars.doLogY=0;
  if (varKin=="phoEt" && kinBinLims[nKinBins]>200){
    pars.doLogX=1;  pars.doLogY=1;
  }
  pars.varKinLabel=varKin;
  if (varKin=="phoEt") pars.varKinLabel="Pt_#gamma";
  pars.strFileOut="fOut.root";

  prep.SetPars(pars);

  //bool SetOneYieldSource(int sourceType, TString name, TString label, int color, TString fileName, TString treeName);

  prep.SetOneYieldSource(prep.DATA, "data", "data", 1, config.GetSelectedName(config.FULLY,channel,vgamma,blind,config.DATA), "selectedEvents");

  if (vgamma==config.W_GAMMA && channel==config.MUON)
    prep.SetOneYieldSource(prep.SIGMC, "Wg_to_munu", "W#gamma#rightarrow#mu#nu#gamma", 634, config.GetSelectedName(config.FULLY,channel,vgamma,blind,config.SIGMC), "selectedEvents");

  if (vgamma==config.W_GAMMA && channel==config.ELECTRON)
    prep.SetOneYieldSource(prep.SIGMC, "Wg_to_munu", "W#gamma#rightarrowe#nu#gamma", 634, config.GetSelectedName(config.FULLY,channel,vgamma,blind,config.SIGMC), "selectedEvents");

  if (vgamma==config.Z_GAMMA)
    prep.SetOneYieldSource(prep.SIGMC, "Zg", "Z#gamma", 879, config.GetSelectedName(config.FULLY,channel,vgamma,blind,config.SIGMC), "selectedEvents");
  
  prep.SetOneYieldSource(prep.BKGMC, "Wjets_to_lnu", "W+jets#rightarrowl#nu+jets", 433, config.GetSelectedName(config.FULLY,channel,vgamma,blind,config.BKGMC,"Wjets_to_lnu"), "selectedEvents");
  
  prep.SetOneYieldSource(prep.BKGMC, "DYjets_to_ll", "DY+jets#rightarrowl#bar{l}", 418, config.GetSelectedName(config.FULLY,channel,vgamma,blind,config.BKGMC,"DYjets_to_ll"), "selectedEvents");
  
  prep.SetOneYieldSource(prep.BKGMC, "Zg", "Z#gamma#rightarrowl#bar{l}", 879, config.GetSelectedName(config.FULLY,channel,vgamma,blind,config.BKGMC,"Zg"), "selectedEvents");

  prep.SetOneYieldSource(prep.BKGMC, "ttbarjets", "t#bar{t}+jets", 631, config.GetSelectedName(config.FULLY,channel,vgamma,blind,config.BKGMC,"ttbarjets") , "selectedEvents");

  prep.SetOneYieldSource(prep.BKGMC, "Wg_to_taunu", " W#gamma#rightarrow#tau#nu#gamma", 401, config.GetSelectedName(config.FULLY,channel,vgamma,blind,config.BKGMC,"Wg_to_taunu") , "selectedEvents");

  prep.SetOneYieldSource(prep.BKGMC, "WWg", " WW#gamma", 884, config.GetSelectedName(config.FULLY,channel,vgamma,blind,config.BKGMC,"Wg_to_taunu") , "selectedEvents");

  prep.PlotPrintSave();
  
}

//void AuxSubtractBackground(int channel, int vgamma, int blind, TString varKin, int nKinBins, float* kinBinLims, bool noDDBkgComputation=0)
//{
//}

