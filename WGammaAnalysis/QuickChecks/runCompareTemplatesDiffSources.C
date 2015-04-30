{
  gROOT->ProcessLine(".L CompareTemplatesDiffSources.C+");
//  CompareFakeTemplatesDiffSources();
//  CompareTrueTemplatesDiffSources();
  CompareTrueTemplatesDiffSourcesWMtCutCheck();
}
