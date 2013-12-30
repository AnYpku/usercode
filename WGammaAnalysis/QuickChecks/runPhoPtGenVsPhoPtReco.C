{
  TFile f("../WGammaOutput/MUON/FullySelected/selectedSIGMC.root");
  TTree* tr = (TTree*)f.Get("selectedEvents");
  int nBins = 9;
  double bins[10]={15,20,25,30,35,40,60,80,200,600};

  TCanvas* c1[nBins]; 
  TH1D* hists[nBins];
  for (int i=0; i<nBins; i++){
    TString cutStr="(phoGenEt-phoEt<20 && phoEt-phoGenEt<20 && phoGenEt>";
    cutStr+=bins[i];
    cutStr+=" && phoGenEt<";
    cutStr+=bins[i+1];
    cutStr+=")*weight";
    TString histName="hist";
    histName+=i;
    hists[i] = new TH1D(histName,histName,100,-20,20);
    tr->Draw(TString("phoGenEt-phoEt>>")+histName,cutStr,"goff");
//    hists[i]->SetLineColor(i+1);
    TString canvName = "c1";
    canvName+=i;
    TString canvTitle = "resolution: ";
    canvTitle+=bins[i];
    canvTitle+=" < Pt < ";
    canvTitle+=bins[i+1];
    canvTitle+=" GeV";
    c1[i] = new TCanvas(canvName,canvTitle);
    hists[i]->DrawNormalized();

  }

  TCanvas* c2 = new TCanvas("c2","profile");
  c2->SetLogx();
  tr->Draw("phoGenEt-phoEt:phoGenEt","(phoGenEt>0 && phoGenEt-phoEt<20 && phoEt-phoGenEt<20)*weight","prof");

  TCanvas* c3 = new TCanvas("c3","properly binned profile");
  c3->SetLogx();
  gStyle->SetPalette(1);
  TProfile* h = new TProfile("hProfile","h profile",nBins,bins);
  h->Sumw2();
  tr->Draw("phoGenEt-phoEt:phoGenEt>>hProfile","(phoGenEt>0)*weight","goff");
  h->Draw();

  TCanvas* c4 = new TCanvas("c4","yields gen (blue) vs reco (black)");
  c4->SetLogx();
  c4->SetLogy();
  TH1D* hGen = new TH1D("hGen","hGen (blue), hRec (black)",nBins,bins);
  TH1D* hRec = new TH1D("hRec","hRec",nBins,bins);
  hGen->Sumw2();
  hRec->Sumw2();
  tr->Draw("phoGenEt>>hGen","(1)*weight","goff");
  tr->Draw("phoEt>>hRec","(1)*weight","goff");
  hGen->SetLineColor(4);
  hGen->SetLineWidth(2);
  hGen->Draw("E0");
  hRec->SetLineWidth(2);
  hRec->Draw("E0 same");

  TCanvas* c5 = new TCanvas("c5","ratio: gen/rec");
  c5->SetLogx();
  TH1D* hGenOverRec = new TH1D("hGen","ratio: gen/rec",nBins,bins);
  //hGenOverRec->Sumw2();
  hGenOverRec->Divide(hGen,hRec);
  for (int i=0; i<nBins; i++){
    float numSq,denSq,dNumSq,dDenSq;
    numSq=hGen->GetBinContent(i)*hGen->GetBinContent(i);
    dNumSq=hGen->GetBinError(i)*hGen->GetBinError(i);
    denSq=hRec->GetBinContent(i)*hRec->GetBinContent(i);
    dDenSq=hRec->GetBinError(i)*hRec->GetBinError(i);
    
    hGenOverRec->SetBinError(i,sqrt((denSq*dNumSq+numSq*dDenSq)/(denSq*denSq)));
  }
  hGenOverRec->SetLineWidth(2);
  hGenOverRec->Draw("E0");
 
}
