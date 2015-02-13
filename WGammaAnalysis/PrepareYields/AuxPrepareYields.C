#include "../Configuration/TConfiguration.h"
#include "../Include/TPhotonCuts.h"

#include "TPrepareYields.h"
#include "TSubtractBackground.h"

void AuxPrepareYieldsCommon(TPrepareYields& prep, TPrepareYields::PrepareYieldsPars& pars, TConfiguration::AnalysisParameters &anPars);

void AuxPrepareYields(TConfiguration::AnalysisParameters &anPars)
{
  TConfiguration conf;
  TPrepareYields prep;
  TPrepareYields::PrepareYieldsPars pars;

  pars.strFileOut=conf.GetYieldsMCtruthFileName(anPars.channel, anPars.vgamma, anPars.varKin);

  AuxPrepareYieldsCommon(prep, pars, anPars);

  prep.PlotPrintSave();
  
}

void AuxSubtractBackgroundOneTempl(TSubtractBackground &prep, TConfiguration::AnalysisParameters &anPars, int templFits)
{
  TConfiguration config;
  TString fileName=config.GetDDTemplateFileName(anPars.channel,anPars.vgamma,templFits,anPars.varKin);
  TString strYields1DTrue[3], strYields1DFake[3], strYieldTotTrue[3], strYieldTotFake[3];
  for (int ieta=config.BARREL; ieta<=config.COMMON; ieta++){
   strYields1DFake[ieta]=config.GetYieldsDDTemplateFakeName(config.ONEDI,ieta);
   strYieldTotFake[ieta]=config.GetYieldsDDTemplateFakeName(config.TOTAL,ieta);
   strYields1DTrue[ieta]=config.GetYieldsDDTemplateTrueName(config.ONEDI,ieta);
   strYieldTotTrue[ieta]=config.GetYieldsDDTemplateTrueName(config.TOTAL,ieta);
  }
  TString name = TString("DD_true")+config.StrTempl(templFits);
  name.ReplaceAll("TEMPL_","_");
  TString label=name;
  label.ReplaceAll("_"," ");
  label.ReplaceAll(" SIHIH",", #sigma_{i#etai#eta} fits");
  label.ReplaceAll(" CHISO",", I_{ch} fits");
  prep.SetYieldsDataDrivenTrue(name, label, 3, fileName, strYields1DTrue, strYieldTotTrue);
  name.ReplaceAll("true","fake");
  label.ReplaceAll("true","fake");
  prep.SetYieldsDataDrivenFake(name, label, 4, fileName, strYields1DFake, strYieldTotFake);
  prep.Increase_nDDsources();
}// end of AuxSubtractBackgroundOneTempl

void AuxSubtractBackground(TConfiguration::AnalysisParameters &anPars)
{
  TConfiguration conf;
  TSubtractBackground prep;
  TPrepareYields::PrepareYieldsPars pars;

  pars.strFileOut=conf.GetYieldsFileName(anPars.channel, anPars.vgamma, anPars.templFits, anPars.varKin);

  AuxPrepareYieldsCommon(prep, pars, anPars);

  TConfiguration config;

  if (anPars.templFits==config.TEMPL_OVERLAY){
    AuxSubtractBackgroundOneTempl(prep, anPars, config.TEMPL_SIHIH);
    AuxSubtractBackgroundOneTempl(prep, anPars, config.TEMPL_CHISO);
  }
  else 
    AuxSubtractBackgroundOneTempl(prep, anPars, anPars.templFits);

  prep.SubtractBackground();
  prep.PlotPrintSave();
}

