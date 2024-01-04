// Authors: Marius Utheim <marius.m.utheim@jyu.fi>;
//          Chloé Gaudu <gaudu@uni-wuppertal.de>

// Keyword: analysis; cross-sections; hA collisions

#include "Pythia8/HeavyIons.h"
#include "Pythia8/Pythia.h"

using namespace Pythia8;

int main() {

  int nEvents = 1000;
  constexpr int nCases = 10; 
  vector<string> caseLabels = { "2", "3", "4", "5", "6", "7", "8", "9", "10", "11" };
  
  float sigmaGenval[nCases];
  float sigmaErrval[nCases];
  
  // Logfile initialization.
  const bool doLog = true;  
  string out = "main1004_pO";      
  ofstream logBuf;
  std::streambuf* oldCout;
  if(doLog) {
    oldCout = std::cout.rdbuf(logBuf.rdbuf());
    logBuf.open((out == "" ? "pythia.log" : out + ".log"));
  }
  
  for (int iCase = 0; iCase < nCases; ++iCase) {
  
    string initEnergy = "1e"+caseLabels[iCase]; 
    string initPrefix = "main1004_pO_"+initEnergy;

    Pythia pythia;
    pythia.readString("Beams:idA = 2212");
    pythia.readString("Beams:idB = 1000080160");
    pythia.readString("Beams:frameType = 2");
    pythia.readString("Beams:eA = "+initEnergy);
    pythia.readString("Beams:eB = 0.93827");
    
    pythia.readString("MultipartonInteractions:reuseInit = 3");
    pythia.readString("HeavyIon:SasdMpiReuseInit = 3");
    pythia.readString("HeavyIon:SigFitReuseInit = 3");
    pythia.readString("MultipartonInteractions:initFile = "+initPrefix+".mpi");
    pythia.readString("HeavyIon:SasdMpiInitFile = "+initPrefix+".sasd.mpi");
    pythia.readString("HeavyIon:SigFitInitFile = "+initPrefix+".sigfit");

    if (!pythia.init()) {
      std::cout << "Pythia failed to initialize." << '\n';
      return -1;
    }
    
    for ( int iEvent = 0; iEvent < nEvents; ++iEvent ) { 
      if (!pythia.next()) continue;           
    }
    
    std::cout << "pythia.info.sigmaGen(0): "   << pythia.info.sigmaGen(0) 
              << ", pythia.info.sigmaErr(0): " << pythia.info.sigmaErr(0) << '\n';
         
    sigmaGenval[iCase] = pythia.info.sigmaGen(0);
    sigmaErrval[iCase] = pythia.info.sigmaErr(0);
   
    pythia.stat();
    
  } 
  
  size_t n = sizeof(sigmaGenval)/sizeof(sigmaGenval[0]);
  std::cout << "sigmaGenval: " << '\n';
  for (size_t i = 0; i < n; i++) {
      std::cout << sigmaGenval[i] << ' ';
  }
  
  size_t m = sizeof(sigmaErrval)/sizeof(sigmaErrval[0]);
  std::cout << "\n sigmaErrval: " << '\n'; 
  for (size_t i = 0; i < m; i++) {
      std::cout << sigmaErrval[i] << ' ';
  }
  
  // Logfile output.
  if (doLog) std::cout.rdbuf(oldCout);

  return 0;
}