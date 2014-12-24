#include "../Configuration/TConfiguration.h"
#include "../Include/TPhotonCuts.h"

#include "TPrepareYields.h"
#include "TSubtractBackground.h"

void AuxPrepareYieldsCommon(TPrepareYields& prep, TPrepareYields::PrepareYieldsPars& pars, int channel, int vgamma, int blind, TString varKin, int nKinBins, float* kinBinLims, TCut cutAdd="1");

void AuxPrepareYields(int channel, int vgamma, int blind, TString varKin, int nKinBins, float* kinBinLims, TCut cutAdd="1")
{

  TPrepareYields prep;
  TPrepareYields::PrepareYieldsPars pars;

  AuxPrepareYieldsCommon(prep, pars, channel, vgamma, blind, varKin, nKinBins, kinBinLims, cutAdd);

  prep.PlotPrintSave();
  
}

void AuxSubtractBackground(int channel, int vgamma, int blind, TString varKin, int nKinBins, float* kinBinLims, TCut cutAdd="1")
{
  TSubtractBackground prep;
  TPrepareYields::PrepareYieldsPars pars;

  AuxPrepareYieldsCommon(prep, pars, channel, vgamma, blind, varKin, nKinBins, kinBinLims, cutAdd);

  TConfiguration config;
  TString fileName=config.GetDDTemplateFileName(channel,vgamma,varKin);
  TString strYields1DTrue[3], strYields1DFake[3], strYieldTotTrue[3], strYieldTotFake[3];
  for (int ieta=config.BARREL; ieta<=config.COMMON; ieta++){
   strYields1DFake[ieta]=config.GetYieldsDDTemplateFakeName(config.ONEDI,ieta);
   strYieldTotFake[ieta]=config.GetYieldsDDTemplateFakeName(config.TOTAL,ieta);
   strYields1DTrue[ieta]=config.GetYieldsDDTemplateTrueName(config.ONEDI,ieta);
   strYieldTotTrue[ieta]=config.GetYieldsDDTemplateTrueName(config.TOTAL,ieta);
  }
  prep.SetYieldsDataDrivenTrue("DDtrue", "DD true", 3, fileName, strYields1DTrue, strYieldTotTrue);
  prep.SetYieldsDataDrivenFake("DDfake", "DD fake", 4, fileName, strYields1DFake, strYieldTotFake);
  prep.SubtractBackground();

  prep.PlotPrintSave();
}

void AuxPrepareYieldsCommon(TPrepareYields& prep, TPrepareYields::PrepareYieldsPars& pars, int channel, int vgamma, int blind, TString varKin, int nKinBins, float* kinBinLims, TCut cutAdd)
{
  TConfiguration config;
  TPhotonCuts photon;

  pars.varKin=varKin;
  pars.nKinBins=nKinBins;
  for (int il=0; il<nKinBins+1; il++)
    pars.kinBinLims[il]=kinBinLims[il];
  if (blind==config.UNBLIND) pars.blindFraction=1;
  if (blind==config.BLIND_PRESCALE) pars.blindFraction=1./config.GetBlindPrescale();
  pars.varWeight="weight";
  pars.cutBarrel=photon.RangeBarrel();
  pars.cutEndcap=photon.RangeEndcap();
  pars.cutAdd=cutAdd;
  pars.doLogX=0;  pars.doLogY=0;
  if (varKin=="phoEt" && kinBinLims[nKinBins]>200){
    pars.doLogX=1;  pars.doLogY=1;
  }
  pars.varKinLabel=varKin;
  if (varKin=="phoEt") pars.varKinLabel="Pt_#gamma";
  pars.strFileOut="fOut.root";

  prep.SetPars(pars);

  //bool SetOneYieldSource(int sourceType, TString name, TString label, int color, TString fileName, TString treeName);
  
  //int selStage=config.FSR;//config.FULLY;
  int selStage=config.FULLY;
  // data
  prep.SetOneYieldSource(prep.DATA, "data", "data", 1, config.GetSelectedName(selStage,channel,vgamma,blind,config.DATA), "selectedEvents");

  // signal MC
  if (vgamma==config.W_GAMMA && channel==config.MUON)
    prep.SetOneYieldSource(prep.SIGMC, "Wg_to_munu", "W#gamma#rightarrow#mu#nu#gamma", 634, config.GetSelectedName(selStage,channel,vgamma,blind,config.SIGMC), "selectedEvents");

  if (vgamma==config.W_GAMMA && channel==config.ELECTRON)
    prep.SetOneYieldSource(prep.SIGMC, "Wg_to_enu", "W#gamma#rightarrowe#nu#gamma", 634, config.GetSelectedName(selStage,channel,vgamma,blind,config.SIGMC), "selectedEvents");

  if (vgamma==config.Z_GAMMA)
    prep.SetOneYieldSource(prep.SIGMC, "Zg", "Z#gamma", 879, config.GetSelectedName(selStage,channel,vgamma,blind,config.SIGMC), "selectedEvents");

  // bkg MC  
  prep.SetOneYieldSource(prep.BKGMC_TRUE, "WWg", " WW#gamma", 884, config.GetSelectedName(selStage,channel,vgamma,blind,config.BKGMC,"WWg") , "selectedEvents");

  prep.SetOneYieldSource(prep.BKGMC_TRUE, "Wg_to_taunu", " W#gamma#rightarrow#tau#nu#gamma", 401, config.GetSelectedName(selStage,channel,vgamma,blind,config.BKGMC,"Wg_to_taunu") , "selectedEvents");

  prep.SetOneYieldSource(prep.BKGMC_TRUE, "Zg", "Z#gamma#rightarrowl#bar{l}", 879, config.GetSelectedName(selStage,channel,vgamma,blind,config.BKGMC,"Zg"), "selectedEvents");

  prep.SetOneYieldSource(prep.BKGMC_FAKE, "DYjets_to_ll", "DY+jets#rightarrowl#bar{l}", 418, config.GetSelectedName(selStage,channel,vgamma,blind,config.BKGMC,"DYjets_to_ll"), "selectedEvents");

  prep.SetOneYieldSource(prep.BKGMC_FAKE, "Wjets_to_lnu", "W+jets#rightarrowl#nu+jets", 433, config.GetSelectedName(selStage,channel,vgamma,blind,config.BKGMC,"Wjets_to_lnu"), "selectedEvents");
  
  prep.SetOneYieldSource(prep.BKGMC_FAKE, "ttbarjets", "t#bar{t}+jets", 631, config.GetSelectedName(selStage,channel,vgamma,blind,config.BKGMC,"ttbarjets") , "selectedEvents");


}// end of AuxPrepareYieldsCommon
