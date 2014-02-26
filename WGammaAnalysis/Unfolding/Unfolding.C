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

  INPUT_ = new TAllInputSamples(channel, configFile);
  channel_=channel; 
  fIn_ = new TFile(config_.GetSelectedName(config_.FULLY,channel_,config_.SIGMC));
  fOut_ = new TFile(config_.GetUnfoldingFileName(channel),"recreate");
  if (fOut_->IsOpen()) fOut_->cd();
  else {
    std::cout<<"ERROR in Unfolding::PrepareUnfoldingMatrix: file "<<fOut_<<" is not open"<<std::endl;
  }
}

Unfolding::~Unfolding()
{
  if (fIn_->IsOpen()) fIn_->Close();
  if (fOut_->IsOpen()) fOut_->Close();
  delete fIn_;
  delete fOut_;
  delete INPUT_;
}

bool Unfolding::PrepareConstants()
{
  if (!PrepareMigrationMatrix()) return 0;
  if (!PrepareUnfoldingMatrix()) return 0;
  return 1;
}

bool Unfolding::ApplyUnfolding(TH1D* inputYields, TH1D* unfoldedYields, int method)
{

//     int {LOCAL_INVERSION, LOCAL_DAGOSTINI, RooUnfold_INVERSION, RooUnfold_DAGOSTINI};
  if (method==LOCAL_INVERSION)
    if (ApplyUnfoldingInversion(inputYields,unfoldedYields)) return 1;
    else return 0;
  if (method==LOCAL_DAGOSTINI)
    if (ApplyUnfoldingDAgostini(inputYields,unfoldedYields)) return 1;
    else return 0;
  if (method==LOCAL_DAGOSTINI_NOITERATIONS)
    if (ApplyUnfoldingDAgostiniNoIterations(inputYields,unfoldedYields)) return 1;
    else return 0;
  if (method==RooUnfold_INVERSION)
    if (ApplyRooUnfold(inputYields,unfoldedYields,RooUnfold::kInvert)) return 1;
    else return 0;
  if (method==RooUnfold_DAGOSTINI)
    if (ApplyRooUnfold(inputYields,unfoldedYields,RooUnfold::kBayes)) return 1;
    else return 0;
  return 0;
}

