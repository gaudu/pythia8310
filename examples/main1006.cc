// Authors: Marius Utheim <marius.m.utheim@jyu.fi>;
//          Chloé Gaudu <gaudu@uni-wuppertal.de>

// Keyword: analysis; cross-sections; hA collisions

#include "Pythia8/HeavyIons.h"
#include "Pythia8/Pythia.h"

using namespace Pythia8;

int main(int argc, char *argv[]) {
  
  if (argc<=1) {
    std::cout << "argc<=1" << '\n';
    return 1; 
  } 
  
  int nEvents = 10;
  Pythia pythia;
  
  std::map<string, string> idA_map = {{"p",  "2212"},
                                       {"pi", "211"},
                                      {"K",  "321"}};
  std::map<string, float> mA_map = {{"p", pythia.particleData.m0(2212)}, 
        			    {"pi", pythia.particleData.m0(211)}, 
      				    {"K", pythia.particleData.m0(321)}};
  double mB = pythia.particleData.m0(1000080160);
  const double min_value = 1e2;
  const double max_value = 1e11;

  const int num_magnitudes = static_cast<int>(std::log10(max_value) - std::log10(min_value)) + 1;
  double magnitude_step = (max_value - min_value) / num_magnitudes;
  double pz_array[num_magnitudes];
  int k = 0;

  for (int i = 0; i < num_magnitudes; ++i) {
      double magnitude = std::pow(10, std::log10(min_value) + i * magnitude_step);
      int multiplier = (i % 2 == 0) ? 1 : 5;
          pz_array[k++] = magnitude * multiplier;
  }
  std::cout << "pz_array:" << '\n';
  for (int i = 0; i < num_magnitudes; ++i) {
      std::cout << pz_array[i] << '\n';
  }

  double pz = pz_array[std::atoi(argv[1])];
  double eCM = (pow(pz + mA_map[argv[2]], 2) + pow(mB,2));
  double initEnergy = pz_array[atoi(argv[1])];
  string initPrefix = std::string("main1006_")+std::string(argv[2])
                      +std::string("O_")+std::to_string(initEnergy);
  
  // Logfile initialization.
  const bool doLog = true;
  string out = std::string("main1006_")+std::string(argv[2])+std::string("O_")
               +std::to_string(initEnergy)+std::string("_10");     
  ofstream logBuf;
  std::streambuf* oldCout;
  if(doLog) {
    oldCout = std::cout.rdbuf(logBuf.rdbuf());
    logBuf.open((out == "" ? "pythia.log" : out + ".log"));
  }

  pythia.readString(std::string("Beams:idA = ")+idA_map[std::string(argv[2])]);
  pythia.readString("Beams:idB = 1000080160");
  pythia.readString("Beams:frameType = 1");
  pythia.readString("Beams:eCM = "+std::to_string(eCM));
  
  pythia.readString("MultipartonInteractions:reuseInit = 3");
  pythia.readString("HeavyIon:SasdMpiReuseInit = 3");
  pythia.readString("HeavyIon:SigFitReuseInit = 3");
  pythia.readString("MultipartonInteractions:initFile = "+initPrefix+".mpi");
  pythia.readString("HeavyIon:SasdMpiInitFile = "+initPrefix+".sasd.mpi");
  pythia.readString("HeavyIon:SigFitInitFile = "+initPrefix+".sigfit");

  if (!pythia.init()) {
    std::cout << "Pythia failed to initialize." << endl;
    return -1;
  }
  
  for ( int iEvent = 0; iEvent < nEvents; ++iEvent ) { 
    if (!pythia.next()) continue;        
  }
   
  pythia.stat(); 
  
  // Logfile output.
  if (doLog) std::cout.rdbuf(oldCout);

  return 0;
}