// main43.cc is a part of the PYTHIA event generator.
// Copyright (C) 2008 Peter Skands, Torbjorn Sjostrand.
// PYTHIA is licenced under the GNU GPL version 2, see COPYING for details.
// Please respect the MCnet Guidelines, see GUIDELINES for details.

// This is a simple test program. 
// It illustrates how to interface an external process with an incoming photon
// in a hadron beam, using the MRST2004QED PDF set.
// All input apart from the name of the external LHEF file is specified in the 
// main43.cmnd file.

#include "Pythia.h"

using namespace Pythia8; 

int main() {

  // Generator. Shorthand for the event and the (static) Settings.
  Pythia pythia;
  Event& event = pythia.event;
  Settings& settings = pythia.settings;

  // Read in commands from external file.
  pythia.readFile("main43.cmnd");    

  // Extract settings to be used in the main program.
  int nEvent = settings.mode("Main:numberOfEvents");
  int nList  = settings.mode("Main:numberToList");
  int nShow  = settings.mode("Main:timesToShow");
  bool showChangedSettings = settings.flag("Main:showChangedSettings");
  bool showAllSettings = settings.flag("Main:showAllSettings");
  bool showChangedParticleData 
    = settings.flag("Main:showChangedParticleData");
  bool showAllParticleData = settings.flag("Main:showAllParticleData");

  // Initialize. Either of two opions, to be piced in main43.cmnd.
  // 1) Read in external event with incoming photon in the ME,
  // from pre-generated .lhe file (thanks to SANC and R. Sadykov).
  // 2) Use internal fermion gamma -> W+- fermion' process.
  pythia.init();

  // List changed data.
  if (showChangedSettings) settings.listChanged();
  if (showAllSettings) settings.listAll();

  // List particle data.  
  if (showChangedParticleData) ParticleDataTable::listChanged();
  if (showAllParticleData) ParticleDataTable::listAll();

  // Histograms for pT distribution in gluon production vertex.
  Hist pTprim( "pT of photon production, no ISR", 100, 0., 100.);
  Hist pTwith( "pT of photon production, with ISR", 100, 0., 100.);

  // Begin event loop.
  int nPace = max(1,nEvent/nShow); 
  for (int iEvent = 0; iEvent < nEvent; ++iEvent) {
    if (iEvent%nPace == 0) cout << " Now begin event " << iEvent << endl;

    // Generate events. Quit if failure.
    if (!pythia.next()) {
      break;
    }
 
    // List first few events, both hard process and complete events.
    if (iEvent < nList) { 
      pythia.process.list();
      event.list();
    }

    // Analyze event to find branching where photon is produced.
    int iGam = (event[3].id() == 22) ? 3 : 4;
    int iGamMother = iGam;
    for ( ; ; ) {
      iGamMother = event[iGam].mother1();
      if (iGamMother < iGam || event[iGamMother].id() != 22) break;
      iGam = iGamMother;
    }

    // Find and histogram pT in this branching.
    if (iGamMother < iGam) pTprim.fill( event[iGam].pT() );
    else {
      int iQ = iGamMother;
      int size = event.size();
      do ++iQ; 
      while (event[iQ].status() != -43 && iQ < size - 1);
      if (event[iQ].status() == -43) pTwith.fill( event[iQ].pT() );
    }

  // End of event loop.
  }

  // Final statistics and histogram output.
  pythia.statistics();
  cout << pTprim << pTwith;

  return 0;
}