bool Unfolding::TestDifferentMethods()
{

  for (int igen=1; igen<=nBinsGen_; igen++){
    for (int imeth=LOCAL_INVERSION; imeth<=RooUnfold_DAGOSTINI;imeth++){
      _unfolded[imeth][igen]=0;
      _unfoldedErrStat[imeth][igen]=0;
      _unfoldedErrSyst[imeth][igen]=0;
    }
    std::cout<<std::endl;
  }

  TH1D* unfYieldsInv = new TH1D("unfYieldsInv","unfolded yields inversion",nBinsGen_,binLimsGen_);
//  ApplyUnfoldingInversion(histYieldsRecSmeared_,unfYieldsInv);

  TH1D* unfYieldsDAg = new TH1D("unfYieldsDAg","unfolded yields D'Agostini",nBinsGen_,binLimsGen_);
//  ApplyUnfoldingDAgostini(histYieldsRecSmeared_,unfYieldsDAg);

  TH1D* unfYieldsDAgNoIter = new TH1D("unfYieldsDAgNoIter","unfolded yields D'Agostini, no iterations",nBinsGen_,binLimsGen_);
//  ApplyUnfoldingDAgostiniNoIterations(histYieldsRecSmeared_,unfYieldsDAgNoIter);

  TH1D* unfYieldsRooUnfInv = new TH1D("unfYieldsDAg","unfolded yields D'Agostini",nBinsGen_,binLimsGen_);
  ApplyRooUnfold(histYieldsRecSmeared_,unfYieldsRooUnfInv,RooUnfold::kInvert);

  TH1D* unfYieldsRooUnfDAg = new TH1D("unfYieldsDAg","unfolded yields D'Agostini",nBinsGen_,binLimsGen_);
  ApplyRooUnfold(histYieldsRecSmeared_,unfYieldsRooUnfDAg,RooUnfold::kBayes);

  std::cout<<std::endl;
  std::cout<<"   limits;     Gen yields;    Rec yields;  Rec smeared; inversion;    D'Agostini;  D'Ag, no iters;  RooUnf inv; RooUnf D'Ag"<<std::endl;  
  //std::cout<<"   limits;     Gen yields;    Rec yields;  Rec smeared; inversion; RooUnf inv; RooUnf D'Ag"<<std::endl;
  std::cout<<std::endl;
  
  for (int i=1; i<=nBinsGen_; i++){
    std::cout<<std::setw(3)<<std::setprecision(0)<<binLimsGen_[i-1]<<" - "<<std::setw(3)<<std::setprecision(0)<<binLimsGen_[i]<<" GeV: ";
    std::cout<<std::setw(5)<<std::setprecision(0)<<histYieldsGen_->GetBinContent(i)<<"+-"<<std::setw(4)<<std::setprecision(0)<<histYieldsGen_->GetBinError(i)<<";  ";
    std::cout<<std::setw(5)<<std::setprecision(0)<<std::setw(4)<<std::setprecision(0)<<histYieldsRec_->GetBinContent(i)<<"+-"<<histYieldsRec_->GetBinError(i)<<";  ";
    std::cout<<std::setw(5)<<std::setprecision(0)<<histYieldsRecSmeared_->GetBinContent(i)<<"+-"<<std::setw(4)<<std::setprecision(0)<<histYieldsRecSmeared_->GetBinError(i)<<";  ";
    std::cout<<std::setw(5)<<std::setprecision(0)<<unfYieldsInv->GetBinContent(i)<<"+-"<<std::setw(4)<<std::setprecision(0)<<unfYieldsInv->GetBinError(i)<<";  ";
    std::cout<<std::setw(5)<<std::setprecision(0)<<unfYieldsDAg->GetBinContent(i)<<"+-"<<std::setw(4)<<std::setprecision(0)<<unfYieldsDAg->GetBinError(i)<<";  ";
    std::cout<<std::setw(5)<<std::setprecision(0)<<unfYieldsDAgNoIter->GetBinContent(i)<<"+-"<<std::setw(4)<<std::setprecision(0)<<unfYieldsDAgNoIter->GetBinError(i)<<";  ";
    std::cout<<std::setw(5)<<std::setprecision(0)<<unfYieldsRooUnfInv->GetBinContent(i)<<"+-"<<std::setw(4)<<std::setprecision(0)<<unfYieldsRooUnfInv->GetBinError(i)<<";  ";
    std::cout<<std::setw(5)<<std::setprecision(0)<<unfYieldsRooUnfDAg->GetBinContent(i)<<"+-"<<std::setw(4)<<std::setprecision(0)<<unfYieldsRooUnfDAg->GetBinError(i)<<std::endl;
  }
  delete unfYieldsInv;
  delete unfYieldsDAg;
  delete unfYieldsDAgNoIter;
  delete unfYieldsRooUnfInv;
  delete unfYieldsRooUnfDAg;

  std::cout<<std::endl;
  for (int igen=1; igen<=nBinsGen_; igen++){
    for (int imeth=LOCAL_INVERSION; imeth<=RooUnfold_DAGOSTINI;imeth++){
      std::cout<<std::setw(5)<<std::setprecision(0)<<_unfolded[imeth][igen]<<"+-";
      std::cout<<std::setw(4)<<std::setprecision(0)<<_unfoldedErrStat[imeth][igen]<<"+-";
      std::cout<<std::setw(4)<<std::setprecision(0)<<_unfoldedErrSyst[imeth][igen]<<";   ";
    }
    std::cout<<std::endl;
  }

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

  binLimsRec_ = new float(nBinsRec_+1);
  for (int i=0; i<nBinsRec_+1; i++){
    binLimsRec_[i]=vecPhoPtLimitsRec_[i];
  }
  binLimsGen_ = new float(nBinsGen_+1);
  for (int i=0; i<nBinsGen_+1; i++){
    binLimsGen_[i]=vecPhoPtLimitsGen_[i];
  }
  fOut_->cd();
  histMigrMatrixNotNormalized_ = new TH2D("migrNotNorm","histMigrMatrixNotNormalized_",nBinsRec_,binLimsRec_,nBinsGen_,binLimsGen_);
  histMigrMatrixNotNormalized_->Sumw2();
  histResponseMatrix_ = new TH2D("responseMatrix","Response Matrix, normalization for given iGen",nBinsRec_,binLimsRec_,nBinsGen_,binLimsGen_); 
  histResponseMatrix_->Sumw2();
  histUnfoMatrixDAg_ = new TH2D("unfoDAgMatrix","Unfolding D'Agostini Matrix, normalization for given iRec",nBinsGen_,binLimsGen_,nBinsRec_,binLimsRec_); 
  histUnfoMatrixDAg_->Sumw2();
  histEventCountMigrMatrix_ = new TH2D("migrMatrix","Migration Matrix: Counts (no weights)",nBinsRec_,binLimsRec_,nBinsGen_,binLimsGen_);

  histYieldsRec_ = new TH1D("yieldsRec","yields rec",nBinsRec_,binLimsRec_);
  histYieldsRec_->Sumw2();
  histYieldsRecSmeared_ = new TH1D("yieldsRecSmeared","yields rec smeared",nBinsRec_,binLimsRec_);
    //the smeared histogram is prepared in order to test the procedure on the 
    //signal MC like if it were data;
    //that is why the errors on rec smeared are sqrt(N)
  histYieldsGen_ = new TH1D("yieldsGen","yields gen",nBinsGen_,binLimsGen_);
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
    histYieldsRecSmeared_->SetBinError(i+1,sqrt(histYieldsRecSmeared_->GetBinContent(i+1)));
      //the smeared histogram is prepared in order to test the procedure on the 
      //signal MC like if it were data;
      //that is why the errors on rec smeared are sqrt(N)
    for (int j=0; j<nBinsGen_; j++){
      std::cout<<std::fixed << std::setw(4) << std::setprecision(2)<<histMigrMatrixNotNormalized_->GetBinContent(i+1,j+1)<<"+-"<<std::fixed << std::setw(4)<< std::setprecision(2)<<histMigrMatrixNotNormalized_->GetBinError(i+1,j+1)<<" ";
    }
    std::cout<<std::endl;
  }
  std::cout<<std::endl;

  //filling out normalized migration matrix (histResponseMatrix_) 
  for (int ig=1; ig<nBinsGen_+1; ig++){
    float totalGen=0;
    float sumErrorsSquared=0;

    for (int ir=1; ir<nBinsRec_+1; ir++){
      totalGen+=histMigrMatrixNotNormalized_->GetBinContent(ir,ig);
      sumErrorsSquared+=histMigrMatrixNotNormalized_->GetBinError(ir,ig)*histMigrMatrixNotNormalized_->GetBinError(ir,ig);
    }
    for (int ir=1; ir<nBinsRec_+1; ir++){
      float errSquared=0;
      float binErrorSquared=histMigrMatrixNotNormalized_->GetBinError(ir,ig)*histMigrMatrixNotNormalized_->GetBinError(ir,ig);
      float binContent=histMigrMatrixNotNormalized_->GetBinContent(ir,ig);
      if (totalGen!=0) 
        errSquared=(binErrorSquared*(totalGen+binContent)*(totalGen+binContent)+binContent*binContent*(sumErrorsSquared-binErrorSquared))/(totalGen*totalGen*totalGen*totalGen);
      if (totalGen!=0) 
histResponseMatrix_->SetBinContent(ir,ig,binContent/totalGen);
      else histResponseMatrix_->SetBinContent(ir,ig,0);
      histResponseMatrix_->SetBinError(ir,ig,sqrt(errSquared));
    }
  }

  //filling out normalized migration matrix (histUnfoMatrixDAg_) 
  for (int ir=1; ir<nBinsRec_+1; ir++){
    float totalRec=0;
    float sumErrorsSquared=0;
    for (int ig=1; ig<nBinsGen_+1; ig++){
      totalRec+=histMigrMatrixNotNormalized_->GetBinContent(ir,ig);
      sumErrorsSquared+=histMigrMatrixNotNormalized_->GetBinError(ir,ig)*histMigrMatrixNotNormalized_->GetBinError(ir,ig);
    }
    for (int ig=1; ig<nBinsGen_+1; ig++){
      float errSquared=0;
      float binErrorSquared=histMigrMatrixNotNormalized_->GetBinError(ir,ig)*histMigrMatrixNotNormalized_->GetBinError(ir,ig);
      float binContent=histMigrMatrixNotNormalized_->GetBinContent(ir,ig);
      if (totalRec!=0) 
        errSquared=(binErrorSquared*(totalRec+binContent)*(totalRec+binContent)+binContent*binContent*(sumErrorsSquared-binErrorSquared))/(totalRec*totalRec*totalRec*totalRec);
      if (totalRec!=0) 
histUnfoMatrixDAg_->SetBinContent(ig,ir,binContent/totalRec);
      else histUnfoMatrixDAg_->SetBinContent(ir,ig,0);
      histUnfoMatrixDAg_->SetBinError(ig,ir,sqrt(errSquared));
    }
  }
  
  std::cout<<std::endl;
  std::cout<<"reco yields = (response matrix 1) * gen yields"<<std::endl;
  TH1D* histMultResult1 = new TH1D("hMR1","hMR1",nBinsRec_,binLimsRec_); 
  if (!MatrixTimesVector(histResponseMatrix_, histYieldsGen_, histMultResult1,  histYieldsRec_))
    return 0;
  delete histMultResult1;
  std::cout<<"gen yields = (response matrix 2) * reco yields"<<std::endl;
  TH1D* histMultResult2 = new TH1D("hMR2","hMR2",nBinsGen_,binLimsGen_); 
  if (!MatrixTimesVector(histUnfoMatrixDAg_, histYieldsRec_, histMultResult2,  histYieldsGen_))
    return 0;
  std::cout<<std::endl;
  delete histMultResult2;

  std::cout<<std::endl;
  std::cout<<"Response matrix (nBinsRec_="<<nBinsRec_<<", nBinsGen_="<<nBinsGen_<<" ): "<<std::endl;
  for (int i=1; i<nBinsRec_+1; i++){
    for (int j=1; j<nBinsGen_+1; j++)
      std::cout<<std::fixed << std::setw(4) << std::setprecision(2)<<histResponseMatrix_->GetBinContent(i,j)<<" ";
    std::cout<<"(bin low edge: "<<histResponseMatrix_->GetBinLowEdge(i)<<")"<<std::endl;
  }
  for (int i=1; i<nBinsRec_+1; i++){
    for (int j=1; j<nBinsGen_+1; j++)
      std::cout<<std::fixed << std::setw(4) << std::setprecision(2)<<histResponseMatrix_->GetBinContent(i,j)<<"+-"<<std::fixed << std::setw(4)<< std::setprecision(2)<<histResponseMatrix_->GetBinError(i,j)<<" ";
    std::cout<<"(bin low edge: "<<histResponseMatrix_->GetBinLowEdge(i)<<")"<<std::endl;
  }
  std::cout<<std::endl;
  std::cout<<"Unfolding D'Agostini matrix (nBinsGen_="<<nBinsGen_<<", nBinsRec_="<<nBinsRec_<<" ): "<<std::endl;
  for (int i=1; i<nBinsGen_+1; i++){
    for (int j=1; j<nBinsRec_+1; j++)
      std::cout<<std::fixed << std::setw(4) << std::setprecision(2)<<histUnfoMatrixDAg_->GetBinContent(i,j)<<" ";
    std::cout<<"(bin low edge: "<<histUnfoMatrixDAg_->GetBinLowEdge(i)<<")"<<std::endl;
  }
  for (int i=1; i<nBinsGen_+1; i++){
    for (int j=1; j<nBinsRec_+1; j++)
      std::cout<<std::fixed << std::setw(4) << std::setprecision(2)<<histUnfoMatrixDAg_->GetBinContent(i,j)<<"+-"<<std::fixed << std::setw(4)<< std::setprecision(2)<<histUnfoMatrixDAg_->GetBinError(i,j)<<" ";
    std::cout<<"(bin low edge: "<<histUnfoMatrixDAg_->GetBinLowEdge(i)<<")"<<std::endl;
  }
  std::cout<<std::endl;
  return 1;
}

