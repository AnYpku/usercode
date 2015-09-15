{
  TFile fSig("../WGammaOutput/MUON_WGamma/FullySelected/selected_WGammaSIGMC.root");
  TFile fDat("../WGammaOutput/MUON_WGamma/FullySelected/selected_WGamma_blindCOMBINED_DATA.root");

  gStyle->SetCanvasColor(kWhite); 
  TTree* tSig = (TTree*)fSig.Get("selectedEvents");
  TTree* tDat = (TTree*)fDat.Get("selectedEvents");
  TFile fOut("fBestWMtCut.root","recreate");
  TH1D* hSig = new TH1D ("hSigWMt","hist signal WMt",1,0,1000000);
  TH1D* hDat = new TH1D ("hDatWMt","hist data WMt",1,0,1000000);

  tSig->Draw("WMt>>hSigWMt","(phoEt>15 && phoEt<45)*weight","goff");
  tDat->Draw("WMt>>hDatWMt","(phoEt>15 && phoEt<45)*weight","goff");

  int nPoints=200;
  float cutMin=0;
  float cutMax=200;
  float WMtCut[nPoints];
  float qualityOverall[nPoints];
  float qualityNoWjets[nPoints];
  float qualityDyOnly[nPoints];

  for (int i=0; i<nPoints; i++){
    WMtCut[i] = cutMin+(cutMax-cutMin)*i/nPoints;
    TString cutStr="weight*(phoEt>15 && phoEt<45 && WMt>";
    cutStr+=WMtCut[i];
    cutStr+=")";
    tSig->Draw("WMt>>hSigWMt",cutStr,"goff");
    tDat->Draw("WMt>>hDatWMt",cutStr,"goff");  
    
    qualityOverall[i]=1.0*hSig->GetBinContent(1)/hDat->GetBinContent(1);

  }
  TGraph* grQ1 = new TGraph(nPoints,WMtCut,qualityOverall);
  
  TLegend* leg = new TLegend(0.7,0.7,0.9,0.9);

  grQ1->SetTitle("WMt cut optimization");
  grQ1->GetXaxis()->SetTitle("WMt, GeV");
//  TLatex latAxisTitle="#alpha";
  TString strAxisTitle="N_{sig}/N_{data}";
  grQ1->GetYaxis()->SetTitle(strAxisTitle);
  grQ1->SetMarkerStyle(20);
  grQ1->SetMarkerColor(1);
  grQ1->SetFillColor(0);
  grQ1->Draw("AP");

  leg->AddEntry(grQ1,"N_{sig}/N_{data}","pf");

  leg->SetFillColor(0);
  leg->SetLineColor(1);
  leg->Draw("same");

}
