{
//  TFile f1=("../WGammaOutput/MUON_WGamma/FullySelected/selected_WGammaSIGMC.root");
//  TFile f2=("../WGammaOutput/MUON_WGamma/FullySelected/selected_WGamma_UNblind_DATA.root");
//  TFile f3=("../WGammaOutput/MUON_WGamma/FullySelected/selected_WGamma_blindPRESCALE_DATA.root");
//  TFile f4=("../WGammaOutput/MUON_WGamma/FullySelected/selected_WGamma_blindCOMBINED_DATA.root");
  TFile f1=("../WGammaOutput/ELECTRON_WGamma/FullySelected/selected_WGammaSIGMC.root");
  TFile f2=("../WGammaOutput/ELECTRON_WGamma/FullySelected/selected_WGamma_UNblind_DATA.root");
  TFile f3=("../WGammaOutput/ELECTRON_WGamma/FullySelected/selected_WGamma_blindPRESCALE_DATA.root");
  TFile f4=("../WGammaOutput/ELECTRON_WGamma/FullySelected/selected_WGamma_blindCOMBINED_DATA.root");
  
  TTree* tr1 = (TTree*)f1->Get("selectedEvents");
  TTree* tr2 = (TTree*)f2->Get("selectedEvents");
  TTree* tr3 = (TTree*)f3->Get("selectedEvents");
  TTree* tr4 = (TTree*)f4->Get("selectedEvents");

  std::cout<<"SIGMC: "<<tr1->GetEntries()<<std::endl;
  std::cout<<"UNblind; blindPRESCALE; blindCOMBINED"<<std::endl;
  std::cout<<"pt<45: "<<tr2->GetEntries("phoEt>15 && phoEt<45")<<", "<<tr3->GetEntries("phoEt>15 && phoEt<45")<<", "<<tr4->GetEntries("phoEt>15 && phoEt<45")<<std::endl;
  std::cout<<"pt>45: "<<tr2->GetEntries("phoEt>45")<<", "<<tr3->GetEntries("phoEt>45")<<", "<<tr4->GetEntries("phoEt>45")<<std::endl;
}