bool Unfolding::PrepareUnfoldingMatrix()
{

  histUnfoMatrixInv_ = new TH2D("unfoInvMatrix","Unfolding Inverted Matrix",nBinsGen_,binLimsGen_,nBinsRec_,binLimsRec_);

  TMatrixD mUnfo(nBinsGen_,nBinsRec_);
  TMatrixD mMigr(nBinsRec_,nBinsGen_);
  TMatrixD mUnfoErr(nBinsGen_,nBinsRec_);
  TMatrixD mMigrErr(nBinsRec_,nBinsGen_);
  for (int i=1; i<=nBinsRec_; i++)
    for (int j=1; j<=nBinsGen_; j++){
      mMigr(i-1,j-1)=histResponseMatrix_->GetBinContent(i,j);
      mMigrErr(i-1,j-1)=histResponseMatrix_->GetBinError(i,j);
    }

  if (!PseudoMatrixInversion(mMigr,mUnfo)) return 0;
  if (!CalculateInvertedMatrixErrors(mMigr, mMigrErr, mMigrErr, mUnfoErr)) return 0;
  for (int i=1; i<=nBinsGen_; i++)
    for (int j=1; j<=nBinsRec_; j++){
      histUnfoMatrixInv_->SetBinContent(i,j,mUnfo(i-1,j-1));
      histUnfoMatrixInv_->SetBinError(i,j,mUnfoErr(i-1,j-1));
    }
  std::cout<<"Unfolding Inverted matrix:"<<std::endl;
  for (int i=1; i<nBinsGen_+1; i++){
    for (int j=1; j<nBinsRec_+1; j++)
      std::cout<<std::fixed << std::setw(4) << std::setprecision(2) << histUnfoMatrixInv_->GetBinContent(i,j)<<"+-"<<std::fixed << std::setw(4) << std::setprecision(2) << histUnfoMatrixInv_->GetBinError(i,j)<<" ";
    std::cout<<std::endl;
  }
  std::cout<<"Unfolding Inverted Matrix closure test: vGen = U * vRec"<<std::endl;

  TH1D* histMultResult2 = new TH1D("hMR2","hMR2",nBinsGen_,binLimsGen_); 
  if (!MatrixTimesVector(histUnfoMatrixDAg_, histYieldsRec_, histMultResult2,  histYieldsGen_))
    return 0;
  std::cout<<std::endl;
  delete histMultResult2;
  return 1;
}

