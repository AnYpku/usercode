#include "Unfolding.h" 
  //this class
#include "../Include/TMathTools.h"
#include "../Include/TMetTools.h" 
#include "../Include/TMuonCuts.h" 
#include "../Include/TElectronCuts.h" 
#include "../Include/TPhotonCuts.h" 
#include "../Include/TFullCuts.h" 
#include "../Include/TPuReweight.h" 
#include "../Configuration/TConfiguration.h" 
#include "../Configuration/TInputSample.h"
#include "../Configuration/TAllInputSamples.h"
  //this package
//#include "../Include/PhosphorCorrectorFunctor.hh"
  //taken from http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/CPena/src/PHOSPHOR_Corr_v2/
  //currently in this package
#include "TMath.h" 
#include "TH1F.h" 
#include "TH1D.h" 
#include "TH2D.h"
#include "TVectorD.h" 
#include "TCanvas.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TLegend.h"
#include "TTree.h"
#include "TFile.h"
#include "TMatrixD.h"
#include "TDecompLU.h"
#include "TDecompSVD.h"
#include "TMatrixDEigen.h"
#include "TStyle.h"
#include "TRandom.h"
#include "TLatex.h"
#include "TLine.h"
  // ROOT class
#include "../RooUnfold/src/RooUnfoldResponse.h"
#include "../RooUnfold/src/RooUnfold.h"
#include "../RooUnfold/src/RooUnfoldBayes.h"

#include <iostream> 
#include <iomanip>
#include <string>
#include <sstream>  
  //standard C++ class

Unfolding::Unfolding(int channel,int vgamma)
{
  _channel=channel; 
  _vgamma=vgamma;
  _fIn = new TFile(_config.GetSelectedName(_config.FULLY,_channel,_vgamma,_config.UNBLIND,_config.SIGMC));  

  _fOut = new TFile("fOut.root","recreate");

//  _fOut = new TFile(_config.GetUnfoldingFileName(_channel),"recreate");
//  if (_fOut->IsOpen()) _fOut->cd();
//  else {
//    std::cout<<"ERROR in Unfolding::PrepareUnfoldingMatrix: file "<<_config.GetUnfoldingFileName(_channel)<<" is not open"<<std::endl;
//  }
//  _INPUT = new TAllInputSamples(channel, configFile);

}

Unfolding::~Unfolding()
{
//  if (_fIn->IsOpen()) _fIn->Close();
//  if (_fOut->IsOpen()) _fOut->Close();
//  delete _fIn;
//  delete _fOut;
//  delete _INPUT;
}

bool Unfolding::TestDifferentMethods()
{

  TH1D* dataSignYields = new TH1D("data","data yields  for test",_nBinsGen,_phoPtLimitsGen);

  dataSignYields->SetBinContent(0,_histYieldsRecSmeared->GetBinContent(0));
  dataSignYields->SetBinError(0,_histYieldsRecSmeared->GetBinError(0));
  

  TH1D* unfYieldsRooUnfInv = new TH1D("unfYieldsInv","unfolded yields Inversion",_nBinsGen,_phoPtLimitsGen);
  TMatrixD errCovStatInv(_nBinsGen,_nBinsGen);
  TVectorD errStatInv(_nBinsGen);
  TVectorD errSystInv(_nBinsGen);
  TVectorD errCovStatVInv(_nBinsGen);
  ApplyRooUnfold(_histYieldsRec,unfYieldsRooUnfInv,RooUnfold::kInvert,errCovStatInv, errStatInv, errSystInv,errCovStatVInv,"testInvert");

  TH1D* unfYieldsRooUnfDAg = new TH1D("unfYieldsDAg","unfolded yields D'Agostini",_nBinsGen,_phoPtLimitsGen);
  TMatrixD errCovStatDAg(_nBinsGen,_nBinsGen);
  TVectorD errStatDAg(_nBinsGen);
  TVectorD errSystDAg(_nBinsGen);
  TVectorD errCovStatVDAg(_nBinsGen);
//  ApplyRooUnfold(_histYieldsRec,unfYieldsRooUnfDAg,RooUnfold::kBayes);
  ApplyRooUnfold(_histYieldsRec,unfYieldsRooUnfDAg,RooUnfold::kBayes,errCovStatDAg, errStatDAg, errSystDAg, errCovStatVDAg,"testBayes");
//  ApplyRooUnfold(_histYieldsRecSmeared,unfYieldsRooUnfDAg,RooUnfold::kBayes,errCovStatDAg, errStatDAg, errSystDAg, errCovStatVDAg);

  std::cout<<std::endl;
  std::cout<<"   limits;      RooUnf inv;       RooUnf D'Ag;      histYieldsGen;        Input Yields"<<std::endl;  

  std::cout<<std::endl;
  
  for (int i=1; i<=_nBinsGen; i++){
    std::cout<<std::setw(3)<<std::setprecision(0)<<_phoPtLimitsGen[i-1]<<" - "<<std::setw(3)<<std::setprecision(0)<<_phoPtLimitsGen[i]<<" GeV: ";
      //limits

    std::cout<<std::setw(5)<<std::setprecision(0)<<unfYieldsRooUnfInv->GetBinContent(i)<<"+-"<<std::setw(4)<<std::setprecision(0)<<errStatInv(i-1)<<"+-"<<std::setw(4)<<std::setprecision(0)<<errSystInv(i-1)<<"=";
   std::cout<<std::setw(5)<<std::setprecision(0)<<unfYieldsRooUnfInv->GetBinContent(i)<<"+-"<<unfYieldsRooUnfInv->GetBinError(i)<<";     ";
      //inversion

    std::cout<<std::setw(5)<<std::setprecision(0)<<unfYieldsRooUnfDAg->GetBinContent(i)<<"+-"<<std::setw(4)<<std::setprecision(0)<<errStatDAg(i-1)<<"+-"<<std::setw(4)<<std::setprecision(0)<<errSystDAg(i-1)<<"=";
    std::cout<<std::setw(5)<<std::setprecision(0)<<unfYieldsRooUnfDAg->GetBinContent(i)<<"+-"<<unfYieldsRooUnfDAg->GetBinError(i)<<";     ";
      //D'Agostini

    std::cout<<std::setw(5)<<std::setprecision(0)<<_histYieldsGen->GetBinContent(i)<<"+-"<<std::setw(4)<<std::setprecision(0)<<_histYieldsGen->GetBinError(i)<<";    ";
      //gen yields

    std::cout<<std::setw(5)<<std::setprecision(0)<<_histYieldsRec->GetBinContent(i)<<"+-"<<std::setw(4)<<std::setprecision(0)<<_histYieldsRec->GetBinError(i)<<";    ";
      //gen yields->GetBinError(i)<<";    ";
      //rec yields

    std::cout<<std::endl;
  }

  std::cout<<std::endl;
  std::cout<<"Covariance Matrix, RooUnf inversion"<<std::endl;  
  for (int i=0; i<_nBinsGen; i++){
    for (int j=0; j<_nBinsGen; j++){
//      std::cout<<std::setw(5)<<std::setprecision(0)<<errCovStatInv(i,j)<<" ";
    }
//    std::cout<<"; diag: "<<std::setw(4)<<std::setprecision(0)<<errCovStatVInv(i);
    std::cout<<std::endl;
  }

  std::cout<<std::endl;
  std::cout<<"Covariance Matrix, RooUnf D'Agostini"<<std::endl;  
  for (int i=0; i<_nBinsGen; i++){
    for (int j=0; j<_nBinsGen; j++){
//      std::cout<<std::setw(5)<<std::setprecision(0)<<errCovStatDAg(i,j)<<" ";
    }
//    std::cout<<"; diag: "<<std::setw(4)<<std::setprecision(0)<<errCovStatVDAg(i);
    std::cout<<std::endl;
  }

  delete unfYieldsRooUnfInv;
  delete unfYieldsRooUnfDAg;

  return 1;
}

