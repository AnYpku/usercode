{
  gStyle->SetCanvasColor(kWhite); 
  gROOT->ProcessLine(".L SignalTemplateStudies.C+");
  DrawIsolationTemplates();
  DrawSihihTemplates();

}
