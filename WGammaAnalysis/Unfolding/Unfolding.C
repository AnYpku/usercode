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
#include "../Include/PhosphorCorrectorFunctor.hh"
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

Unfolding::Unfolding(int channel, string configFile)
{
  _channel=channel; 
  fIn_ = new TFile(config_.GetSelectedName(config_.FULLY,_channel,config_.SIGMC));  

  fOut_ = new TFile("fOut.root","recreate");

//  fOut_ = new TFile(config_.GetUnfoldingFileName(_channel),"recreate");
//  if (fOut_->IsOpen()) fOut_->cd();
//  else {
//    std::cout<<"ERROR in Unfolding::PrepareUnfoldingMatrix: file "<<config_.GetUnfoldingFileName(_channel)<<" is not open"<<std::endl;
//  }
  INPUT_ = new TAllInputSamples(channel, configFile);

}

Unfolding::~Unfolding()
{
//  if (fIn_->IsOpen()) fIn_->Close();
//  if (fOut_->IsOpen()) fOut_->Close();
//  delete fIn_;
//  delete fOut_;
//  delete INPUT_;
}

bool Unfolding::TestDifferentMethods()
{
  float binLimsRec[nBinsRec_+1]; 
  float binLimsGen[nBinsGen_+1]; 
  FillBinLimits(nBinsRec_, vecPhoPtLimitsRec_, binLimsRec);
  FillBinLimits(nBinsGen_, vecPhoPtLimitsGen_, binLimsGen);

  TH1D* dataSignYields = new TH1D("data","data yields  for test",nBinsGen_,binLimsGen);

//  dataSignYields->SetBinContent(0,histYieldsRecSmeared_->GetBinContent(0));
//  dataSignYields->SetBinError(0,histYieldsRecSmeared_->GetBinError(0));

  dataSignYields->SetBinContent(0,0);
  dataSignYields->SetBinError(0,0);

  dataSignYields->SetBinContent(1,94400-74200);
  dataSignYields->SetBinError(1,sqrt(330*330+300*300));

  dataSignYields->SetBinContent(2,50600-28300);
  dataSignYields->SetBinError(2,sqrt(200*200+220*220));

  dataSignYields->SetBinContent(3,32120-12000);
  dataSignYields->SetBinError(3,sqrt(2700*2700+180*180));

  dataSignYields->SetBinContent(4,23000-8360);
  dataSignYields->SetBinError(4,sqrt(110*110+150*150));

  dataSignYields->SetBinContent(5,16600-4880);
  dataSignYields->SetBinError(5,sqrt(90*90+130*130));

  dataSignYields->SetBinContent(6,24850-7950);
  dataSignYields->SetBinError(6,sqrt(160*160+110*110));

  dataSignYields->SetBinContent(7,6760-1450);
  dataSignYields->SetBinError(7,sqrt(220*220+80*80));

  dataSignYields->SetBinContent(8,5655-970);
  dataSignYields->SetBinError(8,sqrt(75*75+40*40));

  dataSignYields->SetBinContent(9,319-28);
  dataSignYields->SetBinError(9,sqrt(18*18+4*4));
  

  TH1D* unfYieldsRooUnfInv = new TH1D("unfYieldsInv","unfolded yields Inversion",nBinsGen_,binLimsGen);
  TMatrixD errCovStatInv(nBinsGen_,nBinsGen_);
  TVectorD errStatInv(nBinsGen_);
  TVectorD errSystInv(nBinsGen_);
  TVectorD errCovStatVInv(nBinsGen_);
//  ApplyRooUnfold(histYieldsRecSmeared_,unfYieldsRooUnfInv,RooUnfold::kInvert,errCovStatInv, errStatInv, errSystInv,errCovStatVInv);

  TH1D* unfYieldsRooUnfDAg = new TH1D("unfYieldsDAg","unfolded yields D'Agostini",nBinsGen_,binLimsGen);
  TMatrixD errCovStatDAg(nBinsGen_,nBinsGen_);
  TVectorD errStatDAg(nBinsGen_);
  TVectorD errSystDAg(nBinsGen_);
  TVectorD errCovStatVDAg(nBinsGen_);
  ApplyRooUnfold(histYieldsRecSmeared_,unfYieldsRooUnfDAg,RooUnfold::kBayes,errCovStatDAg, errStatDAg, errSystDAg, errCovStatVDAg);
//  ApplyRooUnfold(histYieldsRecSmeared_,unfYieldsRooUnfDAg,RooUnfold::kBayes,errCovStatDAg, errStatDAg, errSystDAg, errCovStatVDAg);
//  ApplyRooUnfold(histYieldsRecSmeared_,unfYieldsRooUnfDAg,RooUnfold::kBayes,errCovStatDAg, errStatDAg, errSystDAg, errCovStatVDAg);

  std::cout<<std::endl;
  std::cout<<"   limits;      RooUnf inv;       RooUnf D'Ag"<<std::endl;  

  std::cout<<std::endl;
  
  for (int i=1; i<=nBinsGen_; i++){
    std::cout<<std::setw(3)<<std::setprecision(0)<<binLimsGen[i-1]<<" - "<<std::setw(3)<<std::setprecision(0)<<binLimsGen[i]<<" GeV: ";
      //limits

    std::cout<<std::setw(5)<<std::setprecision(0)<<unfYieldsRooUnfInv->GetBinContent(i)<<"+-"<<std::setw(4)<<std::setprecision(0)<<errStatInv(i-1)<<"+-"<<std::setw(4)<<std::setprecision(0)<<errSystInv(i-1)<<"=";
    std::cout<<std::setw(5)<<std::setprecision(0)<<unfYieldsRooUnfInv->GetBinContent(i)<<"+-"<<unfYieldsRooUnfInv->GetBinError(i)<<";     ";
      //inversion

    std::cout<<std::setw(5)<<std::setprecision(0)<<unfYieldsRooUnfDAg->GetBinContent(i)<<"+-"<<std::setw(4)<<std::setprecision(0)<<errStatDAg(i-1)<<"+-"<<std::setw(4)<<std::setprecision(0)<<errSystDAg(i-1)<<"=";
    std::cout<<std::setw(5)<<std::setprecision(0)<<unfYieldsRooUnfDAg->GetBinContent(i)<<"+-"<<unfYieldsRooUnfDAg->GetBinError(i)<<";     ";
      //D'Agostini

    std::cout<<std::endl;
  }

  std::cout<<std::endl;
  std::cout<<"Stat Covariance Matrix, RooUnf inversion"<<std::endl;  
  for (int i=0; i<nBinsGen_; i++){
    for (int j=0; j<nBinsGen_; j++){
      std::cout<<std::setw(5)<<std::setprecision(0)<<errCovStatInv(i,j)<<" ";
    }
    std::cout<<"; diag: "<<std::setw(4)<<std::setprecision(0)<<errCovStatVInv(i);
    std::cout<<std::endl;
  }

  std::cout<<std::endl;
  std::cout<<"Stat Covariance Matrix, RooUnf D'Agostini"<<std::endl;  
  for (int i=0; i<nBinsGen_; i++){
    for (int j=0; j<nBinsGen_; j++){
      std::cout<<std::setw(5)<<std::setprecision(0)<<errCovStatDAg(i,j)<<" ";
    }
    std::cout<<"; diag: "<<std::setw(4)<<std::setprecision(0)<<errCovStatVDAg(i);
    std::cout<<std::endl;
  }

  delete unfYieldsRooUnfInv;
  delete unfYieldsRooUnfDAg;

  return 1;
}

