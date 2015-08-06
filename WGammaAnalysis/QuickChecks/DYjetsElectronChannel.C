#include "TCut.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"

#include <iostream>

void DYjets_WeightedYield(TTree* tr, TCut cut, float& val, float& err)
{
  TH1F* h = new TH1F("h","h",1,-3.0,3.0);
//  std::cout<<"tr->GetEntries()="<<tr->GetEntries(cut)<<std::endl;
  tr->Draw("phoSCEta>>h",cut,"goff");
  val = h->GetBinContent(1);
  err = h->GetBinError(1);
  delete h;
}// end of DYjets_WeightedYield

void DYjets_PrintDYtoW(TTree* trW, TTree* trZ, TCut cut)
{
  float valZ, valW, errZ, errW;
  DYjets_WeightedYield(trW, cut, valW, errW);
  DYjets_WeightedYield(trZ, cut, valZ, errZ);
  float frac, fracErr;
  if (valW<=0) {frac=0; fracErr=0;}
  else{
    frac=valZ/valW;
    fracErr=(1/(valW*valW))*sqrt(valZ*valZ*errW*errW+valW*valW*errZ*errZ);
  }
  std::cout<<"Wjets: "<<valW<<"+-"<<errW<<", Zjets: "<<valZ<<"+-"<<errZ<<", W/Z = "<<frac<<"+-"<<fracErr<<std::endl;
}// end of DYjets_WeightedYield

void DYjets_PrintPortions()
{
  TCut cutWMt = "WMt>70";
  TCut cutPhoEt = "phoEt>15 && phoEt<20";
  TCut cutB = "phoSCEta>-1.4442 && phoSCEta<1.4442";
  TCut cutE = "(phoSCEta>-2.5 && phoSCEta<-1.566) || (phoSCEta<2.5 && phoSCEta>1.566)";
  TCut cutWeight="weight";

  TFile* fEleZjets = new TFile("../WGammaOutput/ELECTRON_WGamma/FullySelected/selected_WGammaBKGMC_DYjets_to_ll.root");
  TFile* fEleWjets = new TFile("../WGammaOutput/ELECTRON_WGamma/FullySelected/selected_WGammaBKGMC_Wjets_to_lnu.root");
  TFile* fMuoZjets = new TFile("../WGammaOutput/MUON_WGamma/FullySelected/selected_WGammaBKGMC_DYjets_to_ll.root");
  TFile* fMuoWjets = new TFile("../WGammaOutput/MUON_WGamma/FullySelected/selected_WGammaBKGMC_Wjets_to_lnu.root");

  TTree* trEleZjets = (TTree*)fEleZjets->Get("selectedEvents");
  TTree* trMuoZjets = (TTree*)fMuoZjets->Get("selectedEvents");
  TTree* trEleWjets = (TTree*)fEleWjets->Get("selectedEvents");
  TTree* trMuoWjets = (TTree*)fMuoWjets->Get("selectedEvents");

  TCut cut = (cutWMt && cutPhoEt && cutB)*cutWeight;
  std::cout<<"Muon, barrel:"<<std::endl;
  DYjets_PrintDYtoW(trMuoWjets, trMuoZjets, cut);
  std::cout<<"Electron, barrel:"<<std::endl;
  DYjets_PrintDYtoW(trEleWjets, trEleZjets, cut);

  cut = (cutWMt && cutPhoEt && cutE)*cutWeight;
  std::cout<<"Muon, endcap:"<<std::endl;
  DYjets_PrintDYtoW(trMuoWjets, trMuoZjets, cut);
  std::cout<<"Electron, endcap:"<<std::endl;
  DYjets_PrintDYtoW(trEleWjets, trEleZjets, cut);

}// end of DYjets_PrintPortions()
