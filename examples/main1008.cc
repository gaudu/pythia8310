// Authors: Marius Utheim <marius.m.utheim@jyu.fi>;
//          Maximilian Reininghaus <maximilian.reininghaus@kit.edu>;
//          Chloé Gaudu <gaudu@uni-wuppertal.de>

// Keyword: analysis; cross-sections; hA collisions; variable beam energy

#include "Pythia8/HeavyIons.h"
#include "Pythia8/Pythia.h"
#include <random>
#include <cmath>
#include <map>

using namespace Pythia8;

int main() {
  
  // Random beam energy generation.
  std::mt19937 rng;
  std::uniform_real_distribution<> dist;

  int nEvents = 100;
  constexpr int nCases = 3; 
  vector<string> caseLabels = { "p", "pi", "K" };
  
  // Logfile initialization.
  const bool doLog = true;  
  string out = std::string("main1008");
  ofstream logBuf;
  std::streambuf* oldCout;
  if(doLog) {
    oldCout = std::cout.rdbuf(logBuf.rdbuf());
    logBuf.open((out == "" ? "pythia.log" : out + ".log"));
  }
  
  for (int iCase = 0; iCase < nCases; ++iCase) {
  
    Pythia pythia;
    
    if (nCases>=1) {
    	std::map<string, string> pid_map = {{"p", "2212" }, {"pi", "211" }, { "K", "321" }};
    	pythia.readString("Beams:idA = "+pid_map[caseLabels[iCase]]);
    }
    pythia.readString("Beams:idB = 1000080160"); // oxygen-16 != proton|neutron
    pythia.readString("Beams:frameType = 1");

    /* When variable energy is enabled, the setKinematics methods can be used to change
    the beam energies. The method used must match the frame type as specified by
    Beams:frameType. There is also a limited support for changing beam particles using
    the setBeamIDs method. Using this method, idA can be changed to any hadron, while idB
    must be a proton or a neutron when using this feature. This is useful for example in
    hadronic cascades in a medium. It is here assumed that only the SoftQCD (and
    LowEnergyQCD if relevant) hadron-nucleon processes are being used. In order to change
    idA, the beam configuration requires additional setup. 
    -> Pythia example not working since idB != proton|neutron */

    // To limit the risks of inadvertent use, you need to initialize with Beams:allowIDAswitch = on.
    pythia.readString("Beams:allowVariableEnergy = on");
    pythia.readString("HeavyIon:varECMMin = 1e1");
    pythia.readString("HeavyIon:varECMMax = 1e12"); 
    // The functionality of MultipartonInteractions:setAntiSame = true is used automatically to limit the number of cases to handle.
    pythia.readString("MultipartonInteractions:setAntiSame = true")
    
    string initPrefix = "main1008_"+caseLabels[iCase]+"O";
    
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
    
    std::ofstream xz{"xsec_"+caseLabels[iCase]+"O.dat"};
    
    int successful{};
    for ( int iEvent = 0; iEvent < nEvents; ++iEvent ) { 
      std::map<string, float> mA_map = {{"p", pythia.particleData.m0(2212)}, 
      					{"pi", pythia.particleData.m0(211)}, 
      					{"K", pythia.particleData.m0(321)}};
      double mB = pythia.particleData.m0(1000080160);
      					
      double const pz = exp(M_LN10 * (dist(rng) * 10 + 1)); // [1e1, 1e11) GeV
      double eCM = (pow(pz + mA_map[caseLabels[iCase]], 2) + pow(mB,2));
      double sCM = eCM * eCM;
      /*double pLab = sqrt((sCM - pow2(mA_map[caseLabels[iCase]] + mB)) 
                    * (sCM - pow2(mA_map[caseLabels[iCase]] - mB)));*/
      pythia.setKinematics(eCM);
      
      if (!pythia.next()) continue;  
      
      double sum = pythia.info.sigmaGen(0);
      double el = 0;
      if (auto hi = pythia.info.hiInfo) {
      	if (hi->nCollEL()>=1) {
      	   cout << "nCollEL()>=1" << '\n';
      	   el = pythia.info.sigmaGen(102);
      	}
      	else {
      	   cout << "nCollEL()<1" << '\n';
      	   el = 0;
      	}
      }       
      double inel = sum - el;
      ++successful;

      //xz << "pLab" << '\t' << "eCM" << '\t' << "inel" << '\n';
      xz << setprecision(4) << scientific << pz << '\t' << eCM << '\t' << inel << '\n';
    
    }
   
    pythia.stat();
    std::cout << "successful events = " << successful << '\n';
    
  } 
  
  // Logfile output.
  if (doLog) std::cout.rdbuf(oldCout);

  return 0;
}