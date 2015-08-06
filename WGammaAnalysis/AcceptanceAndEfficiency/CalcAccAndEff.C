#include "CalcAccAndEff.h" 
  //this class
#include "../Include/TMathTools.h"
#include "../Include/TFullCuts.h" 
#include "../Configuration/TConfiguration.h" 
#include "../Configuration/TInputSample.h"
#include "../Configuration/TAllInputSamples.h"

  //taken from git
#include "TMath.h" 
#include "TH1F.h" 
#include "TVectorF.h" 
#include "TCanvas.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TLegend.h"
#include "TLorentzVector.h"
  // ROOT class
#include <iostream> 
#include <string>
#include <sstream>  
#include <vector>
#include <iomanip>
  //standard C++ class

CalcAccAndEff::CalcAccAndEff(int channel, int vgamma, bool isDebugMode, string configFile)
{

  _channel = channel;
  _vgamma = vgamma;
  _isDebugMode = isDebugMode;
  _INPUT = new TAllInputSamples(channel, vgamma, configFile);
  std::cout<<_config.StrChannel(channel)<<", "<<_config.StrVgType(vgamma)<<std::endl;
//  _INPUT->Print();


}

CalcAccAndEff::~CalcAccAndEff()
{
   _eventTree.fChain = 0;
   //field of TEventTree 
}

void CalcAccAndEff::LoopOverInputFiles()
{
  int nSources = _INPUT->nSources_;
  for (int iSource=0; iSource<nSources; iSource++){
    int sample = _INPUT->allInputs_[iSource].sample_;
    if (sample!=_config.SIGMC_UNSKIMMED) continue;
      // acceptance and efficiency are calculated only through signal MC


    TTree* tree;
    int inputFileNMax = _INPUT->allInputs_[iSource].nFiles_;

    for (int inputFileN=0; inputFileN<inputFileNMax; inputFileN++){
      TFile f((_INPUT->allInputs_[iSource].fileNames_[inputFileN]) );
      _lumiWeight=_INPUT->allInputs_[iSource].lumiWeights_[inputFileN];
      if (f.IsOpen()) 
        std::cout<<std::endl<<"processing file "<<_INPUT->allInputs_[iSource].fileNames_[inputFileN]<<std::endl;
      else{
        std::cout<<"ERROR detected in CalcAccAndEff::LoopOverInputFiles: file "<<_INPUT->allInputs_[iSource].fileNames_[inputFileN]<<" was not found"<<std::endl;
        return;
      } 
      f.cd("ggNtuplizer");
      tree = (TTree*)gDirectory->Get("EventTree");

      TH1F* hEvents = (TH1F*)gDirectory->Get("hEvents");
      _Nentries = hEvents->GetBinContent(1);
      _csWholeMCfile = _INPUT->allInputs_[iSource].cs_[inputFileN];

      if (tree){
        _eventTree.Init(tree);
        //if (sample_!=TInputSample::DATA) SetMCSpecificAddresses();
      }   
      else{
        std::cout<<"ERROR detected in CalcAccAndEff::LoopOverInputFiles: tree in the file "<<_INPUT->allInputs_[iSource].fileNames_[inputFileN]<<" does not exist"<<std::endl;
        return;
      }  

      LoopOverTreeEvents();
        //method of this class (CalcAccAndEff)
      _eventTree.fChain=0;
        //field of TEventTree

    }//loop over inputFileN_ ends

    std::cout<<std::endl;
    std::cout<<"the output will be saved to "<<std::endl<<std::endl;
  } //loop over iSource ends
}// end of LoopOverInputFiles