bool Unfolding::PrepareMigrationMatrix()
{

  tr_ = (TTree*)fIn_->Get("selectedEvents");
  Float_t phoEtRec;
  Float_t phoEtGen;
  Float_t weight;
  TBranch* b_phoEtRec;
  TBranch* b_phoEtGen;
  TBranch* b_weight;
  tr_->SetBranchAddress("phoEt",&phoEtRec,&b_phoEtRec);
  tr_->SetBranchAddress("phoGenEt",&phoEtGen,&b_phoEtGen);
  tr_->SetBranchAddress("weight",&weight,&b_weight);

  vecPhoPtLimitsRec_ = config_.GetPhoPtUnfBinsLimits(0);
  nBinsRec_ = config_.GetNPhoPtUnfBins(0);

  TString strOverflowRec = "phoEt>";
  strOverflowRec+=vecPhoPtLimitsRec_[nBinsRec_];
  isOverflowRec_ = tr_->GetEntries(strOverflowRec);
  TString strOverflowGen = "phoGenEt>";
  strOverflowGen+=vecPhoPtLimitsRec_[nBinsRec_];
  isOverflowGen_ = tr_->GetEntries(strOverflowGen);
  vecPhoPtLimitsRec_ = config_.GetPhoPtUnfBinsLimits(isOverflowRec_);
  nBinsRec_ = config_.GetNPhoPtUnfBins(isOverflowRec_);
  vecPhoPtLimitsGen_ = config_.GetPhoPtUnfBinsLimits(isOverflowGen_);
  nBinsGen_ = config_.GetNPhoPtUnfBins(isOverflowGen_);

  std::cout<<"tr->GetEntries("<<strOverflowRec<<")="<<isOverflowRec_<<", mBinsRec_="<<nBinsRec_<<std::endl;
  std::cout<<"tr->GetEntries("<<strOverflowGen<<")="<<isOverflowGen_<<", mBinsGen_="<<nBinsGen_<<std::endl;

  //good up to here

  float binLimsRec[nBinsRec_+1]; 
  float binLimsGen[nBinsGen_+1]; 
  FillBinLimits(nBinsRec_, vecPhoPtLimitsRec_, binLimsRec);
  FillBinLimits(nBinsGen_, vecPhoPtLimitsGen_, binLimsGen);

  fOut_->cd();

  histMigrMatrixNotNormalized_ = new TH2D("migrNotNorm","histMigrMatrixNotNormalized_",nBinsRec_,binLimsRec,nBinsGen_,binLimsGen);
  histMigrMatrixNotNormalized_->Sumw2();
  histEventCountMigrMatrix_ = new TH2D("migrMatrix","Migration Matrix: Counts (no weights)",nBinsRec_,binLimsRec,nBinsGen_,binLimsGen);

  histYieldsRec_ = new TH1D("yieldsRec","yields rec",nBinsRec_,binLimsRec);
  histYieldsRec_->Sumw2();
  histYieldsRecSmeared_ = new TH1D("yieldsRecSmeared","yields rec smeared",nBinsRec_,binLimsRec);
    //the smeared histogram is prepared in order to test the procedure on the 
    //signal MC like if it were data;
    //that is why the errors on rec smeared are sqrt(N)
  histYieldsGen_ = new TH1D("yieldsGen","yields gen",nBinsGen_,binLimsGen);
  histYieldsGen_->Sumw2();

  
 
  int nEntries = tr_->GetEntries();
  
  //filling out 
  // - generated yields vector (histYieldsGen_)
  // - reconstructed yields vector (histYieldsRec_)
  // - migration matrix (histMigrMatrixNotNormalized_)
  // - migration counts matrix (histEventCountMigrMatrix_)
  for (int entry=0; entry<nEntries; entry++){
    b_phoEtRec->GetEntry(entry);
    b_phoEtGen->GetEntry(entry);
    b_weight->GetEntry(entry);
    if (phoEtGen<0 || phoEtRec<0) continue;

    histEventCountMigrMatrix_->Fill(phoEtRec,phoEtGen,1);
    histMigrMatrixNotNormalized_->Fill(phoEtRec,phoEtGen,weight);
    histYieldsGen_->Fill(phoEtGen,weight);
    histYieldsRec_->Fill(phoEtRec,weight);
  } // loop over entries

  std::cout<<std::endl;
  std::cout<<"contents and errors before normalization"<<std::endl;
  for (int i=0; i<nBinsRec_; i++){
    TRandom rnd;
    double cont = rnd.Gaus(histYieldsRec_->GetBinContent(i+1),histYieldsRec_->GetBinError(i+1));
    histYieldsRecSmeared_->SetBinContent(i+1,cont);
    histYieldsRecSmeared_->SetBinError(i+1,sqrt(cont));
      //the smeared histogram is prepared in order to test the procedure on the 
      //signal MC like if it were data;
      //that is why the errors on rec smeared are sqrt(N)
    for (int j=0; j<nBinsGen_; j++){
      std::cout<<std::fixed << std::setw(4) << std::setprecision(2)<<histMigrMatrixNotNormalized_->GetBinContent(i+1,j+1)<<"+-"<<std::fixed << std::setw(4)<< std::setprecision(2)<<histMigrMatrixNotNormalized_->GetBinError(i+1,j+1)<<" ";
    }
    std::cout<<std::endl;
  }
  std::cout<<std::endl;

  return 1;
}





