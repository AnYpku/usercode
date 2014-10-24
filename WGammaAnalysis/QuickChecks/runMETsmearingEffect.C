{
  TFile f("../WGammaOutput/MUON/PreliminaryForMEtCutSelected/selected_WGammaBKGMC_DYjets_to_ll.root");
  TString strSampleName="DYjets";
  TTree* tr = (TTree*)f.Get("selectedEvents");
  TCanvas* canv1 = new TCanvas(strSampleName+TString("pfMET"),strSampleName+TString("pfMET")); 
  tr->SetLineWidth(2);
  tr->Draw("pfMET");
  tr->SetLineStyle(9);
  tr->SetLineColor(2);
  tr->Draw("pfMET_notSmeared","","same");
  TCanvas* canv2 = new TCanvas(strSampleName+TString("pfMETPhi"),strSampleName+TString("pfMETPhi")); 
  tr->SetLineWidth(2);
  tr->SetLineStyle(0);
  tr->SetLineColor(1);
  tr->Draw("pfMETPhi");
  tr->SetLineStyle(9);
  tr->SetLineColor(2);
  tr->Draw("pfMETPhi_notSmeared","","same");
  canv1->SaveAs(TString(canv1->GetTitle())+TString(".png"));
  canv2->SaveAs(TString(canv2->GetTitle())+TString(".png")); 
}
