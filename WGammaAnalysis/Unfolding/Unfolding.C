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
  // ROOT class
#include <iostream> 
#include <iomanip>
#include <string>
#include <sstream>  
  //standard C++ class

Unfolding::Unfolding(int channel, string configFile)
{

  INPUT_ = new TAllInputSamples(channel, configFile);
  channel_=channel; 
  fOut_ = new TFile(config_.GetUnfoldingFileName(channel),"recreate");

}

Unfolding::~Unfolding()
{
}

void Unfolding::PrepareMigrationMatrix()
{

  isOverflowUsed_=1;

  TFile fIn(config_.GetSelectedFullyName(channel_,config_.SIGMC));
  TTree* tr = (TTree*)fIn.Get("selectedEvents");
  Float_t phoEtRec;
  Float_t phoEtGen;
  Float_t weight;
  TBranch* b_phoEtRec;
  TBranch* b_phoEtGen;
  TBranch* b_weight;
  tr->SetBranchAddress("phoEt",&phoEtRec,&b_phoEtRec);
  tr->SetBranchAddress("phoGenEt",&phoEtGen,&b_phoEtGen);
  tr->SetBranchAddress("weight",&weight,&b_weight);

  vecPhoPtLimits_ = config_.GetPhoPtUnfBinsLimits(isOverflowUsed_);
  int nBins = config_.GetNPhoPtUnfBins(isOverflowUsed_);


  float binLims[nBins+1];
  for (int i=0; i<nBins+1; i++){
    binLims[i]=vecPhoPtLimits_[i];
  }
  fOut_->cd();
  histMigrMatrix_ = new TH2D("migrMatrix","Migration Matrix",nBins,binLims,nBins,binLims); 

  histYieldsRec_ = new TH1F("yieldsRec","yields rec",nBins,binLims);
  histYieldsGen_ = new TH1F("yieldsGen","yields gen",nBins,binLims);

  
  
  int nEntries = tr->GetEntries();
  float binContent[nBins][nBins];
  float binErrorSquared[nBins][nBins];
  float binGen[nBins];
  float binRec[nBins];
  float binGenErrSquared[nBins];
  float binRecErrSquared[nBins];
  for (int i=0; i<nBins; i++){
    binGen[i]=0;
    binRec[i]=0;
    binGenErrSquared[i]=0;
    binRecErrSquared[i]=0;
    for (int j=0; j<nBins; j++){
      binErrorSquared[i][j]=0;
      binContent[i][j]=0;
    }
  }
  for (int entry=0; entry<nEntries; entry++){
    b_phoEtRec->GetEntry(entry);
    b_phoEtGen->GetEntry(entry);
    b_weight->GetEntry(entry);

 //   weight=1;

    int binG = config_.FindPhoPtUnfBinByPhoPt(phoEtGen,isOverflowUsed_);
    int binR = config_.FindPhoPtUnfBinByPhoPt(phoEtRec,isOverflowUsed_);
    binContent[binG][binR] += weight;
    binErrorSquared[binG][binR] += weight*weight;
      //bin error = sqrt (SUMM(weight*weight))
    binGen[binG]+=weight;
    binRec[binR]+=weight;
    binGenErrSquared[binG]+=weight*weight;
    binRecErrSquared[binR]+=weight*weight;
  } // loop over entries

  TH2D* matrMigrNotNormalized = new TH2D("migrNotNorm","matrMigrNotNormalized",nBins,binLims,nBins,binLims);
  for (int i=0; i<nBins; i++)
    for (int j=0; j<nBins; j++){
      matrMigrNotNormalized->SetBinContent(i+1,j+1,binContent[j][i]);
      matrMigrNotNormalized->SetBinError(i+1,j+1,sqrt(binErrorSquared[j][i]));
    }
  TCanvas* canv = new TCanvas("cMatrMigrNotNorm","cMatrMigrNotNorm");
  canv->SetLogx();
  canv->SetLogy();
  matrMigrNotNormalized->Draw("COLZ TEXT");
  



  std::cout<<std::endl;
  std::cout<<"contents and errors before normalization"<<std::endl;
  for (int i=0; i<nBins; i++){
    for (int j=0; j<nBins; j++)
      std::cout<<std::fixed << std::setw(4) << std::setprecision(2)<<binContent[i][j]<<"+-"<<std::fixed << std::setw(4)<< std::setprecision(2)<<sqrt(binErrorSquared[i][j])<<" ";
    std::cout<<std::endl;
  }
  std::cout<<std::endl;

  bool isGenOverflow = 0;
  bool isRecOverflow = 0;

  for (int ix=0; ix<nBins; ix++){
    float totalGen=0;
    float sumErrorsSquared=0;

    histYieldsGen_->SetBinContent(ix+1,binGen[ix]);
    histYieldsGen_->SetBinError(ix+1,sqrt(binGenErrSquared[ix]));
    histYieldsRec_->SetBinContent(ix+1,binRec[ix]);
    histYieldsRec_->SetBinError(ix+1,sqrt(binRecErrSquared[ix]));

    for (int iy=0; iy<nBins; iy++){
      totalGen+=binContent[ix][iy];
      sumErrorsSquared+=binErrorSquared[ix][iy];
    }
    for (int iy=0; iy<nBins; iy++){
      if (ix==nBins-1 && binContent[ix][iy]!=0)
        isRecOverflow = 1;
      float errSquared=0;
      if (totalGen!=0) 
        errSquared=(binErrorSquared[ix][iy]*(totalGen+binContent[ix][iy])*(totalGen+binContent[ix][iy])+binContent[ix][iy]*binContent[ix][iy]*(sumErrorsSquared-binErrorSquared[ix][iy]))/(totalGen*totalGen*totalGen*totalGen);
      if (totalGen!=0) binContent[ix][iy]=binContent[ix][iy]/totalGen;
      else binContent[ix][iy]=0;
      binErrorSquared[ix][iy]=errSquared;
    }
    if (binContent[ix][nBins-1]!=0)
      isGenOverflow = 1;
  }
  
  if (isRecOverflow && isGenOverflow)
    isOverflowUsed_=1;
  else isOverflowUsed_=0;

  //to transpose the matrix
  for (int i=0; i<nBins; i++)
    for (int j=0; j<nBins; j++){
      histMigrMatrix_->SetBinContent(i+1,j+1,binContent[j][i]);
      histMigrMatrix_->SetBinError(i+1,j+1,sqrt(binErrorSquared[j][i]));
    }

 
  std::cout<<std::endl;
  std::cout<<"reco yields = migration matrix * gen yields"<<std::endl;
  for (int i=0; i<nBins; i++){
    std::cout<<std::fixed << std::setw(5) << std::setprecision(0)<<binRec[i]/*<<"+-"<<std::fixed << std::setw(4)<< std::setprecision(0)<<sqrt(binRecErrSquared[i])*/<<" = (";
    for (int j=0; j<nBins; j++)
      std::cout<<std::fixed << std::setw(4) << std::setprecision(2)<<binContent[i][j]<</*"+-"<<std::fixed << std::setw(4)<< std::setprecision(2)<<sqrt(binErrorSquared[i][j])<<*/" ";
    std::cout<<std::fixed << std::setw(5) << std::setprecision(0)<<" )T * "<<binGen[i]/*<<"+-"<<std::fixed << std::setw(4)<< std::setprecision(0)<<sqrt(binGenErrSquared[i])*/<<"";
    std::cout<<std::endl;
  }
  std::cout<<std::endl;

  std::cout<<std::endl;
  std::cout<<"Migration matrix:"<<std::endl;
  for (int i=1; i<nBins+1; i++){
    for (int j=1; j<nBins+1; j++)
      std::cout<<std::fixed << std::setw(4) << std::setprecision(2)<<histMigrMatrix_->GetBinContent(i,j)<<"+-"<<std::fixed << std::setw(4)<< std::setprecision(2)<<histMigrMatrix_->GetBinError(i,j)<<" ";
    std::cout<<"(bin low edge: "<<histMigrMatrix_->GetBinLowEdge(i)<<")"<<std::endl;
  }
  std::cout<<std::endl;

}

