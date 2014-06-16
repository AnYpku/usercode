#include "TFile.h"
#include "TTree.h"
#include "TCut.h"
#include "TLine.h"
#include "TH1F.h"
#include "TCanvas.h"

void DrawDataVsSignalMC()
{
  TFile fData("../WGammaOutput/MUON/PreliminaryForTemplateMethodSelected/selected_blindPRESCALE_DATA.root");
  TTree* trData = (TTree*)fData.Get("selectedEvents");
  TFile fSign("../WGammaOutput/MUON/PreliminaryForTemplateMethodSelected/selectedSIGMC.root");
  TTree* trSign = (TTree*)fSign.Get("selectedEvents");
  TCut cutSigmaB="phoSigmaIEtaIEta>0.005 && phoSigmaIEtaIEta<0.022";
  TCut cutSigmaE="phoSigmaIEtaIEta>0.019 && phoSigmaIEtaIEta<0.067";
  TCut cutB="phoEta>-1.4442 && phoEta<1.4442";
  TCut cutE="(phoEta>-2.5 && phoEta<-1.566)||(phoEta>1.566 && phoEta<2.5)";
  TCut cutWeight="weight";
  TLine* lineSidebandB = new TLine(0.012,0,0.012,0.3);
  TLine* lineSidebandE = new TLine(0.034,0,0.034,0.14);
  TH1F* hDataB = new TH1F("hDataB","",34,0.005,0.022);
  TH1F* hSignB = new TH1F("hSignB","",34,0.005,0.022);
  TH1F* hDataE = new TH1F("hDataE","",48,0.019,0.067);
  TH1F* hSignE = new TH1F("hSignE","",48,0.019,0.067);
  trData->Draw("phoSigmaIEtaIEta>>hDataB",(cutB && cutSigmaB)*cutWeight,"goff");
  trData->Draw("phoSigmaIEtaIEta>>hDataE",(cutE && cutSigmaE)*cutWeight,"goff");
  trSign->Draw("phoSigmaIEtaIEta>>hSignB",(cutB && cutSigmaB)*cutWeight,"goff");
  trSign->Draw("phoSigmaIEtaIEta>>hSignE",(cutE && cutSigmaE)*cutWeight,"goff");
  hSignB->Print();
  hDataB->Print();
  hSignE->Print();
  hDataE->Print();
  hSignB->SetStats(0);
  hSignB->SetLineColor(8);
  hSignB->SetLineWidth(2);
  hSignB->SetTitle("#sigma_{i#etai#eta}, blinded Data vs Signal MC, Barrel");
  hSignB->GetXaxis()->SetTitle("#sigma_{i#etai#eta}");
  hSignE->SetStats(0);
  hSignE->SetLineColor(8);
  hSignE->SetLineWidth(2);
  hSignE->SetTitle("#sigma_{i#etai#eta}, blinded Data vs Signal MC, Endcap");
  hSignE->GetXaxis()->SetTitle("#sigma_{i#etai#eta}");
  hDataB->SetLineColor(2);
  hDataB->SetLineWidth(2);
  hDataE->SetLineColor(2);
  hDataE->SetLineWidth(2);
  TCanvas* cB = new TCanvas("c_phoSigmaIEtaIEta_B","c_phoSigmaIEtaIEta_B");
  hSignB->DrawNormalized();
  hDataB->DrawNormalized("same");
  lineSidebandB->SetLineWidth(2);
  lineSidebandB->Draw("same");
  TCanvas* cE = new TCanvas("c_phoSigmaIEtaIEta_E","c_phoSigmaIEtaIEta_E");
  hSignE->DrawNormalized();
  hDataE->DrawNormalized("same");
  lineSidebandE->SetLineWidth(2);
  lineSidebandE->Draw("same");
}

void DrawRanges()
{
  TFile f("../WGammaOutput/MUON/PreliminaryForTemplateMethodSelected/selected_blindPRESCALE_DATA.root");
  TTree* tr = (TTree*)f.Get("selectedEvents");
  TCut cutG="phoSigmaIEtaIEta<0.07";
  TCut cutB="phoEta>-1.4442 && phoEta<1.4442";
  TCut cutE="(phoEta>-2.5 && phoEta<-1.566)||(phoEta>1.566 && phoEta<2.5)";
  tr->SetLineColor(1);
  tr->SetLineWidth(2);
  tr->Draw("phoSigmaIEtaIEta",cutG);
  tr->SetLineColor(4);
  tr->SetLineStyle(7);
  tr->SetLineWidth(2);
  tr->Draw("phoSigmaIEtaIEta",cutG && cutB,"same");
  TLine* lineB = new TLine(0.011,0,0.011,1000000);
  lineB->SetLineColor(4);
  lineB->SetLineWidth(2);
  lineB->Draw("same");
  TLine* lineB2 = new TLine(0.005,0,0.005,1000000);
  lineB2->SetLineColor(4);
  lineB2->SetLineWidth(2);
  lineB2->Draw("same");
  tr->SetLineColor(46);
  tr->SetLineStyle(7);
  tr->SetLineWidth(2);
  tr->Draw("phoSigmaIEtaIEta",cutG && cutE,"same");
  TLine* lineE = new TLine(0.033,0,0.033,1000000);
  lineE->SetLineColor(46);
  lineE->SetLineWidth(2);
  lineE->Draw("same");
  TLine* lineE2 = new TLine(0.019,0,0.019,1000000);
  lineE2->SetLineColor(46);
  lineE2->SetLineWidth(2);
  lineE2->Draw("same");
}
