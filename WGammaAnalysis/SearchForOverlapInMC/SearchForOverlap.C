#include "SearchForOverlap.h" 
  //this class
#include "../Include/TPuReweight.h" 
#include "../Configuration/TConfiguration.h" 
#include "../Configuration/TInputSample.h"
#include "../Configuration/TAllInputSamples.h"
  //this package
#include "TMath.h" 
#include "TCanvas.h" 
#include "TLorentzVector.h" 
#include "TLegend.h" 
#include "TCanvas.h" 
#include "TH1F.h" 
#include "TLatex.h" 
  //ROOT class
#include <iostream> 
#include <iomanip>
#include <string>
#include <sstream>  
  //standard C++ class

SearchForOverlap::SearchForOverlap(int channel, int vgamma, int cutType, bool isDebugMode, string analyzedSampleNames, string configFile, bool isNoPuReweight)
{

  _INPUT = new TAllInputSamples(channel, vgamma, configFile);

  _channel=channel;
  _cutType=cutType;
  _isNoPuReweight=isNoPuReweight;
  _isDebugMode=isDebugMode;

  std::cout<<"_isDegubMode: "<<_isDebugMode<<std::endl;  

  stringstream ss(analyzedSampleNames);
  vector <string> names;
  string name;
  while (ss >> name) 
    names.push_back(name);
  int nNames = names.size();

  std::cout<<"1 - will do, 0 - will not do"<<std::endl;
  for (int i=0; i<_INPUT->nSources_; i++)
    {
      _doAnalizeSample.push_back(0);  
      for (int j=0; j<nNames; j++)
        {
          if (names[j]==_INPUT->allInputs_[i].sourceName_)
            _doAnalizeSample.back()=1; 
        }
      std::cout<<_INPUT->allInputs_[i].sourceName_<<": "<<_doAnalizeSample[i]<<std::endl;
    }  
  if ((int)_doAnalizeSample.size()!=_INPUT->nSources_)
    std::cout<<"ERROR in SearchForOverlap::SearchForOverlap: wrong _doAnalizeSample.size()"<<std::endl;

  _fOut = new TFile("fOut.root","recreate");

}

SearchForOverlap::~SearchForOverlap()
{
   _eventTree.fChain = 0;
     //field of TEventTree 
}

void SearchForOverlap::LoopOverInputFiles()
{
  int nSources = _INPUT->nSources_; 
  //if (isDebugMode_ && nSources>3)
  //  nSources = 3;
  _hasWGamma=0;
  _hasWJets=0;
  _hasZGamma=0;
  _hasZJets=0;
  for (_iSource=0; _iSource<nSources; _iSource++)
    {
       std::cout<<"iSource="<<_iSource<<": "<<_INPUT->allInputs_[_iSource].sourceName_<<std::endl;
       _sample = _INPUT->allInputs_[_iSource].sample_;
       if (_sample==TConfiguration::DATA){
         _lumiData=_INPUT->allInputs_[_iSource].lumiTotal_;
       }
       if (!_doAnalizeSample[_iSource])
         continue;

       if(_INPUT->allInputs_[_iSource].sourceName_=="Wg"){ 
          _hasWGamma=1;
          _idWGamma=_iSource;
       }
       else if(_INPUT->allInputs_[_iSource].sourceName_=="Wjets_to_lnu"){  
         _hasWJets=1;
         _idWJets=_iSource;
       }
       else if(_INPUT->allInputs_[_iSource].sourceName_=="Zg"){
         _hasZGamma=1;
         _idZGamma=_iSource;
       }
       else if(_INPUT->allInputs_[_iSource].sourceName_=="DYjets_to_ll"){
          _hasZJets=1;
          _idZJets=_iSource;
       }      
      std::cout<<"_idZGamma="<<_idZGamma<<", _idZJets="<<_idZJets<<std::endl;
      std::cout<<"_hasZGamma="<<_hasZGamma<<", _hasZJets="<<_hasZJets<<std::endl;
      std::cout<<"_idWGmma="<<_idWGamma<<", _idWJets="<<_idWJets<<std::endl;
      std::cout<<"_hasWGamma="<<_hasWGamma<<", _hasWJets="<<_hasWJets<<std::endl;
       TTree* tree;
       int inputFileNMax = _INPUT->allInputs_[_iSource].nFiles_;
       if (inputFileNMax>1) return;

       TFile* f = new TFile(_INPUT->allInputs_[_iSource].fileNames_[0]);
        if (f->IsOpen()) 
          std::cout<<std::endl<<"processing file "<<_INPUT->allInputs_[_iSource].fileNames_[0]<<std::endl;
         else{
           std::cout<<"ERROR detected in SearchForOverlap::LoopOverInputFiles: file "<<_INPUT->allInputs_[_iSource].fileNames_[0]<<" was not found"<<std::endl;
           return;
         } 
         f->cd("ggNtuplizer");
         tree = (TTree*)gDirectory->Get("EventTree");
         if (tree) 
           _eventTree.Init(tree);
         else{
           std::cout<<"ERROR detected in SearchForOverlap::LoopOverInputFiles: tree in the file "<<_INPUT->allInputs_[_iSource].fileNames_[0]<<" does not exist"<<std::endl;
           return;
         } 
         TH1F* hEvents = (TH1F*)gDirectory->Get("hEvents");
         _nentries = (double)hEvents->GetBinContent(1);
         float csMC=_INPUT->allInputs_[_iSource].cs_[0];
         _lumiWeight=_lumiData*csMC/_nentries;
         int bosonType;
         if (_INPUT->allInputs_[_iSource].sourceName_=="Wg" ||
_INPUT->allInputs_[_iSource].sourceName_=="Wjets_to_lnu")
           bosonType=W_BOSON;
         if (_INPUT->allInputs_[_iSource].sourceName_=="Zg" ||
_INPUT->allInputs_[_iSource].sourceName_=="DYjets_to_ll")
           bosonType=Z_BOSON;
         _puWeight=new TPuReweight(_config.GetPileupDataFileName(),_INPUT->allInputs_[_iSource].fileNames_[0]);
         _histsSet[_iSource]=0;
         SetHists();           
         LoopOverTreeEvents(bosonType);
             //method of this class (SearchForOverlap)
         _eventTree.fChain=0;
             //field of TEventTree
         delete _puWeight;
    } //loop over iSource ends
    PlotHists();
}// end of LoopOverInputFiles()

