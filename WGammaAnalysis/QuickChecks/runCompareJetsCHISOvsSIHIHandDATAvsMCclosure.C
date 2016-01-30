{
  gROOT->ProcessLine(".L CompareJetsCHISOvsSIHIHandDATAvsMCclosure.C+");
  std::cout<<"I will compare jets"<<std::endl;
  CompareJets();
}
