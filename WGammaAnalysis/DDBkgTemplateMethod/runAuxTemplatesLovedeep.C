{
  gROOT->ProcessLine(".L TTemplates.C+");
  gROOT->ProcessLine(".L TTemplatesSyst.C+");
  gROOT->ProcessLine(".L AuxTemplatesLovedeep.C+");

  TString varKin="pho_pt";
  const int nKinBins=5;
  float kinBinLims[nKinBins+1]={15,20,25,30,35,40};

  AuxTemplates(varKin, nKinBins, kinBinLims);
//  AuxTemplatesSystSidebandVariation(varKin, nKinBins, kinBinLims);
}

