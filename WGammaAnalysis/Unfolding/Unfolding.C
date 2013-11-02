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
#include "TVectorF.h" 
#include "TCanvas.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TLegend.h"
#include "TTree.h"
#include "TFile.h"
#include "TMatrixD.h" 
#include "TMatrixF.h"
#include "TDecompLU.h"
#include "TDecompSVD.h"
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
  fOut_ = new TFile("fOut.root","recreate");

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
  histMigrMatrix_ = new TH2F("migrMatrix","Migration Matrix",nBins,binLims,nBins,binLims); 

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

  TCanvas* cMatr = new TCanvas("cMigrMatr","migration matrix");
  cMatr->SetLogx();
  cMatr->SetLogy();
  histMigrMatrix_->SetStats(0);
  histMigrMatrix_->GetXaxis()->SetMoreLogLabels(1);
  histMigrMatrix_->GetYaxis()->SetMoreLogLabels(1);
  histMigrMatrix_->GetXaxis()->SetNoExponent(1);
  histMigrMatrix_->GetYaxis()->SetNoExponent(1);
  std::cout<<"axis limits: "<<binLims[0]<<" - "<<binLims[nBins]<<std::endl;
  histMigrMatrix_->GetXaxis()->SetRangeUser(binLims[0],binLims[nBins]);
  histMigrMatrix_->GetYaxis()->SetRangeUser(binLims[0],binLims[nBins]);
  histMigrMatrix_->SetMarkerSize(1);
  histMigrMatrix_->Draw("COLZ TEXT");
  
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
  histUnfoMatrix_ = new TH2F("unfoMatrix","Unfolding Matrix",nBins,binLims,nBins,binLims);

  TMatrixF mUnfo(nBins,nBins);
  TMatrixF mMigr(nBins,nBins);
  TMatrixF mUnfoErr(nBins,nBins);
  TMatrixF mMigrErr(nBins,nBins);
  for (int i=1; i<=nBins; i++)
    for (int j=1; j<=nBins; j++){
      mUnfo(i-1,j-1)=histMigrMatrix_->GetBinContent(i,j);
      mMigr(i-1,j-1)=histMigrMatrix_->GetBinContent(i,j);
      mMigrErr(i-1,j-1)=histMigrMatrix_->GetBinError(i,j);
    }

  if (mUnfo.Determinant()==0){
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


  TCanvas* cMatr = new TCanvas("cUnfoMatr","Unfolding matrix");
  cMatr->SetLogx();
  cMatr->SetLogy();
  histUnfoMatrix_->SetStats(0);
  histUnfoMatrix_->GetXaxis()->SetMoreLogLabels(1);
  histUnfoMatrix_->GetYaxis()->SetMoreLogLabels(1);
  histUnfoMatrix_->GetXaxis()->SetNoExponent(1);
  histUnfoMatrix_->GetYaxis()->SetNoExponent();
  histUnfoMatrix_->SetMarkerSize(1);
  histUnfoMatrix_->Draw("COLZ TEXT");
  
  TDecompLU decompLUMigr(mMigr);
  TDecompLU decompLUUnfo(mUnfo);
  float condLUMigr = decompLUMigr.Condition();
  float condLUUnfo = decompLUUnfo.Condition();
  TDecompSVD decompSVDMigr(mMigr);
  TDecompSVD decompSVDUnfo(mUnfo);
  float condSVDMigr = decompSVDMigr.Condition();
  float condSVDUnfo = decompSVDUnfo.Condition();
  std::cout<<std::endl;
  std::cout<<"condLUMigr="<<condLUMigr<<", condLUUnfo="<<condLUUnfo<<std::endl;
  std::cout<<"condSVDMigr="<<condSVDMigr<<", condSVDUnfo="<<condSVDUnfo<<std::endl;
  std::cout<<"decomposition status: "<<decompSVDUnfo.Decompose()<<std::endl;
  TMatrixD mU =  decompSVDUnfo.GetU();
  TMatrixD mV =  decompSVDUnfo.GetV();
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

void Unfolding::CalculateInvertedMatrixErrors(TMatrixF &T, TMatrixF &TErrPos, TMatrixF &TErrNeg, TMatrixF &TinvErr){
  //the function is borrowed from the DrellYanDMDY package
  // Calculate errors on the inverted matrix by the Monte Carlo
  // method

  Double_t det;
  int nRow = T.GetNrows();
  int nCol = T.GetNcols();
  TMatrixF TinvSum(nRow,nCol);
  TMatrixF TinvSumSquares(nRow,nCol);

  // Reset Matrix where we will be accumulating RMS/sigma:
  TinvSum        = 0;
  TinvSumSquares = 0;
  TinvErr        = 0;

  // Do many tries, accumulate RMS
  int N = 10000;
  for(int iTry = 0; iTry<N; iTry++){
    // Find the smeared matrix
    TMatrixF Tsmeared = T;
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
    TMatrixF TinvSmeared = Tsmeared;
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
  TMatrixF TinvAverage = TinvSum;
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
  TH1F* hGenYields = (TH1F*)fYields.Get(config_.GetYieldsSelectedSignalMCGenHistName());
  TH1F* hRecYields = (TH1F*)fYields.Get(config_.GetYieldsSelectedHistName(config_.SIGMC, config_.COMMON));
  int nBins = config_.GetNPhoPtUnfBins(1);
  TMathTools math;
  std::cout<<std::endl;
  std::cout<<"Closure test #1: vRec = M * vGen = vRec (local) = M * vGen (local)"<<std::endl;
  for (int i=1; i<nBins+1;i++){
    float recCalc1=0;
    float recCalcErrSquared1=0;
    float recCalc2=0;
    float recCalcErrSquared2=0;
    for (int j=1; j<nBins+1; j++){
      recCalc1+=histMigrMatrix_->GetBinContent(i,j)*hGenYields->GetBinContent(j);
      recCalcErrSquared1+=histMigrMatrix_->GetBinError(i,j)*hGenYields->GetBinContent(j)*histMigrMatrix_->GetBinError(i,j)*hGenYields->GetBinContent(j)+histMigrMatrix_->GetBinContent(i,j)*hGenYields->GetBinError(j)*histMigrMatrix_->GetBinContent(i,j)*hGenYields->GetBinError(j);
      recCalc2+=histMigrMatrix_->GetBinContent(i,j)*histYieldsGen_->GetBinContent(j);
      recCalcErrSquared2+=histMigrMatrix_->GetBinError(i,j)*histYieldsGen_->GetBinContent(j)*histMigrMatrix_->GetBinError(i,j)*histYieldsGen_->GetBinContent(j)+histMigrMatrix_->GetBinContent(i,j)*histYieldsGen_->GetBinError(j)*histMigrMatrix_->GetBinContent(i,j)*histYieldsGen_->GetBinError(j);
    }
    std::cout<<hRecYields->GetBinContent(i)<<"+-"<<hRecYields->GetBinError(i)<<" = "<<recCalc1<<"+-"<<sqrt(recCalcErrSquared1)<<" = "<<histYieldsRec_->GetBinContent(i)<<"+-"<<histYieldsRec_->GetBinError(i)<<" = "<<recCalc2<<"+-"<<sqrt(recCalcErrSquared2)<<std::endl;
  }

  std::cout<<std::endl;

  std::cout<<"Closure test #2: vGen = U * vRec = vGen (local) = U * vRec (local)"<<std::endl;
  for (int i=1; i<nBins+1;i++){
    float recCalc1=0;
    float recCalcErrSquared1=0;
    float recCalc2=0;
    float recCalcErrSquared2=0;
    for (int j=1; j<nBins+1; j++){
      recCalc1+=histUnfoMatrix_->GetBinContent(i,j)*hRecYields->GetBinContent(j);
      recCalcErrSquared1+=histUnfoMatrix_->GetBinError(i,j)*hRecYields->GetBinContent(j)*histUnfoMatrix_->GetBinError(i,j)*hRecYields->GetBinContent(j)+histUnfoMatrix_->GetBinContent(i,j)*hRecYields->GetBinError(j)*histUnfoMatrix_->GetBinContent(i,j)*hRecYields->GetBinError(j);
      recCalc2+=histUnfoMatrix_->GetBinContent(i,j)*histYieldsRec_->GetBinContent(j);
      recCalcErrSquared2+=histUnfoMatrix_->GetBinError(i,j)*histYieldsRec_->GetBinContent(j)*histUnfoMatrix_->GetBinError(i,j)*histYieldsRec_->GetBinContent(j)+histUnfoMatrix_->GetBinContent(i,j)*histYieldsRec_->GetBinError(j)*histUnfoMatrix_->GetBinContent(i,j)*histYieldsRec_->GetBinError(j);
    }
    std::cout<<hGenYields->GetBinContent(i)<<"+-"<<hGenYields->GetBinError(i)<<" = "<<recCalc1<<"+-"<<sqrt(recCalcErrSquared1)<<" = "<<histYieldsGen_->GetBinContent(i)<<"+-"<<histYieldsGen_->GetBinError(i)<<" = "<<recCalc2<<"+-"<<sqrt(recCalcErrSquared2)<<std::endl;
  }
    


}
