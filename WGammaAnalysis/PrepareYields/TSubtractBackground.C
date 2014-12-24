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
    for(int ib=1; ib<=source.hist[ieta]->GetNbinsX(); ib++){
      float cont=source.hist[ieta]->GetBinContent(ib);
      float err=source.hist[ieta]->GetBinError(ib);
//      source.hist[ieta]->SetBinContent(ib,cont*_pyPars.blindFraction);
//      source.hist[ieta]->SetBinError(ib,err*_pyPars.blindFraction);
    }
    source.hist[ieta]->SetFillColor(source.color);
//    source.hist[ieta]->SetLineColor(source.color);
    TH1F* hTot = (TH1F*)f->Get(strYieldsNameTot[ieta]);
//    source.yieldTotVal[ieta]=hTot->GetBinContent(1)*_pyPars.blindFraction;
//    source.yieldTotErr[ieta]=hTot->GetBinError(1)*_pyPars.blindFraction;
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
  CompareDDvsMC(ieta, "Fake", BKGMC_FAKE, _sourceDDFake, _canvFakeDDvsMC[ieta]);
}// end of CompareFakeDDvsMC

void TSubtractBackground::CompareTrueDDvsMC(int ieta)
{
  CompareDDvsMC(ieta, "True", BKGMC_TRUE, _sourceDDTrue, _canvTrueDDvsMC[ieta]);
}// end of CompareTrueDDvsMC

void TSubtractBackground::CompareDDvsMC(int ieta, TString strDD, int bkgType, YieldsSource& sourceDD, TCanvas* canv)
{
  _pyPars.fOut->cd(); 
  TString canvName=strDD+TString("DDvsMC");
  canvName+=StrLabelEta(ieta);
  canv= new TCanvas(canvName,canvName,800,800);
  TLegend* legend = new TLegend(0.7,0.7,0.95,0.95);
  THStack* mcHists = new THStack("mcHistsTot",strDD+TString(" DD vs MC"));

  TH1F* hSum;
  bool hSumStarted=0;

  int isSign=-1;

  for (int is=0; is<_sources.size(); is++){
    if (_sources[is].sourceType==SIGMC) isSign=is;
    if (_sources[is].sourceType!=bkgType) continue;
    std::cout<<TString("Compare")+strDD+TString("DDvsMC: processing isource ")<<_sources[is].name<<std::endl;
    mcHists->Add(_sources[is].hist[ieta]);
    if (!hSumStarted){
      hSum=(TH1F*)_sources[is].hist[ieta]->Clone("hSum_WholeMC");
      hSumStarted=1;
    }
    else hSum->Add(_sources[is].hist[ieta]);
    legend->AddEntry(_sources[is].hist[ieta],_sources[is].label);
  }//end of loop over is

  if (isSign!=-1 && bkgType==BKGMC_TRUE) {
    mcHists->Add(_sources[isSign].hist[ieta]);  
    legend->AddEntry(_sources[isSign].hist[ieta],_sources[isSign].label);  
    if (!hSumStarted){
      hSum=(TH1F*)_sources[isSign].hist[ieta]->Clone("hSum_WholeMC");
      hSumStarted=1;
    }
    else hSum->Add(_sources[isSign].hist[ieta]);
  }

  legend->AddEntry(sourceDD.hist[ieta],sourceDD.label,"l");
  sourceDD.hist[ieta]->SetTitle("");

  CompareStackVsHist(strDD+TString(" DD vs MC"), sourceDD.hist[ieta], hSum, legend, canv, 1, mcHists);

  canv->Write();
}// end of CompareDDvsMC

void TSubtractBackground::CompareDATAvsDDsum(int ieta)
{
  _pyPars.fOut->cd(); 
  TString canvName=TString("DATAvsDDsum");
  canvName+=StrLabelEta(ieta);
  _canvDATAvsDDsum[ieta]= new TCanvas(canvName,canvName,800,800);
  TLegend* legend = new TLegend(0.7,0.7,0.95,0.95);
  THStack* mcHists = new THStack("mcHistsTot",TString("DATAvsDDsum"));
  TH1F* hSum;

  mcHists->Add(_sourceDDFake.hist[ieta]);
  legend->AddEntry(_sourceDDFake.hist[ieta],_sourceDDFake.label,"f");
  hSum=(TH1F*)_sourceDDFake.hist[ieta]->Clone("hSum_WholeMC");

  mcHists->Add(_sourceDDTrue.hist[ieta]);
  legend->AddEntry(_sourceDDTrue.hist[ieta],_sourceDDTrue.label,"f");
  hSum->Add(_sourceDDTrue.hist[ieta]);

  int isData=-1;

  for (int is=0; is<_sources.size(); is++)
    if (_sources[is].sourceType==DATA) isData=is;

  if (isData==-1) return;
  legend->AddEntry(_sources[isData].hist[ieta],_sources[isData].label,"l");
  
  _sources[isData].hist[ieta]->SetTitle("");

  _sourceDDFake.hist[ieta]->SetLineColor(_sourceDDFake.color);
  _sourceDDTrue.hist[ieta]->SetLineColor(_sourceDDTrue.color);
  CompareStackVsHist(TString("DATA vs DD fake + true"), _sources[isData].hist[ieta], hSum, legend, _canvDATAvsDDsum[ieta], 1, mcHists);
  _sourceDDFake.hist[ieta]->SetLineColor(1);
  _sourceDDTrue.hist[ieta]->SetLineColor(1);

  _canvDATAvsDDsum[ieta]->Write();
}// end of CompareDATAvsDDsum

void TSubtractBackground::PlotPrintSave()
{

  //Plot
  for (int ieta=_BARREL; ieta<=_COMMON; ieta++){
    CompareTotalDATAvsMC(ieta);
    CompareFakeDDvsMC(ieta);
    CompareTrueDDvsMC(ieta);
    if (ieta!=_COMMON) CompareDATAvsDDsum(ieta);
  }

  //Print Yields
  for (int is=0; is<_sources.size(); is++){
    for (int ieta=_BARREL; ieta<=_COMMON; ieta++){
      PrintYieldsOne(_sources[is].name+TString(": "), _sources[is].yieldTotVal[ieta], _sources[is].yieldTotErr[ieta], _sources[is].hist[ieta]);
    }
  }//end of loop over is

}
