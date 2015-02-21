//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Nov 27 16:28:14 2013 by ROOT version 5.28/00
// from TTree EventTree/Event data
// found on file: /mnt/hadoop/user/uscms01/pnfs/unl.edu/data4/cms/store/user/eavdeeva2/WGammaMCsummer12New/job_summer12_Wg.root
//////////////////////////////////////////////////////////

#ifndef TEventTree_h
#define TEventTree_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <vector>

#include <../Configuration/TConfiguration.h>

class TEventTree {
public :
   const static Int_t kMaxnHLT = 456;

//   const static Int_t kMaxnVtx = 70;
//   const static Int_t kMaxnVtxBS = 70;
//   const static Int_t kMaxnPFPho = 48;
//   const static Int_t kMaxnPFEle = 6;
   const static Int_t kMaxnEle = 10;
   const static Int_t kMaxnPho = 16;
   const static Int_t kMaxnMu = 33;
//   const static Int_t kMaxnJet = 97;
//   const static Int_t kMaxnLowPtJet = 69;
//   const static Int_t kMaxnConv = 0;//500
     //since they are not used in the analysis,
     //I don't want to creat these big arrays of Float_t
     //for converted photons
   const static Int_t kMaxnMC = 93;
//   const static Int_t kMaxnPUInfo = 3;

public:
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain
   TObjArray*      listOfBranches;
   TConfiguration config;