bool Unfolding::PrepareMigrationMatrix()
{

  _tr = (TTree*)_fIn->Get("selectedEvents");
  Float_t phoEtRec;
  Float_t phoEtGen;
  Float_t weight;
  Int_t event;
  TBranch* b_phoEtRec;
  TBranch* b_phoEtGen;
  TBranch* b_weight;
  TBranch* b_event;

  _tr->SetBranchAddress("phoEt",&phoEtRec,&b_phoEtRec);
  _tr->SetBranchAddress("phoGenEt",&phoEtGen,&b_phoEtGen);
  _tr->SetBranchAddress("weight",&weight,&b_weight);
  _tr->SetBranchAddress("event",&event,&b_event);
  
  _nBinsRec = _config.GetNPhoPtUnfBins(0);
  
  float* limsNoOverflow = new float[_nBinsRec+1];
  std::cout<<"_nBinsRec="<<_nBinsRec<<std::endl;
    _config.GetPhoPtUnfBinsLimits(limsNoOverflow,0);
  TString strOverflowRec = "phoEt>";
  strOverflowRec+=limsNoOverflow[_nBinsRec];
  _isOverflowRec = _tr->GetEntries(strOverflowRec);
  
  TString strOverflowGen = "phoGenEt>";
  strOverflowGen+=limsNoOverflow[_nBinsRec];
  _isOverflowGen = _tr->GetEntries(strOverflowGen);
  delete limsNoOverflow;
  
  _nBinsRec = _config.GetNPhoPtUnfBins(_isOverflowRec);
  _phoPtLimitsRec = new float[_nBinsRec+1];
  _config.GetPhoPtUnfBinsLimits(_phoPtLimitsRec,_isOverflowRec);

  _nBinsGen = _config.GetNPhoPtUnfBins(_isOverflowGen);
  _phoPtLimitsGen = new float[_nBinsGen+1];
  _config.GetPhoPtUnfBinsLimits(_phoPtLimitsGen,_isOverflowGen);

  
  std::cout<<"tr->GetEntries("<<strOverflowRec<<")="<<_isOverflowRec<<", mBinsRec_="<<_nBinsRec<<std::endl;
  std::cout<<"tr->GetEntries("<<strOverflowGen<<")="<<_isOverflowGen<<", mBinsGen_="<<_nBinsGen<<std::endl;


  _fOut->cd();

  TConfiguration config;
  TString strAffix = "_";
  strAffix+=config.StrChannel(_channel);
  strAffix+="_";
  strAffix+=config.StrVgType(_vgamma);
  
  _histMigrMatrixNotNormalized = new TH2D(TString("migrNotNorm")+strAffix,TString("_histMigrMatrixNotNormalized")+strAffix,_nBinsRec,_phoPtLimitsRec,_nBinsGen,_phoPtLimitsGen);
  _histMigrMatrixNotNormalized->Sumw2();
  _histEventCountMigrMatrix = new TH2D(TString("migrMatrix")+strAffix,TString("Migration Matrix: Counts (no weights)")+strAffix,_nBinsRec,_phoPtLimitsRec,_nBinsGen,_phoPtLimitsGen);

  _histYieldsRec = new TH1D(TString("yieldsRec")+strAffix,TString("yields rec")+strAffix,_nBinsRec,_phoPtLimitsRec);
  _histYieldsRec->Sumw2();
  _histYieldsRecSmeared = new TH1D(TString("yieldsRecSmeared")+strAffix,TString("yields rec smeared")+strAffix,_nBinsRec,_phoPtLimitsRec);
    //the smeared histogram is prepared in order to test the procedure on the 
    //signal MC like if it were data;
    //that is why the errors on rec smeared are sqrt(N)
  _histYieldsGen = new TH1D(TString("yieldsGen")+strAffix,TString("yields gen")+strAffix,_nBinsGen,_phoPtLimitsGen);
  _histYieldsGen->Sumw2();
  
  
 
  int nEntries = _tr->GetEntries();
  
  //filling out 
  // - generated yields vector (_histYieldsGen)
  // - reconstructed yields vector (_histYieldsRec)
  // - migration matrix (_histMigrMatrixNotNormalized)
  // - migration counts matrix (_histEventCountMigrMatrix)
  for (int entry=0; entry<nEntries; entry++){
    b_phoEtRec->GetEntry(entry);
    b_phoEtGen->GetEntry(entry);
    b_weight->GetEntry(entry);
    b_event->GetEntry(entry);
    if (phoEtGen<0 || phoEtRec<0) continue;
//    std::cout<<"phoEtGen="<<phoEtGen<<", phoEtRec="<<phoEtRec<<", weight="<<weight<<std::endl;
    _histEventCountMigrMatrix->Fill(phoEtRec,phoEtGen,1);
    _histMigrMatrixNotNormalized->Fill(phoEtRec,phoEtGen,weight);
    _histYieldsGen->Fill(phoEtGen,weight);
    _histYieldsRec->Fill(phoEtRec,weight);
  } // loop over entries
  
  _histEventCountMigrMatrix->Print();
  _histMigrMatrixNotNormalized->Print();
  std::cout<<std::endl;
  std::cout<<"contents and errors before normalization"<<std::endl;
  for (int i=0; i<_nBinsRec; i++){
    TRandom rnd;
    double cont = rnd.Gaus(_histYieldsRec->GetBinContent(i+1),_histYieldsRec->GetBinError(i+1));
    _histYieldsRecSmeared->SetBinContent(i+1,cont);
    _histYieldsRecSmeared->SetBinError(i+1,sqrt(cont));
      //the smeared histogram is prepared in order to test the procedure on the 
      //signal MC like if it were data;
      //that is why the errors on rec smeared are sqrt(N)
    for (int j=0; j<_nBinsGen; j++){
      std::cout<<std::fixed << std::setw(5) << std::setprecision(0)<<_histMigrMatrixNotNormalized->GetBinContent(i+1,j+1)<<"+-"<<std::fixed << std::setw(5)<< std::setprecision(0)<<_histMigrMatrixNotNormalized->GetBinError(i+1,j+1)<<" ";
    }
    std::cout<<std::endl;
  }// end of loop over i
  
  std::cout<<std::endl;
  std::cout<<"unweighted counts:"<<std::endl;
  for (int i=0; i<_nBinsRec; i++){
    for (int j=0; j<_nBinsGen; j++){
      std::cout<<std::fixed << std::setw(5) << std::setprecision(0)<<_histEventCountMigrMatrix->GetBinContent(i+1,j+1)<<"+-"<<std::fixed << std::setw(5)<< std::setprecision(0)<<_histEventCountMigrMatrix->GetBinError(i+1,j+1)<<" ";
    }
    std::cout<<std::endl;
  }
  std::cout<<std::endl;
  std::cout<<"gen yields:"<<std::endl;
  for (int j=0; j<_nBinsGen; j++){
      std::cout<<std::fixed << std::setw(5) << std::setprecision(0)<<_histYieldsGen->GetBinLowEdge(j+1)<<"- "<<std::fixed << std::setw(5)<< std::setprecision(0)<<_histYieldsGen->GetBinLowEdge(j+1)+_histYieldsGen->GetBinWidth(j+1)<<" ";
  }
  std::cout<<std::endl;
  for (int j=0; j<_nBinsGen; j++){
      std::cout<<std::fixed << std::setw(5) << std::setprecision(0)<<_histYieldsGen->GetBinContent(j+1)<<"+-"<<std::fixed << std::setw(5)<< std::setprecision(0)<<_histYieldsGen->GetBinError(j+1)<<" ";
  }
  std::cout<<std::endl;
  std::cout<<std::endl;
  std::cout<<"rec yields:"<<std::endl;
  for (int j=0; j<_nBinsRec; j++){
      std::cout<<std::fixed << std::setw(5) << std::setprecision(0)<<_histYieldsRec->GetBinContent(j+1)<<"+-"<<std::fixed << std::setw(5)<< std::setprecision(0)<<_histYieldsRec->GetBinError(j+1)<<" ";
  }
  std::cout<<std::endl;
  std::cout<<std::endl;
  
  return 1;
}// end of PrepareMigrationMatrix(...)

