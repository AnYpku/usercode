//////////////////////////////////////////////////////////
// Input tree structure
//////////////////////////////////////////////////////////

#ifndef TEventTree_h
#define TEventTree_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
  //ROOT classes

class TEventTree {
public:
   const static Int_t kMaxnHLT = 456;
   const static Int_t kMaxnVtx = 70;
   const static Int_t kMaxnVtxBS = 70;
   const static Int_t kMaxnPFPho = 48;
   const static Int_t kMaxnPFEle = 6;
   const static Int_t kMaxnEle = 10;
   const static Int_t kMaxnPho = 16;
   const static Int_t kMaxnMu = 33;
   const static Int_t kMaxnJet = 97;
   const static Int_t kMaxnLowPtJet = 69;
   const static Int_t kMaxnConv = 0;//500
     //since they are not used in the analysis,
     //I don't want to creat these big arrays of Float_t
     //for converted photons
   const static Int_t kMaxnMC = 13;
   const static Int_t kMaxnPUInfo = 3;

public:
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Int_t           run;
   Long64_t        event;
   Int_t           lumis;
   Bool_t          isData;
   Int_t           nHLT;
   Int_t           HLT[kMaxnHLT];   //[nHLT]
   Int_t           HLTIndex[70];
   Float_t         bspotPos[3];
   Int_t           nVtx;
   Float_t         vtx[kMaxnVtx][3];   //[nVtx]
   Int_t           IsVtxGood;
   Int_t           nVtxBS;
   Float_t         vtxbs[kMaxnVtxBS][3];   //[nVtxBS]
//MC specific variables start here:
   Float_t         pdf[7];
   Float_t         pthat;
   Float_t         processID;
   Int_t           nMC;
   Int_t           mcPID[kMaxnMC];   //[nMC]
   Float_t         mcVtx[kMaxnMC][3];   //[nMC]
   Float_t         mcPt[kMaxnMC];   //[nMC]
   Float_t         mcMass[kMaxnMC];   //[nMC]
   Float_t         mcEta[kMaxnMC];   //[nMC]
   Float_t         mcPhi[kMaxnMC];   //[nMC]
   Float_t         mcE[kMaxnMC];   //[nMC]
   Float_t         mcEt[kMaxnMC];   //[nMC]
   Int_t           mcGMomPID[kMaxnMC];   //[nMC]
   Int_t           mcMomPID[kMaxnMC];   //[nMC]
   Float_t         mcMomPt[kMaxnMC];   //[nMC]
   Float_t         mcMomMass[kMaxnMC];   //[nMC]
   Float_t         mcMomEta[kMaxnMC];   //[nMC]
   Float_t         mcMomPhi[kMaxnMC];   //[nMC]
   Int_t           mcIndex[kMaxnMC];   //[nMC]
   Int_t           mcDecayType[kMaxnMC];   //[nMC]
   Float_t         genMET;
   Float_t         genMETPhi;
   Int_t           nPUInfo;
   Int_t           nPU[kMaxnPUInfo];   //[nPUInfo]
   Int_t           puBX[kMaxnPUInfo];   //[nPUInfo]
   Float_t         puTrue[kMaxnPUInfo];   //[nPUInfo]
//MC specific variables ends here
   Float_t         MET;
   Float_t         METPhi;
   Float_t         METsumEt;
   Float_t         pfMET;
   Float_t         pfMETPhi;
   Float_t         pfMETsumEt;
   Float_t         pfMETmEtSig;
   Float_t         pfMETSig;
   Float_t         recoPfMET;
   Float_t         recoPfMETPhi;
   Float_t         recoPfMETsumEt;
   Float_t         recoPfMETmEtSig;
   Float_t         recoPfMETSig;
   Int_t           metFilters[10];
   Int_t           nEle;
   Int_t           eleTrg[kMaxnEle][16];   //[nEle]
   Int_t           eleClass[kMaxnEle];   //[nEle]
   Int_t           eleIsEcalDriven[kMaxnEle];   //[nEle]
   Int_t           eleCharge[kMaxnEle];   //[nEle]
   Float_t         eleEn[kMaxnEle];   //[nEle]
   Float_t         eleEcalEn[kMaxnEle];   //[nEle]
   Float_t         eleSCRawEn[kMaxnEle];   //[nEle]
   Float_t         eleSCEn[kMaxnEle];   //[nEle]
   Float_t         eleESEn[kMaxnEle];   //[nEle]
   Float_t         elePt[kMaxnEle];   //[nEle]
   Float_t         eleEta[kMaxnEle];   //[nEle]
   Float_t         elePhi[kMaxnEle];   //[nEle]
   Float_t         eleEtaVtx[kMaxnEle][100];   //[nEle]
   Float_t         elePhiVtx[kMaxnEle][100];   //[nEle]
   Float_t         eleEtVtx[kMaxnEle][100];   //[nEle]
   Float_t         eleSCEta[kMaxnEle];   //[nEle]
   Float_t         eleSCPhi[kMaxnEle];   //[nEle]
   Float_t         eleSCEtaWidth[kMaxnEle];   //[nEle]
   Float_t         eleSCPhiWidth[kMaxnEle];   //[nEle]
   Float_t         eleVtx[kMaxnEle][3];   //[nEle]
   Float_t         eleD0[kMaxnEle];   //[nEle]
   Float_t         eleDz[kMaxnEle];   //[nEle]
   Float_t         eleD0GV[kMaxnEle];   //[nEle]
   Float_t         eleDzGV[kMaxnEle];   //[nEle]
   Float_t         eleD0Vtx[kMaxnEle][100];   //[nEle]
   Float_t         eleDzVtx[kMaxnEle][100];   //[nEle]
   Float_t         eleHoverE[kMaxnEle];   //[nEle]
   Float_t         eleHoverE12[kMaxnEle];   //[nEle]
   Float_t         eleEoverP[kMaxnEle];   //[nEle]
   Float_t         elePin[kMaxnEle];   //[nEle]
   Float_t         elePout[kMaxnEle];   //[nEle]
   Float_t         eleTrkMomErr[kMaxnEle];   //[nEle]
   Float_t         eleBrem[kMaxnEle];   //[nEle]
   Float_t         eledEtaAtVtx[kMaxnEle];   //[nEle]
   Float_t         eledPhiAtVtx[kMaxnEle];   //[nEle]
   Float_t         eleSigmaIEtaIEta[kMaxnEle];   //[nEle]
   Float_t         eleSigmaIEtaIPhi[kMaxnEle];   //[nEle]
   Float_t         eleSigmaIPhiIPhi[kMaxnEle];   //[nEle]
   Float_t         eleEmax[kMaxnEle];   //[nEle]
   Float_t         eleE1x5[kMaxnEle];   //[nEle]
   Float_t         eleE3x3[kMaxnEle];   //[nEle]
   Float_t         eleE5x5[kMaxnEle];   //[nEle]
   Float_t         eleE2x5Right[kMaxnEle];   //[nEle]
   Float_t         eleE2x5Left[kMaxnEle];   //[nEle]
   Float_t         eleE2x5Top[kMaxnEle];   //[nEle]
   Float_t         eleE2x5Bottom[kMaxnEle];   //[nEle]
   Float_t         eleE2x5Max[kMaxnEle];   //[nEle]
   Float_t         eleRegrE[kMaxnEle];   //[nEle]
   Float_t         eleRegrEerr[kMaxnEle];   //[nEle]
   Float_t         elePhoRegrE[kMaxnEle];   //[nEle]
   Float_t         elePhoRegrEerr[kMaxnEle];   //[nEle]
   Float_t         eleSeedTime[kMaxnEle];   //[nEle]
   Int_t           eleRecoFlag[kMaxnEle];   //[nEle]
   Int_t           elePos[kMaxnEle];   //[nEle]
//MC specific variables start here:
   Int_t           eleGenIndex[kMaxnEle];   //[nEle]
   Int_t           eleGenGMomPID[kMaxnEle];   //[nEle]
   Int_t           eleGenMomPID[kMaxnEle];   //[nEle]
   Float_t         eleGenMomPt[kMaxnEle];   //[nEle]
//MC specific variables end here
   Float_t         eleIsoTrkDR03[kMaxnEle];   //[nEle]
   Float_t         eleIsoEcalDR03[kMaxnEle];   //[nEle]
   Float_t         eleIsoHcalDR03[kMaxnEle];   //[nEle]
   Float_t         eleIsoHcalDR0312[kMaxnEle];   //[nEle]
   Float_t         eleIsoTrkDR04[kMaxnEle];   //[nEle]
   Float_t         eleIsoEcalDR04[kMaxnEle];   //[nEle]
   Float_t         eleIsoHcalDR04[kMaxnEle];   //[nEle]
   Float_t         eleIsoHcalDR0412[kMaxnEle];   //[nEle]
   Int_t           eleMissHits[kMaxnEle];   //[nEle]
   Float_t         eleConvDist[kMaxnEle];   //[nEle]
   Float_t         eleConvDcot[kMaxnEle];   //[nEle]
   Int_t           eleConvVtxFit[kMaxnEle];   //[nEle]
   Float_t         eleIP3D[kMaxnEle];   //[nEle]
   Float_t         eleIP3DErr[kMaxnEle];   //[nEle]
   Float_t         eleIDMVANonTrig[kMaxnEle];   //[nEle]
   Float_t         eleIDMVATrig[kMaxnEle];   //[nEle]
   Float_t         eleIDMVATrigIDIso[kMaxnEle];   //[nEle]
   Float_t         elePFChIso03[kMaxnEle];   //[nEle]
   Float_t         elePFPhoIso03[kMaxnEle];   //[nEle]
   Float_t         elePFNeuIso03[kMaxnEle];   //[nEle]
   Float_t         elePFChIso04[kMaxnEle];   //[nEle]
   Float_t         elePFPhoIso04[kMaxnEle];   //[nEle]
   Float_t         elePFNeuIso04[kMaxnEle];   //[nEle]
   Float_t         eleESEffSigmaRR[kMaxnEle][3];   //[nEle]
   Int_t           nPho;
   Int_t           phoTrg[kMaxnPho][8];   //[nPho]
   Int_t           phoTrgFilter[kMaxnPho][50];   //[nPho]
   Bool_t          phoIsPhoton[kMaxnPho];   //[nPho]
   Float_t         phoSCPos[kMaxnPho][3];   //[nPho]
   Float_t         phoCaloPos[kMaxnPho][3];   //[nPho]
   Float_t         phoE[kMaxnPho];   //[nPho]
   Float_t         phoEt[kMaxnPho];   //[nPho]
   Float_t         phoEta[kMaxnPho];   //[nPho]
   Float_t         phoVtx[kMaxnPho][3];   //[nPho]
   Float_t         phoPhi[kMaxnPho];   //[nPho]
   Float_t         phoEtVtx[kMaxnPho][100];   //[nPho]
   Float_t         phoEtaVtx[kMaxnPho][100];   //[nPho]
   Float_t         phoPhiVtx[kMaxnPho][100];   //[nPho]
   Float_t         phoR9[kMaxnPho];   //[nPho]
   Float_t         phoTrkIsoHollowDR03[kMaxnPho];   //[nPho]
   Float_t         phoEcalIsoDR03[kMaxnPho];   //[nPho]
   Float_t         phoHcalIsoDR03[kMaxnPho];   //[nPho]
   Float_t         phoHcalIsoDR0312[kMaxnPho];   //[nPho]
   Float_t         phoTrkIsoHollowDR04[kMaxnPho];   //[nPho]
   Float_t         phoCiCTrkIsoDR03[kMaxnPho][100];   //[nPho]
   Float_t         phoCiCTrkIsoDR04[kMaxnPho][100];   //[nPho]
   Float_t         phoCiCdRtoTrk[kMaxnPho];   //[nPho]
   Float_t         phoEcalIsoDR04[kMaxnPho];   //[nPho]
   Float_t         phoHcalIsoDR04[kMaxnPho];   //[nPho]
   Float_t         phoHcalIsoDR0412[kMaxnPho];   //[nPho]
   Float_t         phoHoverE[kMaxnPho];   //[nPho]
   Float_t         phoHoverE12[kMaxnPho];   //[nPho]
   Int_t           phoEleVeto[kMaxnPho];   //[nPho]
   Float_t         phoSigmaIEtaIEta[kMaxnPho];   //[nPho]
   Float_t         phoSigmaIEtaIPhi[kMaxnPho];   //[nPho]
   Float_t         phoSigmaIPhiIPhi[kMaxnPho];   //[nPho]
   Float_t         phoCiCPF4phopfIso03[kMaxnPho];   //[nPho]
   Float_t         phoCiCPF4phopfIso04[kMaxnPho];   //[nPho]
   Float_t         phoCiCPF4chgpfIso02[kMaxnPho][100];   //[nPho]
   Float_t         phoCiCPF4chgpfIso03[kMaxnPho][100];   //[nPho]
   Float_t         phoCiCPF4chgpfIso04[kMaxnPho][100];   //[nPho]
   Float_t         phoEmax[kMaxnPho];   //[nPho]
   Float_t         phoEtop[kMaxnPho];   //[nPho]
   Float_t         phoEbottom[kMaxnPho];   //[nPho]
   Float_t         phoEleft[kMaxnPho];   //[nPho]
   Float_t         phoEright[kMaxnPho];   //[nPho]
   Float_t         phoE3x3[kMaxnPho];   //[nPho]
   Float_t         phoE3x1[kMaxnPho];   //[nPho]
   Float_t         phoE1x3[kMaxnPho];   //[nPho]
   Float_t         phoE5x5[kMaxnPho];   //[nPho]
   Float_t         phoE1x5[kMaxnPho];   //[nPho]
   Float_t         phoE2x2[kMaxnPho];   //[nPho]
   Float_t         phoE2x5Max[kMaxnPho];   //[nPho]
   Float_t         phoE2x5Right[kMaxnPho];   //[nPho]
   Float_t         phoE2x5Left[kMaxnPho];   //[nPho]
   Float_t         phoE2x5Top[kMaxnPho];   //[nPho]
   Float_t         phoE2x5Bottom[kMaxnPho];   //[nPho]
   Float_t         phoPFChIso[kMaxnPho];   //[nPho]
   Float_t         phoPFPhoIso[kMaxnPho];   //[nPho]
   Float_t         phoPFNeuIso[kMaxnPho];   //[nPho]
   Float_t         phoSCRChIso[kMaxnPho];   //[nPho]
   Float_t         phoSCRPhoIso[kMaxnPho];   //[nPho]
   Float_t         phoSCRNeuIso[kMaxnPho];   //[nPho]
   Float_t         phoRegrE[kMaxnPho];   //[nPho]
   Float_t         phoRegrEerr[kMaxnPho];   //[nPho]
   Float_t         phoSeedTime[kMaxnPho];   //[nPho]
   Int_t           phoSeedDetId1[kMaxnPho];   //[nPho]
   Int_t           phoSeedDetId2[kMaxnPho];   //[nPho]
   Int_t           phoRecoFlag[kMaxnPho];   //[nPho]
   Int_t           phoPos[kMaxnPho];   //[nPho]
//MC specific variables start here
   Int_t           phoGenIndex[kMaxnPho];   //[nPho]
   Int_t           phoGenGMomPID[kMaxnPho];   //[nPho]
   Int_t           phoGenMomPID[kMaxnPho];   //[nPho]
   Float_t         phoGenMomPt[kMaxnPho];   //[nPho]
//MC specific variables end here
   Float_t         phoSCE[kMaxnPho];   //[nPho]
   Float_t         phoSCRawE[kMaxnPho];   //[nPho]
   Float_t         phoESEn[kMaxnPho];   //[nPho]
   Float_t         phoSCEt[kMaxnPho];   //[nPho]
   Float_t         phoSCEta[kMaxnPho];   //[nPho]
   Float_t         phoSCPhi[kMaxnPho];   //[nPho]
   Float_t         phoSCEtaWidth[kMaxnPho];   //[nPho]
   Float_t         phoSCPhiWidth[kMaxnPho];   //[nPho]
   Float_t         phoSCBrem[kMaxnPho];   //[nPho]
   Int_t           phoOverlap[kMaxnPho];   //[nPho]
   Int_t           phohasPixelSeed[kMaxnPho];   //[nPho]
   Int_t           pho_hasConvPf[kMaxnPho];   //[nPho]
   Int_t           pho_hasSLConvPf[kMaxnPho];   //[nPho]
   Float_t         pho_pfconvVtxZ[kMaxnPho];   //[nPho]
   Float_t         pho_pfconvVtxZErr[kMaxnPho];   //[nPho]
   Int_t           pho_nSLConv[kMaxnPho];   //[nPho]
   Float_t         pho_pfSLConvPos[kMaxnPho][3];   //[nPho]
   Float_t         pho_pfSLConvVtxZ[kMaxnPho][20];   //[nPho]
   Int_t           phoIsConv[kMaxnPho];   //[nPho]
   Int_t           phoNConv[kMaxnPho];   //[nPho]
   Float_t         phoConvInvMass[kMaxnPho];   //[nPho]
   Float_t         phoConvCotTheta[kMaxnPho];   //[nPho]
   Float_t         phoConvEoverP[kMaxnPho];   //[nPho]
   Float_t         phoConvZofPVfromTrks[kMaxnPho];   //[nPho]
   Float_t         phoConvMinDist[kMaxnPho];   //[nPho]
   Float_t         phoConvdPhiAtVtx[kMaxnPho];   //[nPho]
   Float_t         phoConvdPhiAtCalo[kMaxnPho];   //[nPho]
   Float_t         phoConvdEtaAtCalo[kMaxnPho];   //[nPho]
   Float_t         phoConvTrkd0[kMaxnPho][2];   //[nPho]
   Float_t         phoConvTrkPin[kMaxnPho][2];   //[nPho]
   Float_t         phoConvTrkPout[kMaxnPho][2];   //[nPho]
   Float_t         phoConvTrkdz[kMaxnPho][2];   //[nPho]
   Float_t         phoConvTrkdzErr[kMaxnPho][2];   //[nPho]
   Float_t         phoConvChi2[kMaxnPho];   //[nPho]
   Float_t         phoConvChi2Prob[kMaxnPho];   //[nPho]
   Int_t           phoConvNTrks[kMaxnPho];   //[nPho]
   Float_t         phoConvCharge[kMaxnPho][2];   //[nPho]
   Float_t         phoConvValidVtx[kMaxnPho];   //[nPho]
   Float_t         phoConvLikeLihood[kMaxnPho];   //[nPho]
   Float_t         phoConvP4[kMaxnPho][4];   //[nPho]
   Float_t         phoConvVtx[kMaxnPho][3];   //[nPho]
   Float_t         phoConvVtxErr[kMaxnPho][3];   //[nPho]
   Float_t         phoConvPairMomentum[kMaxnPho][3];   //[nPho]
   Float_t         phoConvRefittedMomentum[kMaxnPho][3];   //[nPho]
   Int_t           SingleLegConv[kMaxnPho];   //[nPho]
   Float_t         phoPFConvVtx[kMaxnPho][3];   //[nPho]
   Float_t         phoPFConvMom[kMaxnPho][3];   //[nPho]
   Float_t         phoESEffSigmaRR[kMaxnPho][3];   //[nPho]
   Int_t           nMu;
   Int_t           muTrg[kMaxnMu][10];   //[nMu]
   Float_t         muEta[kMaxnMu];   //[nMu]
   Float_t         muPhi[kMaxnMu];   //[nMu]
   Int_t           muCharge[kMaxnMu];   //[nMu]
   Float_t         muPt[kMaxnMu];   //[nMu]
   Float_t         muPz[kMaxnMu];   //[nMu]
   Float_t         muVtx[kMaxnMu][3];   //[nMu]
   Float_t         muVtxGlb[kMaxnMu][3];   //[nMu]
//MC specific variable starts
   Int_t           muGenIndex[kMaxnMu];   //[nMu]
//MC specific variable ends
   Float_t         mucktPt[kMaxnMu];   //[nMu]
   Float_t         mucktPtErr[kMaxnMu];   //[nMu]
   Float_t         mucktEta[kMaxnMu];   //[nMu]
   Float_t         mucktPhi[kMaxnMu];   //[nMu]
   Float_t         mucktdxy[kMaxnMu];   //[nMu]
   Float_t         mucktdz[kMaxnMu];   //[nMu]
   Float_t         muIsoTrk[kMaxnMu];   //[nMu]
   Float_t         muIsoCalo[kMaxnMu];   //[nMu]
   Float_t         muIsoEcal[kMaxnMu];   //[nMu]
   Float_t         muIsoHcal[kMaxnMu];   //[nMu]
   Float_t         muChi2NDF[kMaxnMu];   //[nMu]
   Float_t         muInnerChi2NDF[kMaxnMu];   //[nMu]
   Float_t         muPFIsoR04_CH[kMaxnMu];   //[nMu]
   Float_t         muPFIsoR04_NH[kMaxnMu];   //[nMu]
   Float_t         muPFIsoR04_Pho[kMaxnMu];   //[nMu]
   Float_t         muPFIsoR04_PU[kMaxnMu];   //[nMu]
   Float_t         muPFIsoR04_CPart[kMaxnMu];   //[nMu]
   Float_t         muPFIsoR04_NHHT[kMaxnMu];   //[nMu]
   Float_t         muPFIsoR04_PhoHT[kMaxnMu];   //[nMu]
   Float_t         muPFIsoR03_CH[kMaxnMu];   //[nMu]
   Float_t         muPFIsoR03_NH[kMaxnMu];   //[nMu]
   Float_t         muPFIsoR03_Pho[kMaxnMu];   //[nMu]
   Float_t         muPFIsoR03_PU[kMaxnMu];   //[nMu]
   Float_t         muPFIsoR03_CPart[kMaxnMu];   //[nMu]
   Float_t         muPFIsoR03_NHHT[kMaxnMu];   //[nMu]
   Float_t         muPFIsoR03_PhoHT[kMaxnMu];   //[nMu]
   Int_t           muType[kMaxnMu];   //[nMu]
   Bool_t          muID[kMaxnMu][6];   //[nMu]
   Float_t         muD0[kMaxnMu];   //[nMu]
   Float_t         muDz[kMaxnMu];   //[nMu]
   Float_t         muD0GV[kMaxnMu];   //[nMu]
   Float_t         muDzGV[kMaxnMu];   //[nMu]
   Float_t         muD0Vtx[kMaxnMu][100];   //[nMu]
   Float_t         muDzVtx[kMaxnMu][100];   //[nMu]
   Float_t         muInnerD0[kMaxnMu];   //[nMu]
   Float_t         muInnerDz[kMaxnMu];   //[nMu]
   Float_t         muInnerD0GV[kMaxnMu];   //[nMu]
   Float_t         muInnerDzGV[kMaxnMu];   //[nMu]
   Int_t           muNumberOfValidTrkLayers[kMaxnMu];   //[nMu]
   Int_t           muNumberOfValidTrkHits[kMaxnMu];   //[nMu]
   Int_t           muNumberOfValidPixelLayers[kMaxnMu];   //[nMu]
   Int_t           muNumberOfValidPixelHits[kMaxnMu];   //[nMu]
   Int_t           muNumberOfValidMuonHits[kMaxnMu];   //[nMu]
   Int_t           muStations[kMaxnMu];   //[nMu]
   Int_t           muChambers[kMaxnMu];   //[nMu]
   Float_t         muIP3D[kMaxnMu];   //[nMu]
   Float_t         muIP3DErr[kMaxnMu];   //[nMu]
   Int_t           nPFPho;
   Float_t         PFPhoEt[kMaxnPFPho];   //[nPFPho_]
   Float_t         PFPhoEta[kMaxnPFPho];   //[nPFPho_]
   Float_t         PFPhoPhi[kMaxnPFPho];   //[nPFPho_]
   Int_t           PFPhoType[kMaxnPFPho];   //[nPFPho_]
   Float_t         PFPhoIso[kMaxnPFPho];   //[nPFPho_]
   Int_t           nPFEle_;
   Float_t         PFElePt_[kMaxnPFEle];   //[nPFEle_]
   Float_t         PFEleEta_[kMaxnPFEle];   //[nPFEle_]
   Float_t         PFElePhi_[kMaxnPFEle];   //[nPFEle_]
   Float_t         PFEleEn_[kMaxnPFEle];   //[nPFEle_]
   Int_t           PFEleCharge[kMaxnPFEle];   //[nPFEle_]
   Float_t         rho25;
   Float_t         rho25_neu;
   Float_t         rho25_muPFiso;
   Float_t         rho25_elePFiso;
   Float_t         rho2011;
   Float_t         rho2012;
   Int_t           nJet;
   Int_t           jetTrg[kMaxnJet][14];   //[nJet]
   Float_t         jetEn[kMaxnJet];   //[nJet]
   Float_t         jetPt[kMaxnJet];   //[nJet]
   Float_t         jetEta[kMaxnJet];   //[nJet]
   Float_t         jetPhi[kMaxnJet];   //[nJet]
   Float_t         jetCharge[kMaxnJet];   //[nJet]
   Float_t         jetEt[kMaxnJet];   //[nJet]
   Float_t         jetRawPt[kMaxnJet];   //[nJet]
   Float_t         jetRawEn[kMaxnJet];   //[nJet]
   Float_t         jetArea[kMaxnJet];   //[nJet]
   Float_t         jetCHF[kMaxnJet];   //[nJet]
   Float_t         jetNHF[kMaxnJet];   //[nJet]
   Float_t         jetCEF[kMaxnJet];   //[nJet]
   Float_t         jetNEF[kMaxnJet];   //[nJet]
   Int_t           jetNCH[kMaxnJet];   //[nJet]
   Float_t         jetHFHAE[kMaxnJet];   //[nJet]
   Float_t         jetHFEME[kMaxnJet];   //[nJet]
   Int_t           jetNConstituents[kMaxnJet];   //[nJet]
   Float_t         jetCombinedSecondaryVtxBJetTags[kMaxnJet];   //[nJet]
   Float_t         jetCombinedSecondaryVtxMVABJetTags[kMaxnJet];   //[nJet]
   Float_t         jetJetProbabilityBJetTags[kMaxnJet];   //[nJet]
   Float_t         jetJetBProbabilityBJetTags[kMaxnJet];   //[nJet]
   Float_t         jetTrackCountingHighPurBJetTags[kMaxnJet];   //[nJet]
   Float_t         jetBetaStar[kMaxnJet][100];   //[nJet]
   Bool_t          jetPFLooseId[kMaxnJet];   //[nJet]
   Float_t         jetDRMean[kMaxnJet];   //[nJet]
   Float_t         jetDR2Mean[kMaxnJet];   //[nJet]
   Float_t         jetDZ[kMaxnJet];   //[nJet]
   Float_t         jetFrac01[kMaxnJet];   //[nJet]
   Float_t         jetFrac02[kMaxnJet];   //[nJet]
   Float_t         jetFrac03[kMaxnJet];   //[nJet]
   Float_t         jetFrac04[kMaxnJet];   //[nJet]
   Float_t         jetFrac05[kMaxnJet];   //[nJet]
   Float_t         jetFrac06[kMaxnJet];   //[nJet]
   Float_t         jetFrac07[kMaxnJet];   //[nJet]
   Float_t         jetBeta[kMaxnJet];   //[nJet]
   Float_t         jetBetaStarCMG[kMaxnJet];   //[nJet]
   Float_t         jetBetaStarClassic[kMaxnJet];   //[nJet]
   Float_t         jetBetaExt[kMaxnJet][100];   //[nJet]
   Float_t         jetBetaStarCMGExt[kMaxnJet][100];   //[nJet]
   Float_t         jetBetaStarClassicExt[kMaxnJet][100];   //[nJet]
   Float_t         jetNNeutrals[kMaxnJet];   //[nJet]
   Float_t         jetNCharged[kMaxnJet];   //[nJet]
   Float_t         jetMVAs[kMaxnJet][4];   //[nJet]
   Int_t           jetWPLevels[kMaxnJet][4];   //[nJet]
   Float_t         jetMVAsExt[kMaxnJet][4][100];   //[nJet]
   Int_t           jetWPLevelsExt[kMaxnJet][4][100];   //[nJet]
//MC specific variables begin
   Int_t           jetPartonID[kMaxnJet];   //[nJet]
   Int_t           jetGenJetIndex[kMaxnJet];   //[nJet]
   Float_t         jetGenJetEn[kMaxnJet];   //[nJet]
   Float_t         jetGenJetPt[kMaxnJet];   //[nJet]
   Float_t         jetGenJetEta[kMaxnJet];   //[nJet]
   Float_t         jetGenJetPhi[kMaxnJet];   //[nJet]
   Int_t           jetGenPartonID[kMaxnJet];   //[nJet]
   Float_t         jetGenEn[kMaxnJet];   //[nJet]
   Float_t         jetGenPt[kMaxnJet];   //[nJet]
   Float_t         jetGenEta[kMaxnJet];   //[nJet]
   Float_t         jetGenPhi[kMaxnJet];   //[nJet]
//MC specific variables end
   Int_t           nLowPtJet;
   Float_t         jetLowPtEn[kMaxnLowPtJet];   //[nLowPtJet]
   Float_t         jetLowPtPt[kMaxnLowPtJet];   //[nLowPtJet]
   Float_t         jetLowPtEta[kMaxnLowPtJet];   //[nLowPtJet]
   Float_t         jetLowPtPhi[kMaxnLowPtJet];   //[nLowPtJet]
   Float_t         jetLowPtCharge[kMaxnLowPtJet];   //[nLowPtJet]
   Float_t         jetLowPtEt[kMaxnLowPtJet];   //[nLowPtJet]
   Float_t         jetLowPtRawPt[kMaxnLowPtJet];   //[nLowPtJet]
   Float_t         jetLowPtRawEn[kMaxnLowPtJet];   //[nLowPtJet]
   Float_t         jetLowPtArea[kMaxnLowPtJet];   //[nLowPtJet]
//MC specific variables begin
   Int_t           jetLowPtPartonID[kMaxnLowPtJet];   //[nLowPtJet]
   Float_t         jetLowPtGenJetEn[kMaxnLowPtJet];   //[nLowPtJet]
   Float_t         jetLowPtGenJetPt[kMaxnLowPtJet];   //[nLowPtJet]
   Float_t         jetLowPtGenJetEta[kMaxnLowPtJet];   //[nLowPtJet]
   Float_t         jetLowPtGenJetPhi[kMaxnLowPtJet];   //[nLowPtJet]
   Int_t           jetLowPtGenPartonID[kMaxnLowPtJet];   //[nLowPtJet]
   Float_t         jetLowPtGenEn[kMaxnLowPtJet];   //[nLowPtJet]
   Float_t         jetLowPtGenPt[kMaxnLowPtJet];   //[nLowPtJet]
   Float_t         jetLowPtGenEta[kMaxnLowPtJet];   //[nLowPtJet]
   Float_t         jetLowPtGenPhi[kMaxnLowPtJet];   //[nLowPtJet]
//MC specific variables end
   Int_t           nConv;
   Float_t         convP4[kMaxnConv][4];   //[nConv]
   Float_t         convVtx[kMaxnConv][3];   //[nConv]
   Float_t         convVtxErr[kMaxnConv][3];   //[nConv]
   Float_t         convPairMomentum[kMaxnConv][3];   //[nConv]
   Float_t         convRefittedMomentum[kMaxnConv][3];   //[nConv]
   Int_t           convNTracks[kMaxnConv];   //[nConv]
   Float_t         convPairInvMass[kMaxnConv];   //[nConv]
   Float_t         convPairCotThetaSep[kMaxnConv];   //[nConv]
   Float_t         convEoverP[kMaxnConv];   //[nConv]
   Float_t         convDistOfMinApproach[kMaxnConv];   //[nConv]
   Float_t         convDPhiTrksAtVtx[kMaxnConv];   //[nConv]
   Float_t         convDPhiTrksAtEcal[kMaxnConv];   //[nConv]
   Float_t         convDEtaTrksAtEcal[kMaxnConv];   //[nConv]
   Float_t         convDxy[kMaxnConv];   //[nConv]
   Float_t         convDz[kMaxnConv];   //[nConv]
   Float_t         convLxy[kMaxnConv];   //[nConv]
   Float_t         convLz[kMaxnConv];   //[nConv]
   Float_t         convZofPrimVtxFromTrks[kMaxnConv];   //[nConv]
   Int_t           convNHitsBeforeVtx[kMaxnConv][2];   //[nConv]
   Int_t           convNSharedHits[kMaxnConv];   //[nConv]
   Int_t           convValidVtx[kMaxnConv];   //[nConv]
   Float_t         convMVALikelihood[kMaxnConv];   //[nConv]
   Float_t         convChi2[kMaxnConv];   //[nConv]
   Float_t         convChi2Probability[kMaxnConv];   //[nConv]
   Float_t         convTk1Dz[kMaxnConv];   //[nConv]
   Float_t         convTk2Dz[kMaxnConv];   //[nConv]
   Float_t         convTk1DzErr[kMaxnConv];   //[nConv]
   Float_t         convTk2DzErr[kMaxnConv];   //[nConv]
   Int_t           convCh1Ch2[kMaxnConv];   //[nConv]
   Float_t         convTk1D0[kMaxnConv];   //[nConv]
   Float_t         convTk1Pout[kMaxnConv];   //[nConv]
   Float_t         convTk1Pin[kMaxnConv];   //[nConv]
   Float_t         convTk2D0[kMaxnConv];   //[nConv]
   Float_t         convTk2Pout[kMaxnConv];   //[nConv]
   Float_t         convTk2Pin[kMaxnConv];   //[nConv]

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
   TBranch        *b_vtx;   //!
   TBranch        *b_IsVtxGood;   //!
   TBranch        *b_nVtxBS;   //!
   TBranch        *b_vtxbs;   //!
   TBranch        *b_pdf;   //!
   TBranch        *b_pthat;   //!
   TBranch        *b_processID;   //!
   TBranch        *b_nMC;   //!
   TBranch        *b_mcPID;   //!
   TBranch        *b_mcVtx;   //!
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
   TBranch        *b_genMET;   //!
   TBranch        *b_genMETPhi;   //!
   TBranch        *b_nPUInfo;   //!
   TBranch        *b_nPU;   //!
   TBranch        *b_puBX;   //!
   TBranch        *b_puTrue;   //!
   TBranch        *b_MET;   //!
   TBranch        *b_METPhi;   //!
   TBranch        *b_METsumEt;   //!
   TBranch        *b_pfMET;   //!
   TBranch        *b_pfMETPhi;   //!
   TBranch        *b_pfMETsumEt;   //!
   TBranch        *b_pfMETmEtSig;   //!
   TBranch        *b_pfMETSig;   //!
   TBranch        *b_recoPfMET;   //!
   TBranch        *b_recoPfMETPhi;   //!
   TBranch        *b_recoPfMETsumEt;   //!
   TBranch        *b_recoPfMETmEtSig;   //!
   TBranch        *b_recoPfMETSig;   //!
   TBranch        *b_metFilters;   //!
   TBranch        *b_nEle;   //!
   TBranch        *b_eleTrg;   //!
   TBranch        *b_eleClass;   //!
   TBranch        *b_eleIsEcalDriven;   //!
   TBranch        *b_eleCharge;   //!
   TBranch        *b_eleEn;   //!
   TBranch        *b_eleEcalEn;   //!
   TBranch        *b_eleSCRawEn;   //!
   TBranch        *b_eleSCEn;   //!
   TBranch        *b_eleESEn;   //!
   TBranch        *b_elePt;   //!
   TBranch        *b_eleEta;   //!
   TBranch        *b_elePhi;   //!
   TBranch        *b_eleEtaVtx;   //!
   TBranch        *b_elePhiVtx;   //!
   TBranch        *b_eleEtVtx;   //!
   TBranch        *b_eleSCEta;   //!
   TBranch        *b_eleSCPhi;   //!
   TBranch        *b_eleSCEtaWidth;   //!
   TBranch        *b_eleSCPhiWidth;   //!
   TBranch        *b_eleVtx;   //!
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
   TBranch        *b_eleE1x5;   //!
   TBranch        *b_eleE3x3;   //!
   TBranch        *b_eleE5x5;   //!
   TBranch        *b_eleE2x5Max;   //!
   TBranch        *b_eleE2x5Right;   //!
   TBranch        *b_eleE2x5Left;   //!
   TBranch        *b_eleE2x5Top;   //!
   TBranch        *b_eleE2x5Bottom;   //!
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
   TBranch        *b_eleMissHits;   //!
   TBranch        *b_eleConvDist;   //!
   TBranch        *b_eleConvDcot;   //!
   TBranch        *b_eleConvVtxFit;   //!
   TBranch        *b_eleIP3D;   //!
   TBranch        *b_eleIP3DErr;   //!
   TBranch        *b_eleIDMVANonTrig;   //!
   TBranch        *b_eleIDMVATrig;   //!
   TBranch        *b_eleIDMVATrigIDIso;   //!
   TBranch        *b_elePFChIso03;   //!
   TBranch        *b_elePFPhoIso03;   //!
   TBranch        *b_elePFNeuIso03;   //!
   TBranch        *b_elePFChIso04;   //!
   TBranch        *b_elePFPhoIso04;   //!
   TBranch        *b_elePFNeuIso04;   //!
   TBranch        *b_eleESEffSigmaRR;   //!
   TBranch        *b_nPho;   //!
   TBranch        *b_phoTrg;   //!
   TBranch        *b_phoTrgFilter;   //!
   TBranch        *b_phoIsPhoton;   //!
   TBranch        *b_phoSCPos;   //!
   TBranch        *b_phoCaloPos;   //!
   TBranch        *b_phoE;   //!
   TBranch        *b_phoEt;   //!
   TBranch        *b_phoEta;   //!
   TBranch        *b_phoVtx;   //!
   TBranch        *b_phoPhi;   //!
   TBranch        *b_phoEtVtx;   //!
   TBranch        *b_phoEtaVtx;   //!
   TBranch        *b_phoPhiVtx;   //!
   TBranch        *b_phoR9;   //!
   TBranch        *b_phoTrkIsoHollowDR03;   //!
   TBranch        *b_phoEcalIsoDR03;   //!
   TBranch        *b_phoHcalIsoDR03;   //!
   TBranch        *b_phoHcalIsoDR0312;   //!
   TBranch        *b_phoTrkIsoHollowDR04;   //!
   TBranch        *b_phoCiCTrkIsoDR03;   //!
   TBranch        *b_phoCiCTrkIsoDR04;   //!
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
   TBranch        *b_phoEtop;   //!
   TBranch        *b_phoEbottom;   //!
   TBranch        *b_phoEleft;   //!
   TBranch        *b_phoEright;   //!
   TBranch        *b_phoE3x3;   //!
   TBranch        *b_phoE3x1;   //!
   TBranch        *b_phoE1x3;   //!
   TBranch        *b_phoE5x5;   //!
   TBranch        *b_phoE1x5;   //!
   TBranch        *b_phoE2x2;   //!
   TBranch        *b_phoE2x5Max;   //!
   TBranch        *b_phoE2x5Right;   //!
   TBranch        *b_phoE2x5Left;   //!
   TBranch        *b_phoE2x5Top;   //!
   TBranch        *b_phoE2x5Bottom;   //!
   TBranch        *b_phoPFChIso;   //!
   TBranch        *b_phoPFPhoIso;   //!
   TBranch        *b_phoPFNeuIso;   //!
   TBranch        *b_phoSCRChIso;   //!
   TBranch        *b_phoSCRPhoIso;   //!
   TBranch        *b_phoSCRNeuIso;   //!
   TBranch        *b_phoRegrE;   //!
   TBranch        *b_phoRegrEerr;   //!
   TBranch        *b_phoSeedTime;   //!
   TBranch        *b_phoSeedDetId1;   //!
   TBranch        *b_phoSeedDetId2;   //!
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
   TBranch        *b_pho_pfSLConvPos;   //!
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
   TBranch        *b_phoConvTrkd0;   //!
   TBranch        *b_phoConvTrkPin;   //!
   TBranch        *b_phoConvTrkPout;   //!
   TBranch        *b_phoConvTrkdz;   //!
   TBranch        *b_phoConvTrkdzErr;   //!
   TBranch        *b_phoConvChi2;   //!
   TBranch        *b_phoConvChi2Prob;   //!
   TBranch        *b_phoConvNTrks;   //!
   TBranch        *b_phoConvCharge;   //!
   TBranch        *b_phoConvValidVtx;   //!
   TBranch        *b_phoConvLikeLihood;   //!
   TBranch        *b_phoConvP4;   //!
   TBranch        *b_phoConvVtx;   //!
   TBranch        *b_phoConvVtxErr;   //!
   TBranch        *b_phoConvPairMomentum;   //!
   TBranch        *b_phoConvRefittedMomentum;   //!
   TBranch        *b_SingleLegConv;   //!
   TBranch        *b_phoPFConvVtx;   //!
   TBranch        *b_phoPFConvMom;   //!
   TBranch        *b_phoESEffSigmaRR;   //!
   TBranch        *b_nMu;   //!
   TBranch        *b_muTrg;   //!
   TBranch        *b_muEta;   //!
   TBranch        *b_muPhi;   //!
   TBranch        *b_muCharge;   //!
   TBranch        *b_muPt;   //!
   TBranch        *b_muPz;   //!
   TBranch        *b_muVtx;   //!
   TBranch        *b_muVtxGlb;   //!
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
   TBranch        *b_muID;   //!
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
   TBranch        *b_muNumberOfValidTrkLayers;   //!
   TBranch        *b_muNumberOfValidTrkHits;   //!
   TBranch        *b_muNumberOfValidPixelLayers;   //!
   TBranch        *b_muNumberOfValidPixelHits;   //!
   TBranch        *b_muNumberOfValidMuonHits;   //!
   TBranch        *b_muStations;   //!
   TBranch        *b_muChambers;   //!
   TBranch        *b_muIP3D;   //!
   TBranch        *b_muIP3DErr;   //!
   TBranch        *b_nPFPho_;   //!
   TBranch        *b_PFPhoEt;   //!
   TBranch        *b_PFPhoEta;   //!
   TBranch        *b_PFPhoPhi;   //!
   TBranch        *b_PFPhoType;   //!
   TBranch        *b_PFPhoIso;   //!
   TBranch        *b_nPFEle_;   //!
   TBranch        *b_PFElePt_;   //!
   TBranch        *b_PFEleEta_;   //!
   TBranch        *b_PFElePhi_;   //!
   TBranch        *b_PFEleEn_;   //!
   TBranch        *b_PFEleCharge;   //!
   TBranch        *b_rho25;   //!
   TBranch        *b_rho25_neu;   //!
   TBranch        *b_rho25_muPFiso;   //!
   TBranch        *b_rho25_elePFiso;   //!
   TBranch        *b_rho2011;   //!
   TBranch        *b_rho2012;   //!
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
   TBranch        *b_jetTrackCountingHighPurBJetTags;   //!
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
   TBranch        *b_jetMVAsExt;   //!
   TBranch        *b_jetWPLevelsExt;   //!
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
   TBranch        *b_nLowPtJet;   //!
   TBranch        *b_jetLowPtEn;   //!
   TBranch        *b_jetLowPtPt;   //!
   TBranch        *b_jetLowPtEta;   //!
   TBranch        *b_jetLowPtPhi;   //!
   TBranch        *b_jetLowPtCharge;   //!
   TBranch        *b_jetLowPtEt;   //!
   TBranch        *b_jetLowPtRawPt;   //!
   TBranch        *b_jetLowPtRawEn;   //!
   TBranch        *b_jetLowPtArea;   //!
   TBranch        *b_jetLowPtPartonID;   //!
   TBranch        *b_jetLowPtGenJetEn;   //!
   TBranch        *b_jetLowPtGenJetPt;   //!
   TBranch        *b_jetLowPtGenJetEta;   //!
   TBranch        *b_jetLowPtGenJetPhi;   //!
   TBranch        *b_jetLowPtGenPartonID;   //!
   TBranch        *b_jetLowPtGenEn;   //!
   TBranch        *b_jetLowPtGenPt;   //!
   TBranch        *b_jetLowPtGenEta;   //!
   TBranch        *b_jetLowPtGenPhi;   //!
   TBranch        *b_nConv;   //!
   TBranch        *b_convP4;   //!
   TBranch        *b_convVtx;   //!
   TBranch        *b_convVtxErr;   //!
   TBranch        *b_convPairMomentum;   //!
   TBranch        *b_convRefittedMomentum;   //!
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
   TBranch        *b_convNHitsBeforeVtx;   //!
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
   void GetEntryNeededBranchesOnly(Long64_t entry);
   void GetEntryMCSpecific(Long64_t entry);
   virtual void     Init(TTree *tree);
//   void SetMCSpecificAddresses();
};

#endif //#ifndef TEventTree_h
