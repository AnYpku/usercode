{
  gStyle->SetCanvasColor(kWhite); 
  gROOT->ProcessLine(".L OneVarInBinsOfTheOther.C+");
  TFile f_Zg("../WGammaOutput/MUON_ZGamma/PreliminaryForTemplateMethodSelected/selected_ZGammaSIGMC.root");
  TFile f_Zjets("../WGammaOutput/MUON_ZGamma/PreliminaryForTemplateMethodSelected/selected_ZGammaBKGMC_DYjets_to_ll.root");
  TFile f_Wg("../WGammaOutput/MUON_WGamma/PreliminaryForTemplateMethodSelected/selected_WGammaSIGMC.root");
  TFile f_Wjets("../WGammaOutput/MUON_WGamma/PreliminaryForTemplateMethodSelected/selected_WGammaBKGMC_Wjets_to_lnu.root");

  TTree* tr_Zg = (TTree*)f_Zg.Get("selectedEvents");
  TTree* tr_Zjets = (TTree*)f_Zjets.Get("selectedEvents");
  TTree* tr_Wg = (TTree*)f_Wg.Get("selectedEvents");
  TTree* tr_Wjets = (TTree*)f_Wjets.Get("selectedEvents");
  
  TCut cutB="phoSCEta<1.4442 && phoSCEta>-1.4442";
  TCut cutE="(phoSCEta > 1.566 && phoSCEta < 2.5)||(phoSCEta <-1.566 && phoSCEta >-2.5)";
  TCut cutWeight="weight";
  TCut cutPtFirstBin="phoEt>15 && phoEt<20";

  //PlotSihihInIsoBins(tr_Zg, tr_Wg, tr_Zjets, tr_Wjets, cutB, cutE, cutPtFirstBin, cutWeight);
  PlotIsoInSihihBins(tr_Zg, tr_Wg, tr_Zjets, tr_Wjets, cutB, cutE, cutPtFirstBin, cutWeight);
  PlotIsoAndSihihInPtBins(tr_Zg, tr_Wg, tr_Zjets, tr_Wjets, cutB, cutE, cutWeight);

/*
  // phoSCRChIso separately for barrel and endcap 
  var2 = "phoSCEta";
  int nBinsVar2_2 = 2;
  float binLimsVar2_2[nBinsVar2_2+1]={0,1.5,2.5};
  TCut cutPtFirstBin="phoEt>15 && phoEt<20";

  OneVarInBinsOfTheOther("Zg_MC_Iso_in_eta_bins", "pho SCR ch Iso", tr_Zg, var1, cutPtFirstBin, cutWeight, var2, nBinsVar2_2, binLimsVar2_2, nBinsHist, histMin, histMax);
  OneVarInBinsOfTheOther("Zjets_MC_Iso_in_eta_bins", "pho SCR ch Iso", tr_Zjets, var1, cutPtFirstBin, cutWeight, var2, nBinsVar2_2, binLimsVar2_2, nBinsHist, histMin, histMax);
  OneVarInBinsOfTheOther("Wg_MC_Iso_in_eta_bins", "pho SCR ch Iso", tr_Wg, var1, cutPtFirstBin, cutWeight, var2, nBinsVar2_2, binLimsVar2_2, nBinsHist, histMin, histMax);
  OneVarInBinsOfTheOther("Wjets_MC_Iso_in_eta_bins", "pho SCR ch Iso", tr_Wjets, var1, cutPtFirstBin, cutWeight, var2, nBinsVar2_2, binLimsVar2_2, nBinsHist, histMin, histMax);
*/

}
