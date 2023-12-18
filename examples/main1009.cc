// Authors: Marius Utheim <marius.m.utheim@jyu.fi>, Chloé Gaudu <gaudu@uni-wuppertal.de>

// Keyword: analysis; rivet; hA collisions; tuning; apprentice

// Usage: ./main1009 TuneFolderNumber parA parB parC
// TODO write a bash script to get the TuneFolderNumber and parameters from apprentice

#include "Pythia8/HeavyIons.h"
#include "Pythia8/Pythia.h"
#include "Pythia8Plugins/Pythia8Rivet.h"

using namespace Pythia8;

int main(int argc, char *argv[]) {

  if (argc<=1) {
    std::cout << "argc<=1" << '\n';
    return 1; 
  }

  int nEvents = 100; #100000
 
  // Logfile initialization
  const bool doLog = true;
  string out = std::string("main1003_tune2_")+std::string(argv[1])+std:string("_100");
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
  pythia.readString("Beams:eA = 158.");
  pythia.readString("Beams:eB = "+pythia.particleData.m0(1000060120)/12);
  // Tune parameters read from apprentice/template.dat in each tune folder
  // MultipartonInteractions:pT0Ref, StringZ:aLund, StringZ:bLund 
  for (int i=2; i<5 ++i) {
    pythia.settings.readString(std:string(argv[i]));
  }
  // TODO add BeamRemnants:dampPopcorn and StringFlav:popcornRate parameters to template

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