{
  gROOT->ProcessLine(".x ../Include/rootlogon.C");
  gROOT->ProcessLine(".L ComputeNumberOfCandidatesPerEvent.C+");
  TConfiguration config;
  TString fInName;

  fInName = config.GetSelectedName(config.VERY_PRELIMINARY,config.MUON,config.BLIND_PRESCALE,config.SIGMC);
  ComputeNumberOfCandidatesPerEvent(fInName);

  fInName = config.GetSelectedName(config.PRELIMINARY_FOR_TEMPLATE_METHOD,config.MUON,config.BLIND_PRESCALE,config.SIGMC);
  ComputeNumberOfCandidatesPerEvent(fInName);

  fInName = config.GetSelectedName(config.FULLY,config.MUON,config.BLIND_PRESCALE,config.SIGMC);
  ComputeNumberOfCandidatesPerEvent(fInName);

  fInName = config.GetSelectedName(config.VERY_PRELIMINARY,config.MUON,config.BLIND_PRESCALE,config.BKGMC,"Zg");
  ComputeNumberOfCandidatesPerEvent(fInName);

  fInName = config.GetSelectedName(config.PRELIMINARY_FOR_TEMPLATE_METHOD,config.MUON,config.BLIND_PRESCALE,config.BKGMC,"Zg");
  ComputeNumberOfCandidatesPerEvent(fInName);

  fInName = config.GetSelectedName(config.FULLY,config.MUON,config.BLIND_PRESCALE,config.BKGMC,"Zg");
  ComputeNumberOfCandidatesPerEvent(fInName);
}
