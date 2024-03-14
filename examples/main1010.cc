// Authors: Chloé Gaudu <gaudu@uni-wuppertal.de>

// Keyword: analysis; hA collisions; cross-section

// Usage: ./main1010 Beams:idA Beams:idB Beams:eA Beams:eB
// can be run using ./run_pleiades_xsec_script on Pleiades
//                  ./xsec_script locally

#include "Pythia8/HeavyIons.h"
#include "Pythia8/Pythia.h"
#include <map>
#include <cmath>
#include <iostream>
#include <fstream>

using namespace Pythia8;

int main(int argc, char *argv[]) {

  if (argc<=1) {
    std::cout << "argc<=1" << '\n';
    if (argc==5) {
      std::cout << "argc==5" << '\n'; 
    }
    return 1; 
  }

  int nEvents = 1000;
  std::map<std::string, std::string> idA_map = {
    {"2212", "p"}, {"-2212", "ap"}, {"2112", "n"}, {"-2112", "an"},
    {"111", "pi0"}, {"211", "pip"}, {"-211", "pim"},
    {"311", "k0"}, {"321", "kp"}, {"-321", "km"}, {"130", "k0l"}, {"310", "k0s"},
    {"3122", "lambda0"}, {"3212", "sigma0"}, {"3222", "sigmap"}, {"3112", "sigmam"}, {"3322", "xi0"}
    };
  std::map<std::string, std::string> idB_map = {
    {"2212", "1H"}, {"1000060120", "12C"}, {"1000070140", "14N"}, {"1000080160", "16O"}, {"1000180400", "40Ar"}
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
  pythia.readString("Beams:eA = 1e" + std::string(argv[3]));
  // "the beam energy of a nucleus is specified by giving the energy per nucleon"
  // use proton mass as a bug fix to initialize Angantyr (carbon mass/12 does not work)
  // does it always do m(proton)*Z for every Beams:idB? (H,C,N,O,Ar)
  pythia.readString("Beams:eB =" + std::to_string(pythia.particleData.m0(2212)));
  //
  // parm  Beams:eB   (default = 7000.; minimum = 0.)
  // The energy of the second incoming particle, moving in the -z direction, to be set if Beams:frameType = 2.
  // If the particle energy is smaller than its mass it is assumed to be at rest. 
  //pythia.readString("Beams:eB = 0.");

  pythia.readString("SoftQCD:all = on");
  // use Angantyr for minimum-bias pp collisions
  pythia.readString("HeavyIon:mode = 2");

  if (!pythia.init()) {
    cout << "Pythia failed to initialize." << endl;
    return -1;
  }

  std::ofstream xz{out + ".dat"};

  // loop over events
  for ( int iEvent = 0; iEvent < nEvents; ++iEvent ) { 
    if (!pythia.next()) continue;
  }

  pythia.stat();

  xz << "elab" << '\t' << argv[3] << '\n';
  xz << "proj_id" << '\t' << argv[1] << '\n';
  xz << "targ_id" << '\t' << argv[2] << '\n';
  xz << "sig_tot" << '\t' << pythia.info.sigmaGen(0) << '\n';
  xz << "err_tot" << '\t' << pythia.info.sigmaErr(0) << '\n';
  xz << "sig_el" << '\t' << pythia.info.sigmaGen(102) << '\n';
  xz << "err_el" << '\t' << pythia.info.sigmaErr(102) << '\n';
  xz << "sig_inel" << '\t' << pythia.info.sigmaGen(0) - pythia.info.sigmaGen(102) << '\n';
  xz << "err_inel" << '\t' << sqrt(pow(pythia.info.sigmaErr(0),2) + pow(pythia.info.sigmaErr(102),2)) << '\n';
  xz << "sig_nd" << '\t' << pythia.info.sigmaGen(101) << '\n';
  xz << "err_nd" << '\t' << pythia.info.sigmaErr(101) << '\n';
  xz << "sig_XB" << '\t' << pythia.info.sigmaGen(103) << '\n';
  xz << "err_XB" << '\t' << pythia.info.sigmaErr(103) << '\n';
  xz << "sig_AX" << '\t' << pythia.info.sigmaGen(104) << '\n';
  xz << "err_AX" << '\t' << pythia.info.sigmaErr(104) << '\n';
  xz << "sig_XX" << '\t' << pythia.info.sigmaGen(105) << '\n';
  xz << "err_XX" << '\t' << pythia.info.sigmaErr(105) << '\n';
  xz << "sig_AXB" << '\t' << pythia.info.sigmaGen(106) << '\n';
  xz << "err_AXB" << '\t' << pythia.info.sigmaErr(106) << '\n';
  
  // logfile output
  if (doLog) std::cout.rdbuf(oldCout);

  return 0;

}