bool Unfolding::ApplyRooUnfold(TH1D* histInputYields, TH1D* unfoldedYields, RooUnfold::Algorithm alg, TString strAnnex)
{  
  std::cout<<"#######################"<<std::endl;
  std::cout<<"DoRooUnfold() starts here:"<<std::endl<<std::endl;

  TMatrixD errCovStat(_nBinsGen,_nBinsGen);
  TVectorD errStatV(_nBinsGen);
  TVectorD errSystV(_nBinsGen);
  TVectorD errCovStatV(_nBinsGen);

  bool isOk = ApplyRooUnfold(histInputYields,unfoldedYields,alg,errCovStat,errStatV, errSystV, errCovStatV, strAnnex);

  std::cout<<std::endl<<"DoRooUnfold() ends here"<<std::endl;
  std::cout<<"#######################"<<std::endl;
  return isOk;
}// end of ApplyRooUnfold(...)

bool Unfolding::ApplyRooUnfold(TH1D* histInputYields, TH1D* unfoldedYields, RooUnfold::Algorithm alg, TMatrixD& errCovStat, TVectorD& errStatV, TVectorD& errSystV, TVectorD& errCovStatV, TString strAnnex)
{  
  std::cout<<"#######################"<<std::endl;
  std::cout<<"DoRooUnfold() starts here:"<<std::endl<<std::endl;

  RooUnfoldResponse response(_histYieldsRec,_histYieldsGen,_histMigrMatrixNotNormalized,"response","RooUnfoldResponse matrix");
  std::cout<<"RooUnfold";
  if (alg==RooUnfold::kInvert) std::cout<<" Invert"<<std::endl;
  else if (alg==RooUnfold::kBayes) std::cout<<" D'Agostini"<<std::endl;
  else std::cout<<std::endl;

  RooUnfold* unfold = RooUnfold::New(alg,&response,histInputYields);
  if (alg==RooUnfold::kBayes) unfold->SetRegParm(5);
  unfold->SetNToys(1000);

//  RooUnfoldBayes* unfold = new RooUnfoldBayes(&response,histInputYields,5,0,0);
//  unfold->SetNToys(1000);
  TH1D* histUnfoldedYields = (TH1D*) unfold->Hreco(RooUnfold::kCovToy);


  errCovStat=unfold->Ereco(RooUnfold::kCovToy);
  errCovStatV=unfold->ErecoV(RooUnfold::kCovToy);
//  TH1D* histUnfoldedYields = (TH1D*) unfold->Hreco(RooUnfold::kErrors);

  //Compute errors due to finite MC statistics

  float errSyst[_nBinsGen+1];
  for (int i=1; i<_nBinsGen+1; i++)
    errSyst[i]=0;

  int NSmears=1000;
  ComputeSystErrors(histInputYields, errSyst, alg, NSmears);
//  ComputeStatErrors(histInputYields, errStat, alg, NSmears);
//  ComputeStatErrors(histInputYields, errSyst, alg, NSmears);

  for (int i=1; i<=_nBinsGen; i++){
    unfoldedYields->SetBinContent(i,histUnfoldedYields->GetBinContent(i));
    unfoldedYields->SetBinError(i,sqrt(histUnfoldedYields->GetBinError(i)*histUnfoldedYields->GetBinError(i)+errSyst[i]*errSyst[i]));
    errStatV[i-1]=histUnfoldedYields->GetBinError(i);
    errSystV[i-1]=errSyst[i];
  }

  TConfiguration config;
  TString strAffix = "_";
  strAffix+=config.StrChannel(_channel);
  strAffix+="_";
  strAffix+=config.StrVgType(_vgamma);
  strAffix+="_";

  PlotMatrixAsTH2D(errCovStat, "matrCovariance",TString("Covariance Matrix")+strAffix, strAnnex);
//  TCanvas* c = new TCanvas("errCovStat","errCovStat");
//  errCovStat.Draw("COLZ");

  TMatrixD matrCorrelation(errCovStat.GetNrows(),errCovStat.GetNcols());
  std::cout<<"Correlation matrix"<<std::endl;
  for (int i=0; i<errCovStat.GetNrows(); i++){
    for (int j=0; j<errCovStat.GetNcols(); j++){
      float Cij = errCovStat(i,j);
      float Cii = errCovStat(i,i);
      float Cjj = errCovStat(j,j);
      if (i>=errCovStat.GetNcols()) Cjj = errCovStat(i,i);
      if (j>=errCovStat.GetNrows()) Cii = errCovStat(j,j);
      if ((Cii*Cjj)!=0) matrCorrelation(i,j)=Cij/sqrt(Cii*Cjj);
      if ((Cii*Cjj)<0.0001 && Cij*Cij<0.0001) matrCorrelation(i,j)=0;
      if ((Cii*Cjj)<0.0001 && Cij*Cij>0.0001) matrCorrelation(i,j)=10;      
      std::cout<<std::setprecision(0)<<"i="<<i<<", j="<<j<<", Cij="<<Cij<<", Cii="<<Cii<<", Cjj="<<Cjj;
      std::cout<<std::setprecision(3)<<", corr(i,j)="<<matrCorrelation(i,j)<<std::endl;
      std::cout<<std::setprecision(0);
    }//end of loop over j
  }//end of loop over i

  PlotMatrixAsTH2D(matrCorrelation, "matrCorrelation",TString("Correlation Matrix")+strAffix, strAnnex);

  std::cout<<"Yields: (MC-gen), (MC-rec), (data-input), (data-unfolded)"<<std::endl;
  for (int i=1; i<=_nBinsRec; i++){
    std::cout<<std::setprecision(0);
    std::cout<<_histYieldsGen->GetBinLowEdge(i)<<"-"<<_histYieldsGen->GetBinLowEdge(i)+_histYieldsGen->GetBinWidth(i)<<" ";
    std::cout<<_histYieldsGen->GetBinContent(i)<<"+-"<<_histYieldsGen->GetBinError(i)<<" ";
    std::cout<<_histYieldsRec->GetBinLowEdge(i)<<"-"<<_histYieldsRec->GetBinLowEdge(i)+_histYieldsRec->GetBinWidth(i)<<" ";
    std::cout<<_histYieldsRec->GetBinContent(i)<<"+-"<<_histYieldsRec->GetBinError(i)<<" ";
    std::cout<<histInputYields->GetBinLowEdge(i)<<"-"<<histInputYields->GetBinLowEdge(i)+histInputYields->GetBinWidth(i)<<" ";
    std::cout<<histInputYields->GetBinContent(i)<<"+-"<<histInputYields->GetBinError(i)<<" ";
    std::cout<<unfoldedYields->GetBinLowEdge(i)<<"-"<<unfoldedYields->GetBinLowEdge(i)+unfoldedYields->GetBinWidth(i)<<" ";
    std::cout<<unfoldedYields->GetBinContent(i)<<"+-"<<unfoldedYields->GetBinError(i)<<" ";
    std::cout<<std::endl;
  }
//  std::cout<<"Unfolded yields:"<<std::endl;
//  for (int i=1; i<=_nBinsGen; i++){
//    std::cout<<std::setprecision(0)<<histUnfoldedYields->GetBinContent(i)<<"+-"<<std::setprecision(0)<<histUnfoldedYields->GetBinError(i)<<"+-"<<std::setprecision(0)<<errSyst[i];
//    std::cout<<" = "<<std::setprecision(0)<<unfoldedYields->GetBinContent(i)<<"+-"<<std::setprecision(0)<<unfoldedYields->GetBinError(i)<<std::endl;
//  }

  std::cout<<std::endl<<"DoRooUnfold() ends here"<<std::endl;
  std::cout<<"#######################"<<std::endl;
  return 1;
}