void SearchForOverlap::LoopOverTreeEvents(int bosonType)
{

   if (_eventTree.fChain == 0) return;
   Long64_t nentries = _eventTree.fChain->GetEntries();
   if (_isDebugMode && nentries>_debugModeNEntries) 
     nentries=_debugModeNEntries;

   if (_sample==_config.DATA){
     std::cout<<"ERROR in SearchForOverlap::LoopOverTreeEvents: search for overlap is impossible for DATA"<<std::endl;
     return;
   }

   _nEvents=0;
   _nEventsWithPhotons=0;
   _nPhotons=0;

   for (int ib=Z_BOSON; ib<=W_BOSON; ib++){
     _nTGC[ib]=0;//( W,Z -> gamma)
     for (int il=0; il<=TAU_LEPTON; il++)
       _nFSR[ib][il]=0;//(W -> (mu,e,tau) -> gamma)
     for (int iq=0; iq<=GLUON; iq++)      
       _nISR[ib][iq]=0;   //(d,u,s,c,b,t) -> gamma, 
   }

   for (int npho=0; npho<91; npho++){
     _nEventsWithGivenNOfPhotons[npho]=0;
     _nEventsWithSomeScheme[npho]=0;
   }
   _nthFakePhoton=0;
   for (_entry=0; _entry<nentries; _entry++){ 
   //loop over events in the tree
     _eventTree.GetEntryMCSpecific(_entry);
     _treeLeaf = _eventTree.treeLeaf;
     _totalWeight = 1;
     _totalWeight = _lumiWeight;
     _totalWeight*= _puWeight->GetPuWeightMc(_treeLeaf.puTrue->at(1));
     _nEvents+=_totalWeight;
     _histWeight[_iSource]->Fill(_totalWeight);
     LoopOverMCParticles(bosonType);
   } //end of loop over events in the tree
   PrintResults();
}// end of LoopOverTreeEvents()

void SearchForOverlap::LoopOverMCParticles(int bosonType)
{
   bool isMuonChannel=0;
   for (int iMC=0; iMC<_treeLeaf.nMC; iMC++){
     if (bosonType==W_BOSON && fabs(_treeLeaf.mcPID->at(iMC))==W_BOSON){
       if (_treeLeaf.mcDecayType->at(iMC)==3)//3 - muon channel 
         isMuonChannel=1;
     }
     if (bosonType==Z_BOSON && fabs(_treeLeaf.mcPID->at(iMC))==Z_BOSON){
       if (_treeLeaf.mcDecayType->at(iMC)==3)//3 - muon channel 
         isMuonChannel=1;
     }
   }// end of loop over iMC
   if (!isMuonChannel) return;
   
   int nPhoInEvent=0;
   _nSomeScheme=0;
   bool isZMuonFSR=0;
   bool isWMuonFSR=0;
   bool isZMuonTGC=0;
   bool isWMuonTGC=0;
   bool isZMuonISR=0;
   bool isWMuonISR=0;
   bool hasGammaInAcc=0;
   bool hasTrueGammaInAcc=0;
   int ilep1=-1;
   int ilep2=-1;
   int ipho=-1;
   float dRCut=0.7;
   FindHighestPtPhotonIndexWithGivenKin(bosonType, dRCut, ipho, ilep1, ilep2);
     //loop is inside this function
   ComputeKinematicVariables(ipho, ilep1, ilep2);
   if (ipho==-1) return;
   if (ilep1==-1) return;

   bool doFill=0;
   if (_cutType==CUT_PAR)
      doFill=CheckIfTrueGammaMCparentage(ipho);
   if (_cutType==CUT_SOME_KIN_NO_PAR)
      doFill=(_treeLeaf.mcEt->at(ipho)>15);
   if (_cutType==CUT_KIN_PAR){
      doFill=CheckIfTrueGammaMCparentage(ipho);
      doFill=doFill && 
        (ilep1>-1 && _dRLep1Pho>0.7 && _treeLeaf.mcPt->at(ilep1)>20);
      if (bosonType==Z_BOSON) 
        doFill=doFill && 
          (ilep2>-1 && _dRLep2Pho>0.7 && _treeLeaf.mcPt->at(ilep2)>20);
      doFill = doFill && (_treeLeaf.mcEt->at(ipho)>15);
   }
   if (_cutType==CUT_KIN_MLL_PAR){
      doFill=CheckIfTrueGammaMCparentage(ipho);
      doFill=doFill && 
        (ilep1>-1 && _dRLep1Pho>0.7 && _treeLeaf.mcPt->at(ilep1)>20);
      if (bosonType==Z_BOSON) 
        doFill=doFill && 
          (ilep2>-1 && _dRLep2Pho>0.7 && _treeLeaf.mcPt->at(ilep2)>20);
      doFill = doFill && (_treeLeaf.mcEt->at(ipho)>15);
      doFill = doFill && (_massLepLep>80 && _massLepLep<100);
   }

   if (doFill) FillHists(ipho, ilep1, ilep2);
   else return;

   if (nPhoInEvent<91)
     _nEventsWithGivenNOfPhotons[nPhoInEvent]+=_totalWeight; 
   if (_nSomeScheme<91)
     _nEventsWithSomeScheme[_nSomeScheme]+=_totalWeight; 

   if ((nPhoInEvent==0 && _nEventsWithGivenNOfPhotons[0]<0) ||
       (_nSomeScheme==0 && _nEventsWithSomeScheme[0]<0)){
     PrintIfSuspicious();
   }

}//end of LoopOverMCParticles()