void CalcAccAndEff::ComputeAccTimesEff()
{

  _fOut=new TFile(_config.GetAccXEffFileName(_channel, _vgamma),"recreate");

  ComputeNumerator();
  std::cout<<"numerator computed"<<std::endl;
  ComputeDenominator();
  std::cout<<"denominator computed"<<std::endl;
   
  if (_denTot!=0){
    _accXeffTot=_numTot/_denTot;
    TMathTools math;
    _accXeffTotErr=math.ErrOfTwoIndependent("x1/(x1+x2)",_numTot,_denTot,_numTotErr,_denTotErr);
    std::cout<<"accXeff Tot="<<"("<<_numTot<<"+-"<<_numTotErr;
    std::cout<<")/("<<_denTot<<"+-"<<_denTotErr<<")="<<_accXeffTot<<"+-"<<_accXeffTotErr<<std::endl;

    _csTheoryTot = _csWholeMCfile*_denNoWeightTot/_Nentries;
  }// end of _denTot!=0

  int nHbins = _Hnumerator1D->GetNbinsX();
  if (_Hdenominator1D->GetNbinsX()!=nHbins){
    std::cout<<"_Hdenominator->GetNbinsX()!=_Hnumerator->GetNbinsX()"<<std::endl;
    return;    
  }
  _fOut->cd();
  std::cout<<"accXeff 1D"<<std::endl;
  _HaccXeff1D=(TH1F*)_Hnumerator1D->Clone(_config.GetAccXEffName(_config.ONEDI));
  _HaccXeff1D->SetTitle(_config.GetAccXEffName(_config.ONEDI)+TString("_")+_config.StrChannel(_channel)+TString("_")+_config.StrVgType(_vgamma));
  if (_Hdenominator1D->GetSumOfWeights()!=0){
    _HaccXeff1D->Divide(_Hdenominator1D);
  }
  for (int ib=1; ib<=nHbins; ib++){
    std::cout<<_Hnumerator1D->GetBinLowEdge(ib)<<"-";
    std::cout<<_Hnumerator1D->GetBinLowEdge(ib)+_Hnumerator1D->GetBinWidth(ib)<<" ";
      std::cout<<"("<<_Hnumerator1D->GetBinContent(ib)<<"+-"<<_Hnumerator1D->GetBinError(ib);
      std::cout<<")/("<<_Hdenominator1D->GetBinContent(ib)<<"+-"<<_Hdenominator1D->GetBinError(ib)<<")=";
    if (_Hdenominator1D->GetBinContent(ib)!=0){
      std::cout<<_HaccXeff1D->GetBinContent(ib)<<"+-"<<_HaccXeff1D->GetBinError(ib);
    } 
    std::cout<<std::endl;

    long neventsVal =  _HdenominatorNoWeight1D->GetBinContent(ib);
    long neventsErr =  _HdenominatorNoWeight1D->GetBinError(ib);
    float binWidth = _HcsTheory1D->GetBinWidth(ib);

    _HcsTheory1D->SetBinContent(ib,_csWholeMCfile*neventsVal/(_Nentries*binWidth));
    _HcsTheory1D->SetBinError(ib,_csWholeMCfile*neventsErr/(_Nentries*binWidth));
  }// end of loop over ib

  std::cout<<"denTot = "<<_denTot<<", ";
  std::cout<<"denNoWeightTot = "<<_denNoWeightTot<<", ";
  std::cout<<std::setprecision(3)<<"csTheoryTot = "<<_csTheoryTot<<std::endl;
  std::cout<<std::setprecision(0);

  _fOut->cd();
  std::cout<<"den, denNoWeight, csTheory 1D"<<std::endl;
  for (int ib=1; ib<=nHbins; ib++){
    std::cout<<_Hdenominator1D->GetBinContent(ib)<<", ";
    std::cout<<_HdenominatorNoWeight1D->GetBinContent(ib)<<", ";
    std::cout<<std::setprecision(3)<<_HcsTheory1D->GetBinContent(ib);
    std::cout<<std::setprecision(0);
    std::cout<<std::endl;
  }// end of loop over ib


  PlotAndSaveOutput();
  
}// end of ComputeAccTimesEff