bool Unfolding::ApplyRooUnfold(TH1D* histInputYields, TH1D* unfoldedYields, RooUnfold::Algorithm alg,TMatrixD& errCovStat, TVectorD& errStatV, TVectorD& errSystV, TVectorD& errCovStatV)
{  
  std::cout<<"#######################"<<std::endl;
  std::cout<<"DoRooUnfold() starts here:"<<std::endl<<std::endl;

  float binLimsRec[nBinsRec_+1]; 
  float binLimsGen[nBinsGen_+1]; 
  FillBinLimits(nBinsRec_, vecPhoPtLimitsRec_, binLimsRec);
  FillBinLimits(nBinsGen_, vecPhoPtLimitsGen_, binLimsGen);

  RooUnfoldResponse response(histYieldsRec_,histYieldsGen_,histMigrMatrixNotNormalized_,"response","RooUnfoldResponse matrix");
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

  float errSyst[nBinsGen_+1];
  for (int i=1; i<nBinsGen_+1; i++)
    errSyst[i]=0;

  int NSmears=1000;
  ComputeSystErrors(histInputYields, errSyst, alg, NSmears);
//  ComputeStatErrors(histInputYields, errStat, alg, NSmears);
//  ComputeStatErrors(histInputYields, errSyst, alg, NSmears);

  std::cout<<"Unfolded yiedls:"<<std::endl;
  for (int i=1; i<=nBinsGen_; i++){
    std::cout<<std::setprecision(0)<<histUnfoldedYields->GetBinContent(i)<<"+-"<<std::setprecision(0)<<histUnfoldedYields->GetBinError(i)<<"+-"<<std::setprecision(0)<<errSyst[i];
    unfoldedYields->SetBinContent(i,histUnfoldedYields->GetBinContent(i));
    unfoldedYields->SetBinError(i,sqrt(histUnfoldedYields->GetBinError(i)*histUnfoldedYields->GetBinError(i)+errSyst[i]*errSyst[i]));
    errStatV[i-1]=histUnfoldedYields->GetBinError(i);
    errSystV[i-1]=errSyst[i];
    std::cout<<" = "<<std::setprecision(0)<<unfoldedYields->GetBinContent(i)<<"+-"<<std::setprecision(0)<<unfoldedYields->GetBinError(i)<<std::endl;
  }

  std::cout<<std::endl<<"DoRooUnfold() ends here"<<std::endl;
  std::cout<<"#######################"<<std::endl;
  return 1;
}

