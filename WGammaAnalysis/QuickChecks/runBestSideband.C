{

  gROOT->ProcessLine(".L BestSideband.C+");

  TFile fSign("../WGammaOutput/MUON/PreliminaryForTemplateMethodSelected/selected_ZGammaSIGMC.root");
  TFile fData("../WGammaOutput/MUON/PreliminaryForTemplateMethodSelected/selected_ZGamma_UNblind_DATA.root");

  gStyle->SetCanvasColor(kWhite); 
  gStyle->SetPalette(1);
  TTree* trSign = (TTree*)fSign.Get("selectedEvents");
  TTree* trData = (TTree*)fData.Get("selectedEvents");

  BestSideband(trData,trSign);

}
