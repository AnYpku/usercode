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
  ApplyRooUnfold(_histYieldsRec,unfYieldsRooUnfInv,RooUnfold::kInvert,errCovStatInv, errStatInv, errSystInv,errCovStatVInv);

  TH1D* unfYieldsRooUnfDAg = new TH1D("unfYieldsDAg","unfolded yields D'Agostini",_nBinsGen,_phoPtLimitsGen);
  TMatrixD errCovStatDAg(_nBinsGen,_nBinsGen);
  TVectorD errStatDAg(_nBinsGen);
  TVectorD errSystDAg(_nBinsGen);
  TVectorD errCovStatVDAg(_nBinsGen);
//  ApplyRooUnfold(_histYieldsRec,unfYieldsRooUnfDAg,RooUnfold::kBayes);
  ApplyRooUnfold(_histYieldsRec,unfYieldsRooUnfDAg,RooUnfold::kBayes,errCovStatDAg, errStatDAg, errSystDAg, errCovStatVDAg);
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
  std::cout<<"Stat Covariance Matrix, RooUnf inversion"<<std::endl;  
  for (int i=0; i<_nBinsGen; i++){
    for (int j=0; j<_nBinsGen; j++){
//      std::cout<<std::setw(5)<<std::setprecision(0)<<errCovStatInv(i,j)<<" ";
    }
//    std::cout<<"; diag: "<<std::setw(4)<<std::setprecision(0)<<errCovStatVInv(i);
    std::cout<<std::endl;
  }

  std::cout<<std::endl;
  std::cout<<"Stat Covariance Matrix, RooUnf D'Agostini"<<std::endl;  
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
  
  _histMigrMatrixNotNormalized = new TH2D("migrNotNorm","_histMigrMatrixNotNormalized",_nBinsRec,_phoPtLimitsRec,_nBinsGen,_phoPtLimitsGen);
  _histMigrMatrixNotNormalized->Sumw2();
  _histEventCountMigrMatrix = new TH2D("migrMatrix","Migration Matrix: Counts (no weights)",_nBinsRec,_phoPtLimitsRec,_nBinsGen,_phoPtLimitsGen);

  _histYieldsRec = new TH1D("yieldsRec","yields rec",_nBinsRec,_phoPtLimitsRec);
  _histYieldsRec->Sumw2();
  _histYieldsRecSmeared = new TH1D("yieldsRecSmeared","yields rec smeared",_nBinsRec,_phoPtLimitsRec);
    //the smeared histogram is prepared in order to test the procedure on the 
    //signal MC like if it were data;
    //that is why the errors on rec smeared are sqrt(N)
  _histYieldsGen = new TH1D("yieldsGen","yields gen",_nBinsGen,_phoPtLimitsGen);
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

bool Unfolding::ApplyRooUnfold(TH1D* histInputYields, TH1D* unfoldedYields, RooUnfold::Algorithm alg)
{  
  std::cout<<"#######################"<<std::endl;
  std::cout<<"DoRooUnfold() starts here:"<<std::endl<<std::endl;

  TMatrixD errCovStat(_nBinsGen,_nBinsGen);
  TVectorD errStatV(_nBinsGen);
  TVectorD errSystV(_nBinsGen);
  TVectorD errCovStatV(_nBinsGen);

  bool isOk = ApplyRooUnfold(histInputYields,unfoldedYields,alg,errCovStat,errStatV, errSystV, errCovStatV);

  std::cout<<std::endl<<"DoRooUnfold() ends here"<<std::endl;
  std::cout<<"#######################"<<std::endl;
  return isOk;
}// end of ApplyRooUnfold(...)

bool Unfolding::ApplyRooUnfold(TH1D* histInputYields, TH1D* unfoldedYields, RooUnfold::Algorithm alg, TMatrixD& errCovStat, TVectorD& errStatV, TVectorD& errSystV, TVectorD& errCovStatV)
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

  std::cout<<"Input yields:"<<std::endl;
  for (int i=1; i<=_nBinsGen; i++){
    std::cout<<std::setprecision(0)<<histInputYields->GetBinContent(i)<<"+-"<<std::setprecision(0)<<histInputYields->GetBinError(i)<<std::endl;
  }
  std::cout<<"Unfolded yields:"<<std::endl;
  for (int i=1; i<=_nBinsGen; i++){
    std::cout<<std::setprecision(0)<<histUnfoldedYields->GetBinContent(i)<<"+-"<<std::setprecision(0)<<histUnfoldedYields->GetBinError(i)<<"+-"<<std::setprecision(0)<<errSyst[i];
    std::cout<<" = "<<std::setprecision(0)<<unfoldedYields->GetBinContent(i)<<"+-"<<std::setprecision(0)<<unfoldedYields->GetBinError(i)<<std::endl;
  }

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