void CalcAccAndEff::PrintAccAndEffSummary()
{

}// end of PrintAccAndEffSummary

void CalcAccAndEff::LoopOverTreeEvents()
{
   if (_eventTree.fChain == 0) return;
   Long64_t nentries = _eventTree.fChain->GetEntries();
   if (_isDebugMode) {
     if (_eventTree.fChain->GetEntries()<_debugModeNEntries) nentries=_eventTree.fChain->GetEntries();
     else nentries=_debugModeNEntries;
   }

   //goodLeptonPhotonPairs(two-dimentional array of bool-s)
   //memory allocation for some variables: 
   int nLeptonMax;
   if (_channel==TConfiguration::MUON) nLeptonMax=_eventTree.kMaxnMu;
   else if (_channel==TConfiguration::ELECTRON) nLeptonMax=_eventTree.kMaxnEle;
     //kMaxnMu - field of TEventTree
   else{
     std::cout<<"Error detected in WGammaSelection::LoopOverTreeEvents: channel must be either MUON or ELECTRON."<<std::cout;
     return;
   }

   CheckMaxNumbersInTree();
  
   //nentries=20;
   std::cout<<"n entries in MC tree: "<<_eventTree.fChain->GetEntries()<<std::endl;
   
   for (Long64_t entry=0; entry<nentries; entry++) {

      //loop over events in the tree{
     //_eventTree.GetEntryNeededBranchesOnly(_channel,TConfiguration::SIGMC,entry);
          //method of TEventTree class

     _eventTree.GetEntryMCSpecific(entry);
          //method of TEventTree class
     float dR=-1;
     float phoPt=-1;
     int mcPattern=-1;
     int imcPho, imcLep1, imcLep2;
     mcPattern=FindMCparticles(_eventTree.treeLeaf, imcPho, imcLep1, imcLep2);
     //mcPattern=FindDeltaRandPhoPt(_eventTree.treeLeaf, dR, phoPt);
     if (mcPattern==-1) _mcPatternNeg++;
     if (mcPattern>=0) _mcPatternPos[mcPattern]++;
     
     if (PassedPhaseSpaceCut(_eventTree.treeLeaf, imcPho, imcLep1, imcLep2)){
       _denTot+=_lumiWeight;
       _Hdenominator1D->Fill(_eventTree.treeLeaf.mcEt->at(imcPho),_lumiWeight);
       _denNoWeightTot+=1;
       _HdenominatorNoWeight1D->Fill(_eventTree.treeLeaf.mcEt->at(imcPho));
     } 

  } //end of loop over events in the tree
 
}// end of LoopOverTreeEvents