bool Unfolding::ComputeSystErrors(TH1D* histInputYields, float* errSyst, RooUnfold::Algorithm alg, int NSmears)
{

  //Compute errors due to finite MC statistics

  TString name="n";
  name+=alg;
  name+=NSmears;
  name+="migr";

  TH2D* histMigrMatrixSmeared = new TH2D(name,name,_nBinsRec,_phoPtLimitsRec,_nBinsGen,_phoPtLimitsGen);
  name.ReplaceAll("migr","rec");
  TH1D* histYieldsRecSmeared = new TH1D(name,name,_nBinsRec,_phoPtLimitsRec);
  name.ReplaceAll("rec","gen");
  TH1D* histYieldsGenSmeared = new TH1D(name,name,_nBinsGen,_phoPtLimitsGen);

//  TH2D* histMigrMatrixSmeared = (TH2D*)_histMigrMatrixNotNormalized->Clone();
// TH1D* histYieldsRecSmeared = (TH1D*)_histYieldsRec->Clone();
//  TH1D* histYieldsGenSmeared = (TH1D*)_histYieldsGen->Clone();


  TRandom rnd;

  double SumValsSqr[_nBinsGen+1];
  double SumVals[_nBinsGen+1];
  double ExSqr[_nBinsGen+1];
  double Ex[_nBinsGen+1];
  for (int i=1; i<=_nBinsGen; i++){
    SumValsSqr[i]=0;
    SumVals[i]=0;
    ExSqr[i]=0;
    Ex[i]=0;
  }

  for (int iSm=0; iSm<NSmears; iSm++){

    for (int irec=1; irec<=_nBinsRec; irec++)
      for (int igen=1; igen<=_nBinsGen; igen++){
        double cont = rnd.Gaus(_histMigrMatrixNotNormalized->GetBinContent(irec,igen),_histMigrMatrixNotNormalized->GetBinError(irec,igen));
        histMigrMatrixSmeared->SetBinContent(irec,igen,cont);
      }
    for (int irec=1; irec<=_nBinsRec; irec++){
      double recYield=0;
      for (int igen=1; igen<=_nBinsGen; igen++){
        recYield+=histMigrMatrixSmeared->GetBinContent(irec,igen);
      }   
      histYieldsRecSmeared->SetBinContent(irec,recYield);
    }     
    for (int igen=1; igen<=_nBinsGen; igen++){
      double genYield=0;
      for (int irec=1; irec<=_nBinsRec; irec++){
        genYield+=histMigrMatrixSmeared->GetBinContent(irec,igen);
      }   
      histYieldsGenSmeared->SetBinContent(igen,genYield);
    }  
    RooUnfoldResponse responseSmeared(histYieldsRecSmeared,histYieldsGenSmeared,histMigrMatrixSmeared);
    RooUnfold* unfoldSmeared = RooUnfold::New(alg,&responseSmeared,histInputYields);

    TH1D* histUnfSmeared = (TH1D*) unfoldSmeared->Hreco(RooUnfold::kNoError);
    for (int igen=1; igen<=_nBinsGen; igen++){
      double val = histUnfSmeared->GetBinContent(igen);
      SumVals[igen]+=val;
      SumValsSqr[igen]+=val*val;
    }

  } //end of loop over smears

  for (int i=1; i<=_nBinsGen; i++){
    Ex[i]=SumVals[i]/NSmears;
    ExSqr[i]=SumValsSqr[i]/NSmears;
    errSyst[i] =  sqrt(ExSqr[i]-Ex[i]*Ex[i]);
  }

  return 1;
}

