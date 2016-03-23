{
  gSystem->Load("../RooUnfold/libRooUnfold.so");

  //gSystem->Load("/cvmfs/cms.cern.ch/slc6_amd64_gcc491/external/fftw3/3.3.2-cms/lib/libfftw3.so");

  gROOT->ProcessLine(".L ../Configuration/TConfiguration.C+");

  gROOT->ProcessLine(".L ../Configuration/TInputSample.C+");
  gROOT->ProcessLine(".L ../Configuration/TAllInputSamples.C+");

  gROOT->ProcessLine(".L ../PHOSPHOR_CORRECTION/PhosphorCorrectorFunctor.cc+");

  gROOT->ProcessLine(".L ../Include/TScaleFactors.C+");
  gROOT->ProcessLine(".L ../Include/TMathTools.C+");
  gROOT->ProcessLine(".L ../Include/TEventTree.C+");
  gROOT->ProcessLine(".L ../Include/TPhotonCuts.C+");
  gROOT->ProcessLine(".L ../Include/TPhotonScaleFactor.C+");
  gROOT->ProcessLine(".L ../Include/TElectronCuts.C+");
  gROOT->ProcessLine(".L ../Include/TMuonCuts.C+");
  gROOT->ProcessLine(".L ../Include/TMetTools.C+");
  gROOT->ProcessLine(".L ../Include/TPuReweight.C+");
  gROOT->ProcessLine(".L ../Include/TFullCuts.C+");

//  gROOT->ProcessLine(".L ../SearchForOverlapInMC/SearchForOverlap.C+");

  gROOT->ProcessLine(".L ../Selection/TSelectedEventsTree.C+");

  gROOT->ProcessLine(".L ../Selection/TSelectionPlots.C+");
  gROOT->ProcessLine(".L ../Selection/Selection.C+");

  gROOT->ProcessLine(".L ../DDBkgTemplateMethod/TTemplates.C+");
  gROOT->ProcessLine(".L ../DDBkgTemplateMethod/TTemplatesSystRandomizeTempl.C+");
  gROOT->ProcessLine(".L ../DDBkgTemplateMethod/TTemplatesSystZgNorm.C+");
//  gROOT->ProcessLine(".L ../DDBkgTemplateMethod/TTemplatesSyst.C+");
  gROOT->ProcessLine(".L ../DDBkgTemplateMethod/AuxTemplates.C+");
//  gROOT->ProcessLine(".L ../DDBkgTemplateMethod/AuxTemplatesPickSideband.C+");
//  gROOT->ProcessLine(".L ../DDBkgTemplateMethod/TComputeSystDueToSbVariation.C+");
  gROOT->ProcessLine(".L ../DDBkgEtoGamma/TEtoGamma.C+");

  gROOT->ProcessLine(".L ../PrepareYields/TPrepareYields.C+");
  gROOT->ProcessLine(".L ../PrepareYields/TSubtractBackground.C+");
  gROOT->ProcessLine(".L ../PrepareYields/AuxPrepareYields.C+");

  gROOT->ProcessLine(".L ../Unfolding/Unfolding.C+");

  gROOT->ProcessLine(".L ../AcceptanceAndEfficiency/CalcAccAndEff.C+");

  gROOT->ProcessLine(".L ../CrossSection/CalcCrossSection.C+");
  gROOT->ProcessLine(".L ../CrossSection/AuxCrossSection.C+");

  gROOT->ProcessLine(".L ../FullChain/FullChain.C+");
//  gROOT->ProcessLine(".L ../FullChain/AuxFchPreliminarySelection.C+");
//  gROOT->ProcessLine(".L ../FullChain/AuxFchExtraSelection.C+");
//  gROOT->ProcessLine(".L ../FullChain/AuxFchPrepareYields.C+");
//  gROOT->ProcessLine(".L ../FullChain/AuxFchMCclosureSidebandVariation.C+");
//  gROOT->ProcessLine(".L ../FullChain/AuxFchTemplatesPickSideband.C+");
//  gROOT->ProcessLine(".L ../FullChain/AuxFchMCclosureFits.C+");
//  gROOT->ProcessLine(".L ../FullChain/AuxFchMCclosureCompareYields.C+");
//  gROOT->ProcessLine(".L ../FullChain/AuxFchTemplateFitsData.C+");
//  gROOT->ProcessLine(".L ../FullChain/AuxFchSubtractBackgroundData.C+");

  gROOT->ProcessLine(".L ../ChangeVeryPreliminarySelected/ChangePUweight.C+");
  gROOT->ProcessLine(".L ../ChangeVeryPreliminarySelected/ApplySF.C+");


  gROOT->ProcessLine(".L ../SkimSplitMerge/TInputOutputTree.C+");
  gROOT->ProcessLine(".L ../SkimSplitMerge/SkimLeptonPhoton.C+");
  gROOT->ProcessLine(".L ../SkimSplitMerge/SplitVDecayMC.C+");
  gROOT->ProcessLine(".L ../SkimSplitMerge/SkimNEvents.C+");

  gStyle->SetPalette(1);
  gStyle->SetPaintTextFormat("0.2f");
  gStyle->SetCanvasColor(kWhite); 

}
