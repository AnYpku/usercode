#include "TString.h"
#include "TCut.h"
#include "TFile.h"
#include "TTree.h"
#include "../Include/TFullCuts.h"
#include <iostream>

void SanityCheckTwo(TString strFile1, TString strFile2, TCut cut1, TCut cut2)
{
  TFile f1(strFile1);
  TFile f2(strFile2);
  TTree* tr1 = (TTree*)f1.Get("selectedEvents");
  TTree* tr2 = (TTree*)f2.Get("selectedEvents");
  int n1 = tr1->GetEntries(cut1);
  int n2 = tr2->GetEntries(cut2);
  std::cout<<n1<<" vs "<<n2<<std::endl;

}

void SanityCheckForSelection()
{
  TFullCuts fullCut;
  int channel = 0;//MUON
  int wp=2;//WP_TIGHT
  TCut cutVeryPremToDDTemplate=fullCut.RangeForTemplateMethodCut(channel,wp, "SCR");
  TCut cutVeryPremToMEt=fullCut.RangeForMetCut(channel,wp, "SCR");
  TCut cutVeryPremToFully=fullCut.RangeFullCut(channel,wp,"SCR");

  TCut cutDDTemplateToFully=fullCut.RangeFullCut(channel,wp,"SCR");
  TCut cutMEtToFully=fullCut.RangeFullCut(channel,wp,"SCR");

  TCut cutB="phoSCEta>-1.442 && phoSCEta<1.442";
  TCut cutE="(phoSCEta>-2.5 && phoSCEta<-1.566) || (phoSCEta>1.566 && phoSCEta<2.5)";
  TCut cutChIsoB="phoSCRChIsoCorr<0.7";
  TCut cutChIsoE="phoSCRChIsoCorr<0.5";
  TCut cutSigmaB="phoSigmaIEtaIEta<1.1";
  TCut cutSigmaE="phoSigmaIEtaIEta<3.1";
  TCut cutWMt="WMt>50";
  cutDDTemplateToFully=(cutB && cutChIsoB && cutSigmaB)||(cutE && cutChIsoE && cutSigmaE);
  cutMEtToFully=cutDDTemplateToFully && cutWMt;

  std::cout<<"DATA, prescale blinding:"<<std::endl;
  SanityCheckTwo("../WGammaOutput/MUON/VeryPreliminarySelected/selected_blindPRESCALE_DATA.root",
                 "../WGammaOutput/MUON/PreliminaryForTemplateMethodSelected/selected_blindPRESCALE_DATA.root",
                 cutVeryPremToDDTemplate,"1");
  SanityCheckTwo("../WGammaOutput/MUON/VeryPreliminarySelected/selected_blindPRESCALE_DATA.root",
                 "../WGammaOutput/MUON/PreliminaryForMEtCutSelected/selected_blindPRESCALE_DATA.root",
                 cutVeryPremToMEt,"1");
  SanityCheckTwo("../WGammaOutput/MUON/VeryPreliminarySelected/selected_blindPRESCALE_DATA.root",
                 "../WGammaOutput/MUON/FullySelected/selected_blindPRESCALE_DATA.root",
                 cutVeryPremToFully,"1");
  SanityCheckTwo("../WGammaOutput/MUON/PreliminaryForTemplateMethodSelected/selected_blindPRESCALE_DATA.root",
                 "../WGammaOutput/MUON/FullySelected/selected_blindPRESCALE_DATA.root",
                 cutVeryPremToFully,"1");
  SanityCheckTwo("../WGammaOutput/MUON/PreliminaryForMEtCutSelected/selected_blindPRESCALE_DATA.root",
                 "../WGammaOutput/MUON/FullySelected/selected_blindPRESCALE_DATA.root",
                 cutVeryPremToFully,"1");

  std::cout<<"signal MC:"<<std::endl;
  SanityCheckTwo("../WGammaOutput/MUON/VeryPreliminarySelected/selectedSIGMC.root",
                 "../WGammaOutput/MUON/PreliminaryForTemplateMethodSelected/selectedSIGMC.root",
                 cutVeryPremToDDTemplate,"1");
  SanityCheckTwo("../WGammaOutput/MUON/VeryPreliminarySelected/selectedSIGMC.root",
                 "../WGammaOutput/MUON/PreliminaryForMEtCutSelected/selectedSIGMC.root",
                 cutVeryPremToMEt,"1");
  SanityCheckTwo("../WGammaOutput/MUON/VeryPreliminarySelected/selectedSIGMC.root",
                 "../WGammaOutput/MUON/FullySelected/selectedSIGMC.root",
                 cutVeryPremToFully,"1");
  SanityCheckTwo("../WGammaOutput/MUON/PreliminaryForTemplateMethodSelected/selectedSIGMC.root",
                 "../WGammaOutput/MUON/FullySelected/selectedSIGMC.root",
                 cutVeryPremToFully,"1");
  SanityCheckTwo("../WGammaOutput/MUON/PreliminaryForMEtCutSelected/selectedSIGMC.root",
                 "../WGammaOutput/MUON/FullySelected/selectedSIGMC.root",
                 cutVeryPremToFully,"1");


}