void AuxPrepareYieldsCommon(TPrepareYields& prep, TPrepareYields::PrepareYieldsPars& pars, TConfiguration::AnalysisParameters &anPars)
{
  TConfiguration config;
  TPhotonCuts photon;

  pars.varKin=anPars.varKin;
  pars.nKinBins=anPars.nKinBins;
  pars.templFits=anPars.templFits;
  for (int il=0; il<anPars.nKinBins+1; il++)
    pars.kinBinLims[il]=anPars.kinBinLims[il];
  if (anPars.blind[anPars.channel][anPars.vgamma]==config.UNBLIND) pars.blindFraction=1;
  if (anPars.blind[anPars.channel][anPars.vgamma]==config.BLIND_PRESCALE) pars.blindFraction=1./config.GetBlindPrescale();
  pars.varWeight="weight";
  pars.cutBarrel=photon.RangeBarrel();
  pars.cutEndcap=photon.RangeEndcap();
  pars.cutAdd=anPars.cutAdd;
  pars.doLogX=0;  pars.doLogY=0;
  if (anPars.varKin=="phoEt" && anPars.kinBinLims[anPars.nKinBins]>200){
    pars.doLogX=1;  pars.doLogY=1;
  }
  pars.varKinLabel=anPars.varKin;
  if (anPars.varKin=="phoEt") pars.varKinLabel="Pt_#gamma";

  pars.strPlotsDir=config.GetPlotsDirName(anPars.channel, anPars.vgamma, config.PLOTS_PREPARE_YIELDS);
  pars.strPlotsBaseName=TString("c_")+config.StrTempl(anPars.templFits)+TString("_")+config.StrBlindType(anPars.blind[anPars.channel][anPars.vgamma])+TString("_");

  prep.SetPars(pars);

  //bool SetOneYieldSource(int sourceType, TString name, TString label, int color, TString fileName, TString treeName);
  
  //int selStage=config.FSR;//config.FULLY;
  int selStage=config.FULLY;
  // data
  prep.SetOneYieldSource(prep.DATA, "data", "data", 1, config.GetSelectedName(selStage,anPars.channel,anPars.vgamma,anPars.blind[anPars.channel][anPars.vgamma],config.DATA), "selectedEvents");

  // signal MC
  if (anPars.vgamma==config.W_GAMMA && anPars.channel==config.MUON)
    prep.SetOneYieldSource(prep.SIGMC, "Wg_to_munu", "W#gamma#rightarrow#mu#nu#gamma", 634, config.GetSelectedName(selStage,anPars.channel,anPars.vgamma,anPars.blind[anPars.channel][anPars.vgamma],config.SIGMC), "selectedEvents");

  if (anPars.vgamma==config.W_GAMMA && anPars.channel==config.ELECTRON)
    prep.SetOneYieldSource(prep.SIGMC, "Wg_to_enu", "W#gamma#rightarrowe#nu#gamma", 634, config.GetSelectedName(selStage,anPars.channel,anPars.vgamma,anPars.blind[anPars.channel][anPars.vgamma],config.SIGMC), "selectedEvents");

  if (anPars.vgamma==config.Z_GAMMA)
    prep.SetOneYieldSource(prep.SIGMC, "Zg", "Z#gamma", 879, config.GetSelectedName(selStage,anPars.channel,anPars.vgamma,anPars.blind[anPars.channel][anPars.vgamma],config.SIGMC), "selectedEvents");

  // bkg MC  
  prep.SetOneYieldSource(prep.BKGMC_TRUE, "WWg", " WW#gamma", 884, config.GetSelectedName(selStage,anPars.channel,anPars.vgamma,anPars.blind[anPars.channel][anPars.vgamma],config.BKGMC,"WWg") , "selectedEvents");

  prep.SetOneYieldSource(prep.BKGMC_TRUE, "Wg_to_taunu", " W#gamma#rightarrow#tau#nu#gamma", 401, config.GetSelectedName(selStage,anPars.channel,anPars.vgamma,anPars.blind[anPars.channel][anPars.vgamma],config.BKGMC,"Wg_to_taunu") , "selectedEvents");

  prep.SetOneYieldSource(prep.BKGMC_TRUE, "Zg", "Z#gamma#rightarrowl#bar{l}", 879, config.GetSelectedName(selStage,anPars.channel,anPars.vgamma,anPars.blind[anPars.channel][anPars.vgamma],config.BKGMC,"Zg"), "selectedEvents");

  prep.SetOneYieldSource(prep.BKGMC_FAKE, "DYjets_to_ll", "DY+jets#rightarrowl#bar{l}", 418, config.GetSelectedName(selStage,anPars.channel,anPars.vgamma,anPars.blind[anPars.channel][anPars.vgamma],config.BKGMC,"DYjets_to_ll"), "selectedEvents");

  prep.SetOneYieldSource(prep.BKGMC_FAKE, "Wjets_to_lnu", "W+jets#rightarrowl#nu+jets", 433, config.GetSelectedName(selStage,anPars.channel,anPars.vgamma,anPars.blind[anPars.channel][anPars.vgamma],config.BKGMC,"Wjets_to_lnu"), "selectedEvents");
  
  prep.SetOneYieldSource(prep.BKGMC_FAKE, "ttbarjets", "t#bar{t}+jets", 631, config.GetSelectedName(selStage,anPars.channel,anPars.vgamma,anPars.blind[anPars.channel][anPars.vgamma],config.BKGMC,"ttbarjets") , "selectedEvents");

}// end of AuxPrepareYieldsCommon
