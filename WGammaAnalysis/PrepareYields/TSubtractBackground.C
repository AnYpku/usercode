#include "TSubtractBackground.h"

#include <iostream>
#include <iomanip>

TSubtractBackground::TSubtractBackground()
{
}

TSubtractBackground::~TSubtractBackground()
{
  
}

void TSubtractBackground::SetYieldsDataDrivenTrue(TString name, TString label, int color, TString fileName, TString strYieldsName1D[3], TString strYieldsNameTot[3])
{
  SetYieldsDataDriven(_sourceDDTrue, name, label, color, fileName,  strYieldsName1D, strYieldsNameTot);
}

void TSubtractBackground::SetYieldsDataDrivenFake(TString name, TString label, int color, TString fileName, TString strYieldsName1D[3], TString strYieldsNameTot[3])
{
  SetYieldsDataDriven(_sourceDDFake, name, label, color, fileName,  strYieldsName1D, strYieldsNameTot);
}

void TSubtractBackground::SetYieldsDataDriven(YieldsSource& source, TString name, TString label, int color, TString fileName, TString strYieldsName1D[3], TString strYieldsNameTot[3])
{
  source.sourceType=DATA_DRIVEN;
  source.tr=0;
  source.name=name;
  source.label=label;
  source.color=color;
  TFile* f = new TFile(fileName);
  if (!f->IsOpen()){
    std::cout<<"ERROR in TSubtractBackground::SetYieldsDataDriven: file "<<fileName<<" can't be open"<<std::endl;
    return;
  }
  for (int ieta=_BARREL; ieta<=_COMMON; ieta++){
    source.hist[ieta]=(TH1F*)f->Get(strYieldsName1D[ieta]);
    TH1F* hTot = (TH1F*)f->Get(strYieldsNameTot[ieta]);
    source.yieldTotVal[ieta]=hTot->GetBinContent(1);
    source.yieldTotErr[ieta]=hTot->GetBinError(1);
  }// end of loop over ieta
}//end of SetYieldsDataDriven

void TSubtractBackground::SubtractBackground()
{
  std::cout<<"#####################"<<std::endl;
  std::cout<<"Subtract Background"<<std::endl;

  for (int ieta=_BARREL; ieta<=_COMMON; ieta++){
    _sourceBkgSubtrData.hist[ieta] = (TH1F*)_sourceDDTrue.hist[ieta]->Clone(_sourceBkgSubtrData.name);
    _sourceBkgSubtrData.yieldTotVal[ieta]=_sourceDDTrue.yieldTotVal[ieta];
    _sourceBkgSubtrData.yieldTotErr[ieta]=_sourceDDTrue.yieldTotErr[ieta]*_sourceDDTrue.yieldTotErr[ieta];
    _sourceBkgSubtrData.hist[ieta]->SetTitle(_sourceBkgSubtrData.name);
    int nBins = _sourceBkgSubtrData.hist[ieta]->GetNbinsX();
    for (int i=0; i<_sources.size(); i++){
      if (_sources[i].sourceType!=BKGMC_TRUE) continue;
      _sourceBkgSubtrData.hist[ieta]->Add(_sources[i].hist[ieta],-1);
      _sourceBkgSubtrData.yieldTotVal[ieta]-=_sources[i].yieldTotVal[ieta];
      _sourceBkgSubtrData.yieldTotErr[ieta]+=_sources[i].yieldTotErr[ieta]*_sources[i].yieldTotErr[ieta];
    }//end of loop over i
    _sourceBkgSubtrData.yieldTotErr[ieta]=sqrt(_sourceBkgSubtrData.yieldTotErr[ieta]);
  }//end of loop over ieta
  std::cout<<"#####################"<<std::endl;
}// end of SubtractBackground

void TSubtractBackground::CompareFakeDDvsMC(int ieta)
{
  _pyPars.fOut->cd();
  TString canvName="FakeDDvsMC";
  canvName+=StrLabelEta(ieta);
  _canvFakeDDvsMC[ieta]= new TCanvas(canvName,canvName,800,800);
  TLegend* legend = new TLegend(0.7,0.7,0.95,0.95);
  THStack* mcHists = new THStack("mcHistsTot","Fake DD vs MC");

  TH1F* hSum;
  bool hSumStarted=0;
  for (int is=0; is<_sources.size(); is++){
    if (_sources[is].sourceType!=BKGMC_FAKE) continue;
    std::cout<<"CompareFakeDDvsMC: processing isource "<<_sources[is].name<<std::endl;
    mcHists->Add(_sources[is].hist[ieta]);
    if (!hSumStarted){
      hSum=(TH1F*)_sources[is].hist[ieta]->Clone("hSum_WholeMC");
      hSumStarted=1;
    }
    else hSum->Add(_sources[is].hist[ieta]);
    legend->AddEntry(_sources[is].hist[ieta],_sources[is].label);
  }//end of loop over is

  legend->AddEntry(_sourceDDFake.hist[ieta],"data driven");

  _sourceDDFake.hist[ieta]->SetTitle("");
  CompareStackVsHist("Fake DD vs MC", _sourceDDFake.hist[ieta], hSum, legend, _canvFakeDDvsMC[ieta], 1, mcHists);

  _canvFakeDDvsMC[ieta]->Write();
}// end of CompareFakeDDvsMC

void TSubtractBackground::PlotPrintSave()
{

  //Plot
  for (int ieta=_BARREL; ieta<=_COMMON; ieta++){
    CompareTotalDATAvsMC(ieta);
    CompareFakeDDvsMC(ieta);
  }

  //Print Yields
  for (int is=0; is<_sources.size(); is++){
    for (int ieta=_BARREL; ieta<=_COMMON; ieta++){
      PrintYieldsOne(_sources[is].name+TString(": "), _sources[is].yieldTotVal[ieta], _sources[is].yieldTotErr[ieta], _sources[is].hist[ieta]);
    }
  }//end of loop over is

}