void SearchForOverlap::FindHighestPtPhotonIndexWithGivenKin(int bosonType, float dRCut, int& ipho, int &ilep1, int &ilep2)
{
  ipho=-1;
  float pt=-1;
  for (int imc=0; imc<_treeLeaf.nMC; imc++){
    if (_treeLeaf.mcPID->at(imc)!=22) continue;
    ilep1=-1;
    ilep2=-1;
    FindLeptonIndexes(bosonType, imc, ilep1, ilep2);
    ComputeKinematicVariables(imc, ilep1, ilep2);
    if (ilep1==-1) continue;
    if (ilep1>-1 && _dRLep1Pho<dRCut) continue;
    if (_treeLeaf.mcEt->at(imc)>pt){
      pt=_treeLeaf.mcEt->at(imc);
      ipho=imc;
    }
  }//end of loop over imc  
  FindLeptonIndexes(bosonType, ipho, ilep1, ilep2);
}

void SearchForOverlap::SetHists()
{
  _fOut->cd();
  TString histName="h";
  histName+=_INPUT->allInputs_[_iSource].sourceName_;

  _histWeight[_iSource]=new TH1F(histName+TString("_weight"),histName+TString("_weight"),1000,0,100);
  _histMCparentage[_iSource]=new TH1F(histName+TString("_MCparentage"),histName+TString("_MCparentage"),31,-0.5,30.5);

  _histPhoPt[_iSource]=new TH1F(histName+TString("_PhoPt"),histName+TString(" Pho Pt"),60,0,60);
  _histPhoEta[_iSource]=new TH1F(histName+TString("_PhoEta"),histName+TString(" Pho Eta"),10,-3.6,3.6);
  _histPhoPhi[_iSource]=new TH1F(histName+TString("_PhoPhi"),histName+TString(" Pho Phi"),8,-1.125*TMath::Pi(),1.125*TMath::Pi());

  _histLep1Pt[_iSource]=new TH1F(histName+TString("_Mu1Pt"),histName+TString(" Mu1 Pt"),20,0,100);
  _histLep1Eta[_iSource]=new TH1F(histName+TString("_Mu1Eta"),histName+TString(" Mu1 Eta"),10,-3.6,3.6);
  _histLep1Phi[_iSource]=new TH1F(histName+TString("_Mu1Phi"),histName+TString(" Mu1 Phi"),8,-1.125*TMath::Pi(),1.125*TMath::Pi());

  _histLep2Pt[_iSource]=new TH1F(histName+TString("_Mu2Pt"),histName+TString(" Mu2 Pt"),20,0,100);
  _histLep2Eta[_iSource]=new TH1F(histName+TString("_Mu2Eta"),histName+TString(" Mu2 Eta"),10,-3.6,3.6);
  _histLep2Phi[_iSource]=new TH1F(histName+TString("_Mu2Phi"),histName+TString(" Mu2 Phi"),8,-1.125*TMath::Pi(),1.125*TMath::Pi());

  _histLep1PhodR[_iSource]=new TH1F(histName+TString("_Lep1PhodR"),histName+TString(" dR(Mu1,Pho)"),90,0,4.5);
  _histLep2PhodR[_iSource]=new TH1F(histName+TString("_Lep2PhodR"),histName+TString(" dR(Mu2,Pho)"),90,0,4.5);
  _histLepLepdR[_iSource]=new TH1F(histName+TString("_LepLepdR"),histName+TString(" dR(Mu1,Mu2)"),40,0,20);
  _histLepLepMassT[_iSource]=new TH1F(histName+TString("_LepLepMassTr"),histName+TString(" MassTr(mu1,mu2)"),12,0,120);
  _histLepLepMass[_iSource]=new TH1F(histName+TString("_LepLepMassInv"),histName+TString(" MassInv(mu1,mu2)"),12,0,120);

  _histsSet[_iSource]=1;
}//end of SetHists()