bool Unfolding::ComputeSystErrors(TH1D* histInputYields, float* errSyst, RooUnfold::Algorithm alg, int NSmears)
{

  //Compute errors due to finite MC statistics
  float binLimsRec[nBinsRec_+1]; 
  float binLimsGen[nBinsGen_+1]; 
  FillBinLimits(nBinsRec_, vecPhoPtLimitsRec_, binLimsRec);
  FillBinLimits(nBinsGen_, vecPhoPtLimitsGen_, binLimsGen);

  TString name="n";
  name+=alg;
  name+=NSmears;
  name+="migr";

  TH2D* histMigrMatrixSmeared = new TH2D(name,name,nBinsRec_,binLimsRec,nBinsGen_,binLimsGen);
  name.ReplaceAll("migr","rec");
  TH1D* histYieldsRecSmeared = new TH1D(name,name,nBinsRec_,binLimsRec);
  name.ReplaceAll("rec","gen");
  TH1D* histYieldsGenSmeared = new TH1D(name,name,nBinsGen_,binLimsGen);

//  TH2D* histMigrMatrixSmeared = (TH2D*)histMigrMatrixNotNormalized_->Clone();
// TH1D* histYieldsRecSmeared = (TH1D*)histYieldsRec_->Clone();
//  TH1D* histYieldsGenSmeared = (TH1D*)histYieldsGen_->Clone();


  TRandom rnd;

  double SumValsSqr[nBinsGen_+1];
  double SumVals[nBinsGen_+1];
  double ExSqr[nBinsGen_+1];
  double Ex[nBinsGen_+1];
  for (int i=1; i<=nBinsGen_; i++){
    SumValsSqr[i]=0;
    SumVals[i]=0;
    ExSqr[i]=0;
    Ex[i]=0;
  }

  for (int iSm=0; iSm<NSmears; iSm++){

    for (int irec=1; irec<=nBinsRec_; irec++)
      for (int igen=1; igen<=nBinsGen_; igen++){
        double cont = rnd.Gaus(histMigrMatrixNotNormalized_->GetBinContent(irec,igen),histMigrMatrixNotNormalized_->GetBinError(irec,igen));
        histMigrMatrixSmeared->SetBinContent(irec,igen,cont);
      }
    for (int irec=1; irec<=nBinsRec_; irec++){
      double recYield=0;
      for (int igen=1; igen<=nBinsGen_; igen++){
        recYield+=histMigrMatrixSmeared->GetBinContent(irec,igen);
      }   
      histYieldsRecSmeared->SetBinContent(irec,recYield);
    }     
    for (int igen=1; igen<=nBinsGen_; igen++){
      double genYield=0;
      for (int irec=1; irec<=nBinsRec_; irec++){
        genYield+=histMigrMatrixSmeared->GetBinContent(irec,igen);
      }   
      histYieldsGenSmeared->SetBinContent(igen,genYield);
    }  
    RooUnfoldResponse responseSmeared(histYieldsRecSmeared,histYieldsGenSmeared,histMigrMatrixSmeared);
    RooUnfold* unfoldSmeared = RooUnfold::New(alg,&responseSmeared,histInputYields);

    TH1D* histUnfSmeared = (TH1D*) unfoldSmeared->Hreco(RooUnfold::kNoError);
    for (int igen=1; igen<=nBinsGen_; igen++){
      double val = histUnfSmeared->GetBinContent(igen);
      SumVals[igen]+=val;
      SumValsSqr[igen]+=val*val;
    }

  } //end of loop over smears

  for (int i=1; i<=nBinsGen_; i++){
    Ex[i]=SumVals[i]/NSmears;
    ExSqr[i]=SumValsSqr[i]/NSmears;
    errSyst[i] =  sqrt(ExSqr[i]-Ex[i]*Ex[i]);
  }

  return 1;
}

