#ifndef TAllInputSamples_h
#define TAllInputSamples_h

#include "TInputSample.h"
#include <vector>
#include <string>

class TAllInputSamples
{
  public:
    TAllInputSamples();
    TAllInputSamples(int channel, string configFile);
    virtual ~TAllInputSamples();

    void ReadConfig(int channel, string configFile);
    bool ReadSampleGeneralInfo(int channel, string line);
    void ReadFileSpecificInfo(string line);

    int ChannelNumber (string channelStr);
    int SampleNumber  (string sampleStr);

    void CalcLumiWeights();

    void Print();
    

    int nSources_;
    vector <TInputSample> allInputs_;

};

#endif //#ifndef TAllInputSamples_h