float Unfolding::CalculateMatrixSpectralNorm(TMatrixD &matrix){
  TMatrixD matrTransposed(matrix.GetNcols(),matrix.GetNrows());
  for (int i=0; i<matrix.GetNcols(); i++){
    for (int j=0; j<matrix.GetNrows(); j++){
      matrTransposed(i,j)=matrix(j,i);
    }
  }
  TMatrixD matrMult(matrix.GetNrows(),matrix.GetNrows());
  matrMult.Mult(matrTransposed,matrix);
  TMatrixDEigen eigenmatrix(matrMult);
  TVectorD vecRe = eigenmatrix.GetEigenValuesRe();
  TVectorD vecIm = eigenmatrix.GetEigenValuesIm();
  float lambdaMax=0;
  for (int i=0; i<vecRe.GetNrows(); i++){
    float lambdaSqr = vecRe(i)*vecRe(i)+vecIm(i)*vecIm(i); 
    if (lambdaSqr>lambdaMax) lambdaMax=lambdaSqr;
  }
  lambdaMax=sqrt(lambdaMax);
  return sqrt(lambdaMax);
}

bool Unfolding::PseudoMatrixInversion(TMatrixD& mOriginal, TMatrixD& mInverted){
  if ((mOriginal.GetNrows() != mInverted.GetNcols()) || (mOriginal.GetNcols() != mInverted.GetNrows())){
    std::cout<<"ERROR in Unfolding::PseudoMatrixInversion: wrong matrix sizes"<<std::endl;
    return 0;
  }

  float det=0;
  //for square matrix - simple inversion
  if (mOriginal.GetNrows() == mOriginal.GetNcols()){
    for (int i=0; i<mOriginal.GetNrows(); i++)
      for (int j=0; j<mOriginal.GetNcols(); j++)
        mInverted(i,j)=mOriginal(i,j);
    det = mInverted.Determinant();
    if (det!=0){
      mInverted.Invert();
      return 1;
    }
  }

  TMatrixD mT(mOriginal.GetNcols(),mOriginal.GetNrows()); 
  mT.Transpose(mOriginal);
//  for (int i=0; i<mOriginal.GetNrows(); i++)
//    for (int j=0; j<mOriginal.GetNcols(); j++)
//      mT(j,i)=mOriginal(i,j);
  TMatrixD mMult1(mT.GetNrows(),mT.GetNrows());
  mMult1.Mult(mT,mOriginal);
  det = mMult1.Determinant();
  if (det==0){
    std::cout<<"mOriginal:"<<std::endl;
    for (int i=0; i<mOriginal.GetNrows(); i++){
      for (int j=0; j<mOriginal.GetNcols(); j++)
        std::cout<<std::setw(6) << std::setprecision(4) <<mOriginal(i,j)<<" ";
      std::cout<<std::endl;
    }
    std::cout<<"mT:"<<std::endl;
    for (int i=0; i<mT.GetNrows(); i++){
      for (int j=0; j<mT.GetNcols(); j++)
        std::cout<<std::setw(6) << std::setprecision(4) <<mT(i,j)<<" ";
      std::cout<<std::endl;
    }
    std::cout<<"mT * mOriginal:"<<std::endl;
    for (int i=0; i<mMult1.GetNrows(); i++){
      for (int j=0; j<mMult1.GetNcols(); j++)
        std::cout<<std::setw(6) << std::setprecision(8) <<mMult1(i,j)<<" ";
      std::cout<<std::endl;
    }
    std::cout<<"ERROR: the (mT * mOriginal) has zero determinant"<<std::endl;
    std::cout<<std::setw(4) << std::setprecision(2);
    return 0;
  }
  else mMult1.Invert();
  mInverted.Mult(mMult1,mT);
  return 1;
}