bool Unfolding::ComputeStatErrors(TH1D* histInputYields, float* errStat, RooUnfold::Algorithm alg, int NSmears)
{

  //Compute errors due to finite MC statistics
  float binLimsRec[nBinsRec_+1]; 
  float binLimsGen[nBinsGen_+1]; 
  FillBinLimits(nBinsRec_, vecPhoPtLimitsRec_, binLimsRec);
  FillBinLimits(nBinsGen_, vecPhoPtLimitsGen_, binLimsGen);

  TString name="n";
  name+=alg;
  name+=NSmears;
  name+="input";

  TH1D* histInputYieldsSmeared = new TH1D(name,name,nBinsGen_,binLimsGen);


  TRandom rnd;

  double SumValsSqr[nBinsGen_+1];
  double SumVals[nBinsGen_+1];
  double ExSqr[nBinsGen_+1];
  double Ex[nBinsGen_+1];
  for (int i=1; i<=nBinsGen_; i++){
    SumValsSqr[i]=0;
    SumVals[i]=0;
    ExSqr[i]=0;
    Ex[i]=0;
  }

  for (int iSm=0; iSm<NSmears; iSm++){
      for (int igen=1; igen<=nBinsGen_; igen++){
        double cont = rnd.Gaus(histInputYields->GetBinContent(igen),histInputYields->GetBinError(igen));
        histInputYieldsSmeared->SetBinContent(igen,cont);
      }
      RooUnfoldResponse response(histYieldsRec_,histYieldsGen_,histMigrMatrixNotNormalized_,"response","RooUnfoldResponse matrix");
    RooUnfold* unfoldSmeared = RooUnfold::New(alg,&response,histInputYieldsSmeared);

    TH1D* histUnfSmeared = (TH1D*) unfoldSmeared->Hreco(RooUnfold::kNoError);
    for (int igen=1; igen<=nBinsGen_; igen++){
      double val = histUnfSmeared->GetBinContent(igen);
      SumVals[igen]+=val;
      SumValsSqr[igen]+=val*val;
    }

  } //end of loop over smears

  for (int i=1; i<=nBinsGen_; i++){
    Ex[i]=SumVals[i]/NSmears;
    ExSqr[i]=SumValsSqr[i]/NSmears;
    errStat[i] =  sqrt(ExSqr[i]-Ex[i]*Ex[i]);
  }

  return 1;
}