void SearchForOverlap::PlotHists()
{
  std::cout<<"Ready to PlotHists"<<std::endl;
  if (_hasZGamma && _hasZJets){
    PlotHistsGammaJetsCompare("Z", _idZGamma, _idZJets);
  }
  if (_hasWGamma && _hasWJets){
    PlotHistsGammaJetsCompare("W", _idWGamma, _idWJets);
  }
}//end of PlotHists()

void SearchForOverlap::PlotHistsGammaJetsCompare(TString strZorW, int ind1, int ind2)
{
    PlotTwoHistograms(strZorW+TString("_PhoPt"), _histPhoPt[ind1], _histPhoPt[ind2]);
    PlotTwoHistograms(strZorW+TString("_PhoEta"), _histPhoEta[ind1], _histPhoEta[ind2]);
    PlotTwoHistograms(strZorW+TString("_PhoPhi"), _histPhoPhi[ind1], _histPhoPhi[ind2]);
    PlotTwoHistograms(strZorW+TString("_Lep1Pt"), _histLep1Pt[ind1], _histLep1Pt[ind2]);
    PlotTwoHistograms(strZorW+TString("_Lep1Eta"), _histLep1Eta[ind1], _histLep1Eta[ind2]);
    PlotTwoHistograms(strZorW+TString("_Lep1Phi"), _histLep1Phi[ind1], _histLep1Phi[ind2]);
    PlotTwoHistograms(strZorW+TString("_Lep2Pt"), _histLep2Pt[ind1], _histLep2Pt[ind2]);
    PlotTwoHistograms(strZorW+TString("_Lep2Eta"), _histLep2Eta[ind1], _histLep2Eta[ind2]);
    PlotTwoHistograms(strZorW+TString("_Lep2Phi"), _histLep2Phi[ind1], _histLep2Phi[ind2]);
    PlotTwoHistograms(strZorW+TString("_Lep1PhodR"), _histLep1PhodR[ind1], _histLep1PhodR[ind2]);
    PlotTwoHistograms(strZorW+TString("_Lep2PhodR"), _histLep2PhodR[ind1], _histLep2PhodR[ind2]);
    PlotTwoHistograms(strZorW+TString("_LepLepdR"), _histLepLepdR[ind1], _histLepLepdR[ind2]);
    PlotTwoHistograms(strZorW+TString("_LepLepMassT"), _histLepLepMassT[ind1], _histLepLepMassT[ind2]);
    PlotTwoHistograms(strZorW+TString("_LepLepMass"), _histLepLepMass[ind1], _histLepLepMass[ind2]);
    PlotTwoHistograms(strZorW+TString("_Weight"), _histWeight[ind1], _histWeight[ind2]);
    PlotTwoHistograms(strZorW+TString("_MCparentage"), _histMCparentage[ind1], _histMCparentage[ind2]);
}//end of PlotHistsGammaJetsCompare(TString strZorW, int ind1, int ind2)