   struct InputTreeLeaves{
     // Declaration of leaf types
     Int_t           run;
     Long64_t        event;
//   Int_t           lumis;
     Bool_t          isData;
     Int_t           nHLT;
     Int_t           HLT[kMaxnHLT];   //[nHLT]
     Int_t           HLTIndex[70];
//   Float_t         bspotPos[3];
//   Int_t           nVtx;
//   vector<float>   *vtx_x;
//   vector<float>   *vtx_y;
//   vector<float>   *vtx_z;
     Int_t           IsVtxGood;
//   Int_t           nGoodVtx;
//   Int_t           nVtxBS;
//   vector<float>   *vtxbs_x;
//   vector<float>   *vtxbs_y;
//   vector<float>   *vtxbs_z;
//   Float_t         pdf[7];
//   Float_t         pthat;
//   Float_t         processID;
     Int_t           nMC;
     vector<int>     *mcPID;
//   vector<float>   *mcVtx_x;
//   vector<float>   *mcVtx_y;
//   vector<float>   *mcVtx_z;
     vector<float>   *mcPt;
//   vector<float>   *mcMass;
     vector<float>   *mcEta;
     vector<float>   *mcPhi;
     vector<float>   *mcE;
     vector<float>   *mcEt;
     vector<int>     *mcGMomPID;
     vector<int>     *mcMomPID;
     vector<float>   *mcMomPt;
//   vector<float>   *mcMomMass;
     vector<float>   *mcMomEta;
     vector<float>   *mcMomPhi;
     vector<int>     *mcIndex;
     vector<int>     *mcDecayType;
     vector<int>     *mcParentage;
//   vector<int>     *mcStatus;
//   Float_t         genMET;
//   Float_t         genMETPhi;
     Int_t           nPUInfo;
//   vector<int>     *nPU;
//   vector<int>     *puBX;
     vector<float>   *puTrue;
     Float_t         pfMET;
     Float_t         pfMETPhi;
//   Float_t         pfMETsumEt;
//   Float_t         pfMETmEtSig;
//   Float_t         pfMETSig;
//   Float_t         pfType01MET;
//   Float_t         pfType01METPhi;
//   Float_t         pfType01METsumEt;
//   Float_t         pfType01METmEtSig;
//   Float_t         pfType01METSig;
//   Float_t         recoPfMET;
//   Float_t         recoPfMETPhi;
//   Float_t         recoPfMETsumEt;
//   Float_t         recoPfMETmEtSig;
//   Float_t         recoPfMETSig;
//   Float_t         trkMETxPV;
//   Float_t         trkMETyPV;
//   Float_t         trkMETPhiPV;
//   Float_t         trkMETPV;
//   vector<float>   *trkMETx;
//   vector<float>   *trkMETy;
//   vector<float>   *trkMETPhi;
//   vector<float>   *trkMET;
     Int_t           metFilters[10];
     Int_t           nEle;
   vector<unsigned long> *eleTrg;
//   vector<int>     *eleClass;
//   vector<int>     *eleIsEcalDriven;
//   vector<int>     *eleCharge;
//   vector<int>     *eleChargeConsistent;
//   vector<float>   *eleEn;
     vector<float>   *eleEcalEn;
//   vector<float>   *eleSCRawEn;
//   vector<float>   *eleSCEn;
//   vector<float>   *eleESEn;
     vector<float>   *elePt;
     vector<float>   *eleEta;
     vector<float>   *elePhi;
//   vector<float>   *eleR9;
//   vector<vector<float> > *eleEtaVtx;
//   vector<vector<float> > *elePhiVtx;
//   vector<vector<float> > *eleEtVtx;
     vector<float>   *eleSCEta;
     vector<float>   *eleSCPhi; 
//   vector<float>   *eleSCEtaWidth;
//   vector<float>   *eleSCPhiWidth;
//   vector<float>   *eleVtx_x;
//   vector<float>   *eleVtx_y;
//   vector<float>   *eleVtx_z;
//   vector<float>   *eleD0;
//   vector<float>   *eleDz;
//   vector<float>   *eleD0GV;
//   vector<float>   *eleDzGV;
     vector<vector<float> > *eleD0Vtx;
     vector<vector<float> > *eleDzVtx;
     vector<float>   *eleHoverE;
     vector<float>   *eleHoverE12;
//   vector<float>   *eleEoverP;
     vector<float>   *elePin;
//   vector<float>   *elePout;
//   vector<float>   *eleTrkMomErr;
//   vector<float>   *eleBrem;
     vector<float>   *eledEtaAtVtx;
     vector<float>   *eledPhiAtVtx;
     vector<float>   *eleSigmaIEtaIEta;
//   vector<float>   *eleSigmaIEtaIPhi;
//   vector<float>   *eleSigmaIPhiIPhi;
//   vector<float>   *eleEmax;
//   vector<float>   *eleE2ndMax;
//   vector<float>   *eleETop;
//   vector<float>   *eleEBottom;
//   vector<float>   *eleELeft;
//   vector<float>   *eleERight;
//   vector<float>   *eleE1x5;
//   vector<float>   *eleE3x3;
//   vector<float>   *eleE5x5;
//   vector<float>   *eleE2x5Max;
//   vector<float>   *eleE2x5Top;
//   vector<float>   *eleE2x5Bottom;
//   vector<float>   *eleE2x5Left;
//   vector<float>   *eleE2x5Right;
//   vector<float>   *eleSeedEta;
//   vector<float>   *eleSeedE;
//   vector<float>   *eleSeedPhi;
//   vector<float>   *eleCrysEta;
//   vector<float>   *eleCrysPhi;
//   vector<int>     *eleCrysIEta;
//   vector<int>     *eleCrysIPhi;
//   vector<float>   *eleRegrE;
//   vector<float>   *eleRegrEerr;
//   vector<float>   *elePhoRegrE;
//   vector<float>   *elePhoRegrEerr;
//   vector<float>   *eleSeedTime;
//   vector<int>     *eleRecoFlag;
//   vector<int>     *elePos;
     vector<int>     *eleGenIndex;
//   vector<int>     *eleGenGMomPID;
//   vector<int>     *eleGenMomPID;
//   vector<float>   *eleGenMomPt;
//   vector<float>   *eleIsoTrkDR03;
//   vector<float>   *eleIsoEcalDR03;
//   vector<float>   *eleIsoHcalDR03;
//   vector<float>   *eleIsoHcalDR0312;
//   vector<float>   *eleIsoTrkDR04;
//   vector<float>   *eleIsoEcalDR04;
//   vector<float>   *eleIsoHcalDR04;
//   vector<float>   *eleIsoHcalDR0412;
//   vector<float>   *eleModIsoTrk;
//   vector<float>   *eleModIsoEcal;
//   vector<float>   *eleModIsoHcal;
     vector<int>     *eleMissHits;
//   vector<float>   *eleConvDist;
//   vector<float>   *eleConvDcot;
     vector<int>     *eleConvVtxFit;
//   vector<float>   *eleIP3D;
//   vector<float>   *eleIP3DErr;
//   vector<float>   *eleIDMVANonTrig;
//   vector<float>   *eleIDMVATrig;
     vector<float>   *elePFChIso03;
     vector<float>   *elePFPhoIso03;
     vector<float>   *elePFNeuIso03;
//   vector<float>   *elePFChIso04;
//   vector<float>   *elePFPhoIso04;
//   vector<float>   *elePFNeuIso04;
//   vector<float>   *eleESEffSigmaRR_x;
//   vector<float>   *eleESEffSigmaRR_y;
//   vector<float>   *eleESEffSigmaRR_z;
     Int_t           nPho;
//   vector<unsigned long> *phoTrg;
//   vector<unsigned long> *phoTrgFilter;
//   vector<bool>    *phoIsPhoton;
//   vector<float>   *phoSCPos_x;
//   vector<float>   *phoSCPos_y;
//   vector<float>   *phoSCPos_z;
//   vector<float>   *phoCaloPos_x;
//   vector<float>   *phoCaloPos_y;
//   vector<float>   *phoCaloPos_z;
//   vector<float>   *phoE;
   vector<float>   *phoEt;
   vector<float>   *phoEta;
//   vector<float>   *phoVtx_x;
//   vector<float>   *phoVtx_y;
//   vector<float>   *phoVtx_z;
   vector<float>   *phoPhi;
//   vector<vector<float> > *phoEtVtx;
//   vector<vector<float> > *phoEtaVtx;
//   vector<vector<float> > *phoPhiVtx;
   vector<float>   *phoR9;
//   vector<int>     *phoNClus;
//   vector<float>   *phoTrkIsoHollowDR03;
//   vector<float>   *phoEcalIsoDR03;
//   vector<float>   *phoHcalIsoDR03;
//   vector<float>   *phoHcalIsoDR0312;
   vector<float>   *phoTrkIsoHollowDR04;
//   vector<float>   *phoCiCdRtoTrk;
   vector<float>   *phoEcalIsoDR04;
   vector<float>   *phoHcalIsoDR04;
//   vector<float>   *phoHcalIsoDR0412;
   vector<float>   *phoHoverE;
   vector<float>   *phoHoverE12;
   vector<int>     *phoEleVeto;
   vector<float>   *phoSigmaIEtaIEta;
//   vector<float>   *phoSigmaIEtaIPhi;
//   vector<float>   *phoSigmaIPhiIPhi;
//   vector<float>   *phoCiCPF4phopfIso03;
//   vector<float>   *phoCiCPF4phopfIso04;
//   vector<vector<float> > *phoCiCPF4chgpfIso02;
//   vector<vector<float> > *phoCiCPF4chgpfIso03;
//   vector<vector<float> > *phoCiCPF4chgpfIso04;
//   vector<float>   *phoEmax;
//   vector<float>   *phoETop;
//   vector<float>   *phoEBottom;
//   vector<float>   *phoELeft;
//   vector<float>   *phoERight;
//   vector<float>   *phoE2ndMax;
//   vector<float>   *phoE3x3;
//   vector<float>   *phoE3x1;
//   vector<float>   *phoE1x3;
//   vector<float>   *phoE5x5;
//   vector<float>   *phoE1x5;
//   vector<float>   *phoE2x2;
//   vector<float>   *phoE2x5Max;
//   vector<float>   *phoE2x5Top;
//   vector<float>   *phoE2x5Bottom;
//   vector<float>   *phoE2x5Left;
//   vector<float>   *phoE2x5Right;
//   vector<float>   *phoSeedE;
//   vector<float>   *phoSeedEta;
//   vector<float>   *phoSeedPhi;
//   vector<float>   *phoCrysEta;
//   vector<float>   *phoCrysPhi;
//   vector<int>     *phoCrysIEta;
//   vector<int>     *phoCrysIPhi;
   vector<float>   *phoPFChIso;
   vector<float>   *phoPFPhoIso;
   vector<float>   *phoPFNeuIso;
   vector<float>   *phoSCRChIso;
   vector<float>   *phoSCRPhoIso;
   vector<float>   *phoSCRNeuIso;
   vector<float>   *phoSCRChIso04;
   vector<float>   *phoSCRPhoIso04;
   vector<float>   *phoSCRNeuIso04;
   vector<float>   *phoRandConeChIso;
   vector<float>   *phoRandConePhoIso;
   vector<float>   *phoRandConeNeuIso;
   vector<float>   *phoRandConeChIso04;
   vector<float>   *phoRandConePhoIso04;
   vector<float>   *phoRandConeNeuIso04;
//   vector<float>   *phoRegrE;
//   vector<float>   *phoRegrEerr;
//   vector<float>   *phoSeedTime;
//   vector<int>     *phoSeedDetId1;
//   vector<int>     *phoSeedDetId2;
//   vector<float>   *phoLICTD;
//   vector<int>     *phoRecoFlag;
//   vector<int>     *phoPos;
   vector<int>     *phoGenIndex;
//   vector<int>     *phoGenGMomPID;
   vector<int>     *phoGenMomPID;
//   vector<float>   *phoGenMomPt;
//   vector<float>   *phoSCE;
//   vector<float>   *phoSCRawE;
//   vector<float>   *phoESEn;
   vector<float>   *phoSCEt;
   vector<float>   *phoSCEta;
   vector<float>   *phoSCPhi;
//   vector<float>   *phoSCEtaWidth;
//   vector<float>   *phoSCPhiWidth;
//   vector<float>   *phoSCBrem;
//   vector<int>     *phoOverlap;
   vector<int>     *phohasPixelSeed;
//   vector<int>     *pho_hasConvPf;
//   vector<int>     *pho_hasSLConvPf;
//   vector<float>   *pho_pfconvVtxZ;
//   vector<float>   *pho_pfconvVtxZErr;
//   vector<int>     *pho_nSLConv;
//   vector<vector<float> > *pho_pfSLConvPos_x;
//   vector<vector<float> > *pho_pfSLConvPos_y;
//   vector<vector<float> > *pho_pfSLConvPos_z;
//   vector<vector<float> > *pho_pfSLConvVtxZ;
//   vector<int>     *phoIsConv;
//   vector<int>     *phoNConv;
//   vector<float>   *phoConvInvMass;
//   vector<float>   *phoConvCotTheta;
//   vector<float>   *phoConvEoverP;
//   vector<float>   *phoConvZofPVfromTrks;
//   vector<float>   *phoConvMinDist;
//   vector<float>   *phoConvdPhiAtVtx;
//   vector<float>   *phoConvdPhiAtCalo;
//   vector<float>   *phoConvdEtaAtCalo;
//   vector<float>   *phoConvTrkd0_x;
//   vector<float>   *phoConvTrkd0_y;
//   vector<float>   *phoConvTrkPin_x;
//   vector<float>   *phoConvTrkPin_y;
//   vector<float>   *phoConvTrkPout_x;
//   vector<float>   *phoConvTrkPout_y;
//   vector<float>   *phoConvTrkdz_x;
//   vector<float>   *phoConvTrkdz_y;
//   vector<float>   *phoConvTrkdzErr_x;
//   vector<float>   *phoConvTrkdzErr_y;
//   vector<float>   *phoConvChi2;
//   vector<float>   *phoConvChi2Prob;
//   vector<int>     *phoConvNTrks;
//   vector<float>   *phoConvCharge1;
//   vector<float>   *phoConvCharge2;
//   vector<int>     *phoConvValidVtx;
//   vector<float>   *phoConvLikeLihood;
//   vector<float>   *phoConvP4_0;
//   vector<float>   *phoConvP4_1;
//   vector<float>   *phoConvP4_2;
//   vector<float>   *phoConvP4_3;
//   vector<float>   *phoConvVtx_x;
//   vector<float>   *phoConvVtx_y;
//   vector<float>   *phoConvVtx_z;
//   vector<float>   *phoConvVtxErr_x;
//   vector<float>   *phoConvVtxErr_y;
//   vector<float>   *phoConvVtxErr_z;
//   vector<float>   *phoConvPairMomentum_x;
//   vector<float>   *phoConvPairMomentum_y;
//   vector<float>   *phoConvPairMomentum_z;
//   vector<float>   *phoConvRefittedMomentum_x;
//   vector<float>   *phoConvRefittedMomentum_y;
//   vector<float>   *phoConvRefittedMomentum_z;
//   vector<int>     *SingleLegConv;
//   vector<vector<float> > *phoPFConvVtx_x;
//   vector<vector<float> > *phoPFConvVtx_y;
//   vector<vector<float> > *phoPFConvVtx_z;
//   vector<vector<float> > *phoPFConvMom_x;
//   vector<vector<float> > *phoPFConvMom_y;
//   vector<vector<float> > *phoPFConvMom_z;
//   vector<float>   *phoESEffSigmaRR_x;
//   vector<float>   *phoESEffSigmaRR_y;
//   vector<float>   *phoESEffSigmaRR_z;
   Int_t           nMu;
   vector<unsigned long> *muTrg;
   vector<float>   *muEta;
   vector<float>   *muPhi;
//   vector<int>     *muCharge;
   vector<float>   *muPt;
//   vector<float>   *muPz;
//   vector<float>   *muVtx_x;
//   vector<float>   *muVtx_y;
//   vector<float>   *muVtx_z;
//   vector<float>   *muVtxGlb_x;
//   vector<float>   *muVtxGlb_y;
//   vector<float>   *muVtxGlb_z;
   vector<int>     *muGenIndex;
//   vector<float>   *mucktPt;
//   vector<float>   *mucktPtErr;
//   vector<float>   *mucktEta;
//   vector<float>   *mucktPhi;
//   vector<float>   *mucktdxy;
//   vector<float>   *mucktdz;
   vector<float>   *muIsoTrk;
//   vector<float>   *muIsoCalo;
   vector<float>   *muIsoEcal;
   vector<float>   *muIsoHcal;
   vector<float>   *muChi2NDF;
//   vector<float>   *muInnerChi2NDF;
   vector<float>   *muPFIsoR04_CH;
   vector<float>   *muPFIsoR04_NH;
   vector<float>   *muPFIsoR04_Pho;
   vector<float>   *muPFIsoR04_PU;
//   vector<float>   *muPFIsoR04_CPart;
//   vector<float>   *muPFIsoR04_NHHT;
//   vector<float>   *muPFIsoR04_PhoHT;
//   vector<float>   *muPFIsoR03_CH;
//   vector<float>   *muPFIsoR03_NH;
//   vector<float>   *muPFIsoR03_Pho;
//   vector<float>   *muPFIsoR03_PU;
//   vector<float>   *muPFIsoR03_CPart;
//   vector<float>   *muPFIsoR03_NHHT;
//   vector<float>   *muPFIsoR03_PhoHT;
   vector<int>     *muType;
   vector<float>   *muD0;
   vector<float>   *muDz;
//   vector<float>   *muD0GV;
//   vector<float>   *muDzGV;
//   vector<vector<float> > *muD0Vtx;
//   vector<vector<float> > *muDzVtx;
//   vector<float>   *muInnerD0;
//   vector<float>   *muInnerDz;
   vector<float>   *muInnerD0GV;
   vector<float>   *muInnerDzGV;
//   vector<float>   *muInnerPt;
//   vector<float>   *muInnerPtErr;
   vector<int>     *muNumberOfValidTrkLayers;
   vector<int>     *muNumberOfValidTrkHits;
//   vector<int>     *muNumberOfValidPixelLayers;
   vector<int>     *muNumberOfValidPixelHits;
   vector<int>     *muNumberOfValidMuonHits;
   vector<int>     *muStations;
//   vector<int>     *muChambers;
//   vector<float>   *muIP3D;
//   vector<float>   *muIP3DErr;
//   Int_t           nTau;
//   vector<bool>    *tauDecayModeFinding;
//   vector<bool>    *tauAgainstElectronLooseMVA3;
//   vector<bool>    *tauAgainstElectronMediumMVA3;
//   vector<bool>    *tauAgainstElectronTightMVA3;
//   vector<bool>    *tauAgainstElectronVTightMVA3;
//   vector<bool>    *tauAgainstElectronDeadECAL;
//   vector<bool>    *tauAgainstMuonLoose2;
//   vector<bool>    *tauAgainstMuonMedium2;
//   vector<bool>    *tauAgainstMuonTight2;
//   vector<bool>    *tauCombinedIsolationDeltaBetaCorrRaw3Hits;
//   vector<bool>    *tauLooseCombinedIsolationDeltaBetaCorr3Hits;
//   vector<bool>    *tauMediumCombinedIsolationDeltaBetaCorr3Hits;
//   vector<bool>    *tauTightCombinedIsolationDeltaBetaCorr3Hits;
//   vector<float>   *tauEta;
//   vector<float>   *tauPhi;
//   vector<float>   *tauPt;
//   vector<float>   *tauEt;
//   vector<float>   *tauCharge;
//   vector<int>     *tauDecayMode;
//   vector<float>   *tauEMFraction;
//   vector<float>   *tauHCAL3x3OverPLead;
//   vector<float>   *tauHCALMaxOverPLead;
//   vector<float>   *tauHCALTotOverPLead;
//   vector<float>   *tauIsolationPFChargedHadrCandsPtSum;
//   vector<float>   *tauIsolationPFGammaCandsEtSum;
//   vector<float>   *tauLeadPFChargedHadrCandsignedSipt;
//   vector<bool>    *tauLeadChargedHadronExists;
//   vector<float>   *tauLeadChargedHadronEta;
//   vector<float>   *tauLeadChargedHadronPhi;
//   vector<float>   *tauLeadChargedHadronPt;
//   Float_t         rho25;
//   Float_t         rho25_neu;
//   Float_t         rho25_muPFiso;
//   Float_t         rho25_elePFiso;
   Float_t         rho2011;
   Float_t         rho2012;
//   Float_t         QGTag_MLP;
//   Float_t         QGTag_likelihood;
//   Int_t           nCA8Jet;
//   vector<float>   *CA8JetPt;
//   vector<float>   *CA8JetEta;
//   vector<float>   *CA8JetPhi;
//   vector<float>   *CA8JetMass;
//   vector<float>   *CA8JetArea;
//   vector<float>   *CA8Jet_tau1;
//   vector<float>   *CA8Jet_tau2;
//   vector<float>   *CA8Jet_tau3;
//   vector<float>   *CA8JetCHF;
//   vector<float>   *CA8JetNHF;
//   vector<float>   *CA8JetCEF;
//   vector<float>   *CA8JetNEF;
//   vector<int>     *CA8JetNCH;
//   vector<int>     *CA8Jetnconstituents;
//   vector<float>   *CA8prunedJetMass;
//   vector<int>     *CA8prunedJet_nSubJets;
//   vector<vector<float> > *CA8prunedJet_SubjetPt;
//   vector<vector<float> > *CA8prunedJet_SubjetEta;
//   vector<vector<float> > *CA8prunedJet_SubjetPhi;
//   vector<vector<float> > *CA8prunedJet_SubjetMass;
   Int_t           nJet;
//   vector<unsigned long> *jetTrg;
//   vector<float>   *jetEn;
   vector<float>   *jetPt;
   vector<float>   *jetEta;
   vector<float>   *jetPhi;
//   vector<float>   *jetCharge;
//   vector<float>   *jetEt;
   vector<float>   *jetRawPt;
   vector<float>   *jetRawEn;
//   vector<float>   *jetArea;
//   vector<float>   *jetCHF;
//   vector<float>   *jetNHF;
//   vector<float>   *jetCEF;
//   vector<float>   *jetNEF;
//   vector<int>     *jetNCH;
//   vector<float>   *jetHFHAE;
//   vector<float>   *jetHFEME;
//   vector<int>     *jetNConstituents;
//   vector<float>   *jetCombinedSecondaryVtxBJetTags;
//   vector<float>   *jetCombinedSecondaryVtxMVABJetTags;
//   vector<float>   *jetJetProbabilityBJetTags;
//   vector<float>   *jetJetBProbabilityBJetTags;
//   vector<vector<float> > *jetBetaStar;
//   vector<bool>    *jetPFLooseId;
//   vector<float>   *jetDRMean;
//   vector<float>   *jetDR2Mean;
//   vector<float>   *jetDZ;
//   vector<float>   *jetFrac01;
//   vector<float>   *jetFrac02;
//   vector<float>   *jetFrac03;
//   vector<float>   *jetFrac04;
//   vector<float>   *jetFrac05;
//   vector<float>   *jetFrac06;
//   vector<float>   *jetFrac07;
//   vector<float>   *jetBeta;
//   vector<float>   *jetBetaStarCMG;
//   vector<float>   *jetBetaStarClassic;
//   vector<vector<float> > *jetBetaExt;
//   vector<vector<float> > *jetBetaStarCMGExt;
//   vector<vector<float> > *jetBetaStarClassicExt;
//   vector<float>   *jetNNeutrals;
//   vector<float>   *jetNCharged;
//   vector<vector<float> > *jetMVAs;
//   vector<vector<int> > *jetWPLevels;
//   vector<vector<float> > *jetMVAsExt_simple;
//   vector<vector<int> > *jetWPLevelsExt_simple;
//   vector<vector<float> > *jetMVAsExt_full;
//   vector<vector<int> > *jetWPLevelsExt_full;
//   vector<vector<float> > *jetMVAsExt_cutBased;
//   vector<vector<int> > *jetWPLevelsExt_cutBased;
//   vector<vector<float> > *jetMVAsExt_philv1;
//   vector<vector<int> > *jetWPLevelsExt_philv1;
//   vector<float>   *jetMt;
//   vector<float>   *jetJECUnc;
//   vector<float>   *jetLeadTrackPt;
//   vector<float>   *jetVtxPt;
//   vector<float>   *jetVtxMass;
//   vector<float>   *jetVtx3dL;
//   vector<float>   *jetVtx3deL;
//   vector<float>   *jetSoftLeptPt;
//   vector<float>   *jetSoftLeptPtRel;
//   vector<float>   *jetSoftLeptdR;
//   vector<float>   *jetSoftLeptIdlooseMu;
//   vector<float>   *jetSoftLeptIdEle95;
//   vector<float>   *jetDPhiMETJet;
//   vector<float>   *jetPuJetIdL;
//   vector<float>   *jetPuJetIdM;
//   vector<float>   *jetPuJetIdT;
//   vector<int>     *jetPartonID;
//   vector<int>     *jetGenJetIndex;
//   vector<float>   *jetGenJetEn;
   vector<float>   *jetGenJetPt;
   vector<float>   *jetGenJetEta;
   vector<float>   *jetGenJetPhi;
//   vector<int>     *jetGenPartonID;
//   vector<float>   *jetGenEn;
//   vector<float>   *jetGenPt;
//   vector<float>   *jetGenEta;
//   vector<float>   *jetGenPhi;
//   vector<int>     *jetGenPartonMomID;
//   Int_t           nConv;
//   vector<float>   *convP4_x;
//   vector<float>   *convP4_y;
//   vector<float>   *convP4_z;
//   vector<float>   *convP4_E;
//   vector<float>   *convVtx_x;
//   vector<float>   *convVtx_y;
//   vector<float>   *convVtx_z;
//   vector<float>   *convVtxErr_x;
//   vector<float>   *convVtxErr_y;
//   vector<float>   *convVtxErr_z;
//   vector<float>   *convPairMomentum_x;
//   vector<float>   *convPairMomentum_y;
//   vector<float>   *convPairMomentum_z;
//   vector<float>   *convRefittedMomentum_x;
//   vector<float>   *convRefittedMomentum_y;
//   vector<float>   *convRefittedMomentum_z;
//   vector<int>     *convNTracks;
//   vector<float>   *convPairInvMass;
//   vector<float>   *convPairCotThetaSep;
//   vector<float>   *convEoverP;
//   vector<float>   *convDistOfMinApproach;
//   vector<float>   *convDPhiTrksAtVtx;
//   vector<float>   *convDPhiTrksAtEcal;
//   vector<float>   *convDEtaTrksAtEcal;
//   vector<float>   *convDxy;
//   vector<float>   *convDz;
//   vector<float>   *convLxy;
//   vector<float>   *convLz;
//   vector<float>   *convZofPrimVtxFromTrks;
//   vector<int>     *convNHitsBeforeVtx_0;
//   vector<int>     *convNHitsBeforeVtx_1;
//   vector<int>     *convNSharedHits;
//   vector<int>     *convValidVtx;
//   vector<float>   *convMVALikelihood;
//   vector<float>   *convChi2;
//   vector<float>   *convChi2Probability;
//   vector<float>   *convTk1Dz;
//   vector<float>   *convTk2Dz;
//   vector<float>   *convTk1DzErr;
//   vector<float>   *convTk2DzErr;
//   vector<int>     *convCh1Ch2;
//   vector<float>   *convTk1D0;
//   vector<float>   *convTk1Pout;
//   vector<float>   *convTk1Pin;
//   vector<float>   *convTk2D0;
//   vector<float>   *convTk2Pout;
//   vector<float>   *convTk2Pin;
   };