int CalcAccAndEff::FindMCparticles(TEventTree::InputTreeLeaves &leaf, int &imcPho, int &imcLep1, int &imcLep2)
{
  int photonID=22;
  int bosonID;
  if (_vgamma==_config.Z_GAMMA) bosonID=23;
  if (_vgamma==_config.W_GAMMA) bosonID=24;
  int lepID;
  if (_channel==_config.MUON) lepID=13;
  if (_channel==_config.ELECTRON) lepID=11;

  vector <int> vec_imc_pho;
  vector <int> vec_imc_lep;
  for (int imc=0; imc<leaf.nMC; imc++){
    if (leaf.mcPID->at(imc)==photonID &&
         (IsFSR(leaf,imc,lepID,bosonID) || IsTGC(leaf,imc,bosonID) || IsISR(leaf,imc,bosonID)) 
       ) vec_imc_pho.push_back(imc);
    if (fabs(leaf.mcPID->at(imc))==lepID && fabs(leaf.mcMomPID->at(imc))==bosonID) vec_imc_lep.push_back(imc);
  }// end of loop over imc

  imcPho=-1;
  imcLep1=-1;
  imcLep2=-1;// ZGamma only
  int patt=-1;

  if (vec_imc_pho.size()==0 && vec_imc_lep.size()==0) {patt=0; _strPattern[0]="0 photons, 0 leptons";}
  if (vec_imc_pho.size()==0 && vec_imc_lep.size()!=0) {patt=1; _strPattern[1]="0 photons, >0 leptons";}
  if (vec_imc_pho.size()!=0 && vec_imc_lep.size()==0) {patt=2; _strPattern[2]="0 leptons, >0 photons";}
  if (vec_imc_pho.size()==1 && vec_imc_lep.size()==1) {patt=3; _strPattern[3]="1 lepton, 1 photon";}
  if (vec_imc_pho.size()==2 && vec_imc_lep.size()==1) {patt=4; _strPattern[4]="1 lepton, 2 photons";}
  if (vec_imc_pho.size()==1 && vec_imc_lep.size()==2) {patt=5; _strPattern[5]="2 leptons, 1 photon";}
  if (vec_imc_pho.size()==2 && vec_imc_lep.size()==2) {patt=6; _strPattern[6]="2 photons, 2 leptons";}
  if (vec_imc_pho.size()<=2 && vec_imc_lep.size()>=3) {patt=7; _strPattern[7]="1-2 photons, >2 leptons";}
  if (vec_imc_pho.size()>=3 && vec_imc_lep.size()==1) {patt=8; _strPattern[8]="1 lepton, >2 photons";}
  if (vec_imc_pho.size()>=3 && vec_imc_lep.size()==2) {patt=9; _strPattern[9]="2 leptons, >2 photons";}

   if (vec_imc_pho.size()==0 || vec_imc_lep.size()==0 ||
       (_vgamma==_config.Z_GAMMA && vec_imc_lep.size()==1)){
    return patt;
  }

  imcLep1=vec_imc_lep[0];
  if (_vgamma==_config.Z_GAMMA) imcLep2=vec_imc_lep[1];

  if (vec_imc_pho.size()==1) {imcPho=vec_imc_pho[0]; }
  else{
    float phoPt=-1;
    for (int iv=0; iv<vec_imc_pho.size(); iv++){
      if (leaf.mcEt->at(vec_imc_pho[iv])>phoPt){
        imcPho=vec_imc_pho[iv]; //phoPt=leaf.mcEt->at(imcPho);
      }
    }// end of loop over iv
  }// end of else

  //  if (imcPho==-1 || imcLep1==-1){return patt;}
  //  if (_vgamma==_config.Z_GAMMA && imcLep1==-1){return patt;}

  //  TMathTools math;
  //  dR=math.DeltaR(leaf.mcPhi->at(imcPho), leaf.mcEta->at(imcPho), leaf.mcPhi->at(imcLep1), leaf.mcEta->at(imcLep1));
  // if (_vgamma==_config.Z_GAMMA){
  //   float dR2=math.DeltaR(leaf.mcPhi->at(imcPho), leaf.mcEta->at(imcPho), leaf.mcPhi->at(imcLep2), leaf.mcEta->at(imcLep2));
  //   if (dR2<dR) dR=dR2;
  // }

  return patt;
}// end of FindMCparticles