bool Unfolding::ComputeStatErrors(TH1D* histInputYields, float* errStat, RooUnfold::Algorithm alg, int NSmears)
{

  //Compute errors due to finite MC statistics

  TString name="n";
  name+=alg;
  name+=NSmears;
  name+="input";

  TH1D* histInputYieldsSmeared = new TH1D(name,name,_nBinsGen,_phoPtLimitsGen);


  TRandom rnd;

  double SumValsSqr[_nBinsGen+1];
  double SumVals[_nBinsGen+1];
  double ExSqr[_nBinsGen+1];
  double Ex[_nBinsGen+1];
  for (int i=1; i<=_nBinsGen; i++){
    SumValsSqr[i]=0;
    SumVals[i]=0;
    ExSqr[i]=0;
    Ex[i]=0;
  }

  for (int iSm=0; iSm<NSmears; iSm++){
      for (int igen=1; igen<=_nBinsGen; igen++){
        double cont = rnd.Gaus(histInputYields->GetBinContent(igen),histInputYields->GetBinError(igen));
        histInputYieldsSmeared->SetBinContent(igen,cont);
      }
      RooUnfoldResponse response(_histYieldsRec,_histYieldsGen,_histMigrMatrixNotNormalized,"response","RooUnfoldResponse matrix");
    RooUnfold* unfoldSmeared = RooUnfold::New(alg,&response,histInputYieldsSmeared);

    TH1D* histUnfSmeared = (TH1D*) unfoldSmeared->Hreco(RooUnfold::kNoError);
    for (int igen=1; igen<=_nBinsGen; igen++){
      double val = histUnfSmeared->GetBinContent(igen);
      SumVals[igen]+=val;
      SumValsSqr[igen]+=val*val;
    }

  } //end of loop over smears

  for (int i=1; i<=_nBinsGen; i++){
    Ex[i]=SumVals[i]/NSmears;
    ExSqr[i]=SumValsSqr[i]/NSmears;
    errStat[i] =  sqrt(ExSqr[i]-Ex[i]*Ex[i]);
  }

  return 1;
}