bool Unfolding::CalculateInvertedMatrixErrors(TMatrixD &T, TMatrixD &TErrPos, TMatrixD &TErrNeg, TMatrixD &TinvErr){
  //the function is borrowed from the DrellYanDMDY package
  // Calculate errors on the inverted matrix by the Monte Carlo
  // method

//  Double_t det;
  int nRow = T.GetNrows();
  int nCol = T.GetNcols();
  TMatrixD TinvSum(nCol,nRow);
  TMatrixD TinvSumSquares(nCol,nRow);

  // Reset Matrix where we will be accumulating RMS/sigma:
  TinvSum        = 0;
  TinvSumSquares = 0;
  TinvErr        = 0;

  // Do many tries, accumulate RMS
  long N = 1000;
  for (long iTry = 0; iTry<N; iTry++){
    // Find the smeared matrix
    TMatrixD Tsmeared = T;
    for(int i = 0; i<nRow; i++){
      for(int j = 0; j<nCol; j++){
        double central = T(i,j);
        double sigPos = TErrPos(i,j);
        double sigNeg = TErrNeg(i,j);
         // Switch to symmetric errors: approximation, but much simpler
        double sig = (sigPos+sigNeg)/2.0;
        Tsmeared(i,j) = gRandom->Gaus(central,sig);
      }
    }
    // Find the inverted to smeared matrix
    TMatrixD TinvSmeared(Tsmeared.GetNcols(),Tsmeared.GetNrows());
//    TinvSmeared.Invert(&det);

    if (!PseudoMatrixInversion(Tsmeared,TinvSmeared)){
       std::cout<<"ERROR in CalculateInvertedMatrixErrors: !PseudoMatrixInversion(Tsmeared,TinvSmeared)"<<std::endl;
       return 0;
    }

    // Accumulate sum and sum of squares for each element
    for(int i2 = 0; i2<nCol; i2++){
      for(int j2 = 0; j2<nRow; j2++){
        TinvSum       (i2,j2) += TinvSmeared(i2,j2);
        TinvSumSquares(i2,j2) += TinvSmeared(i2,j2)*TinvSmeared(i2,j2);
      }
    }
  }

  // Calculate the error matrix
  TMatrixD TinvAverage = TinvSum;
  for(int i = 0; i<nCol; i++){
    for(int j = 0; j<nRow; j++){
      TinvErr(i,j) = sqrt( TinvSumSquares(i,j)/double(N) 
                           - (TinvSum(i,j)/double(N))*(TinvSum(i,j)/double(N)) );
      TinvAverage(i,j) = TinvSum(i,j)/double(N);
    }
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
  TH2D* histRespMatrixDrawGen = new TH2D("histRespMatrixGen","Response Matrix, given gen normalized",nBinsRec_,limsRecDraw,nBinsGen_,limsGenDraw);
  TH2D* histRespMatrixDrawRec = new TH2D("histRespMatrixRec","Response Matrix, given rec normalized",nBinsGen_,limsGenDraw,nBinsRec_,limsRecDraw);
  TH2D* histUnfoMatrixDraw = new TH2D("histUnfoMatrix","Unfolding Matrix",nBinsGen_,limsGenDraw,nBinsRec_,limsRecDraw);

  for (int ir=1; ir<nBinsRec_+1; ir++)
    for (int ig=1; ig<nBinsGen_+1; ig++){
      histEventCountDraw->SetBinContent(ir,ig,histEventCountMigrMatrix_->GetBinContent(ir,ig));
      histMigrMatrixDraw->SetBinContent(ir,ig,histMigrMatrixNotNormalized_->GetBinContent(ir,ig));
      histRespMatrixDrawGen->SetBinContent(ir,ig,histResponseMatrix_->GetBinContent(ir,ig));
      histRespMatrixDrawRec->SetBinContent(ig,ir,histUnfoMatrixDAg_->GetBinContent(ig,ir));
      histUnfoMatrixDraw->SetBinContent(ig,ir,histUnfoMatrixInv_->GetBinContent(ig,ir));
    }
    
  TStyle* style = new TStyle("style","style");

  style->SetPalette(1);
  style->SetPaintTextFormat("0.1f");

  TCanvas* cMigrMatrGen = new TCanvas("cMigrMatrGen","migration matrix, given gen normalized");
  cMigrMatrGen->SetLogx();
  cMigrMatrGen->SetLogy();
  histRespMatrixDrawGen->SetStats(0);
  histRespMatrixDrawGen->GetXaxis()->SetMoreLogLabels(1);
  histRespMatrixDrawGen->GetYaxis()->SetMoreLogLabels(1);
  histRespMatrixDrawGen->GetXaxis()->SetNoExponent(1);
  histRespMatrixDrawGen->GetYaxis()->SetNoExponent(1);
  histRespMatrixDrawGen->SetMarkerSize(1);
  histRespMatrixDrawGen->Draw("COLZ TEXT");

  TCanvas* cMigrMatrRec = new TCanvas("cMigrMatrRec","migration matrix, given rec normalized");
  cMigrMatrRec->SetLogx();
  cMigrMatrRec->SetLogy();
  histRespMatrixDrawRec->SetStats(0);
  histRespMatrixDrawRec->GetXaxis()->SetMoreLogLabels(1);
  histRespMatrixDrawRec->GetYaxis()->SetMoreLogLabels(1);
  histRespMatrixDrawRec->GetXaxis()->SetNoExponent(1);
  histRespMatrixDrawRec->GetYaxis()->SetNoExponent(1);
  histRespMatrixDrawRec->SetMarkerSize(1);
  histRespMatrixDrawRec->Draw("COLZ TEXT");

  TCanvas* cUnfoMatr = new TCanvas("cUnfoMatr","Unfolding matrix");
  cUnfoMatr->SetLogx();
  cUnfoMatr->SetLogy();
  histUnfoMatrixDraw->SetStats(0);
  histUnfoMatrixDraw->GetXaxis()->SetMoreLogLabels(1);
  histUnfoMatrixDraw->GetYaxis()->SetMoreLogLabels(1);
  histUnfoMatrixDraw->GetXaxis()->SetNoExponent(1);
  histUnfoMatrixDraw->GetYaxis()->SetNoExponent();
  histUnfoMatrixDraw->SetMarkerSize(1);
  histUnfoMatrixDraw->Draw("COLZ TEXT");

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

  histResponseMatrix_->Write(config_.GetMatrMigr1DName());
  histUnfoMatrixInv_->Write(config_.GetMatrUnfo1DName());
  histYieldsRec_->Write(config_.GetYieldsRec1DName());
  histYieldsGen_->Write(config_.GetYieldsGen1DName());
  cMigrMatrGen->Write(config_.GetMatrMigr1DName());
  cUnfoMatr->Write(config_.GetMatrUnfo1DName());
  cEventCount->Write("cEventCount");
  cMigrMatrix->Write("cMigrMatrix");

  return 1;
}

bool Unfolding::ApplyUnfoldingInversion(TH1D* histInputYields, TH1D* histUnfoldedYields){
  std::cout<<"Apply Unfolding, Simple Matrix Inversion Method"<<std::endl;
  if (!MatrixTimesVector(histUnfoMatrixInv_, histInputYields, histUnfoldedYields, histYieldsGen_)){
    std::cout<<"ERROR in Unfolding::ApplyUnfoldingInversion: MatrixTimesVector return 0"<<std::endl;
    return 0;
  }
  return 1;
}

bool Unfolding::ApplyUnfoldingDAgostiniNoIterations(TH1D* histInputYields, TH1D* histUnfoldedYields){
  std::cout<<"Apply Unfolding, D'Agostini Method with no iterations"<<std::endl;
  bool res = MatrixTimesVector(histUnfoMatrixDAg_, histInputYields, histUnfoldedYields, histYieldsGen_);
  if (!res){
    std::cout<<"ERROR in Unfolding::ApplyUnfoldingInversion: MatrixTimesVector return 0"<<std::endl;
    return 0;
  }
    return 1;
}
 
bool Unfolding::ApplyUnfoldingDAgostini(TH1D* histInputYields, TH1D* histUnfoldedYields){

  std::cout<<"$$$$$$$$$$$$$$$$$$$$$$"<<std::endl;
  std::cout<<"D'Agostini Unfolding:"<<std::endl;
  
  TH2D* histUnfoBayes = new TH2D("histUnfoBayes","histUnfoBayes",nBinsGen_,binLimsGen_,nBinsRec_,binLimsRec_);
  TH2D* histUnfoBayesForErr = new TH2D("histUnfoBayesForErr","histUnfoBayesForErr",nBinsGen_,binLimsGen_,nBinsRec_,binLimsRec_);

  DoIterations(histResponseMatrix_,histYieldsGen_,histUnfoBayes,histInputYields);

  TH2D* histRespMatrix = new TH2D("histRespMatrix","histRespMatrix",nBinsRec_,binLimsRec_,nBinsGen_,binLimsGen_);  
//  TH1D* histGenYields = new TH1D("histGenYiedls","histGenYields",nBinsGen_,binLimsGen_); 
  int N=1000;
  TRandom rnd;
  TMatrixD TSum(nBinsGen_,nBinsRec_);
  TMatrixD TSumSquares(nBinsGen_,nBinsRec_);

  for (int i=0; i<N; i++){
    //smear response matrix and generated yields
    for (int ig=1; ig<=nBinsGen_; ig++){
      double cont = rnd.Gaus(histYieldsGen_->GetBinContent(ig),histYieldsGen_->GetBinError(ig));
//      histGenYields->SetBinContent(ig,cont);
//      histGenYields->SetBinError(ig,histYieldsGen_->GetBinError(ig));
      for (int ir=1; ir<=nBinsRec_; ir++){
        cont = rnd.Gaus(histResponseMatrix_->GetBinContent(ir,ig),histResponseMatrix_->GetBinError(ir,ig));
        histRespMatrix->SetBinContent(ir,ig,cont);
        histRespMatrix->SetBinError(ir,ig,histResponseMatrix_->GetBinError(ir,ig));
      }
    }
    //apply unfolding with smeared constants
    DoIterations(histRespMatrix,histYieldsGen_,histUnfoBayesForErr,histInputYields);

    // Accumulate sum and sum of squares for each element
    for(int ig = 0; ig<nBinsGen_; ig++){
      for(int ir = 0; ir<nBinsRec_; ir++){
        TSum       (ig,ir) += histUnfoBayesForErr->GetBinContent(ig+1,ir+1);
        TSumSquares(ig,ir) += histUnfoBayesForErr->GetBinContent(ig+1,ir+1)*histUnfoBayesForErr->GetBinContent(ig+1,ir+1);
      }
    }

  }//for (int i=0; i<N ...
  

  // Calculate the error matrix
//  TMatrixD TAverage = TSum;
  for(int ig = 0; ig<nBinsGen_; ig++){
    for(int ir = 0; ir<nBinsRec_; ir++){
      histUnfoBayes->SetBinError (ig+1,ir+1,sqrt( TSumSquares(ig,ir)/double(N) 
                           - (TSum(ig,ir)/double(N))*(TSum(ig,ir)/double(N))) );
    }
  }

  std::cout<<"histUnfoBayes:"<<std::endl;
  for (int ig=1; ig<=nBinsGen_; ig++){
    for (int ir=1; ir<=nBinsRec_; ir++){
      std::cout<<std::setw(4)<<std::setprecision(2)<<histUnfoBayes->GetBinContent(ig,ir)<<"+-"<<std::setw(4)<<std::setprecision(2)<<histUnfoBayes->GetBinError(ig,ir)<<" ";
    }
    std::cout<<std::endl;
  }  

  MatrixTimesVector(histUnfoBayes,histInputYields,histUnfoldedYields);

  std::cout<<"end of D'Agostini Unfolding:"<<std::endl; 
  std::cout<<"$$$$$$$$$$$$$$$$$$$$$$"<<std::endl; 

  return 1;
}
   
bool Unfolding::DoIterations(TH2D* histRespMatrix, TH1D* histGenYields, TH2D* histUnfoBayes, TH1D* histInputYields)
{
  TH1D* pOld = new TH1D("pOld","pOld",nBinsGen_,binLimsGen_); 
  TH1D* pNew = new TH1D("pNew","pNew",nBinsGen_,binLimsGen_);  
  TH1D* nOld = new TH1D("nOld","nOld",nBinsGen_,binLimsGen_); 
  TH1D* nNew = new TH1D("nNew","nNew",nBinsGen_,binLimsGen_);  

  for (int i=1; i<=nBinsGen_; i++){
    pOld->SetBinContent(i,histGenYields->GetBinContent(i)/histGenYields->GetSumOfWeights());
    nOld->SetBinContent(i,pOld->GetBinContent(i)*histInputYields->GetSumOfWeights());
    nNew->SetBinContent(i,0);
    pNew->SetBinContent(i,0);
  }
  
  for (int iter=0; iter<4; iter++){
//    std::cout<<"##############"<<std::endl;
//    std::cout<<"iteration # "<<iter<<std::endl;
    double chi2=DoOneIteration(histRespMatrix,histUnfoBayes,histInputYields,nOld,nNew,pOld,pNew);
    for (int i=1; i<=nBinsGen_; i++){ 
      nOld->SetBinContent(i,nNew->GetBinContent(i));
      pOld->SetBinContent(i,pNew->GetBinContent(i));
    }
//    std::cout<<"chi2="<<chi2<<std::endl;
 //   std::cout<<"##############"<<std::endl;
//    if (chi2<1) break;
  }

  delete pOld;
  delete pNew;
  delete nOld;
  delete nNew;

  return 1;
}
   
double Unfolding::DoOneIteration(TH2D* histRespMatrix, TH2D* histUnfoBayes, TH1D* nObserved, TH1D* nOld, TH1D* nNew, TH1D* pOld, TH1D* pNew)
{
  if (nOld->GetNbinsX()!=nBinsGen_ || nNew->GetNbinsX()!=nBinsGen_ || pNew->GetNbinsX()!=nBinsGen_ || pOld->GetNbinsX()!=nBinsGen_){
    std::cout<<"ERROR in Unfolding::DoOneIteration: binnings don't match"<<std::endl;
    return 0;
  }

  for (int ir=1; ir<=nBinsRec_; ir++){
    float sum=0;
    for (int ig=1; ig<=nBinsGen_; ig++){
      sum+=histResponseMatrix_->GetBinContent(ir,ig)*pOld->GetBinContent(ig);
    }
    for (int ig=1; ig<=nBinsGen_; ig++){
      histUnfoBayes->SetBinContent(ig,ir,histRespMatrix->GetBinContent(ir,ig)*pOld->GetBinContent(ig)/sum);
    }
  }
  
  if (!MatrixTimesVector(histUnfoBayes,nObserved,nNew,1)) return -1;

  float sum = nNew->GetSumOfWeights();
  for (int ig=1; ig<=nBinsGen_; ig++){
    pNew->SetBinContent(ig,nNew->GetBinContent(ig)/sum);
  }

//  std::cout<<std::setprecision(2)<<"Unfolding matrix for given iteration:"<<std::endl;
  for (int ig=1; ig<=nBinsGen_; ig++){
    for (int ir=1; ir<=nBinsRec_; ir++){
//      std::cout<<std::setprecision(2)<<histUnfoBayes->GetBinContent(ig,ir)<<" ";
    }
//    std::cout<<std::endl;
  }
//  std::cout<<"nNew,    nOld,    pNew,     pOld"<<std::endl;
  for (int i=1; i<=nBinsGen_; i++){
//    std::cout<<std::setw(5)<<std::setprecision(0)<<nNew->GetBinContent(i)<<"  ";
//    std::cout<<std::setw(5)<<std::setprecision(0)<<nOld->GetBinContent(i)<<"  ";
//    std::cout<<std::setw(6)<<std::setprecision(5)<<pNew->GetBinContent(i)<<"  ";
//    std::cout<<std::setw(6)<<std::setprecision(5)<<pOld->GetBinContent(i)<<"  ";
//    std::cout<<std::endl;
  }

//  double chi2 = nOld->Chi2Test(nNew,"P CHI2/NDF");
  double chi2 = 1;//nOld->Chi2Test(nNew,"CHI2/NDF");
  return chi2;
}

bool Unfolding::ApplyRooUnfold(TH1D* histInputYields, TH1D* unfoldedYields, RooUnfold::Algorithm alg)
{  
  std::cout<<"#######################"<<std::endl;
  std::cout<<"DoRooUnfold() starts here:"<<std::endl<<std::endl;

  RooUnfoldResponse response(histYieldsRec_,histYieldsGen_,histMigrMatrixNotNormalized_,"response","RooUnfoldResponse matrix");
  std::cout<<"RooUnfold";
  if (alg==RooUnfold::kInvert) std::cout<<" Invert"<<std::endl;
  else if (alg==RooUnfold::kBayes) std::cout<<" D'Agostini"<<std::endl;
  else std::cout<<std::endl;

  RooUnfold* unfold = RooUnfold::New(alg,&response,histInputYields);

  if (alg==RooUnfold::kBayes) unfold->SetRegParm(5);
  unfold->SetNToys(1000);
  TH1D* histUnfoldedYields = (TH1D*) unfold->Hreco(RooUnfold::kCovToy);

  //Compute errors due to finite MC statistics
  TH2D* histMigrMatrixSmeared = new TH2D("migrSmeared","histMigrMatrixSmeared",nBinsRec_,binLimsRec_,nBinsGen_,binLimsGen_);
  TH1D* histUnfSmeared = 0;
  TRandom rnd;

  double ExSqr[nBinsGen_+1];
  double Ex[nBinsGen_+1];
  for (int i=1; i<=nBinsGen_; i++){
    ExSqr[i]=0;
    Ex[i]=0;
  }
  int NSmears=1000;
  for (int iSm=0; iSm<NSmears; iSm++){
    for (int irec=1; irec<=nBinsRec_; irec++)
      for (int igen=1; igen<=nBinsGen_; igen++){
        double cont = rnd.Gaus(histMigrMatrixNotNormalized_->GetBinContent(irec,igen),histMigrMatrixNotNormalized_->GetBinError(irec,igen));
        histMigrMatrixSmeared->SetBinContent(irec,igen,cont);
      }
    RooUnfoldResponse responseSmeared(histYieldsRec_,histYieldsGen_,histMigrMatrixSmeared);
    RooUnfold* unfoldSmeared = RooUnfold::New(alg,&responseSmeared,histInputYields);
    histUnfSmeared = (TH1D*) unfoldSmeared->Hreco(RooUnfold::kNoError);
    for (int igen=1; igen<=nBinsGen_; igen++){
      double val = histUnfSmeared->GetBinContent(igen);
      Ex[igen]+=val;
      ExSqr[igen]+=val*val;
    }
  }

  delete histMigrMatrixSmeared;

  int imeth = 0;
  if (alg==RooUnfold::kInvert) imeth = RooUnfold_INVERSION;
  if (alg==RooUnfold::kBayes) imeth = RooUnfold_DAGOSTINI;

  std::cout<<"Unfolded yiedls:"<<std::endl;
  for (int i=1; i<=nBinsGen_; i++){
    Ex[i]=Ex[i]/NSmears;
    ExSqr[i]=ExSqr[i]/NSmears;
    std::cout<<histUnfoldedYields->GetBinContent(i)<<"+-"<<histUnfoldedYields->GetBinError(i)<<"+-"<<sqrt(ExSqr[i]-Ex[i]*Ex[i]);
    unfoldedYields->SetBinContent(i,histUnfoldedYields->GetBinContent(i));
    unfoldedYields->SetBinError(i,sqrt(histUnfoldedYields->GetBinError(i)*histUnfoldedYields->GetBinError(i)+(ExSqr[i]-Ex[i]*Ex[i])));
    std::cout<<" = "<<unfoldedYields->GetBinContent(i)<<"+-"<<unfoldedYields->GetBinError(i)<<std::endl;
    _unfolded[imeth][i] = histUnfoldedYields->GetBinContent(i);
    _unfoldedErrStat[imeth][i] = histUnfoldedYields->GetBinError(i);
    _unfoldedErrSyst[imeth][i] = sqrt(ExSqr[i]-Ex[i]*Ex[i]);
  }


  //unfoldBayes->PrintTable(cout,histUnfoldedYields);
  std::cout<<std::endl<<"DoRooUnfold() ends here"<<std::endl;
  std::cout<<"#######################"<<std::endl;
  return 1;
}

bool Unfolding::MatrixTimesVector(TH2D* mat, TH1D* vec, TH1D* resultVec,  bool noPrint, bool hasReference, TH1D* referenceVec){
  int nBinsX = mat->GetNbinsX();
  int nBinsY = mat->GetNbinsY();
  if (vec->GetNbinsX()!=nBinsY){
    std::cout<<"ERROR in Unfolding::MatrixTimesVector: multiplication impossible, vector and matrix binnings don't match"<<std::endl;
    return 0;
  }
  if (resultVec->GetNbinsX()!=nBinsX){
    std::cout<<"ERROR in Unfolding::MatrixTimesVector: multiplication impossible, resultVector and Matrix binnings don't match"<<std::endl;
    return 0;
  }

  bool isRef = hasReference && referenceVec && (referenceVec->GetNbinsX()==nBinsX);

  if (noPrint);
  else if (!isRef)
    std::cout<<"multiplication result:"<<std::endl;
  else
    std::cout<<"reference yields; multiplication result:"<<std::endl;

  for (int ix=1; ix<=nBinsX; ix++){

    float val=0;
    float errSquared1=0;
    float errSquared2=0;

    for (int iy=1; iy<=nBinsY; iy++){

      val+=mat->GetBinContent(ix,iy)*vec->GetBinContent(iy);
      errSquared1+=mat->GetBinContent(ix,iy)*mat->GetBinContent(ix,iy)*vec->GetBinError(iy)*vec->GetBinError(iy);
      errSquared2+=mat->GetBinError(ix,iy)*mat->GetBinError(ix,iy)*vec->GetBinContent(iy)*vec->GetBinContent(iy);

    }
    
    resultVec->SetBinContent(ix,val);
    resultVec->SetBinError(ix,sqrt(errSquared1+errSquared2));  

    if (noPrint);
    else if (!isRef)
      std::cout<<std::setprecision(0)<<resultVec->GetBinLowEdge(ix)<<" - "<<resultVec->GetBinLowEdge(ix)+resultVec->GetBinWidth(ix)<<": "<<resultVec->GetBinContent(ix)<<"+-"<<resultVec->GetBinError(ix)<<" = "<<val<<"+-"<<sqrt(errSquared1)<<"+-"<<sqrt(errSquared2)<<std::endl;
    else
      std::cout<<std::setprecision(0)<<resultVec->GetBinLowEdge(ix)<<" - "<<resultVec->GetBinLowEdge(ix)+resultVec->GetBinWidth(ix)<<": "<<referenceVec->GetBinContent(ix)<<"+-"<<referenceVec->GetBinError(ix)<<"; "<<resultVec->GetBinContent(ix)<<"+-"<<resultVec->GetBinError(ix)<<" = "<<val<<"+-"<<sqrt(errSquared1)<<"+-"<<sqrt(errSquared2)<<std::endl;
  }
  return 1;
}