bool CalcAccAndEff::PassedPhaseSpaceCut(TEventTree::InputTreeLeaves &leaf, int imcPho, int imcLep1, int imcLep2)
{
  if (imcPho==-1) return 0; _passed.imcPho++;
  if (imcLep1==-1) return 0; _passed.imcLep1++;
  if (_vgamma==_config.Z_GAMMA && imcLep2==-1) return 0; _passed.imcLep2++;

  TMathTools math;
  float dR=math.DeltaR(leaf.mcPhi->at(imcPho), leaf.mcEta->at(imcPho), leaf.mcPhi->at(imcLep1), leaf.mcEta->at(imcLep1));
  if (!(dR>0.7)) return 0; _passed.dR++;
  if (!(fabs(leaf.mcEta->at(imcPho))<2.5)) return 0; _passed.phoEta++;
  if (!(fabs(leaf.mcEta->at(imcLep1))<2.5)) return 0; _passed.lep1Eta++;
  if (!(leaf.mcPt->at(imcLep1)>20)) return 0; _passed.lep2Eta++;

  if (_vgamma==_config.Z_GAMMA){
     float dR2=math.DeltaR(leaf.mcPhi->at(imcPho), leaf.mcEta->at(imcPho), leaf.mcPhi->at(imcLep2), leaf.mcEta->at(imcLep2));
     if (!(dR2>0.7)) return 0; _passed.dR2++;
     if (!(fabs(leaf.mcEta->at(imcLep2))<2.5)) return 0; _passed.lep2Eta++;
     if (!(leaf.mcPt->at(imcLep2)>20)) return 0; _passed.lep2Pt++;
     TLorentzVector vlep1, vlep2;
     vlep1.SetPtEtaPhiM(leaf.mcPt->at(imcLep1),leaf.mcEta->at(imcLep1),leaf.mcPhi->at(imcLep1),0);
     vlep2.SetPtEtaPhiM(leaf.mcPt->at(imcLep2),leaf.mcEta->at(imcLep2),leaf.mcPhi->at(imcLep2),0);
     if(!((vlep1 + vlep2).M() > 50)) return 0; _passed.Mll++;
  }// end of if (_vgamma==_config.Z_GAMMA)

  //I_gen03 < 5 GeV for photon
  float Igen=0;
  for (int imc=0; imc<leaf.nMC; imc++){
    if (imc==imcPho) continue;
    float dRIgen = math.DeltaR(leaf.mcPhi->at(imcPho), leaf.mcEta->at(imcPho), leaf.mcPhi->at(imc), leaf.mcEta->at(imc));
    if (dRIgen<0.3) Igen+=leaf.mcPt->at(imc);
  }// end of imc  
  if (!(Igen<5)) return 0; _passed.phoIgen++;
//  if (!(leaf.mcEt->at(imcPho)>15)) return 0; _passed.phoPt++;

  return 1;
}// end of PassedPhaseSpaceCut()


bool CalcAccAndEff::IsFSR(TEventTree::InputTreeLeaves &leaf, int imcPho, int lepID, int bosonID)
{
  if (leaf.mcPID->at(imcPho)!=22) return 0;
  if (fabs(leaf.mcMomPID->at(imcPho))!=lepID) return 0;
  if (fabs(leaf.mcGMomPID->at(imcPho))!=bosonID) return 0;
  return 1;
}// end of IsFSR

bool CalcAccAndEff::IsTGC(TEventTree::InputTreeLeaves &leaf, int imcPho, int bosonID)
{
  if (leaf.mcPID->at(imcPho)!=22) return 0;
  if (fabs(leaf.mcMomPID->at(imcPho))!=bosonID) return 0;
  return 1;
}// end of IsTGC

bool CalcAccAndEff::IsISR(TEventTree::InputTreeLeaves &leaf, int imcPho, int bosonID)
{
  if (leaf.mcPID->at(imcPho)!=22) return 0;
  for (int imc=0; imc<leaf.nMC; imc++){
    if (fabs(leaf.mcPID->at(imc))==bosonID && leaf.mcMomPID->at(imc)==leaf.mcMomPID->at(imcPho))
      if (fabs(leaf.mcMomPID->at(imc))<=6 || fabs(leaf.mcMomPID->at(imc))==21)
        return 1;
      // PID 1-6 - quarks, 21 - gluon 
      // (sometimes photon Mom is gluon even though gluon physically can't radiate photon)
  }// end of loop over imc
  return 0;
}// end of IsISR

