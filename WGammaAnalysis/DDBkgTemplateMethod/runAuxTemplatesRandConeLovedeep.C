{
  gROOT->ProcessLine(".L TTemplatesRandCone.C+");
  gROOT->ProcessLine(".L TTemplatesRandConeSyst.C+");
  gROOT->ProcessLine(".L AuxTemplatesRandConeLovedeep.C+");

  TString varKin="pho_pt";
  const int nKinBins=5;
  float kinBinLims[nKinBins+1]={15,20,25,30,35,40};

  AuxTemplatesRandCone(varKin, nKinBins, kinBinLims);
//  AuxTemplatesRandConeSystSidebandVariation(varKin, nKinBins, kinBinLims);
}