void SearchForOverlap::PlotTwoHistograms(TString canvName, TH1F* hGamma, TH1F* hJets)
{
   if (hGamma->GetSumOfWeights()==0 && hJets->GetSumOfWeights()==0) 
     return;
   TCanvas* canv1 = new TCanvas(canvName,canvName,600,600);
   hJets->SetLineWidth(2);
   hGamma->SetLineWidth(2);
   hJets->SetLineColor(1);
   hGamma->SetLineColor(2);
   float maxTotal=0;
   bool isGammaFirstTotal=0;
   float maxNormalized=0;
   bool isGammaFirstNormalized=0;
   float sumGamma=0;
   float sumJets=0;
   //for total histograms
   hGamma->SetStats(0);
   hJets->SetStats(0);
   TString strVar=hGamma->GetTitle();
   strVar.ReplaceAll("hWg_","Wg/Wjets: ");
   strVar.ReplaceAll("hZg_","Zg/Zjets: ");
   
   hGamma->SetTitle("");
   hJets->SetTitle("");
   for (int ib=1; ib<=hJets->GetNbinsX(); ib++){
     sumGamma+=hGamma->GetBinContent(ib);
     sumJets+=hJets->GetBinContent(ib);
     if (hGamma->GetBinContent(ib)>hJets->GetBinContent(ib)){
       if (hGamma->GetBinContent(ib)>maxTotal){
         maxTotal=hGamma->GetBinContent(ib);
         isGammaFirstTotal=1;
       }
     }
     else{
       if (hJets->GetBinContent(ib)>maxTotal){
         maxTotal=hJets->GetBinContent(ib);
         isGammaFirstTotal=0;
       }
     }
   }//end of loop over ib
   //for normalized histograms
   for (int ib=1; ib<=hJets->GetNbinsX(); ib++){
     if ((hGamma->GetBinContent(ib)/sumGamma)>(hJets->GetBinContent(ib)/sumJets)){
       if ((hGamma->GetBinContent(ib)/sumGamma)>maxNormalized){
         maxNormalized=hGamma->GetBinContent(ib)/sumGamma;
         isGammaFirstNormalized=1;
       }
     }
     else{
       if (hJets->GetBinContent(ib)/sumJets>maxNormalized){
         maxNormalized=hJets->GetBinContent(ib)/sumJets;
         isGammaFirstNormalized=0;
       }
     }
   }//end of loop over ib

   TLegend* leg = new TLegend(0.75,0.85,0.95,0.95);
   leg->AddEntry(hGamma,"hGamma","l");
   leg->AddEntry(hJets,"hJets","l");

   if (isGammaFirstTotal){
     hGamma->Draw();
     hJets->Draw("same");
   }
   else{
     hJets->Draw();
     hGamma->Draw("same");
   }
   TLatex* latexTitle1 = new TLatex(0.05,0.95,strVar);
   latexTitle1->SetNDC();
   latexTitle1->Draw("same");
   TCanvas* canv2 = new TCanvas(canvName+TString("_Normalized"),canvName+TString("_Normalized"),600,600);
   if (isGammaFirstNormalized){
     hGamma->DrawNormalized();
     hJets->DrawNormalized("same");
   }
   else{
     hJets->DrawNormalized();
     hGamma->DrawNormalized("same");
   }
   strVar+=" Normalized";
   TLatex* latexTitle2 = new TLatex(0.05,0.95,strVar);
   latexTitle2->SetNDC();
   latexTitle2->Draw("same");
   canv1->SaveAs(canvName+TString(".png"));
   canv2->SaveAs(canvName+TString("_Normalized.png"));
   //hGamma->Print();
   //hJets->Print();
}//end of PlotTwoHistograms(TString canvName, TH1F* hGamma, TH1F* hJets)

bool SearchForOverlap::CheckIfTrueGammaMCparentage(int imc)
{
  if (!(_treeLeaf.mcParentage->at(imc)&4)){
//  if ((_treeLeaf.mcParentage->at(imc) & 0x12)==0x12){
    _nTrue+=_totalWeight;
    return 1;
  }
  return 0;
}

void SearchForOverlap::CheckIfFSR(int imc, bool& isZMuonFSR, bool& isWMuonFSR)
{
   unsigned int ib = fabs(_treeLeaf.mcGMomPID->at(imc));
   unsigned int il = fabs(_treeLeaf.mcMomPID->at(imc));
   if (fabs(_treeLeaf.mcGMomPID->at(imc))==Z_BOSON || fabs(_treeLeaf.mcGMomPID->at(imc))==W_BOSON){ 
      if (fabs(_treeLeaf.mcMomPID->at(imc))==E_LEPTON ||
          fabs(_treeLeaf.mcMomPID->at(imc))==MU_LEPTON ||
          fabs(_treeLeaf.mcMomPID->at(imc))==TAU_LEPTON){
         _nFSR[ib][ANY_LEPTON]+=_totalWeight;
         _nFSR[ib][il]+=_totalWeight;
         _nSomeScheme++;
         if(((fabs(_treeLeaf.mcGMomPID->at(imc))==Z_BOSON) &&
            (fabs(_treeLeaf.mcMomPID->at(imc))==MU_LEPTON)))
           isZMuonFSR=1;    
         if(((fabs(_treeLeaf.mcGMomPID->at(imc))==W_BOSON) &&
            (fabs(_treeLeaf.mcMomPID->at(imc))==MU_LEPTON)))
           isWMuonFSR=1;           
      }  //end of  if (fabs(_treeLeaf.mcMomPID->at(imc))==E_LEPTON      
   }//end of if (fabs(_treeLeaf.mcGMomPID->at(imc))==Z_BOSON
}//end of CheckIfFSR

void SearchForOverlap::CheckIfTGC(int imc, bool& isZMuonTGC, bool& isWMuonTGC)
{
   if (fabs(_treeLeaf.mcMomPID->at(imc))==Z_BOSON || fabs(_treeLeaf.mcMomPID->at(imc))==W_BOSON){
   unsigned int ib = fabs(_treeLeaf.mcMomPID->at(imc));
   if ((fabs(_treeLeaf.mcGMomPID->at(imc))>=D_QUARK &&
        fabs(_treeLeaf.mcGMomPID->at(imc))<=T_QUARK) ||
          _treeLeaf.mcGMomPID->at(imc)==GLUON){
            _nTGC[ib]+=_totalWeight;
            _nSomeScheme++;
   }   
   for (int imc2=0; imc2<_treeLeaf.nMC; imc2++){
     if (fabs(_treeLeaf.mcPID->at(imc2))==MU_LEPTON &&
         fabs(_treeLeaf.mcMomPID->at(imc2))==Z_BOSON &&
         fabs(_treeLeaf.mcMomPID->at(imc))==Z_BOSON)
       isZMuonTGC=1;
     }
   }
}//end of CheckIfTGC