   InputTreeLeaves treeLeaf;

   // List of branches
   TBranch        *b_run;   //!
   TBranch        *b_event;   //!
   TBranch        *b_lumis;   //!
   TBranch        *b_isData;   //!
   TBranch        *b_nHLT;   //!
   TBranch        *b_HLT;   //!
   TBranch        *b_HLTIndex;   //!
   TBranch        *b_bspotPos;   //!
   TBranch        *b_nVtx;   //!
   TBranch        *b_vtx_x;   //!
   TBranch        *b_vtx_y;   //!
   TBranch        *b_vtx_z;   //!
   TBranch        *b_IsVtxGood;   //!
   TBranch        *b_nGoodVtx;   //!
   TBranch        *b_nVtxBS;   //!
   TBranch        *b_vtxbs_x;   //!
   TBranch        *b_vtxbs_y;   //!
   TBranch        *b_vtxbs_z;   //!
   TBranch        *b_pdf;   //!
   TBranch        *b_pthat;   //!
   TBranch        *b_processID;   //!
   TBranch        *b_nMC;   //!
   TBranch        *b_mcPID;   //!
   TBranch        *b_mcVtx_x;   //!
   TBranch        *b_mcVtx_y;   //!
   TBranch        *b_mcVtx_z;   //!
   TBranch        *b_mcPt;   //!
   TBranch        *b_mcMass;   //!
   TBranch        *b_mcEta;   //!
   TBranch        *b_mcPhi;   //!
   TBranch        *b_mcE;   //!
   TBranch        *b_mcEt;   //!
   TBranch        *b_mcGMomPID;   //!
   TBranch        *b_mcMomPID;   //!
   TBranch        *b_mcMomPt;   //!
   TBranch        *b_mcMomMass;   //!
   TBranch        *b_mcMomEta;   //!
   TBranch        *b_mcMomPhi;   //!
   TBranch        *b_mcIndex;   //!
   TBranch        *b_mcDecayType;   //!
   TBranch        *b_mcParentage;   //!
   TBranch        *b_mcStatus;   //!
   TBranch        *b_genMET;   //!
   TBranch        *b_genMETPhi;   //!
   TBranch        *b_nPUInfo;   //!
   TBranch        *b_nPU;   //!
   TBranch        *b_puBX;   //!
   TBranch        *b_puTrue;   //!
   TBranch        *b_pfMET;   //!
   TBranch        *b_pfMETPhi;   //!
   TBranch        *b_pfMETsumEt;   //!
   TBranch        *b_pfMETmEtSig;   //!
   TBranch        *b_pfMETSig;   //!
   TBranch        *b_pfType01MET;   //!
   TBranch        *b_pfType01METPhi;   //!
   TBranch        *b_pfType01METsumEt;   //!
   TBranch        *b_pfType01METmEtSig;   //!
   TBranch        *b_pfType01METSig;   //!
   TBranch        *b_recoPfMET;   //!
   TBranch        *b_recoPfMETPhi;   //!
   TBranch        *b_recoPfMETsumEt;   //!
   TBranch        *b_recoPfMETmEtSig;   //!
   TBranch        *b_recoPfMETSig;   //!
   TBranch        *b_trkMETxPV;   //!
   TBranch        *b_trkMETyPV;   //!
   TBranch        *b_trkMETPhiPV;   //!
   TBranch        *b_trkMETPV;   //!
   TBranch        *b_trkMETx;   //!
   TBranch        *b_trkMETy;   //!
   TBranch        *b_trkMETPhi;   //!
   TBranch        *b_trkMET;   //!
   TBranch        *b_metFilters;   //!
   TBranch        *b_nEle;   //!
   TBranch        *b_eleTrg;   //!
   TBranch        *b_eleClass;   //!
   TBranch        *b_eleIsEcalDriven;   //!
   TBranch        *b_eleCharge;   //!
   TBranch        *b_eleChargeConsistent;   //!
   TBranch        *b_eleEn;   //!
   TBranch        *b_eleEcalEn;   //!
   TBranch        *b_eleSCRawEn;   //!
   TBranch        *b_eleSCEn;   //!
   TBranch        *b_eleESEn;   //!
   TBranch        *b_elePt;   //!
   TBranch        *b_eleEta;   //!
   TBranch        *b_elePhi;   //!
   TBranch        *b_eleR9;   //!
   TBranch        *b_eleEtaVtx;   //!
   TBranch        *b_elePhiVtx;   //!
   TBranch        *b_eleEtVtx;   //!
   TBranch        *b_eleSCEta;   //!
   TBranch        *b_eleSCPhi;   //!
   TBranch        *b_eleSCEtaWidth;   //!
   TBranch        *b_eleSCPhiWidth;   //!
   TBranch        *b_eleVtx_x;   //!
   TBranch        *b_eleVtx_y;   //!
   TBranch        *b_eleVtx_z;   //!
   TBranch        *b_eleD0;   //!
   TBranch        *b_eleDz;   //!
   TBranch        *b_eleD0GV;   //!
   TBranch        *b_eleDzGV;   //!
   TBranch        *b_eleD0Vtx;   //!
   TBranch        *b_eleDzVtx;   //!
   TBranch        *b_eleHoverE;   //!
   TBranch        *b_eleHoverE12;   //!
   TBranch        *b_eleEoverP;   //!
   TBranch        *b_elePin;   //!
   TBranch        *b_elePout;   //!
   TBranch        *b_eleTrkMomErr;   //!
   TBranch        *b_eleBrem;   //!
   TBranch        *b_eledEtaAtVtx;   //!
   TBranch        *b_eledPhiAtVtx;   //!
   TBranch        *b_eleSigmaIEtaIEta;   //!
   TBranch        *b_eleSigmaIEtaIPhi;   //!
   TBranch        *b_eleSigmaIPhiIPhi;   //!
   TBranch        *b_eleEmax;   //!
   TBranch        *b_eleE2ndMax;   //!
   TBranch        *b_eleETop;   //!
   TBranch        *b_eleEBottom;   //!
   TBranch        *b_eleELeft;   //!
   TBranch        *b_eleERight;   //!
   TBranch        *b_eleE1x5;   //!
   TBranch        *b_eleE3x3;   //!
   TBranch        *b_eleE5x5;   //!
   TBranch        *b_eleE2x5Max;   //!
   TBranch        *b_eleE2x5Top;   //!
   TBranch        *b_eleE2x5Bottom;   //!
   TBranch        *b_eleE2x5Left;   //!
   TBranch        *b_eleE2x5Right;   //!
   TBranch        *b_eleSeedEta;   //!
   TBranch        *b_eleSeedE;   //!
   TBranch        *b_eleSeedPhi;   //!
   TBranch        *b_eleCrysEta;   //!
   TBranch        *b_eleCrysPhi;   //!
   TBranch        *b_eleCrysIEta;   //!
   TBranch        *b_eleCrysIPhi;   //!
   TBranch        *b_eleRegrE;   //!
   TBranch        *b_eleRegrEerr;   //!
   TBranch        *b_elePhoRegrE;   //!
   TBranch        *b_elePhoRegrEerr;   //!
   TBranch        *b_eleSeedTime;   //!
   TBranch        *b_eleRecoFlag;   //!
   TBranch        *b_elePos;   //!
   TBranch        *b_eleGenIndex;   //!
   TBranch        *b_eleGenGMomPID;   //!
   TBranch        *b_eleGenMomPID;   //!
   TBranch        *b_eleGenMomPt;   //!
   TBranch        *b_eleIsoTrkDR03;   //!
   TBranch        *b_eleIsoEcalDR03;   //!
   TBranch        *b_eleIsoHcalDR03;   //!
   TBranch        *b_eleIsoHcalDR0312;   //!
   TBranch        *b_eleIsoTrkDR04;   //!
   TBranch        *b_eleIsoEcalDR04;   //!
   TBranch        *b_eleIsoHcalDR04;   //!
   TBranch        *b_eleIsoHcalDR0412;   //!
   TBranch        *b_eleModIsoTrk;   //!
   TBranch        *b_eleModIsoEcal;   //!
   TBranch        *b_eleModIsoHcal;   //!
   TBranch        *b_eleMissHits;   //!
   TBranch        *b_eleConvDist;   //!
   TBranch        *b_eleConvDcot;   //!
   TBranch        *b_eleConvVtxFit;   //!
   TBranch        *b_eleIP3D;   //!
   TBranch        *b_eleIP3DErr;   //!
   TBranch        *b_eleIDMVANonTrig;   //!
   TBranch        *b_eleIDMVATrig;   //!
   TBranch        *b_elePFChIso03;   //!
   TBranch        *b_elePFPhoIso03;   //!
   TBranch        *b_elePFNeuIso03;   //!
   TBranch        *b_elePFChIso04;   //!
   TBranch        *b_elePFPhoIso04;   //!
   TBranch        *b_elePFNeuIso04;   //!
   TBranch        *b_eleESEffSigmaRR_x;   //!
   TBranch        *b_eleESEffSigmaRR_y;   //!
   TBranch        *b_eleESEffSigmaRR_z;   //!
   TBranch        *b_nPho;   //!
   TBranch        *b_phoTrg;   //!
   TBranch        *b_phoTrgFilter;   //!
   TBranch        *b_phoIsPhoton;   //!
   TBranch        *b_phoSCPos_x;   //!
   TBranch        *b_phoSCPos_y;   //!
   TBranch        *b_phoSCPos_z;   //!
   TBranch        *b_phoCaloPos_x;   //!
   TBranch        *b_phoCaloPos_y;   //!
   TBranch        *b_phoCaloPos_z;   //!
   TBranch        *b_phoE;   //!
   TBranch        *b_phoEt;   //!
   TBranch        *b_phoEta;   //!
   TBranch        *b_phoVtx_x;   //!
   TBranch        *b_phoVtx_y;   //!
   TBranch        *b_phoVtx_z;   //!
   TBranch        *b_phoPhi;   //!
   TBranch        *b_phoEtVtx;   //!
   TBranch        *b_phoEtaVtx;   //!
   TBranch        *b_phoPhiVtx;   //!
   TBranch        *b_phoR9;   //!
   TBranch        *b_phoNClus;   //!
   TBranch        *b_phoTrkIsoHollowDR03;   //!
   TBranch        *b_phoEcalIsoDR03;   //!
   TBranch        *b_phoHcalIsoDR03;   //!
   TBranch        *b_phoHcalIsoDR0312;   //!
   TBranch        *b_phoTrkIsoHollowDR04;   //!
   TBranch        *b_phoCiCdRtoTrk;   //!
   TBranch        *b_phoEcalIsoDR04;   //!
   TBranch        *b_phoHcalIsoDR04;   //!
   TBranch        *b_phoHcalIsoDR0412;   //!
   TBranch        *b_phoHoverE;   //!
   TBranch        *b_phoHoverE12;   //!
   TBranch        *b_phoEleVeto;   //!
   TBranch        *b_phoSigmaIEtaIEta;   //!
   TBranch        *b_phoSigmaIEtaIPhi;   //!
   TBranch        *b_phoSigmaIPhiIPhi;   //!
   TBranch        *b_phoCiCPF4phopfIso03;   //!
   TBranch        *b_phoCiCPF4phopfIso04;   //!
   TBranch        *b_phoCiCPF4chgpfIso02;   //!
   TBranch        *b_phoCiCPF4chgpfIso03;   //!
   TBranch        *b_phoCiCPF4chgpfIso04;   //!
   TBranch        *b_phoEmax;   //!
   TBranch        *b_phoETop;   //!
   TBranch        *b_phoEBottom;   //!
   TBranch        *b_phoELeft;   //!
   TBranch        *b_phoERight;   //!
   TBranch        *b_phoE2ndMax;   //!
   TBranch        *b_phoE3x3;   //!
   TBranch        *b_phoE3x1;   //!
   TBranch        *b_phoE1x3;   //!
   TBranch        *b_phoE5x5;   //!
   TBranch        *b_phoE1x5;   //!
   TBranch        *b_phoE2x2;   //!
   TBranch        *b_phoE2x5Max;   //!
   TBranch        *b_phoE2x5Top;   //!
   TBranch        *b_phoE2x5Bottom;   //!
   TBranch        *b_phoE2x5Left;   //!
   TBranch        *b_phoE2x5Right;   //!
   TBranch        *b_phoSeedE;   //!
   TBranch        *b_phoSeedEta;   //!
   TBranch        *b_phoSeedPhi;   //!
   TBranch        *b_phoCrysEta;   //!
   TBranch        *b_phoCrysPhi;   //!
   TBranch        *b_phoCrysIEta;   //!
   TBranch        *b_phoCrysIPhi;   //!
   TBranch        *b_phoPFChIso;   //!
   TBranch        *b_phoPFPhoIso;   //!
   TBranch        *b_phoPFNeuIso;   //!
   TBranch        *b_phoSCRChIso;   //!
   TBranch        *b_phoSCRPhoIso;   //!
   TBranch        *b_phoSCRNeuIso;   //!
   TBranch        *b_phoSCRChIso04;   //!
   TBranch        *b_phoSCRPhoIso04;   //!
   TBranch        *b_phoSCRNeuIso04;   //!
   TBranch        *b_phoRandConeChIso;   //!
   TBranch        *b_phoRandConePhoIso;   //!
   TBranch        *b_phoRandConeNeuIso;   //!
   TBranch        *b_phoRandConeChIso04;   //!
   TBranch        *b_phoRandConePhoIso04;   //!
   TBranch        *b_phoRandConeNeuIso04;   //!
   TBranch        *b_phoRegrE;   //!
   TBranch        *b_phoRegrEerr;   //!
   TBranch        *b_phoSeedTime;   //!
   TBranch        *b_phoSeedDetId1;   //!
   TBranch        *b_phoSeedDetId2;   //!
   TBranch        *b_phoLICTD;   //!
   TBranch        *b_phoRecoFlag;   //!
   TBranch        *b_phoPos;   //!
   TBranch        *b_phoGenIndex;   //!
   TBranch        *b_phoGenGMomPID;   //!
   TBranch        *b_phoGenMomPID;   //!
   TBranch        *b_phoGenMomPt;   //!
   TBranch        *b_phoSCE;   //!
   TBranch        *b_phoSCRawE;   //!
   TBranch        *b_phoESEn;   //!
   TBranch        *b_phoSCEt;   //!
   TBranch        *b_phoSCEta;   //!
   TBranch        *b_phoSCPhi;   //!
   TBranch        *b_phoSCEtaWidth;   //!
   TBranch        *b_phoSCPhiWidth;   //!
   TBranch        *b_phoSCBrem;   //!
   TBranch        *b_phoOverlap;   //!
   TBranch        *b_phohasPixelSeed;   //!
   TBranch        *b_pho_hasConvPf;   //!
   TBranch        *b_pho_hasSLConvPf;   //!
   TBranch        *b_pho_pfconvVtxZ;   //!
   TBranch        *b_pho_pfconvVtxZErr;   //!
   TBranch        *b_pho_nSLConv;   //!
   TBranch        *b_pho_pfSLConvPos_x;   //!
   TBranch        *b_pho_pfSLConvPos_y;   //!
   TBranch        *b_pho_pfSLConvPos_z;   //!
   TBranch        *b_pho_pfSLConvVtxZ;   //!
   TBranch        *b_phoIsConv;   //!
   TBranch        *b_phoNConv;   //!
   TBranch        *b_phoConvInvMass;   //!
   TBranch        *b_phoConvCotTheta;   //!
   TBranch        *b_phoConvEoverP;   //!
   TBranch        *b_phoConvZofPVfromTrks;   //!
   TBranch        *b_phoConvMinDist;   //!
   TBranch        *b_phoConvdPhiAtVtx;   //!
   TBranch        *b_phoConvdPhiAtCalo;   //!
   TBranch        *b_phoConvdEtaAtCalo;   //!
   TBranch        *b_phoConvTrkd0_x;   //!
   TBranch        *b_phoConvTrkd0_y;   //!
   TBranch        *b_phoConvTrkPin_x;   //!
   TBranch        *b_phoConvTrkPin_y;   //!
   TBranch        *b_phoConvTrkPout_x;   //!
   TBranch        *b_phoConvTrkPout_y;   //!
   TBranch        *b_phoConvTrkdz_x;   //!
   TBranch        *b_phoConvTrkdz_y;   //!
   TBranch        *b_phoConvTrkdzErr_x;   //!
   TBranch        *b_phoConvTrkdzErr_y;   //!
   TBranch        *b_phoConvChi2;   //!
   TBranch        *b_phoConvChi2Prob;   //!
   TBranch        *b_phoConvNTrks;   //!
   TBranch        *b_phoConvCharge1;   //!
   TBranch        *b_phoConvCharge2;   //!
   TBranch        *b_phoConvValidVtx;   //!
   TBranch        *b_phoConvLikeLihood;   //!
   TBranch        *b_phoConvP4_0;   //!
   TBranch        *b_phoConvP4_1;   //!
   TBranch        *b_phoConvP4_2;   //!
   TBranch        *b_phoConvP4_3;   //!
   TBranch        *b_phoConvVtx_x;   //!
   TBranch        *b_phoConvVtx_y;   //!
   TBranch        *b_phoConvVtx_z;   //!
   TBranch        *b_phoConvVtxErr_x;   //!
   TBranch        *b_phoConvVtxErr_y;   //!
   TBranch        *b_phoConvVtxErr_z;   //!
   TBranch        *b_phoConvPairMomentum_x;   //!
   TBranch        *b_phoConvPairMomentum_y;   //!
   TBranch        *b_phoConvPairMomentum_z;   //!
   TBranch        *b_phoConvRefittedMomentum_x;   //!
   TBranch        *b_phoConvRefittedMomentum_y;   //!
   TBranch        *b_phoConvRefittedMomentum_z;   //!
   TBranch        *b_SingleLegConv;   //!
   TBranch        *b_phoPFConvVtx_x;   //!
   TBranch        *b_phoPFConvVtx_y;   //!
   TBranch        *b_phoPFConvVtx_z;   //!
   TBranch        *b_phoPFConvMom_x;   //!
   TBranch        *b_phoPFConvMom_y;   //!
   TBranch        *b_phoPFConvMom_z;   //!
   TBranch        *b_phoESEffSigmaRR_x;   //!
   TBranch        *b_phoESEffSigmaRR_y;   //!
   TBranch        *b_phoESEffSigmaRR_z;   //!
   TBranch        *b_nMu;   //!
   TBranch        *b_muTrg;   //!
   TBranch        *b_muEta;   //!
   TBranch        *b_muPhi;   //!
   TBranch        *b_muCharge;   //!
   TBranch        *b_muPt;   //!
   TBranch        *b_muPz;   //!
   TBranch        *b_muVtx_x;   //!
   TBranch        *b_muVtx_y;   //!
   TBranch        *b_muVtx_z;   //!
   TBranch        *b_muVtxGlb_x;   //!
   TBranch        *b_muVtxGlb_y;   //!
   TBranch        *b_muVtxGlb_z;   //!
   TBranch        *b_muGenIndex;   //!
   TBranch        *b_mucktPt;   //!
   TBranch        *b_mucktPtErr;   //!
   TBranch        *b_mucktEta;   //!
   TBranch        *b_mucktPhi;   //!
   TBranch        *b_mucktdxy;   //!
   TBranch        *b_mucktdz;   //!
   TBranch        *b_muIsoTrk;   //!
   TBranch        *b_muIsoCalo;   //!
   TBranch        *b_muIsoEcal;   //!
   TBranch        *b_muIsoHcal;   //!
   TBranch        *b_muChi2NDF;   //!
   TBranch        *b_muInnerChi2NDF;   //!
   TBranch        *b_muPFIsoR04_CH;   //!
   TBranch        *b_muPFIsoR04_NH;   //!
   TBranch        *b_muPFIsoR04_Pho;   //!
   TBranch        *b_muPFIsoR04_PU;   //!
   TBranch        *b_muPFIsoR04_CPart;   //!
   TBranch        *b_muPFIsoR04_NHHT;   //!
   TBranch        *b_muPFIsoR04_PhoHT;   //!
   TBranch        *b_muPFIsoR03_CH;   //!
   TBranch        *b_muPFIsoR03_NH;   //!
   TBranch        *b_muPFIsoR03_Pho;   //!
   TBranch        *b_muPFIsoR03_PU;   //!
   TBranch        *b_muPFIsoR03_CPart;   //!
   TBranch        *b_muPFIsoR03_NHHT;   //!
   TBranch        *b_muPFIsoR03_PhoHT;   //!
   TBranch        *b_muType;   //!
   TBranch        *b_muD0;   //!
   TBranch        *b_muDz;   //!
   TBranch        *b_muD0GV;   //!
   TBranch        *b_muDzGV;   //!
   TBranch        *b_muD0Vtx;   //!
   TBranch        *b_muDzVtx;   //!
   TBranch        *b_muInnerD0;   //!
   TBranch        *b_muInnerDz;   //!
   TBranch        *b_muInnerD0GV;   //!
   TBranch        *b_muInnerDzGV;   //!
   TBranch        *b_muInnerPt;   //!
   TBranch        *b_muInnerPtErr;   //!
   TBranch        *b_muNumberOfValidTrkLayers;   //!
   TBranch        *b_muNumberOfValidTrkHits;   //!
   TBranch        *b_muNumberOfValidPixelLayers;   //!
   TBranch        *b_muNumberOfValidPixelHits;   //!
   TBranch        *b_muNumberOfValidMuonHits;   //!
   TBranch        *b_muStations;   //!
   TBranch        *b_muChambers;   //!
   TBranch        *b_muIP3D;   //!
   TBranch        *b_muIP3DErr;   //!
   TBranch        *b_nTau;   //!
   TBranch        *b_tauDecayModeFinding;   //!
   TBranch        *b_tauAgainstElectronLooseMVA3;   //!
   TBranch        *b_tauAgainstElectronMediumMVA3;   //!
   TBranch        *b_tauAgainstElectronTightMVA3;   //!
   TBranch        *b_tauAgainstElectronVTightMVA3;   //!
   TBranch        *b_tauAgainstElectronDeadECAL;   //!
   TBranch        *b_tauAgainstMuonLoose2;   //!
   TBranch        *b_tauAgainstMuonMedium2;   //!
   TBranch        *b_tauAgainstMuonTight2;   //!
   TBranch        *b_tauCombinedIsolationDeltaBetaCorrRaw3Hits;   //!
   TBranch        *b_tauLooseCombinedIsolationDeltaBetaCorr3Hits;   //!
   TBranch        *b_tauMediumCombinedIsolationDeltaBetaCorr3Hits;   //!
   TBranch        *b_tauTightCombinedIsolationDeltaBetaCorr3Hits;   //!
   TBranch        *b_tauEta;   //!
   TBranch        *b_tauPhi;   //!
   TBranch        *b_tauPt;   //!
   TBranch        *b_tauEt;   //!
   TBranch        *b_tauCharge;   //!
   TBranch        *b_tauDecayMode;   //!
   TBranch        *b_tauEMFraction;   //!
   TBranch        *b_tauHCAL3x3OverPLead;   //!
   TBranch        *b_tauHCALMaxOverPLead;   //!
   TBranch        *b_tauHCALTotOverPLead;   //!
   TBranch        *b_tauIsolationPFChargedHadrCandsPtSum;   //!
   TBranch        *b_tauIsolationPFGammaCandsEtSum;   //!
   TBranch        *b_tauLeadPFChargedHadrCandsignedSipt;   //!
   TBranch        *b_tauLeadChargedHadronExists;   //!
   TBranch        *b_tauLeadChargedHadronEta;   //!
   TBranch        *b_tauLeadChargedHadronPhi;   //!
   TBranch        *b_tauLeadChargedHadronPt;   //!
   TBranch        *b_rho25;   //!
   TBranch        *b_rho25_neu;   //!
   TBranch        *b_rho25_muPFiso;   //!
   TBranch        *b_rho25_elePFiso;   //!
   TBranch        *b_rho2011;   //!
   TBranch        *b_rho2012;   //!
   TBranch        *b_QGTag_MLP;   //!
   TBranch        *b_QGTag_likelihood;   //!
   TBranch        *b_nCA8Jet;   //!
   TBranch        *b_CA8JetPt;   //!
   TBranch        *b_CA8JetEta;   //!
   TBranch        *b_CA8JetPhi;   //!
   TBranch        *b_CA8JetMass;   //!
   TBranch        *b_CA8JetArea;   //!
   TBranch        *b_CA8Jet_tau1;   //!
   TBranch        *b_CA8Jet_tau2;   //!
   TBranch        *b_CA8Jet_tau3;   //!
   TBranch        *b_CA8JetCHF;   //!
   TBranch        *b_CA8JetNHF;   //!
   TBranch        *b_CA8JetCEF;   //!
   TBranch        *b_CA8JetNEF;   //!
   TBranch        *b_CA8JetNCH;   //!
   TBranch        *b_CA8Jetnconstituents;   //!
   TBranch        *b_CA8prunedJetMass;   //!
   TBranch        *b_CA8prunedJet_nSubJets;   //!
   TBranch        *b_CA8prunedJet_SubjetPt;   //!
   TBranch        *b_CA8prunedJet_SubjetEta;   //!
   TBranch        *b_CA8prunedJet_SubjetPhi;   //!
   TBranch        *b_CA8prunedJet_SubjetMass;   //!
   TBranch        *b_nJet;   //!
   TBranch        *b_jetTrg;   //!
   TBranch        *b_jetEn;   //!
   TBranch        *b_jetPt;   //!
   TBranch        *b_jetEta;   //!
   TBranch        *b_jetPhi;   //!
   TBranch        *b_jetCharge;   //!
   TBranch        *b_jetEt;   //!
   TBranch        *b_jetRawPt;   //!
   TBranch        *b_jetRawEn;   //!
   TBranch        *b_jetArea;   //!
   TBranch        *b_jetCHF;   //!
   TBranch        *b_jetNHF;   //!
   TBranch        *b_jetCEF;   //!
   TBranch        *b_jetNEF;   //!
   TBranch        *b_jetNCH;   //!
   TBranch        *b_jetHFHAE;   //!
   TBranch        *b_jetHFEME;   //!
   TBranch        *b_jetNConstituents;   //!
   TBranch        *b_jetCombinedSecondaryVtxBJetTags;   //!
   TBranch        *b_jetCombinedSecondaryVtxMVABJetTags;   //!
   TBranch        *b_jetJetProbabilityBJetTags;   //!
   TBranch        *b_jetJetBProbabilityBJetTags;   //!
   TBranch        *b_jetBetaStar;   //!
   TBranch        *b_jetPFLooseId;   //!
   TBranch        *b_jetDRMean;   //!
   TBranch        *b_jetDR2Mean;   //!
   TBranch        *b_jetDZ;   //!
   TBranch        *b_jetFrac01;   //!
   TBranch        *b_jetFrac02;   //!
   TBranch        *b_jetFrac03;   //!
   TBranch        *b_jetFrac04;   //!
   TBranch        *b_jetFrac05;   //!
   TBranch        *b_jetFrac06;   //!
   TBranch        *b_jetFrac07;   //!
   TBranch        *b_jetBeta;   //!
   TBranch        *b_jetBetaStarCMG;   //!
   TBranch        *b_jetBetaStarClassic;   //!
   TBranch        *b_jetBetaExt;   //!
   TBranch        *b_jetBetaStarCMGExt;   //!
   TBranch        *b_jetBetaStarClassicExt;   //!
   TBranch        *b_jetNNeutrals;   //!
   TBranch        *b_jetNCharged;   //!
   TBranch        *b_jetMVAs;   //!
   TBranch        *b_jetWPLevels;   //!
   TBranch        *b_jetMVAsExt_simple;   //!
   TBranch        *b_jetWPLevelsExt_simple;   //!
   TBranch        *b_jetMVAsExt_full;   //!
   TBranch        *b_jetWPLevelsExt_full;   //!
   TBranch        *b_jetMVAsExt_cutBased;   //!
   TBranch        *b_jetWPLevelsExt_cutBased;   //!
   TBranch        *b_jetMVAsExt_philv1;   //!
   TBranch        *b_jetWPLevelsExt_philv1;   //!
   TBranch        *b_jetMt;   //!
   TBranch        *b_jetJECUnc;   //!
   TBranch        *b_jetLeadTrackPt;   //!
   TBranch        *b_jetVtxPt;   //!
   TBranch        *b_jetVtxMass;   //!
   TBranch        *b_jetVtx3dL;   //!
   TBranch        *b_jetVtx3deL;   //!
   TBranch        *b_jetSoftLeptPt;   //!
   TBranch        *b_jetSoftLeptPtRel;   //!
   TBranch        *b_jetSoftLeptdR;   //!
   TBranch        *b_jetSoftLeptIdlooseMu;   //!
   TBranch        *b_jetSoftLeptIdEle95;   //!
   TBranch        *b_jetDPhiMETJet;   //!
   TBranch        *b_jetPuJetIdL;   //!
   TBranch        *b_jetPuJetIdM;   //!
   TBranch        *b_jetPuJetIdT;   //!
   TBranch        *b_jetPartonID;   //!
   TBranch        *b_jetGenJetIndex;   //!
   TBranch        *b_jetGenJetEn;   //!
   TBranch        *b_jetGenJetPt;   //!
   TBranch        *b_jetGenJetEta;   //!
   TBranch        *b_jetGenJetPhi;   //!
   TBranch        *b_jetGenPartonID;   //!
   TBranch        *b_jetGenEn;   //!
   TBranch        *b_jetGenPt;   //!
   TBranch        *b_jetGenEta;   //!
   TBranch        *b_jetGenPhi;   //!
   TBranch        *b_jetGenPartonMomID;   //!
   TBranch        *b_nConv;   //!
   TBranch        *b_convP4_x;   //!
   TBranch        *b_convP4_y;   //!
   TBranch        *b_convP4_z;   //!
   TBranch        *b_convP4_E;   //!
   TBranch        *b_convVtx_x;   //!
   TBranch        *b_convVtx_y;   //!
   TBranch        *b_convVtx_z;   //!
   TBranch        *b_convVtxErr_x;   //!
   TBranch        *b_convVtxErr_y;   //!
   TBranch        *b_convVtxErr_z;   //!
   TBranch        *b_convPairMomentum_x;   //!
   TBranch        *b_convPairMomentum_y;   //!
   TBranch        *b_convPairMomentum_z;   //!
   TBranch        *b_convRefittedMomentum_x;   //!
   TBranch        *b_convRefittedMomentum_y;   //!
   TBranch        *b_convRefittedMomentum_z;   //!
   TBranch        *b_convNTracks;   //!
   TBranch        *b_convPairInvMass;   //!
   TBranch        *b_convPairCotThetaSep;   //!
   TBranch        *b_convEoverP;   //!
   TBranch        *b_convDistOfMinApproach;   //!
   TBranch        *b_convDPhiTrksAtVtx;   //!
   TBranch        *b_convDPhiTrksAtEcal;   //!
   TBranch        *b_convDEtaTrksAtEcal;   //!
   TBranch        *b_convDxy;   //!
   TBranch        *b_convDz;   //!
   TBranch        *b_convLxy;   //!
   TBranch        *b_convLz;   //!
   TBranch        *b_convZofPrimVtxFromTrks;   //!
   TBranch        *b_convNHitsBeforeVtx_0;   //!
   TBranch        *b_convNHitsBeforeVtx_1;   //!
   TBranch        *b_convNSharedHits;   //!
   TBranch        *b_convValidVtx;   //!
   TBranch        *b_convMVALikelihood;   //!
   TBranch        *b_convChi2;   //!
   TBranch        *b_convChi2Probability;   //!
   TBranch        *b_convTk1Dz;   //!
   TBranch        *b_convTk2Dz;   //!
   TBranch        *b_convTk1DzErr;   //!
   TBranch        *b_convTk2DzErr;   //!
   TBranch        *b_convCh1Ch2;   //!
   TBranch        *b_convTk1D0;   //!
   TBranch        *b_convTk1Pout;   //!
   TBranch        *b_convTk1Pin;   //!
   TBranch        *b_convTk2D0;   //!
   TBranch        *b_convTk2Pout;   //!
   TBranch        *b_convTk2Pin;   //!

public:
   TEventTree(TTree *tree=0);
   virtual ~TEventTree();
   void GetEntryNeededBranchesOnly(int channel, int sample, Long64_t entry);
   void GetEntryCommon(Long64_t entry);
   void GetEntryMuoSpecific(Long64_t entry);
   void GetEntryEleSpecific(Long64_t entry);
   void GetEntryMCSpecific(Long64_t entry);
   void GetEntryRandCone(Long64_t entry);
   virtual void     Init(TTree *tree);
//   void SetMCSpecificAddresses();
};

#endif //#ifndef TEventTree_h

