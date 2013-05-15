{

  gROOT->ProcessLine(".L TConfiguration.C+");
  gROOT->ProcessLine(".L TInputSample.C+");
  gROOT->ProcessLine(".L TAllInputSamples.C+");
  
  TAllInputSamples INPUT(TInputSample::MUON,"config.txt");
  INPUT.Print();

}
