{
  gROOT->ProcessLine(".L ../Configuration/TConfiguration.C+");
  gROOT->ProcessLine(".L ../Configuration/TInputSample.C+");
  gROOT->ProcessLine(".L ../Configuration/TAllInputSamples.C+");

  gROOT->ProcessLine(".L ../Include/PhosphorCorrectorFunctor.cc+");
  gROOT->ProcessLine(".L ../Include/TEventTree.C+");
  gROOT->ProcessLine(".L ../Include/TPhotonCuts.C+");
  gROOT->ProcessLine(".L ../Include/TElectronCuts.C+");
  gROOT->ProcessLine(".L ../Include/TMuonCuts.C+");
  gROOT->ProcessLine(".L ../Include/TMetTools.C+");
  gROOT->ProcessLine(".L ../Include/TFullCuts.C+");

  gROOT->ProcessLine(".L ../AcceptanceAndEfficiency/TAcceptanceAndEfficiency.C+");

  gROOT->ProcessLine(".L ../Selection/TSelectedEventsTree.C+");
  gROOT->ProcessLine(".L ../Selection/TSelectionPlots.C+");
  gROOT->ProcessLine(".L ../Selection/WGammaSelection.C+");

  gROOT->ProcessLine(".L ../SkimSplitMerge/TInputOutputTree.C+");
  gROOT->ProcessLine(".L ../SkimSplitMerge/SkimLeptonPhoton.C+");
  gROOT->ProcessLine(".L ../SkimSplitMerge/SplitWGammaMC.C+");

}