bool Unfolding::PlotAndStore()
{

  _fOut->cd();

  float limsRecDraw[_nBinsRec+1];
  float limsGenDraw[_nBinsGen+1];
 

   if (_phoPtLimitsRec[0]==0) limsRecDraw[0]= _phoPtLimitsRec[1]/2;
  else limsRecDraw[0]= _phoPtLimitsRec[0];
  if (_phoPtLimitsGen[0]==0) limsGenDraw[0]= _phoPtLimitsGen[1]/2;
  else limsGenDraw[0]= _phoPtLimitsGen[0];
  for (int ir=1; ir<=_nBinsRec; ir++)
    limsRecDraw[ir]= _phoPtLimitsRec[ir];
  for (int ig=1; ig<=_nBinsRec; ig++)
    limsGenDraw[ig]= _phoPtLimitsGen[ig];

  TH2D* histEventCountDraw = new TH2D("histEventCount","Migration Matrix: Counts (no weights)",_nBinsRec,limsRecDraw,_nBinsGen,limsGenDraw);
  TH2D* histMigrMatrixDraw = new TH2D("histMigrMatrix","Migration Matrix: no normalization",_nBinsRec,limsRecDraw,_nBinsGen,limsGenDraw);


  for (int ir=1; ir<_nBinsRec+1; ir++)
    for (int ig=1; ig<_nBinsGen+1; ig++){
      histEventCountDraw->SetBinContent(ir,ig,_histEventCountMigrMatrix->GetBinContent(ir,ig));
      histMigrMatrixDraw->SetBinContent(ir,ig,_histMigrMatrixNotNormalized->GetBinContent(ir,ig));

    }
 
  TStyle* style = new TStyle("style","style");

  style->SetPalette(1);
  style->SetPaintTextFormat("0.1f");

  style->SetPaintTextFormat("g");
 
  TString strAffixTitle=TString("_")+_config.StrChannel(_channel)+TString("_")+_config.StrVgType(_vgamma)+TString("_");
  
  TCanvas* cEventCount = new TCanvas(TString("cEventCount")+strAffixTitle,TString("cEventCount")+strAffixTitle);
  cEventCount->SetLogx();
  cEventCount->SetLogy();
  histEventCountDraw->SetStats(0);
  histEventCountDraw->GetXaxis()->SetMoreLogLabels(1);
  histEventCountDraw->GetYaxis()->SetMoreLogLabels(1);
  histEventCountDraw->GetXaxis()->SetNoExponent(1);
  histEventCountDraw->GetYaxis()->SetNoExponent();
  histEventCountDraw->SetMarkerSize(1);
  histEventCountDraw->Draw("COLZ TEXT");

  TCanvas* cMigrMatrix = new TCanvas(TString("cMigrMatrix")+strAffixTitle,TString("cMigrMatrix")+strAffixTitle);
  cMigrMatrix->SetLogx();
  cMigrMatrix->SetLogy();
  histMigrMatrixDraw->SetStats(0);
  histMigrMatrixDraw->GetXaxis()->SetMoreLogLabels(1);
  histMigrMatrixDraw->GetYaxis()->SetMoreLogLabels(1);
  histMigrMatrixDraw->GetXaxis()->SetNoExponent(1);
  histMigrMatrixDraw->GetYaxis()->SetNoExponent();
  histMigrMatrixDraw->SetMarkerSize(1);
  histMigrMatrixDraw->Draw("COLZ TEXT");


  TCanvas* cPtSpectra = new TCanvas(TString("cPtSpectra")+strAffixTitle,TString("cPtSpectra")+strAffixTitle);
  cPtSpectra->SetLogx();
  cPtSpectra->SetLogy();
  _histYieldsRec->GetXaxis()->SetMoreLogLabels();
  _histYieldsRec->GetXaxis()->SetNoExponent();
  _histYieldsRec->SetLineColor(1);
  _histYieldsRec->SetLineWidth(2);
  _histYieldsRec->Draw();
  _histYieldsGen->SetLineColor(4);
  _histYieldsGen->SetLineWidth(2);
  _histYieldsGen->Draw("same");
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

  return 1;
}