void CalcAccAndEff::ComputeNumerator()
{
  TString fName = _config.GetYieldsFileName(_channel, _vgamma, _config.TEMPL_CHISO, "phoEt");
  TFile* fYields = new TFile(fName);
  _Hnumerator1D = (TH1F*)fYields->Get(_config.GetYieldsSignalMCGenBinsName(_config.ONEDI, _config.COMMON));
  TH1F* hTemp = (TH1F*)fYields->Get(_config.GetYieldsSelectedName(_config.ONEDI, _config.COMMON, _config.SIGMC));
  _numTot=0;
  _numTotErr=0;
  for (int ib=1; ib<=hTemp->GetNbinsX(); ib++){
    _numTot += hTemp->GetBinContent(ib);
    _numTotErr = hTemp->GetBinError(ib)*hTemp->GetBinError(ib);
  }
  _numTotErr=sqrt(_numTotErr);

}// end of ComputeNumerator

void CalcAccAndEff::ComputeDenominator()
{
  _mcPatternNeg=0;
  for (int ip=0; ip<_nPosPatts; ip++){
    _mcPatternPos[ip]=0;
  }

  _Hdenominator1D = (TH1F*)_Hnumerator1D->Clone(_Hnumerator1D->GetName()+TString("_PhaseSpaceOnly"));
  _denTot=0;
  _denTotErr=0;
  _Hdenominator1D->Sumw2();

  _HdenominatorNoWeight1D = (TH1F*)_Hnumerator1D->Clone(_Hnumerator1D->GetName()+TString("_denNoWeight"));
  _denNoWeightTot=0;
  _denNoWeightTotErr=0;

  _HcsTheory1D = (TH1F*)_Hnumerator1D->Clone(_config.GetTheoryCSname(_config.ONEDI));
  _HcsTheory1D->SetTitle(_config.GetTheoryCSname(_config.ONEDI));
  _csTheoryTot=0;
  _csTheoryTotErr=0;

  for (int ib=1; ib<=_Hdenominator1D->GetNbinsX(); ib++){
    _Hdenominator1D->SetBinContent(ib,0);
    _Hdenominator1D->SetBinError(ib,0);
    _HdenominatorNoWeight1D->SetBinContent(ib,0);
    _HdenominatorNoWeight1D->SetBinError(ib,0);
    _HcsTheory1D->SetBinContent(ib,0);
    _HcsTheory1D->SetBinError(ib,0);
  }

  SetNullPassedLevels();

  LoopOverInputFiles();

  PrintPassedLevels();

  _denTotErr=sqrt(_denTot);
  std::cout<<"_mcPatternNeg="<<_mcPatternNeg<<std::endl;
  for (int ip=0; ip<_nPosPatts; ip++){
    std::cout<<"_mcPatternPos["<<ip<<"]="<<_mcPatternPos[ip]<<", ("<<_strPattern[ip]<<")"<<std::endl;
  }
}// end of ComputeDenominator

void CalcAccAndEff::PlotAndSaveOutput()
{
  _fOut->cd();
  TString cName="C_accXeff_";
  cName+=_config.StrChannel(_channel);
  cName+="_";
  cName+=_config.StrVgType(_vgamma);
  TCanvas* canv = new TCanvas(cName,cName);
  canv->SetLogx();
  _HaccXeff1D->SetLineWidth(2);
  _HaccXeff1D->GetXaxis()->SetMoreLogLabels(); 
  _HaccXeff1D->GetXaxis()->SetNoExponent();
  _HaccXeff1D->Draw();

  _HaccXeff1D->Write();
  _HcsTheory1D->Write();

  TMathTools math;
  TH1F* HaccXeffTot = math.ComputeHistTotal(_config.GetAccXEffName(_config.TOTAL), _HaccXeff1D);
  TH1F* HcsTheoryTot = math.ComputeHistTotal(_config.GetTheoryCSname(_config.TOTAL),_HcsTheory1D);
  HaccXeffTot->SetBinContent(1,_accXeffTot);
  HaccXeffTot->SetBinError(1,_accXeffTotErr);
  HcsTheoryTot->SetBinContent(1,_csTheoryTot);
  HcsTheoryTot->SetBinError(1,_csTheoryTotErr);
  HaccXeffTot->Write();
  HcsTheoryTot->Write();
}// end of PlotAndSaveOutput

