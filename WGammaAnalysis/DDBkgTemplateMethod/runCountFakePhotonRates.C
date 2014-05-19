{
  gROOT->ProcessLine(".x ../Include/rootlogon.C");
  gROOT->ProcessLine(".L CountFakePhotonRates.C+");
  CountPhotonSources();
  //CountFakePhotonRates();
//  GetList(TConfiguration::BKGMC,"Zg", 444);//gamma from Z
//  GetList(TConfiguration::SIGMC,"Wg_to_munu", 2236);//gamma from gluon
//  GetList(TConfiguration::SIGMC,"Wg_to_munu", 4682928);//FSR
}
