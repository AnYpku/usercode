RemoveOneChannelOneVgamma(){
  # $1 - MUON or ELECTRON
  # $2 - _WGamma or _ZGamma
#  rm WGammaOutput/$1$2/VeryPreliminarySelected/*.root
  rm WGammaOutput/$1$2/PreliminaryForTemplateMethodSelected/*.root
  rm WGammaOutput/$1$2/FsrSelected/*.root
  rm WGammaOutput/$1$2/FsrExcludedSelected/*.root
  rm WGammaOutput/$1$2/FullySelected/*.root
  rm WGammaOutput/$1$2/YieldsAndBackground/*.root
  rm WGammaOutput/$1$2/Constants/*.root

  for d in WGammaOutput/$1$2/Plots/*
    do
      rm $d/*.png
      rm $d/*.pdf
      rm $d/*.root
    done 
}
#end of RemoveOneChannelOneVgamma()

RemoveOneChannelOneVgamma MUON _WGamma
RemoveOneChannelOneVgamma MUON _ZGamma
RemoveOneChannelOneVgamma ELECTRON _WGamma
RemoveOneChannelOneVgamma ELECTRON _ZGamma
RemoveOneChannelOneVgamma ChannelsMERGED _WGamma
RemoveOneChannelOneVgamma ChannelsMERGED _ZGamma