bool Unfolding::PlotAndStore()
{

  fOut_->cd();

  float limsRecDraw[nBinsRec_+1];
  float limsGenDraw[nBinsGen_+1];
 

   if (vecPhoPtLimitsRec_[0]==0) limsRecDraw[0]= vecPhoPtLimitsRec_[1]/2;
  else limsRecDraw[0]= vecPhoPtLimitsRec_[0];
  if (vecPhoPtLimitsGen_[0]==0) limsGenDraw[0]= vecPhoPtLimitsGen_[1]/2;
  else limsGenDraw[0]= vecPhoPtLimitsGen_[0];
  for (int ir=1; ir<=nBinsRec_; ir++)
    limsRecDraw[ir]= vecPhoPtLimitsRec_[ir];
  for (int ig=1; ig<=nBinsRec_; ig++)
    limsGenDraw[ig]= vecPhoPtLimitsGen_[ig];

  TH2D* histEventCountDraw = new TH2D("histEventCount","Migration Matrix: Counts (no weights)",nBinsRec_,limsRecDraw,nBinsGen_,limsGenDraw);
  TH2D* histMigrMatrixDraw = new TH2D("histMigrMatrix","Migration Matrix: no normalization",nBinsRec_,limsRecDraw,nBinsGen_,limsGenDraw);


  for (int ir=1; ir<nBinsRec_+1; ir++)
    for (int ig=1; ig<nBinsGen_+1; ig++){
      histEventCountDraw->SetBinContent(ir,ig,histEventCountMigrMatrix_->GetBinContent(ir,ig));
      histMigrMatrixDraw->SetBinContent(ir,ig,histMigrMatrixNotNormalized_->GetBinContent(ir,ig));

    }
    
  TStyle* style = new TStyle("style","style");

  style->SetPalette(1);
  style->SetPaintTextFormat("0.1f");

  style->SetPaintTextFormat("g");

  TCanvas* cEventCount = new TCanvas("cEventCount","Event Counts Migration Matrix");
  cEventCount->SetLogx();
  cEventCount->SetLogy();
  histEventCountDraw->SetStats(0);
  histEventCountDraw->GetXaxis()->SetMoreLogLabels(1);
  histEventCountDraw->GetYaxis()->SetMoreLogLabels(1);
  histEventCountDraw->GetXaxis()->SetNoExponent(1);
  histEventCountDraw->GetYaxis()->SetNoExponent();
  histEventCountDraw->SetMarkerSize(1);
  histEventCountDraw->Draw("COLZ TEXT");

  TCanvas* cMigrMatrix = new TCanvas("cMigrMatrix","Event Counts Migration Matrix");
  cMigrMatrix->SetLogx();
  cMigrMatrix->SetLogy();
  histMigrMatrixDraw->SetStats(0);
  histMigrMatrixDraw->GetXaxis()->SetMoreLogLabels(1);
  histMigrMatrixDraw->GetYaxis()->SetMoreLogLabels(1);
  histMigrMatrixDraw->GetXaxis()->SetNoExponent(1);
  histMigrMatrixDraw->GetYaxis()->SetNoExponent();
  histMigrMatrixDraw->SetMarkerSize(1);
  histMigrMatrixDraw->Draw("COLZ TEXT");


  TCanvas* cPtSpectra = new TCanvas("cPtSpectra","cPtSpectra");
  cPtSpectra->SetLogx();
  cPtSpectra->SetLogy();
  histYieldsRec_->SetLineColor(1);
  histYieldsRec_->SetLineWidth(2);
  histYieldsRec_->Draw();
  histYieldsGen_->SetLineColor(4);
  histYieldsGen_->SetLineWidth(2);
  histYieldsGen_->Draw("same");

  int nBinsAnalysis = config_.GetNPhoPtBins(); 
  vector <float> limsAnalysis = config_.GetPhoPtBinsLimits();
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
    std::cout<<"tr_->Draw(\""<<strDraw<<"\",\""<<strCut<<"\");"<<std::endl;
 
    tr_->SetLineWidth(2);
    tr_->SetLineColor(ib+1);

    TString canvName="cResolution";
    canvName+=ib;
    cResolution[ib] = new TCanvas(canvName,"pho Pt resolution in different bins");
    cResolution[ib]->SetLogy();
    tr_->Draw(strDraw,strCut);
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
//      std::cout<<"tr_->Draw(\""<<strDraw<<"\",\""<<strCut<<"\");"<<std::endl;
 
      tr_->SetLineWidth(2);
      tr_->SetLineColor(ib2+1);

      tr_->Draw(strDraw2,strCut2,"same");
    }
  }


//  histYieldsRec_->Write(config_.GetYieldsRec1DName());
//  histYieldsGen_->Write(config_.GetYieldsGen1DName());
  cEventCount->Write("cEventCount");
  cMigrMatrix->Write("cMigrMatrix");

  return 1;
}

bool Unfolding::FillBinLimits(int nBins, vector<float> vecLims, float* lims){
  if (vecLims.size()!=(unsigned int)nBins+1){
    std::cout<<"ERROR in Unfolding::FillBinLimits: (vecLims.size()="<<vecLims.size()<<") != (nBins+1 = "<<nBins+1<<" ) "<<std::endl;
    return 0;
  }
  for (int i=0; i<nBins+1; i++){
    lims[i]=vecLims[i];
  }
  return 1;
}