bool Unfolding::PlotAndStore(TString strAnnex)
{

  _fOut->cd();

  TString strAffixTitle=TString("_")+_config.StrChannel(_channel)+TString("_")+_config.StrVgType(_vgamma)+TString("_");

  _histMigrMatrixNotNormalized->SetTitle(TString("Migration Matrix: Counts (no weights)")+strAffixTitle);
  _histEventCountMigrMatrix->SetTitle(TString("Migration Matrix: not normalized")+strAffixTitle);
  PlotTH2D(_histMigrMatrixNotNormalized, TString("cMigrMatrix")+strAffixTitle, strAnnex);
  PlotTH2D(_histEventCountMigrMatrix, TString("cEventCount")+strAffixTitle, strAnnex);

  TH2D* hResponse = (TH2D*)_histMigrMatrixNotNormalized->Clone("hResponse");
  for (int ir=1; ir<=_nBinsRec; ir++){
    float sum=0;
    for (int ig=1; ig<=_nBinsGen; ig++){
      sum+=_histMigrMatrixNotNormalized->GetBinContent(ir,ig);
    }// end of loop over ig
    if (sum==0) sum=1.0;
    for (int ig=1; ig<=_nBinsGen; ig++){
      hResponse->SetBinContent(ir,ig,_histMigrMatrixNotNormalized->GetBinContent(ir,ig)/sum);
      hResponse->SetBinError(ir,ig,_histMigrMatrixNotNormalized->GetBinError(ir,ig)/sum);
    }// end of loop over ig
  }// end of loop over ir
  hResponse->SetTitle(TString("Response Matrix: normalized by N_{gen} ")+strAffixTitle);
  PlotTH2D(hResponse, TString("cResponseMatr")+strAffixTitle, strAnnex);
 
  TCanvas* cPtSpectra = new TCanvas(TString("cPtSpectra")+strAffixTitle,TString("cPtSpectra")+strAffixTitle,600,600);
  cPtSpectra->Divide(2,1);
  TPad* pad1 = (TPad*)cPtSpectra->GetPad(1);
  TPad* pad2 = (TPad*)cPtSpectra->GetPad(2);
  pad1->SetLogx();
  pad1->SetLogy();
  pad2->SetLogx();
  pad1->SetPad(0,0.3,1.0,1.0);
  pad2->SetPad(0,0,  1.0,0.28);
  pad1->SetLeftMargin(0.18);
  pad1->SetTopMargin(0.08);
  pad1->SetRightMargin(0.07);
  pad1->SetBottomMargin(0.01); // All X axis labels and titles are thus cut off
  pad2->SetLeftMargin(0.18);
  pad2->SetTopMargin(0.01);
  pad2->SetRightMargin(0.07);
  pad2->SetBottomMargin(0.45);
  pad1->SetLogx();
  pad1->SetLogy();
  pad1->cd();
  _histYieldsRec->GetXaxis()->SetMoreLogLabels();
  _histYieldsRec->GetXaxis()->SetNoExponent();
  _histYieldsRec->SetLineColor(1);
  _histYieldsRec->SetLineWidth(2);
  _histYieldsRec->Draw();
  _histYieldsGen->SetLineColor(4);
  _histYieldsGen->SetLineWidth(2);
  _histYieldsGen->Draw("same");
  pad2->cd();
  TH1F* hRatio = (TH1F*) _histYieldsRec->Clone("hRatio");
  hRatio->Divide(_histYieldsGen);
  float max=1.1; float min=0.9;
  for (int ib=1; ib<=hRatio->GetNbinsX(); ib++){
    if (hRatio->GetBinContent(ib)+hRatio->GetBinError(ib)>max) 
      max=hRatio->GetBinContent(ib)+hRatio->GetBinError(ib);
    if (hRatio->GetBinContent(ib)-hRatio->GetBinError(ib)<min) 
      min=hRatio->GetBinContent(ib)-hRatio->GetBinError(ib);
  }// end of loop over ib
  min=0.7; max=1.3;
  hRatio->SetStats(0);
  hRatio->SetLineWidth(2);
  hRatio->GetYaxis()->SetRangeUser(min,max);
  hRatio->GetYaxis()->SetLabelSize(0.1);
  hRatio->GetXaxis()->SetLabelSize(0.1);
  hRatio->GetXaxis()->SetTitleOffset(1.0);
  hRatio->GetXaxis()->SetTitleSize(0.12);
  hRatio->GetXaxis()->SetMoreLogLabels();
  hRatio->GetXaxis()->SetNoExponent();
  hRatio->SetTitle("; P_{T}^{#gamma}, GeV;");
  hRatio->Draw();
/*
  int nBinsAnalysis = _config.GetNPhoPtBins(); 
  float* limsAnalysis = new float[nBinsAnalysis+1];
  _config.GetPhoPtBinsLimits(limsAnalysis);
  TCanvas* cResolution[nBinsAnalysis];
  for (int ib=0; ib<nBinsAnalysis; ib++){

    float low = limsAnalysis[ib];
    float up = limsAnalysis[ib+1];
    float middle = 0.5*(up+low);
    TString strDraw = "phoEt-phoGenEt+";
    strDraw+=middle;

    TString strCut = "phoGenEt>=";
    strCut+=low;
    strCut+=" && phoGenEt<";
    strCut+=up;
    strCut+=" && phoGenEt>0 && (";
    strCut+=strDraw;
    strCut+=">10)";
    std::cout<<"_tr->Draw(\""<<strDraw<<"\",\""<<strCut<<"\");"<<std::endl;
 
    _tr->SetLineWidth(2);
    _tr->SetLineColor(ib+1);

    TString canvName="cResolution";
    canvName+=ib;
    cResolution[ib] = new TCanvas(canvName,"pho Pt resolution in different bins");
    cResolution[ib]->SetLogy();
 //   std::cout<<"strDraw="<<strDraw<<", strCut="<<strCut<<std::endl;
   
    _tr->Draw(strDraw,strCut);
    for (int ib2=0; ib2<nBinsAnalysis; ib2++){
      float low2 = limsAnalysis[ib2];
      float up2 = limsAnalysis[ib2+1];
      float middle2 = 0.5*(up2+low2);
      TString strDraw2 = "phoEt-phoGenEt+";
      strDraw2+=middle2;

      TString strCut2 = "phoGenEt>=";
      strCut2+=low2;
      strCut2+=" && phoGenEt<";
      strCut2+=up2;
      strCut2+=" && phoGenEt>0 && (";
      strCut2+=strDraw2;
      strCut2+=">10)";
//      std::cout<<"_tr->Draw(\""<<strDraw<<"\",\""<<strCut<<"\");"<<std::endl;
 
      _tr->SetLineWidth(2);
      _tr->SetLineColor(ib2+1);

      _tr->Draw(strDraw2,strCut2,"same");
    }
  }
*/

//  _histYieldsRec->Write(_config.GetYieldsRec1DName());
//  _histYieldsGen->Write(_config.GetYieldsGen1DName());
/*
  cEventCount->Write("cEventCount");
  cMigrMatrix->Write("cMigrMatrix");

  TString nameSave=_config.GetPlotsDirName(_channel,_vgamma,_config.PLOTS_CONSTANTS);
  nameSave+=cEventCount->GetTitle();
  nameSave+=".png";
  cEventCount->SaveAs(nameSave);
  nameSave.ReplaceAll(".png",".pdf");
  cEventCount->SaveAs(nameSave);
  nameSave.ReplaceAll(".pdf",".root");
  cEventCount->SaveAs(nameSave);

  nameSave=_config.GetPlotsDirName(_channel,_vgamma,_config.PLOTS_CONSTANTS);
  nameSave+=cMigrMatrix->GetTitle();
  nameSave+=".png";
  cMigrMatrix->SaveAs(nameSave);
  nameSave.ReplaceAll(".png",".pdf");
  cMigrMatrix->SaveAs(nameSave);
  nameSave.ReplaceAll(".pdf",".root");
  cMigrMatrix->SaveAs(nameSave);
*/
  return 1;
}// end of PlotAndStore

