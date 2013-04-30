{
  gROOT->ProcessLine(".L TInputSample.C+");
  gROOT->ProcessLine(".L TConfiguration.C+");
  
  TConfiguration config;
  TInputSample input;

  input = config.GetInputSample(config.MUON, config.DATA);
  input.Print();


  input = config.GetInputSample(config.MUON, config.SIGMC);
  input.Print();


  for (int i=0; i<config.GetNSources(config.BKGMC); i++)
    {
      input = config.GetInputSample(config.MUON, config.BKGMC, i);
      input.Print(); 
    }

}
