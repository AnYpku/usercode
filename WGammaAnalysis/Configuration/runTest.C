{

  gROOT->ProcessLine(".x ../Include/rootlogon.C");
  
//  TAllInputSamples INPUT(TConfiguration::MUON,TConfiguration::Z_GAMMA,"config.txt");
//  INPUT.Print();

  TConfiguration config;
  config.Print();

}