bool Unfolding::PlotMatrixAsTH2D(TMatrixD& matr, TString saveName, TString hTitle, TString strAnnex){

  TString hName=TString("h")+saveName;
  hName.ReplaceAll("/","_");
  hName.ReplaceAll(".","_");
  hName.ReplaceAll("__","_");
  TH2D* hist2D = new TH2D(hName,hName,_nBinsRec,_phoPtLimitsRec,_nBinsGen,_phoPtLimitsGen);   
  for (int ir=1; ir<_nBinsRec+1; ir++)
    for (int ig=1; ig<_nBinsGen+1; ig++){
      hist2D->SetBinContent(ir,ig,matr(ir-1,ig-1));
    }
  hist2D->SetTitle(hTitle);
  PlotTH2D(hist2D,saveName,strAnnex);
  return 1;
}//end of PlotMatrixAsTH2D

bool Unfolding::PlotTH2D(TH2D* hist2D, TString saveName, TString strAnnex){

  TStyle* style = new TStyle("style","style");
  style->SetPalette(1);
  style->SetPaintTextFormat("0.1f");
  style->SetPaintTextFormat("g");

  hist2D->GetXaxis()->SetTitle("P_{T}^{#gamma} reco, GeV");
  hist2D->GetYaxis()->SetTitle("P_{T}^{#gamma} gen, GeV");
  TString canvName=saveName;
  canvName.ReplaceAll("/","_");
  canvName.ReplaceAll(".","_");
  canvName.ReplaceAll("__","_");
  int canvWidth=1000;
  int canvHeight=600; 
  TConfiguration conf;
//  if (fabs(_phoPtLimitsGen[_nBinsGen]-conf.GetPhoPtMax())<0.01) canvWidth=canvWidth*1.2;
  hist2D->GetYaxis()->SetMoreLogLabels(1);
//  if (fabs(_phoPtLimitsRec[_nBinsRec]-conf.GetPhoPtMax())<0.01){
//    hist2D->GetYaxis()->SetMoreLogLabels(0);
//    canvHeight=canvHeight*1.2;
//  }
  TCanvas* canv = new TCanvas(canvName,canvName,canvWidth,canvHeight);
  canv->SetLogx();
  canv->SetLogy();
  canv->SetRightMargin(0.12);
  canv->SetLeftMargin(0.10);
  canv->SetBottomMargin(0.11);
  canv->SetTopMargin(0.12);
  TString hTitle = hist2D->GetTitle();
  hTitle.ReplaceAll("_MUON_"," MUON ");
  hTitle.ReplaceAll("_ELECTRON_"," ELECTRON ");
  hTitle.ReplaceAll("Gamma_","Gamma ");
  hTitle.ReplaceAll("Gamma","#gamma");
  TLatex* titleLatex =  new TLatex(0.09,0.94,hTitle);
  titleLatex->SetNDC();
  titleLatex->SetTextFont(62);
  titleLatex->SetTextSize(0.05);
  hist2D->SetTitle("");
  hist2D->SetStats(0);
  hist2D->GetXaxis()->SetMoreLogLabels(1);
  hist2D->GetXaxis()->SetNoExponent(1);
  hist2D->GetYaxis()->SetNoExponent();
  hist2D->GetYaxis()->SetTitleOffset(1.00);
  hist2D->GetXaxis()->SetTitleOffset(1.05);
  hist2D->GetXaxis()->SetRangeUser(10,499);
  hist2D->GetYaxis()->SetRangeUser(10,499);
  hist2D->SetMarkerSize(1);
  hist2D->Draw("COLZ TEXT");
  titleLatex->Draw("same");
  // horizontal lines
  for (int ig=1; ig<=_nBinsGen; ig++){
    TLine* line = new TLine(_phoPtLimitsRec[0],_phoPtLimitsGen[ig],500,_phoPtLimitsGen[ig]);
    line->Draw("same");
  }//end of loop over ig
  // vertical lines
  for (int ir=1; ir<=_nBinsRec; ir++){
    TLine* line = new TLine(_phoPtLimitsRec[ir],_phoPtLimitsGen[0],_phoPtLimitsRec[ir],500);
    line->Draw("same");
  }//end of loop over ig
  TString strDir=_config.GetPlotsDirName(_channel,_vgamma,_config.PLOTS_CONSTANTS);
  saveName=strDir+saveName+TString("_")+strAnnex;
  canv->SaveAs(saveName+TString(".png"));
  canv->SaveAs(saveName+TString(".pdf"));
  canv->SaveAs(saveName+TString(".root"));
  return 1;
}//end of PlotMatrixAsTH2D
