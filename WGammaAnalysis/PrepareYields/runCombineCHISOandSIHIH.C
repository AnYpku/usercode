{
  gROOT->ProcessLine(".x ../Include/rootlogon.C");
  gROOT->ProcessLine(".L CombineCHISOandSIHIH.C+");
  TConfiguration conf;
  int channel=conf.ELECTRON;
  int vgamma=conf.W_GAMMA;
  CombineCHISOandSIHIH(channel, vgamma);
}