void SearchForOverlap::CheckIfISR(int imc, bool& isZMuonISR, bool& isWMuonISR)
{
   if ((fabs(_treeLeaf.mcMomPID->at(imc))>=D_QUARK &&
        fabs(_treeLeaf.mcMomPID->at(imc))<=T_QUARK) ||
        _treeLeaf.mcMomPID->at(imc)==GLUON){
     unsigned int iq=fabs(_treeLeaf.mcMomPID->at(imc));
     bool hasBosonFromTheSameQ=0;
     unsigned int ib = 0;
     for (int imc2=0; imc2<_treeLeaf.nMC; imc2++){
       if (fabs(_treeLeaf.mcPID->at(imc2))==Z_BOSON || fabs(_treeLeaf.mcPID->at(imc2))==W_BOSON){
         ib = fabs(_treeLeaf.mcPID->at(imc2));
         if (fabs(_treeLeaf.mcMomPID->at(imc2))==iq){
           hasBosonFromTheSameQ=1;
           break;
         }
       }
     }//end of loop over imc2
     if (hasBosonFromTheSameQ){
       _nISR[ib][ANY_QUARK]+=_totalWeight;
       _nISR[ib][iq]+=_totalWeight;
       _nSomeScheme++;
     }
   }//end of if (#smth#>=D_QUARK <...>
}

void SearchForOverlap::FindLeptonIndexes(int bosonType, int imc, int& ilep1, int& ilep2)
//Z_BOSON or W_BOSON
{
  if (bosonType==Z_BOSON) FindLeptonIndexesZ(imc, ilep1, ilep2);
  if (bosonType==W_BOSON) FindLeptonIndexesW(imc, ilep1, ilep2);
}//end of FindLeptonIndexes()

void SearchForOverlap::FindLeptonIndexesZ(int imc, int& ilep1, int& ilep2)
{
  for (int imc=0; imc<_treeLeaf.nMC; imc++){
    if (fabs(_treeLeaf.mcPID->at(imc))==MU_LEPTON)
      if (fabs(_treeLeaf.mcMomPID->at(imc))==Z_BOSON){
        if (ilep1==-1) ilep1=imc;
        else if (_treeLeaf.mcPt->at(imc)<_treeLeaf.mcPt->at(ilep1)){
           ilep2=imc;
           continue;
         }    
         else{
           ilep2=ilep1;
           ilep1=imc;
           continue;
          }
      }
  }//end of loop over imc
}

void SearchForOverlap::FindLeptonIndexesW(int imc, int& ilep1, int& ilep2)
{
  for (int imc=0; imc<_treeLeaf.nMC; imc++){
    if (fabs(_treeLeaf.mcPID->at(imc))==MU_LEPTON)
      if (fabs(_treeLeaf.mcMomPID->at(imc))==W_BOSON)
        if (ilep1==-1) ilep1=imc;
    if (fabs(_treeLeaf.mcPID->at(imc))==MU_NEUTRINO)
      if (fabs(_treeLeaf.mcMomPID->at(imc))==W_BOSON)
        if (ilep2==-1) ilep2=imc;
      
  }//end of loop over imc
}

void SearchForOverlap::FillHists(int imc, int ilep1, int ilep2)
{
  _histPhoPt[_iSource]->Fill(_treeLeaf.mcEt->at(imc),_totalWeight);
  _histPhoEta[_iSource]->Fill(_treeLeaf.mcEta->at(imc),_totalWeight);
  _histPhoPhi[_iSource]->Fill(_treeLeaf.mcPhi->at(imc),_totalWeight);
  if (ilep1>-1){
    _histLep1Pt[_iSource]->Fill(_treeLeaf.mcPt->at(ilep1),_totalWeight); 
    _histLep1Eta[_iSource]->Fill(_treeLeaf.mcEta->at(ilep1),_totalWeight); 
    _histLep1Phi[_iSource]->Fill(_treeLeaf.mcPhi->at(ilep1),_totalWeight); 
    _histLep1PhodR[_iSource]->Fill(_dRLep1Pho,_totalWeight);
  }
  if (ilep2>-1){
    _histLep2Pt[_iSource]->Fill(_treeLeaf.mcPt->at(ilep2),_totalWeight);
    _histLep2Eta[_iSource]->Fill(_treeLeaf.mcEta->at(ilep2),_totalWeight); 
    _histLep2Phi[_iSource]->Fill(_treeLeaf.mcPhi->at(ilep2),_totalWeight); 
    _histLep2PhodR[_iSource]->Fill(_dRLep2Pho,_totalWeight);  
  }
  if (ilep1>-1 && ilep2>-1){
    _histLepLepdR[_iSource]->Fill(_dRLepLep,_totalWeight); 
    _histLepLepMassT[_iSource]->Fill(_massTLepLep,_totalWeight); 
    _histLepLepMass[_iSource]->Fill(_massLepLep,_totalWeight); 
  }  
  _histMCparentage[_iSource]->Fill(_treeLeaf.mcParentage->at(imc),_totalWeight);
}

