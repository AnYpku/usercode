{

  gROOT->ProcessLine(".x ../Include/rootlogon.C");
  
  //TAllInputSamples INPUT(TConfiguration::MUON,"config.txt");
  //INPUT.Print();

  TConfiguration config;
  config.Print();

}
