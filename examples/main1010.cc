// Authors: Chloé Gaudu <gaudu@uni-wuppertal.de>

// Keyword: analysis; hA collisions; cross-section

// Usage: ./main1010 Beams:idA Beams:idB Beams:eA Beams:eB
// can be run using ./run_pleiades_xsec_script on Pleiades
//                  ./pleiades_xsec_script locally

#include "Pythia8/HeavyIons.h"
#include "Pythia8/Pythia.h"
#include <cmath>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>

template <typename K, typename V>
void printMap(const std::map<K, V>& m) {
    for (const auto& pair : m) {
        std::cout << "Key: " << pair.first << ", Value: " << pair.second << std::endl;
    }
}
template <typename K, typename V>
std::vector<K> getMapKeys(const std::map<K, V>& inputMap) {
    std::vector<K> keys;
    for (const auto& pair : inputMap) {
        keys.push_back(pair.first);
    }
    return keys;
}

using namespace Pythia8;

int main(int argc, char *argv[]) {

  if (argc<=1) {
    std::cout << "argc<=1" << '\n';
    if (argc==5) {
      std::cout << "argc==5" << '\n'; 
    }
    return 1; 
  }

  int nEvents = 1e5;
  std::map<std::string, std::string> idA_map = {
    {"2212", "p"}, {"-2212", "ap"}, {"2112", "n"}, {"-2112", "an"},
    {"111", "pi0"}, {"211", "pip"}, {"-211", "pim"},
    {"311", "k0"}, {"321", "kp"}, {"-321", "km"}, {"130", "k0l"}, {"310", "k0s"},
    {"3122", "lambda0"}, {"3212", "sigma0"}, {"3222", "sigmap"}, {"3112", "sigmam"}, {"3322", "xi0"}
    };
  std::map<std::string, std::string> idB_map = {
    {"2212", "1H"}, {"1000060120", "12C"}, {"1000070140", "14N"}, {"1000080160", "16O"}, {"1000180400", "40Ar"}
    };
  std::map<int, std::string> subprocess_map = {
    {101, "nd"}, {102, "el"}, {103, "XB"}, {104, "AX"}, {105, "XX"}, {106, "AXB"}
    };

  // logfile initialization
  const bool doLog = true;
  std::string out = "main1010_"+idA_map[std::string(argv[1])]+"_"+idB_map[std::string(argv[2])]+"_1e"+std::string(argv[3])+"_"+std::to_string(nEvents);
  ofstream logBuf;
  std::streambuf* oldCout;
  if(doLog) {
    oldCout = std::cout.rdbuf(logBuf.rdbuf());
    logBuf.open((out == "" ? "pythia.log" : out + ".log"));
  }
 
  // Pythia configuration
  Pythia pythia; 
  pythia.readString("Beams:idA = " + std::string(argv[1]));
  pythia.readString("Beams:idB = " + std::string(argv[2]));

  pythia.readString("Beams:frameType = 2");
  string eA = std::to_string(pow(10, std::stoi(argv[3])));
  pythia.readString("Beams:eA = " + eA);
  // "the beam energy of a nucleus is specified by giving the energy per nucleon"
  // use proton mass as a bug fix to initialize Angantyr (carbon mass/12 does not work)
  // does it always do m(proton)*Z for every Beams:idB? (H,C,N,O,Ar)
  pythia.readString("Beams:eB =" + std::to_string(pythia.particleData.m0(2212)));
  //
  // parm  Beams:eB   (default = 7000.; minimum = 0.)
  // The energy of the second incoming particle, moving in the -z direction, to be set if Beams:frameType = 2.
  // If the particle energy is smaller than its mass it is assumed to be at rest. 
  //pythia.readString("Beams:eB = 0.");

  //pythia.readString("SoftQCD:all = on");
  // use Angantyr for minimum-bias pp collisions
  pythia.readString("HeavyIon:mode = 2");

  if (!pythia.init()) {
    std::cout << "Pythia failed to initialize." << std::endl;
    return -1;
  }

  std::ofstream xz{out + ".dat"};

  // loop over events
  for ( int iEvent = 0; iEvent < nEvents; ++iEvent ) { 
    if (!pythia.next()) continue;
  }

  /*std::cout << "Contents of sigGenM:" << std::endl;
  printMap(pythia.info.sigGenM);
  std::cout << "Contents of sigErrM:" << std::endl;
  printMap(pythia.info.sigErrM);*/
  
  std::vector<int> sigGenMKeys = getMapKeys(pythia.info.sigGenM);
  //std::vector<int> sigErrMKeys = getMapKeys(pythia.info.sigErrM);
  /*for (const auto& key : sigGenMKeys) {
  	std::cout << "sigGenMKeys" << std::endl;
        std::cout << key << std::endl;
    } 
  for (const auto& key : sigErrMKeys) {
  	std::cout << "sigErrMKeys" << std::endl;
        std::cout << key << std::endl;
    }*/

  pythia.stat();

  if (!xz.is_open()) {
    std::cerr << "Error: failed to open file for writing." << std::endl;
    return 1; 
  }

  xz << "elab" << '\t' << "1e"+std::string(argv[3]) << '\n';
  xz << "proj_id" << '\t' << argv[1] << '\n';
  xz << "targ_id" << '\t' << argv[2] << '\n';
  xz << "sig_tot" << '\t' << pythia.info.sigmaGen(0) << '\n';
  xz << "err_tot" << '\t' << pythia.info.sigmaErr(0) << '\n';
  for (const auto& key : sigGenMKeys) {
        std::string label_sig = "sig_" + subprocess_map[key];
        std::string label_err = "err_" + subprocess_map[key];
        xz << label_sig << '\t' << pythia.info.sigmaGen(key) << '\n';
        xz << label_err << '\t' << pythia.info.sigmaGen(key) << '\n';
    }
  bool has102 = std::find(sigGenMKeys.begin(), sigGenMKeys.end(), 102) != sigGenMKeys.end();
  if (has102) {
  xz << "sig_inel" << '\t' << pythia.info.sigmaGen(0) - pythia.info.sigmaGen(102) << '\n';
  xz << "err_inel" << '\t' << sqrt(pow(pythia.info.sigmaErr(0),2) + pow(pythia.info.sigmaErr(102),2)) << '\n';
  }

  if (!xz) {
    std::cerr << "Error: failed to write data to the file." << std::endl;
    return 1; 
  }
  
  xz.close();
  std::cout << "Data written to output file successfully." << std::endl;
  
  // logfile output
  if (doLog) std::cout.rdbuf(oldCout);

  return 0;

}