void SearchForOverlap::ComputeKinematicVariables(int ipho, int ilep1, int ilep2)
{
  if (ipho==-1) return;
  float dphi, deta;
  if (ilep1>-1){
  // Lep1 Pho dR
    _dRLep1Pho=DeltaR(_treeLeaf.mcPhi->at(ilep1),_treeLeaf.mcPhi->at(
ipho),_treeLeaf.mcEta->at(ilep1),_treeLeaf.mcEta->at(ipho));  
  }
  if (ilep2>-1){  
    // Lep2 Pho dR 
    _dRLep2Pho=DeltaR(_treeLeaf.mcPhi->at(ilep2),_treeLeaf.mcPhi->at(ipho),_treeLeaf.mcEta->at(ilep2),_treeLeaf.mcEta->at(ipho));  
   }
   if (ilep1>-1 && ilep2>-1){ 
     // Lep Lep dR  
     _dRLepLep=DeltaR(_treeLeaf.mcPhi->at(ilep2),_treeLeaf.mcPhi->at(ilep1),_treeLeaf.mcEta->at(ilep2),_treeLeaf.mcEta->at(ilep1));  
      // Lep Lep Mass Transversed
      _massTLepLep = sqrt(2*_treeLeaf.mcPt->at(ilep1)*_treeLeaf.mcPt->at(ilep2)*(1-TMath::Cos(_treeLeaf.mcPhi->at(ilep2)-_treeLeaf.mcPhi->at(ilep1)))); 
      // Lep Lep Mass Invariant
      TLorentzVector vLep1;
      float mass1;
      if (_treeLeaf.mcPID->at(ilep1)==MU_LEPTON) mass1=0.1;
      vLep1.SetPtEtaPhiM(_treeLeaf.mcPt->at(ilep1),_treeLeaf.mcEta->at(ilep1),_treeLeaf.mcPhi->at(ilep1),mass1);
      TLorentzVector vLep2;
      float mass2;
      if (_treeLeaf.mcPID->at(ilep1)==MU_LEPTON) mass2=0.1;
      else if (_treeLeaf.mcPID->at(ilep1)==MU_NEUTRINO) mass2=0.0;
      vLep2.SetPtEtaPhiM(_treeLeaf.mcPt->at(ilep2),_treeLeaf.mcEta->at(ilep2),_treeLeaf.mcPhi->at(ilep2),mass2);
      TLorentzVector vLepLepSum=vLep1+vLep2;
      _massLepLep = vLepLepSum.M();
    }
}

