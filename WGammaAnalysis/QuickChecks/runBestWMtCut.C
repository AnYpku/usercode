{
  TFile fSig("../WGammaOutput/MUON/PreliminaryForMEtCutSelected/selectedSIGMC.root");
  TFile fbDY("../WGammaOutput/MUON/PreliminaryForMEtCutSelected/selectedBKGMC_DYjets_to_ll.root");
  TFile fbWj("../WGammaOutput/MUON/PreliminaryForMEtCutSelected/selectedBKGMC_Wjets_to_lnu.root");
  TFile fbmu("../WGammaOutput/MUON/PreliminaryForMEtCutSelected/selectedBKGMC_multibosons.root");
  TFile fbtt("../WGammaOutput/MUON/PreliminaryForMEtCutSelected/selectedBKGMC_ttbarjets.root");

  gStyle->SetCanvasColor(kWhite); 
  TTree* tSig = (TTree*)fSig.Get("selectedEvents");
  TTree* tbDY = (TTree*)fbDY.Get("selectedEvents");
  TTree* tbWj = (TTree*)fbWj.Get("selectedEvents");
  TTree* tbmu = (TTree*)fbmu.Get("selectedEvents");
  TTree* tbtt = (TTree*)fbtt.Get("selectedEvents");
  TFile fOut("fBestWMtCut.root","recreate");
  TH1D* hSig = new TH1D ("hSigWMt","hist signal WMt",1,0,1000000);
  TH1D* hbDY = new TH1D ("hbDYWMt","hist bkg DrellYan WMt",1,0,1000000);
  TH1D* hbWj = new TH1D ("hbWjWMt","hist bkg Wjets WMt",1,0,1000000);
  TH1D* hbmu = new TH1D ("hbmuWMt","hist bkg multibosons WMt",1,0,1000000);
  TH1D* hbtt = new TH1D ("hbttWMt","hist bkg ttbarjets WMt",1,0,1000000);
  tSig->Draw("WMt>>hSigWMt","weight","goff");
  tbDY->Draw("WMt>>hbDYWMt","weight","goff");
  tbWj->Draw("WMt>>hbWjWMt","weight","goff");
  tbmu->Draw("WMt>>hbmuWMt","weight","goff");
  tbtt->Draw("WMt>>hbttWMt","weight","goff");
  int nPoints=200;
  float cutMin=0;
  float cutMax=200;
  float WMtCut[nPoints];
  float qualityOverall[nPoints];
  float qualityNoWjets[nPoints];
  float qualityDyOnly[nPoints];

  for (int i=0; i<nPoints; i++){
    WMtCut[i] = cutMin+(cutMax-cutMin)*i/nPoints;
    TString cutStr="weight*(WMt>";
    cutStr+=WMtCut[i];
    cutStr+=")";
    tSig->Draw("WMt>>hSigWMt",cutStr,"goff");
    tbDY->Draw("WMt>>hbDYWMt",cutStr,"goff");
    tbWj->Draw("WMt>>hbWjWMt",cutStr,"goff");
    tbmu->Draw("WMt>>hbmuWMt",cutStr,"goff");
    tbtt->Draw("WMt>>hbttWMt",cutStr,"goff");   
    
    qualityOverall[i]=1.0*hSig->GetBinContent(1)/sqrt(hbDY->GetBinContent(1)+hbWj->GetBinContent(1)+hbmu->GetBinContent(1)+hbtt->GetBinContent(1));
    qualityNoWjets[i]=1.0*hSig->GetBinContent(1)/sqrt(hSig->GetBinContent(1)+hbDY->GetBinContent(1)+hbmu->GetBinContent(1)+hbtt->GetBinContent(1));
    qualityDyOnly[i]=1.0*hSig->GetBinContent(1)/sqrt(hSig->GetBinContent(1)+hbDY->GetBinContent(1));

//  std::cout<<WMtCut[i]<<": Ysig="<<hSig->GetBinContent(1)<<", Ydy="<<hbDY->GetBinContent(1)<<", Ywj="<<hbWj->GetBinContent(1)<<", Ymu="<<hbmu->GetBinContent(1)<<", Ytt="<<hbtt->GetBinContent(1)<<std::endl;
  }
  TGraph* grQ1 = new TGraph(nPoints,WMtCut,qualityDyOnly);
  TGraph* grQ2 = new TGraph(nPoints,WMtCut,qualityNoWjets);
  TGraph* grQ3 = new TGraph(nPoints,WMtCut,qualityOverall);
  
  TLegend* leg = new TLegend(0.5,0.5,0.9,0.9);

  grQ1->SetTitle("WMt cut optimization");
  grQ1->GetXaxis()->SetTitle("WMt, GeV");
//  TLatex latAxisTitle="#alpha";
  TString strAxisTitle="N_{sig}/#sqrt{N_{sig}+N_{bkg}}";
  grQ1->GetYaxis()->SetTitle(strAxisTitle);
  grQ1->SetMarkerStyle(20);
  grQ1->SetMarkerColor(1);
  grQ1->SetFillColor(0);
  grQ1->Draw("AP");
  grQ2->SetMarkerStyle(20);
  grQ2->SetMarkerColor(2);
  grQ2->SetFillColor(0);
  grQ2->Draw("P");
  grQ3->SetMarkerStyle(20);
  grQ3->SetMarkerColor(3);
  grQ3->SetFillColor(0);
  grQ3->Draw("P");

  leg->AddEntry(grQ1,"#frac{N_{sig}}{#sqrt{N_{sig}+N_{DY}}}","pf");
  leg->AddEntry(grQ2,"#frac{N_{sig}}{#sqrt{N_{sig}+N_{DY}+N_{multibosons}+N_{ttjets}}}","pf");
  leg->AddEntry(grQ3,"#frac{N_{sig}}{#sqrt{N_{DY}+N_{multibosons}+N_{ttjets}+N_{Wjets}}}","pf");
  leg->SetFillColor(0);
  leg->SetLineColor(1);
  leg->Draw("same");

}
