{
  gStyle->SetPaintTextFormat("0.2f");
  TString strAffix[4]={"MUON_WGamma","ELECTRON_WGamma","MUON_ZGamma","ELECTRON_ZGamma"};
  for (int iplot=0; iplot<4; iplot++){
    TString strName=TString("../WGammaOutput/")+strAffix[iplot]+TString("/Plots/Constants/cResponseMatr_")+strAffix[iplot]+TString("_");
    TString canvName=TString("cResponseMatr_")+strAffix[iplot]+TString("_");
    TFile* f = new TFile(strName+TString(".root"));
    if (!f->IsOpen()) continue;
    TCanvas* canv = (TCanvas*)f->Get(canvName);
    canv->Draw();
    canv->SaveAs(strName+TString(".root"));
    canv->SaveAs(strName+TString(".png"));
    canv->SaveAs(strName+TString(".pdf"));
  }//end of loop over iplot
//  gStyle->SetPaintTextFormat("0.0f");
}