void SearchForOverlap::PrintResults()
{
     std::cout<<"weighted number of events:"<<std::endl;
     std::cout<<"nTot="<<_nEvents<<", nWithPhotons="<<_nEventsWithPhotons<<", nPhotons="<<_nPhotons<<", nPassedAcc="<<_nPhotonsPassedAcc<<", nTrueGammaInAcc="<<_nPhotonsPassedTrueGammaCondition<<", trueEff=";
     if (_nPhotonsPassedAcc==0) std::cout<<0<<std::endl;
     else std::cout<<1.0*_nPhotonsPassedTrueGammaCondition/_nPhotonsPassedAcc<<std::endl;
/*
     std::cout<<"n of events with diff n of photons:"<<std::endl;
     for (int nphos=0; nphos<91; nphos++){
       if (_nEventsWithGivenNOfPhotons[nphos]!=0)
         std::cout<<nphos<<": "<<_nEventsWithGivenNOfPhotons[nphos]<<"; ";
     }
     std::cout<<std::endl;
     std::cout<<"n of events with diff n of schemes:"<<std::endl;
     for (int nphos=0; nphos<91; nphos++){
       if (_nEventsWithSomeScheme[nphos]!=0)
         std::cout<<nphos<<": "<<_nEventsWithSomeScheme[nphos]<<"; ";
     }
     std::cout<<std::endl;
     std::cout<<"nTGC(W)="<<_nTGC[W_BOSON]<<std::endl;
     std::cout<<"nFSR(W)="<<_nFSR[W_BOSON][ANY_LEPTON]<<", e="<<_nFSR[W_BOSON][E_LEPTON]<<", mu="<<_nFSR[W_BOSON][MU_LEPTON]<<", tau="<<_nFSR[W_BOSON][TAU_LEPTON]<<std::endl;
     std::cout<<"nISR(W)="<<_nISR[W_BOSON][ANY_QUARK]<<", d="<<_nISR[W_BOSON][D_QUARK]<<", u="<<_nISR[W_BOSON][U_QUARK]<<", s="<<_nISR[W_BOSON][S_QUARK]<<", c="<<_nISR[W_BOSON][C_QUARK]<<", b="<<_nISR[W_BOSON][B_QUARK]<<", t="<<_nISR[W_BOSON][T_QUARK]<<", gluon="<<_nISR[W_BOSON][GLUON]<<std::endl;
     std::cout<<"fractions:"<<std::endl;
     std::cout<<"nTGC(W)="<<100*_nTGC[W_BOSON]/_nPhotons<<"%"<<std::endl;
     std::cout<<"nFSR(W)="<<100*_nFSR[W_BOSON][ANY_LEPTON]/_nPhotons<<"%, e="<<100*_nFSR[W_BOSON][E_LEPTON]/_nPhotons<<"%, mu="<<100*_nFSR[W_BOSON][MU_LEPTON]/_nPhotons<<"%, tau="<<100*_nFSR[W_BOSON][TAU_LEPTON]/_nPhotons<<"%"<<std::endl;
     std::cout<<"nISR(W)="<<100*_nISR[W_BOSON][ANY_QUARK]/_nPhotons<<"%, d="<<100*_nISR[W_BOSON][D_QUARK]/_nPhotons<<"%, u="<<100*_nISR[W_BOSON][U_QUARK]/_nPhotons<<"%, s="<<100*_nISR[W_BOSON][S_QUARK]/_nPhotons<<"%, c="<<100*_nISR[W_BOSON][C_QUARK]/_nPhotons<<"%, b="<<100*_nISR[W_BOSON][B_QUARK]/_nPhotons<<"%, t="<<100*_nISR[W_BOSON][T_QUARK]/_nPhotons<<"%, gluon="<<100*_nISR[W_BOSON][GLUON]/_nPhotons<<"%"<<std::endl;
*/
     std::cout<<std::endl;
     std::cout<<"nTGC(Z)="<<_nTGC[Z_BOSON]<<std::endl;
     std::cout<<"nFSR(Z)="<<_nFSR[Z_BOSON][ANY_LEPTON]<<", e="<<_nFSR[Z_BOSON][E_LEPTON]<<", mu="<<_nFSR[Z_BOSON][MU_LEPTON]<<", tau="<<_nFSR[Z_BOSON][TAU_LEPTON]<<std::endl;

//     std::cout<<"nISR(Z)="<<_nISR[Z_BOSON][ANY_QUARK]<<", d="<<_nISR[Z_BOSON][D_QUARK]<<", u="<<_nISR[Z_BOSON][U_QUARK]<<", s="<<_nISR[Z_BOSON][S_QUARK]<<", c="<<_nISR[Z_BOSON][C_QUARK]<<", b="<<_nISR[Z_BOSON][B_QUARK]<<", t="<<_nISR[Z_BOSON][T_QUARK]<<", gluon="<<_nISR[Z_BOSON][GLUON]<<std::endl;
     std::cout<<"fractions:"<<std::endl;
     std::cout<<"nTGC(Z)="<<100*_nTGC[Z_BOSON]/_nPhotons<<"%"<<std::endl;
     std::cout<<"nFSR(Z)="<<100*_nFSR[Z_BOSON][ANY_LEPTON]/_nPhotons<<"%, e="<<100*_nFSR[Z_BOSON][E_LEPTON]/_nPhotons<<"%, mu="<<100*_nFSR[Z_BOSON][MU_LEPTON]/_nPhotons<<"%, tau="<<100*_nFSR[Z_BOSON][TAU_LEPTON]/_nPhotons<<"%"<<std::endl;
//     std::cout<<"nISR(Z)="<<100*_nISR[Z_BOSON][ANY_QUARK]/_nPhotons<<"%, d="<<100*_nISR[Z_BOSON][D_QUARK]/_nPhotons<<"%, u="<<100*_nISR[Z_BOSON][U_QUARK]/_nPhotons<<"%, s="<<100*_nISR[Z_BOSON][S_QUARK]/_nPhotons<<"%, c="<<100*_nISR[Z_BOSON][C_QUARK]/_nPhotons<<"%, b="<<100*_nISR[Z_BOSON][B_QUARK]/_nPhotons<<"%, t="<<100*_nISR[Z_BOSON][T_QUARK]/_nPhotons<<"%, gluon="<<100*_nISR[Z_BOSON][GLUON]/_nPhotons<<"%"<<std::endl;
}

void SearchForOverlap::PrintIfSuspicious()
{
  std::cout<<"event "<<_entry<<" has no photons or no schemes;"<<std::endl;
  std::cout<<"mcPIDs:    ";
  for (int imc3=0; imc3<_treeLeaf.nMC; imc3++)
    std::cout<<std::setw(2)<<_treeLeaf.mcPID->at(imc3)<<", ";
  std::cout<<std::endl;
  std::cout<<"mcMomPIDs: ";
  for (int imc3=0; imc3<_treeLeaf.nMC; imc3++)
    std::cout<<std::setw(2)<<_treeLeaf.mcMomPID->at(imc3)<<", ";
  std::cout<<std::endl;
  std::cout<<"mcGMomPIDs:";
  for (int imc3=0; imc3<_treeLeaf.nMC; imc3++)
     std::cout<<std::setw(2)<<_treeLeaf.mcGMomPID->at(imc3)<<", ";
  std::cout<<std::endl;
}

float SearchForOverlap::DeltaR(float phi1,float phi2,float eta1,float eta2)
{
   float dphi = fabs(phi2-phi1);  
   if (dphi>2*TMath::Pi())
     dphi=dphi-2*TMath::Pi();  
   float deta =  fabs(eta2-eta1);
   return sqrt(dphi*dphi+deta*deta);
}//end of DeltaR(float phi1,float phi2,float eta1,float eta2)
