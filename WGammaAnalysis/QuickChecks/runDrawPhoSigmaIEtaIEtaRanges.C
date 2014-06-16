{
  gROOT->ProcessLine(".L DrawPhoSigmaIEtaIEtaRanges.C+");
  gStyle->SetCanvasColor(kWhite); 
  DrawDataVsSignalMC();
//  DrawRanges();
}
