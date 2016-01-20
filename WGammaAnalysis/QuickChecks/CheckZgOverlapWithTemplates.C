#include "TFile.h"
#include "TTree.h"
#include "TCut.h"

#include <iostream>

void CheckZgOverlapWithTemplates()
{
  TFile* fZgMuo_Data = new TFile("../WGammaOutput/MUON_ZGamma/PreliminaryForTemplateMethodSelected/selected_ZGamma_UNblind_DATA.root");
  TFile* fZgMuo_FSR = new TFile("../WGammaOutput/MUON_ZGamma/FsrSelected/selected_ZGamma_UNblind_DATA.root");
  TFile* fZgMuo_ISR = new TFile("../WGammaOutput/MUON_ZGamma/FsrExcludedSelected/selected_ZGamma_UNblind_DATA.root");
  TFile* fZgMerged_FSR = new TFile("../WGammaOutput/ChannelsMERGED_ZGamma/FsrSelected/selected_ZGamma_UNblind_DATA.root");
  TFile* fZgMerged_ISR = new TFile("../WGammaOutput/ChannelsMERGED_ZGamma/FsrExcludedSelected/selected_ZGamma_UNblind_DATA.root");

  TTree* trZgMuo_Data = (TTree*)fZgMuo_Data->Get("selectedEvents");
  TTree* trZgMuo_FSR = (TTree*)fZgMuo_FSR->Get("selectedEvents");
  TTree* trZgMuo_ISR = (TTree*)fZgMuo_ISR->Get("selectedEvents");
  TTree* trZgMerged_FSR = (TTree*)fZgMerged_FSR->Get("selectedEvents");
  TTree* trZgMerged_ISR = (TTree*)fZgMerged_ISR->Get("selectedEvents");

  TCut cutPt="phoEt>15 && phoEt<20";
  TCut cutB="phoSCEta>-1.4442 && phoSCEta<1.4442";
  TCut cutSihihB="phoSigmaIEtaIEta<0.011";

  std::cout<<"N events, data to fit: "<<trZgMuo_Data->GetEntries(cutPt && cutB && cutSihihB)<<std::endl;
  std::cout<<"N events, FSR in data: "<<trZgMuo_FSR->GetEntries(cutPt && cutB && cutSihihB)<<std::endl;
  std::cout<<"N events, ISR in data: "<<trZgMuo_ISR->GetEntries(cutPt && cutB && cutSihihB)<<std::endl;
  std::cout<<"N events, FSR in template: "<<trZgMerged_FSR->GetEntries(cutB && cutSihihB)<<std::endl;
  std::cout<<"N events, ISR in template: "<<trZgMerged_ISR->GetEntries(cutPt && cutB && cutSihihB)<<std::endl;

}// end of CheckZgOverlapWithTemplates 
