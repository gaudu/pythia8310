// main202.cc is a part of the PYTHIA event generator.
// Copyright (C) 2023 Torbjorn Sjostrand.
// PYTHIA is licenced under the GNU GPL v2 or later, see COPYING for details.
// Please respect the MCnet Guidelines, see GUIDELINES for details.

// Authors: Peter Skands <peter.skands@monash.edu>

// Keywords: Vincia; Dire; top

// This test program is a basic check of Vincia showers for pp > tt at LHC.
// Also illustrates how various components of Vincia can be switched on/off
// in a command file, and measures the run time (eg to compare options
// and/or compare with Pythia).

#include <time.h>
#include "Pythia8/Pythia.h"
using namespace Pythia8;

int main() {

  //************************************************************************
  // Generator.
  Pythia pythia;

  // Read user settings from file
  pythia.readFile("main202.cmnd");

  //************************************************************************

  // Extract settings to be used in the main program.
  // Number of events, generated and listed ones.
  int nEvent         = pythia.settings.mode("Main:numberOfEvents");
  int showerModel    = pythia.settings.mode("PartonShowers:Model");
  bool hadronisation = pythia.settings.flag("HadronLevel:all");
  Event& event       = pythia.event;

  //************************************************************************

  // Initialize
  if(!pythia.init()) { return EXIT_FAILURE; }

  //************************************************************************

  // Histograms
  string modelName = "Pythia";
  if (showerModel == 2) modelName = "Vincia";
  else if (showerModel == 3) modelName = "Dire";
  double scale = 1;
  if (hadronisation) scale = 4;
  Hist hNFinal(modelName + " nFinal",100,-0.5,double(scale*200.-0.5));
  Hist hNGam(modelName + " nPhotons",100,-0.5,double(scale*100.-0.5));
  Hist hNEle(modelName + " nElectrons",100,-0.5,99.5);

  // Measure the cpu runtime.
  clock_t start, stop;
  double t = 0.0;
  start = clock();

  // Begin event loop. Generate event. Abort if error.
  for (int iEvent = 0; iEvent < nEvent; ++iEvent) {

    if (!pythia.next()) {
      cout << " Event generation aborted prematurely, owing to error!\n";
      cout<< "Event number was : "<<iEvent<<endl;
      break;
    }

    // Check for weights
    double weight = pythia.info.weight();

    // Count number of final-state particles.
    // Also count photons and electrons, to test QED.
    double nFinal = 0;
    double nGam   = 0;
    double nEle   = 0;
    for (int i=1; i<event.size(); ++i) {
      if (!event[i].isFinal()) continue;
      nFinal++;
      if (event[i].idAbs() == 22) ++nGam;
      else if (event[i].id() == 11) ++nEle;
    }
    hNFinal.fill(nFinal, weight);
    hNGam.fill(nGam, weight);
    hNEle.fill(nEle, weight);

  }

  // End of event loop. Determine run time.
  stop = clock(); // Stop timer
  t = (double) (stop-start)/CLOCKS_PER_SEC;

  // Statistics. Histograms.
  pythia.stat();
  cout<<hNFinal<<hNGam<<hNEle;

  // Print runtime
  cout << "\n" << "|----------------------------------------|" << endl;
  cout << "| CPU Runtime = " << t << " sec" << endl;
  cout << "|----------------------------------------|" << "\n" << endl;

  // Done.
  return 0;
}