bool CalcAccAndEff::CheckMaxNumbersInTree()
{
  if (_eventTree.fChain->GetMaximum("nMC")>_eventTree.kMaxnMC){
    std::cout<<"ERROR in CalcAccAndEff::LoopOverTreeEvents: _eventTree.treeLeaf.nMC="<<_eventTree.fChain->GetMaximum("nMC")<<" > kMaxnMC="<<_eventTree.kMaxnMC<<"; go to ../Include/TEventTree.h to increase kMaxnMC"<<std::endl;
    return 0;
  }
  if (_eventTree.fChain->GetMaximum("nPho")>_eventTree.kMaxnPho){
     std::cout<<"ERROR in CalcAccAndEff::LoopOverTreeEvents: _eventTree.treeLeaf.nPho="<<_eventTree.fChain->GetMaximum("nPho")<<" > kMaxnPho="<<_eventTree.kMaxnPho<<"; go to ../Include/TEventTree.h to increase kMaxnPho"<<std::endl;
     return 0;
   }
   if (_channel==TConfiguration::MUON && _eventTree.fChain->GetMaximum("nMu")>_eventTree.kMaxnMu){
     std::cout<<"ERROR in CalcAccAndEff::LoopOverTreeEvents: _eventTree.treeLeaf.nMu="<<_eventTree.fChain->GetMaximum("nMu")<<" > kMaxnMu="<<_eventTree.kMaxnMu<<"; go to ../Include/TEventTree.h to increase kMaxnMu"<<std::endl;
     return 0;
   }
   if (_channel==TConfiguration::ELECTRON && _eventTree.fChain->GetMaximum("nEle")>_eventTree.kMaxnEle){
     std::cout<<"ERROR in CalcAccAndEff::LoopOverTreeEvents: _eventTree.treeLeaf.nEle="<<_eventTree.fChain->GetMaximum("nEle")<<" > kMaxnEle="<<_eventTree.kMaxnEle<<"; go to ../Include/TEventTree.h to increase kMaxnEle"<<std::endl;
     return 0;
   }
  return 1;
}// end of CheckMaxNumbersInTree

void CalcAccAndEff::SetNullPassedLevels()
{
  _passed.imcPho=0;
  _passed.imcLep1=0;
  _passed.imcLep2=0;
  _passed.dR=0;
  _passed.phoEta=0;
  _passed.lep1Eta=0;
  _passed.lep2Eta=0;
  _passed.lep2Pt=0;
  _passed.dR2=0;
  _passed.lep2Eta=0;
  _passed.Mll=0;
  _passed.phoIgen=0;
  _passed.phoPt=0;
}// end of SetNullPassedLevels()

void CalcAccAndEff::PrintPassedLevels()
{
  std::cout<<std::endl;
  std::cout<<"_passed:"<<std::endl;
  std::cout<<"imcPho="<<_passed.imcPho<<", ";
  std::cout<<"imcLep1="<< _passed.imcLep1<<", ";
  std::cout<<"imcLep2="<< _passed.imcLep2<<", ";
  std::cout<<"dR="<<_passed.dR<<", ";
  std::cout<<"phoEta="<<_passed.phoEta<<", ";
  std::cout<<"lep1Eta="<<_passed.lep1Eta<<", ";
  std::cout<<"dR="<<_passed.dR2<<", ";
  std::cout<<"lep2Eta="<<_passed.lep2Eta<<", ";
  std::cout<<"lep2Pt="<<_passed.lep2Pt<<", ";
  std::cout<<"Mll="<<_passed.Mll<<", ";
  std::cout<<"phoIgen="<<_passed.phoIgen<<", ";
  std::cout<<"phoPt="<<_passed.phoPt;
  std::cout<<std::endl;
  std::cout<<std::endl;
}// end of PrintPassedLevels()
