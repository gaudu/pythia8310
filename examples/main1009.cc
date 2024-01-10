// Authors: Marius Utheim <marius.m.utheim@jyu.fi>;
//          Chloé Gaudu <gaudu@uni-wuppertal.de>

// Keyword: analysis; rivet; hA collisions; tuning; apprentice

// Usage: ./main1009 TuneFolderNumber parA parB parC
// Can be run using ./apprentice_script

#include "Pythia8/HeavyIons.h"
#include "Pythia8/Pythia.h"
#include "Pythia8Plugins/Pythia8Rivet.h"

using namespace Pythia8;

int main(int argc, char *argv[]) {

  if (argc<=1) {
    std::cout << "argc<=1" << '\n';
    if (argc==7) {
      std::cout << "argc==7" << '\n'; 
    }
    return 1; 
  }

  int nEvents = 10000;

  // Logfile initialization
  const bool doLog = true;
  string out = "main1009_tune2_"+std::string(argv[1])+"_"+std::string(argv[2])+"_"+std::to_string(nEvents);
  ofstream logBuf;
  std::streambuf* oldCout;
  if(doLog) {
    oldCout = cout.rdbuf(logBuf.rdbuf());
    logBuf.open((out == "" ? "pythia.log" : out + ".log"));
  }
 
  // Pythia configuration
  Pythia pythia; 
  pythia.readString("Beams:idA = -211");
  pythia.readString("Beams:idB = 1000060120");
  pythia.readString("Beams:frameType = 2");
  pythia.readString("Beams:eA = "+std::string(argv[1]));
  // Use proton mass as a bug fix to initialize Angantyr (carbon mass/12 does not work)
  pythia.readString("Beams:eB = "+std::to_string(pythia.particleData.m0(2212)));
  // Tune parameters read from apprentice/template.dat in each tune folder
  // MultipartonInteractions:pT0Ref, StringZ:aLund, StringZ:bLund
  // BeamRemnants:dampPopcorn, StringFlav:popcornRate
  for (int i=3; i<8; ++i) {
    pythia.settings.readString(std::string(argv[i]));
    std::cout <<"Check for (i=2; i<5; i++): i = " << i << ", argv[i] = " << argv[i] << '\n';
  }

  if (!pythia.init()) {
    cout << "Pythia failed to initialize." << endl;
    return -1;
  }

  // Rivet initialization
  Pythia8Rivet rivet(pythia, out + ".yoda");
  rivet.ignoreBeams(true);
  rivet.addAnalysis("NA61SHINE_2022_I2155140");

  // Loop over events
  for ( int iEvent = 0; iEvent < nEvents; ++iEvent ) { 
    if (!pythia.next()) continue;
    rivet();
  }
  rivet.done();
  pythia.stat();
  
  // Logfile output
  if (doLog) cout.rdbuf(oldCout);

  return 0;

}