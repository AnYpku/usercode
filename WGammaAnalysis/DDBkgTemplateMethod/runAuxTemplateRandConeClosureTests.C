{
  gROOT->ProcessLine(".x ../Include/rootlogon.C");
  gROOT->ProcessLine(".L AuxTemplatesRandConeClosureTests.C+");

  TString varKin="phoEt";
  const int nKinBins=5;
  float kinBinLims[nKinBins+1]={15,20,25,30,35,40};
  int channel = TConfiguration::MUON;
  int vgamma = TConfiguration::Z_GAMMA;
  int blind = TConfiguration::UNBLIND;
  int phoWP=TPhotonCuts::WP_MEDIUM;

  AuxTemplatesRandConeClosure(channel, vgamma, blind, phoWP, varKin, nKinBins, kinBinLims);
//  AuxTemplatesRandConeClosureSystSidebandVariation(varKin, nKinBins, kinBinLims);
}

