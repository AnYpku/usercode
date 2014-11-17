#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include <iostream>

void CheckMCparentageTree(TTree* tr)
{
  std::cout<<"nentries="<<tr->GetEntries()<<std::endl;
  std::cout<<"true pho (phoGenPID==22) ="<<tr->GetEntries("phoGenPID==22")<<std::endl;
  int allPars=0;
  for (int imcpar=0; imcpar<=30; imcpar++){
    TString str = "phoGenPID==22 && phoGenParentage==";
    str+=imcpar;
    int nEntrPar=tr->GetEntries(str);
    if (nEntrPar){
      std::cout<<imcpar;
      if      (imcpar==0) std::cout<<"(none):                           ";
      else if (imcpar==2) std::cout<<"(qcd):                            ";
      else if (imcpar==6) std::cout<<"(qcd, non-prompt):                ";
      else if (imcpar==10) std::cout<<"(qcd, boson):                    ";
      else if (imcpar==14) std::cout<<"(qcd, non-prompt, boson):        ";
      else if (imcpar==22) std::cout<<"(qcd, non-prompt, lepton):       ";
      else if (imcpar==26) std::cout<<"(qcd, boson, lepton):            ";
      else if (imcpar==30) std::cout<<"(qcd, non-prompt, boson, lepton):";
      else std::cout<<"():";
      std::cout<<nEntrPar<<std::endl;
      allPars+=nEntrPar;
    }
  }
  std::cout<<"all pars="<<allPars<<std::endl;
}

void CheckMCparentageFile(TString strFileName)
{
  TFile f(strFileName);
  TTree* tr=(TTree*)f.Get("selectedEvents");
  strFileName.ReplaceAll("../WGammaOutput/MUON/FullySelected/","");
  strFileName.ReplaceAll("selected_","selection of ");
  strFileName.ReplaceAll(".root","");
  strFileName.ReplaceAll("SIGMC",", SIGMC");
  strFileName.ReplaceAll("BKGMC",", BKGMC");
  std::cout<<strFileName<<": "<<std::endl;
  CheckMCparentageTree(tr);
  std::cout<<std::endl;
}

void CheckMCparentage()
{
  CheckMCparentageFile("../WGammaOutput/MUON/FullySelected/selected_ZGammaSIGMC.root");
  CheckMCparentageFile("../WGammaOutput/MUON/FullySelected/selected_ZGammaBKGMC_DYjets_to_ll.root");
  CheckMCparentageFile("../WGammaOutput/MUON/FullySelected/selected_WGammaSIGMC.root");
  CheckMCparentageFile("../WGammaOutput/MUON/FullySelected/selected_WGammaBKGMC_Wjets_to_lnu.root");
  CheckMCparentageFile("../WGammaOutput/MUON/FullySelected/selected_WGammaBKGMC_Zg.root");
  CheckMCparentageFile("../WGammaOutput/MUON/FullySelected/selected_WGammaBKGMC_DYjets_to_ll.root");
  CheckMCparentageFile("../WGammaOutput/MUON/FullySelected/selected_WGammaBKGMC_multibosons.root");
}
