#include "TFile.h"
#include "TTree.h"
#include "TCut.h"
#include "TCanvas.h"
#include "TH1D.h"

void makePlotWithSelection(TTree* tr, TString varToPlot, TString canvName, TString xAxisName, TCut cutLoose, TCut cutAdditional);

void makePresentationPlots()
{

  TFile f("../WGammaOutput/selected_MUON_DATA.root");
  TTree* tr=(TTree*)f.Get("selectedEvents"); 

  TCut cutWMt="WMt>70.";
  TCut cutdR="lePhoDeltaR>0.7";
  TCut cut=cutWMt+cutdR;

  makePlotWithSelection(tr,"WMt","cWMt","WMt, GeV/c^2",cutdR+TCut("WMt<200"),cut);
  makePlotWithSelection(tr,"lePhoDeltaR","cdR","delta R",cutWMt+TCut("lePhoDeltaR<8"),cut);
  makePlotWithSelection(tr,"leptonPt","clePt","muon Pt, GeV/c","leptonPt>30 && leptonPt<150",cut);
  makePlotWithSelection(tr,"leptonEta","cleEta","muon Eta","1",cut);
  makePlotWithSelection(tr,"leptonPhi","clePhi","muon Phi","1",cut);
  makePlotWithSelection(tr,"phoEt","cphoEt","photon Et, GeV/c","phoEt<150",cut);
  makePlotWithSelection(tr,"phoEta","cphoEta","photon Eta","1",cut);
  makePlotWithSelection(tr,"phoPhi","cphoPhi","photon Phi","1",cut);
}

void makePlotWithSelection(TTree* tr, TString varToPlot, TString canvName, TString xAxisName, TCut cutLoose, TCut cutAdditional)
{

  TTree* trCutted = tr->CopyTree(cutLoose);
  TH1D* histLoose = new TH1D("histLoose",varToPlot,50,trCutted->GetMinimum(varToPlot)-0.1*fabs(trCutted->GetMinimum(varToPlot)),trCutted->GetMaximum(varToPlot)+0.1*fabs(trCutted->GetMaximum(varToPlot)));
  trCutted->Draw(varToPlot+TString(">>histLoose"));
  TCanvas canv(canvName,canvName);
  //TAxis* xAxis = histLoose->GetXaxis();
  histLoose->GetXaxis()->SetTitle(xAxisName);
  histLoose->GetYaxis()->SetRange(0,histLoose->GetMaximumStored());
  histLoose->SetFillStyle(3004);
  histLoose->SetFillColor(2);
  histLoose->Draw();
  trCutted->SetFillStyle(1001);
  trCutted->SetFillColor(3);
  trCutted->Draw(varToPlot,cutAdditional,"same");
  canv.SaveAs(canvName+".png");
  delete histLoose;
  histLoose=0;
  trCutted=0;
  //xAxis=0;
}