void Unfolding::PrepareUnfoldingMatrix()
{

  vecPhoPtLimits_ = config_.GetPhoPtUnfBinsLimits(isOverflowUsed_);
  int nBins = config_.GetNPhoPtUnfBins(isOverflowUsed_);
  float binLims[nBins+1];
  for (int i=0; i<nBins+1; i++){
    binLims[i]=vecPhoPtLimits_[i];
  }
  fOut_->cd();
  histUnfoMatrix_ = new TH2D("unfoMatrix","Unfolding Matrix",nBins,binLims,nBins,binLims);

  TMatrixD mUnfo(nBins,nBins);
  TMatrixD mMigr(nBins,nBins);
  TMatrixD mUnfoErr(nBins,nBins);
  TMatrixD mMigrErr(nBins,nBins);
  for (int i=1; i<=nBins; i++)
    for (int j=1; j<=nBins; j++){
      mUnfo(i-1,j-1)=histMigrMatrix_->GetBinContent(i,j);
      mMigr(i-1,j-1)=histMigrMatrix_->GetBinContent(i,j);
      mMigrErr(i-1,j-1)=histMigrMatrix_->GetBinError(i,j);
    }

  float det = mUnfo.Determinant();
  if (det==0){
    std::cout<<"ERROR: the migration matrix has zero determinant"<<std::endl;
    return;
  }
  else mUnfo.Invert();

  CalculateInvertedMatrixErrors(mMigr, mMigrErr, mMigrErr, mUnfoErr);
  for (int i=1; i<=nBins; i++)
    for (int j=1; j<=nBins; j++){
      histUnfoMatrix_->SetBinContent(i,j,mUnfo(i-1,j-1));
      histUnfoMatrix_->SetBinError(i,j,mUnfoErr(i-1,j-1));
    }


 
  TDecompLU decompLUMigr(mMigr);
  TDecompLU decompLUUnfo(mUnfo);
  float condLUMigr = decompLUMigr.Condition();
  float condLUUnfo = decompLUUnfo.Condition();
  TDecompSVD decompSVDMigr(mMigr);
  TDecompSVD decompSVDUnfo(mUnfo);
  float condSVDMigr = decompSVDMigr.Condition();
  float condSVDUnfo = decompSVDUnfo.Condition();

  std::cout<<std::endl;
  std::cout<<"cond=||M||*||M^-1|| (RowNorm)="<<mMigr.RowNorm()<<"*"<<mUnfo.RowNorm()<<"="<<mMigr.RowNorm()*mUnfo.RowNorm()<<std::endl;
  std::cout<<"cond=||M||*||M^-1|| (ColNorm)="<<mMigr.ColNorm()<<"*"<<mUnfo.ColNorm()<<"="<<mMigr.ColNorm()*mUnfo.ColNorm()<<std::endl;
  std::cout<<"cond=||M||*||M^-1|| (E2Norm)="<<sqrt(mMigr.E2Norm())<<"*"<<sqrt(mUnfo.E2Norm())<<"="<<sqrt(mMigr.E2Norm()*mUnfo.E2Norm())<<std::endl;
  std::cout<<"cond=||M||*||M^-1|| (spectral norm)="<<CalculateMatrixSpectralNorm(mMigr)<<"*"<<CalculateMatrixSpectralNorm(mUnfo)<<"="<<CalculateMatrixSpectralNorm(mMigr)*CalculateMatrixSpectralNorm(mUnfo)<<std::endl;
  std::cout<<"condLUMigr="<<condLUMigr<<", condLUUnfo="<<condLUUnfo<<std::endl;
  std::cout<<"condSVDMigr="<<condSVDMigr<<", condSVDUnfo="<<condSVDUnfo<<std::endl;
  std::cout<<"decomposition status: "<<decompSVDMigr.Decompose()<<std::endl;
  TMatrixD mU =  decompSVDMigr.GetU();
  TMatrixD mV =  decompSVDMigr.GetV();
  TMatrixD mVT(TMatrixD::kTransposed,mV);
  TVectorD mS =  decompSVDUnfo.GetSig();
  std::cout<<"U * vSig * V':"<<std::endl;
  for (int i=0; i<mU.GetNrows(); i++){
    std::cout<<"( ";
    for (int j=0; j<mU.GetNcols(); j++){
      std::cout<<mU(i,j)<<" ";
    }
    std::cout<<" ) * ( "<<mS(i)<<" ) * ( ";
    for (int j=0; j<mVT.GetNcols(); j++){
      std::cout<<mVT(i,j)<<" ";
    }
    std::cout<<" ) "<<std::endl;
  }
  std::cout<<std::endl;

  std::cout<<"Unfolding matrix:"<<std::endl;
  for (int i=1; i<nBins+1; i++){
    for (int j=1; j<nBins+1; j++)
      std::cout<<std::fixed << std::setw(4) << std::setprecision(2) << histUnfoMatrix_->GetBinContent(i,j)<<"+-"<<std::fixed << std::setw(4) << std::setprecision(2) << histUnfoMatrix_->GetBinError(i,j)<<" ";
    std::cout<<std::endl;
  }
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

void Unfolding::CalculateInvertedMatrixErrors(TMatrixD &T, TMatrixD &TErrPos, TMatrixD &TErrNeg, TMatrixD &TinvErr){
  //the function is borrowed from the DrellYanDMDY package
  // Calculate errors on the inverted matrix by the Monte Carlo
  // method

  Double_t det;
  int nRow = T.GetNrows();
  int nCol = T.GetNcols();
  TMatrixD TinvSum(nRow,nCol);
  TMatrixD TinvSumSquares(nRow,nCol);

  // Reset Matrix where we will be accumulating RMS/sigma:
  TinvSum        = 0;
  TinvSumSquares = 0;
  TinvErr        = 0;

  // Do many tries, accumulate RMS
  long N = 1000000;
  for (long iTry = 0; iTry<N; iTry++){
    // Find the smeared matrix
    TMatrixD Tsmeared = T;
    for(int i = 0; i<nRow; i++){
      for(int j = 0; j<nCol; j++){
        double central = T(i,j);
        double sigPos = TErrPos(i,j)/2;
        double sigNeg = TErrNeg(i,j)/2;
         // Switch to symmetric errors: approximation, but much simpler
        double sig = (sigPos+sigNeg)/2.0;
        Tsmeared(i,j) = gRandom->Gaus(central,sig);
      }
    }
    // Find the inverted to smeared matrix
    TMatrixD TinvSmeared = Tsmeared;
    TinvSmeared.Invert(&det);
    // Accumulate sum and sum of squares for each element
    for(int i2 = 0; i2<nRow; i2++){
      for(int j2 = 0; j2<nCol; j2++){
        TinvSum       (i2,j2) += TinvSmeared(i2,j2);
        TinvSumSquares(i2,j2) += TinvSmeared(i2,j2)*TinvSmeared(i2,j2);
      }
    }
  }

  // Calculate the error matrix
  TMatrixD TinvAverage = TinvSum;
  for(int i = 0; i<nRow; i++){
    for(int j = 0; j<nCol; j++){
      TinvErr(i,j) = sqrt( TinvSumSquares(i,j)/double(N) 
                           - (TinvSum(i,j)/double(N))*(TinvSum(i,j)/double(N)) );
      TinvAverage(i,j) = TinvSum(i,j)/double(N);
    }
  }

  return;
}

void Unfolding::ClosureTest()
{
  TFile fYields(config_.GetYieldsFileName(channel_));
  TH1D* hGenYields = (TH1D*)fYields.Get(config_.GetYieldsSelectedSignalMCGenHistName());
  TH1D* hRecYields = (TH1D*)fYields.Get(config_.GetYieldsSelectedHistName(config_.SIGMC, config_.COMMON));
  int nBins = config_.GetNPhoPtUnfBins(1);
  TMathTools math;
  std::cout<<std::endl;
  std::cout<<"Closure test #1: vRec = M * vGen = vRec (local) = M * vGen (local)"<<std::endl;
  for (int i=1; i<nBins+1;i++){
    float recCalc1=0;
    float recCalcErrSquared11=0;
    float recCalcErrSquared12=0;
    float recCalc2=0;
    float recCalcErrSquared21=0;
    float recCalcErrSquared22=0;
    for (int j=1; j<nBins+1; j++){
      recCalc1+=histMigrMatrix_->GetBinContent(i,j)*hGenYields->GetBinContent(j);
      recCalcErrSquared11+=histMigrMatrix_->GetBinError(i,j)*hGenYields->GetBinContent(j)*histMigrMatrix_->GetBinError(i,j)*hGenYields->GetBinContent(j);
      recCalcErrSquared12+=histMigrMatrix_->GetBinContent(i,j)*hGenYields->GetBinError(j)*histMigrMatrix_->GetBinContent(i,j)*hGenYields->GetBinError(j);
      recCalc2+=histMigrMatrix_->GetBinContent(i,j)*histYieldsGen_->GetBinContent(j);
      recCalcErrSquared21+=histMigrMatrix_->GetBinError(i,j)*histYieldsGen_->GetBinContent(j)*histMigrMatrix_->GetBinError(i,j)*histYieldsGen_->GetBinContent(j);
      recCalcErrSquared22+=histMigrMatrix_->GetBinContent(i,j)*histYieldsGen_->GetBinError(j)*histMigrMatrix_->GetBinContent(i,j)*histYieldsGen_->GetBinError(j);
    }
    std::cout<<hRecYields->GetBinContent(i)<<"+-"<<hRecYields->GetBinError(i)<<" = "<<recCalc1<<"+-"<<sqrt(recCalcErrSquared11)<<"+-"<<sqrt(recCalcErrSquared12)<<" = "<<histYieldsRec_->GetBinContent(i)<<"+-"<<histYieldsRec_->GetBinError(i)<<" = "<<recCalc2<<"+-"<<sqrt(recCalcErrSquared21)<<"+-"<<sqrt(recCalcErrSquared22)<<std::endl;
  }

  std::cout<<std::endl;

  std::cout<<"Closure test #2: vGen = U * vRec = vGen (local) = U * vRec (local)"<<std::endl;
  for (int i=1; i<nBins+1;i++){
    float recCalc1=0;
    float recCalcErrSquared11=0;
    float recCalcErrSquared12=0;
    float recCalc2=0;
    float recCalcErrSquared21=0;
    float recCalcErrSquared22=0;
    for (int j=1; j<nBins+1; j++){
      recCalc1+=histUnfoMatrix_->GetBinContent(i,j)*hRecYields->GetBinContent(j);
      recCalcErrSquared11+=histUnfoMatrix_->GetBinError(i,j)*hRecYields->GetBinContent(j)*histUnfoMatrix_->GetBinError(i,j)*hRecYields->GetBinContent(j);
      recCalcErrSquared12+=histUnfoMatrix_->GetBinContent(i,j)*hRecYields->GetBinError(j)*histUnfoMatrix_->GetBinContent(i,j)*hRecYields->GetBinError(j);
      recCalc2+=histUnfoMatrix_->GetBinContent(i,j)*histYieldsRec_->GetBinContent(j);
      recCalcErrSquared21+=histUnfoMatrix_->GetBinError(i,j)*histYieldsRec_->GetBinContent(j)*histUnfoMatrix_->GetBinError(i,j)*histYieldsRec_->GetBinContent(j);
      //recCalcErrSquared22+=histUnfoMatrix_->GetBinContent(i,j)*histYieldsRec_->GetBinError(j)*histUnfoMatrix_->GetBinContent(i,j)*histYieldsRec_->GetBinError(j);
      recCalcErrSquared22+=histUnfoMatrix_->GetBinContent(i,j)*sqrt(histYieldsRec_->GetBinContent(j))*histUnfoMatrix_->GetBinContent(i,j)*sqrt(histYieldsRec_->GetBinContent(j));
    }
    std::cout<<histYieldsGen_->GetBinLowEdge(i)<<" - "<<histYieldsGen_->GetBinLowEdge(i)+histYieldsGen_->GetBinWidth(i)<<" GeV: "<</*hGenYields->GetBinContent(i)<<"+-"<<hGenYields->GetBinError(i)<<" = "<<recCalc1<<"+-"<<sqrt(recCalcErrSquared11)<<"+-"<<sqrt(recCalcErrSquared12)<<" = "<<*/histYieldsGen_->GetBinContent(i)<<"+-"<</*histYieldsGen_->GetBinError(i)*/sqrt(histYieldsGen_->GetBinContent(i))<<" = "<<recCalc2<<"+-"<<sqrt(recCalcErrSquared21)<<"+-"<<sqrt(recCalcErrSquared22)<<std::endl;
  }
  std::cout<<"the last bin value and error:"<<std::endl;
  for (int j=1; j<nBins+1; j++){
    std::cout<<"j="<<j<<", contPortion="<<histUnfoMatrix_->GetBinContent(nBins-1,j)*histYieldsRec_->GetBinContent(j)<<", err1="<<histUnfoMatrix_->GetBinError(nBins-1,j)<<"*"<<histYieldsRec_->GetBinContent(j)<<", err2="<<histUnfoMatrix_->GetBinContent(nBins-1,j)<<"*"<<histYieldsRec_->GetBinError(j)<<std::endl;
  }
   
}

void Unfolding::Store()
{
  fOut_->cd();

  TCanvas* cMigrMatr = new TCanvas("cMigrMatr","migration matrix");
  cMigrMatr->SetLogx();
  cMigrMatr->SetLogy();
  histMigrMatrix_->SetStats(0);
  histMigrMatrix_->GetXaxis()->SetMoreLogLabels(1);
  histMigrMatrix_->GetYaxis()->SetMoreLogLabels(1);
  histMigrMatrix_->GetXaxis()->SetNoExponent(1);
  histMigrMatrix_->GetYaxis()->SetNoExponent(1);
  histMigrMatrix_->GetXaxis()->SetRangeUser(10,1000);
  histMigrMatrix_->GetYaxis()->SetRangeUser(10,1000);
  histMigrMatrix_->SetMarkerSize(1);
  histMigrMatrix_->Draw("COLZ TEXT");

  TCanvas* cUnfoMatr = new TCanvas("cUnfoMatr","Unfolding matrix");
  cUnfoMatr->SetLogx();
  cUnfoMatr->SetLogy();
  histUnfoMatrix_->SetStats(0);
  histUnfoMatrix_->GetXaxis()->SetMoreLogLabels(1);
  histUnfoMatrix_->GetYaxis()->SetMoreLogLabels(1);
  histUnfoMatrix_->GetXaxis()->SetNoExponent(1);
  histUnfoMatrix_->GetYaxis()->SetNoExponent();
  histUnfoMatrix_->SetMarkerSize(1);
  histUnfoMatrix_->Draw("COLZ TEXT");


  histMigrMatrix_->Write(config_.GetMatrMigr1DName());
  histUnfoMatrix_->Write(config_.GetMatrUnfo1DName());
  histYieldsRec_->Write(config_.GetYieldsRec1DName());
  histYieldsGen_->Write(config_.GetYieldsGen1DName());
  cMigrMatr->Write(config_.GetMatrMigr1DName());
  cUnfoMatr->Write(config_.GetMatrUnfo1DName());
}
