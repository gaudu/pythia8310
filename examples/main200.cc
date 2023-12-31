// main200.cc is a part of the PYTHIA event generator.
// Copyright (C) 2023 Torbjorn Sjostrand.
// PYTHIA is licenced under the GNU GPL v2 or later, see COPYING for details.
// Please respect the MCnet Guidelines, see GUIDELINES for details.

// Authors: Peter Skands <peter.skands@monash.edu>

// Keywords: Vincia; Dire; electron-positron

// Simple example of the VINCIA (or DIRE) shower model(s), on Z decays at
// LEP I, with some basic event shapes, spectra, and multiplicity counts.
// Also useful as a basic test of the respective final-state showers.

// Include Pythia8 header(s) and namespace.
#include "Pythia8/Pythia.h"
using namespace Pythia8;

// Main Program
int main() {

  //************************************************************************
  // Define Pythia 8 generator

  Pythia pythia;

  // Read user settings from file
  pythia.readFile("main200.cmnd");

  //************************************************************************

  // Shorthands
  Event& event = pythia.event;
  Settings& settings = pythia.settings;

  // Extract settings to be used in the main program.
  int    nEvent     = settings.mode("Main:numberOfEvents");
  int    nAbort     = settings.mode("Main:timesAllowErrors");
  bool   vinciaOn   = settings.mode("PartonShowers:model") == 2;
  bool   helicityOn = vinciaOn && settings.flag("Vincia:helicityShower");
  int    iEventPri  = -1;
  double eCM        = settings.parm("Beams:eCM");

  //************************************************************************

  // Initialize
  if(!pythia.init()) { return EXIT_FAILURE; }

  //************************************************************************

  // Define a few PYTHIA utilities
  Thrust Thr(1);
  Sphericity SphLin(1,2);

  //************************************************************************

  // Define PYTHIA histograms
  // Rootlink: to use ROOT instead, HIST -> TH1F ("tag",...)

  // Number of quarks, partons, and charged particles
  double nQsum      = 0.0;
  double nPsum      = 0.0;
  double nCsum      = 0.0;
  double nStrangeSum = 0.0;
  double nCharmSum  = 0.0;
  double nBottomSum = 0.0;
  double nBaryonSum = 0.0;
  double nGammaSum  = 0.0;
  Hist histNQuarks("nQuarks", 50, -0.5, 49.5);
  Hist histNPartons("nPartons", 100, -0.5, 99.5);
  Hist histNCharged("nCharged", 100, -1.0, 199.0);
  Hist histNGamma("nPhotons", 50, -0.5, 49.5);
  Hist histXStrange("Ln(x) for strange quarks", 25, -5.0, 0.0);
  Hist histXCharm("Ln(x) for charm quarks", 25, -5.0, 0.0);
  Hist histXBottom("Ln(x) for bottom quarks", 25, -5.0, 0.0);
  Hist histMSS("Invariant mass of s-sbar pairs",50,0.,10.);
  Hist histMCC("Invariant mass of c-cbar pairs",50,0.,25.);
  Hist histMBB("Invariant mass of b-bbar pairs",50,0.,50.);
  Hist histX1("Ln(x) for 1st branching (QCD)",100,-7.5,0.0);
  Hist histX1Gamma("Ln(x) for 1st branching (QED)",100,-7.5,0.0);
  Hist histX2Gluon("Ln(x) for 2nd branching (gluons)",100,-7.5,0.0);
  Hist histX2Quark("Ln(x) for 2nd branching (quarks)",100,-7.5,0.0);

  // Thrust, C, and D parameters
  int nBinsShapes=100;
  Hist histT("1-T",nBinsShapes, 0.0, 0.5);
  Hist histC("C",nBinsShapes,0.0,1.0);
  Hist histD("D",nBinsShapes,0.0,1.0);
  double wHistT=nBinsShapes/0.5;
  double wHistCD=nBinsShapes/1.0;

  //************************************************************************

  // EVENT GENERATION LOOP.
  // Generation, event-by-event printout, analysis, and histogramming.

  // Counter for negative-weight events
  double weight=1.0;
  double sumWeights = 0.0;

  // Begin event loop
  int iAbort = 0;
  for (int iEvent = 0; iEvent < nEvent; ++iEvent) {

    bool aborted = !pythia.next();
    if(aborted){
      event.list();
      if (++iAbort < nAbort){
        continue;
      }
      cout << " Event generation aborted prematurely, owing to error!\n";
      cout<< "Event number was : "<<iEvent<<endl;
      break;
    }

    // Check for weights
    weight = pythia.info.weight();
    sumWeights += weight;

    // Print event with helicities
    if (iEvent == 0 || iEvent == iEventPri)
      if (helicityOn) event.list(helicityOn);

    // Count FS charged hadrons, partons, and quarks
    double nc=0;
    int np=0;
    int nq=0;
    int nGam=0;
    int nStrange=0;
    int nCharm  =0;
    int nBottom =0;
    int nStrangeBorn = (event[6].idAbs() == 3) ? 2 : 0;
    int nCharmBorn   = (event[6].idAbs() == 4) ? 2 : 0;
    int nBottomBorn  = (event[6].idAbs() == 5) ? 2 : 0;
    vector<int> iStrange;
    vector<int> iCharm;
    vector<int> iBottom;
    for (int i=1;i<event.size();i++) {
      // Plot x distributions of first two emissions
      if (i == 9) {
        if (event[i].id() != 22)
          histX1.fill(log(2*event[i].pAbs()/eCM),weight);
        else
          histX1Gamma.fill(log(2*event[i].pAbs()/eCM),weight);
      }
      if (i == 12) {
        if (event[i].id() == 21)
          histX2Gluon.fill(log(2*event[i].pAbs()/eCM),weight);
        else if (event[i].idAbs() < 10)
          histX2Quark.fill(log(2*event[i].pAbs()/eCM),weight);
      }
      // Count up final-state charged hadrons
      if (event[i].isCharged() && event[i].status() > 80) nc++;
      // Find last parton-level partons
      int iDau1 = event[i].daughter1();
      if (iDau1 == 0 || abs(event[iDau1].status()) > 80) {
        // Count up partons and quarks
        if (event[i].isQuark() || event[i].isGluon()) np++;
        if (event[i].id() == 22) nGam++;
        if (event[i].isQuark()) nq++;
        if (event[i].idAbs() == 3) {
          nStrange++;
          histXStrange.fill(log(2*event[i].pAbs()/eCM),weight);
          for (int j=0; j<(int)iStrange.size(); ++j) {
            double m2qq = m2(event[i],event[iStrange[j]]);
            histMSS.fill(sqrt(m2qq),weight);
          }
          iStrange.push_back(i);
        }
        if (event[i].idAbs() == 4) {
          nCharm++;
          histXCharm.fill(log(2*event[i].pAbs()/eCM),weight);
          for (int j=0; j<(int)iCharm.size(); ++j) {
            double m2qq = m2(event[i],event[iCharm[j]]);
            histMCC.fill(sqrt(m2qq),weight);
          }
          iCharm.push_back(i);
        }
        if (event[i].idAbs() == 5) {
          nBottom++;
          histXBottom.fill(log(2*event[i].pAbs()/eCM),weight);
          for (int j=0; j<(int)iBottom.size(); ++j) {
            double m2qq = m2(event[i],event[iBottom[j]]);
            histMBB.fill(sqrt(m2qq),weight);
          }
          iBottom.push_back(i);
        }
      }
      if (event[i].isHadron() && event[i].isFinal()) {
        int idAbs = abs(event[i].id());
        if (idAbs > 1000 && idAbs < 10000) nBaryonSum++;
      }
    }
    histNQuarks.fill(0.5*nq,weight);
    histNPartons.fill(1.0*np,weight);
    histNCharged.fill(1.0*nc,weight);
    histNGamma.fill(1.0*nGam,weight);
    nQsum += 0.5*nq * weight;
    nPsum += np * weight;
    nCsum += nc * weight;
    nStrangeSum += (nStrange - nStrangeBorn) * weight;
    nCharmSum   += (nCharm - nCharmBorn) * weight;
    nBottomSum  += (nBottom - nBottomBorn) * weight;
    nGammaSum   += nGam * weight;

    // Histogram thrust
    Thr.analyze( event );
    histT.fill(1.0-Thr.thrust(),wHistT*weight);

    // Histogram Linear Sphericity values
    if (np >= 2.0) {
      SphLin.analyze( event );
      double evC=3*(SphLin.eigenValue(1)*SphLin.eigenValue(2)
                    + SphLin.eigenValue(2)*SphLin.eigenValue(3)
                    + SphLin.eigenValue(3)*SphLin.eigenValue(1));
      double evD=27*SphLin.eigenValue(1)*SphLin.eigenValue(2)
      *SphLin.eigenValue(3);
      histC.fill(evC,wHistCD*weight);
      histD.fill(evD,wHistCD*weight);
    }

  }

  //************************************************************************

  // POST-RUN FINALIZATION
  // Normalization, Statistics, Output.

  //Normalize histograms to effective number of positive-weight events.
  double normFac=1.0/sumWeights;
  histT          *= normFac;
  histC          *= normFac;
  histD          *= normFac;
  histNQuarks    *= normFac;
  histNPartons   *= normFac;
  histNCharged   *= normFac;
  histNGamma     *= normFac;
  histXStrange   *= normFac;
  histXCharm     *= normFac;
  histXBottom    *= normFac;
  histX1         *= normFac;
  histX1Gamma    *= normFac;
  histX2Gluon    *= normFac;
  histX2Quark    *= normFac;

  // Print a few histograms.
  cout<<histNPartons<<endl;
  if (nGammaSum > 0) cout<<histNGamma<<endl;
  cout<<histNCharged<<endl;
  cout<<histT<<endl;
  cout<<histC<<endl;
  cout<<histD<<endl;
  cout<<histX1<<endl;
  cout<<histX1Gamma<<endl;
  cout<<histX2Gluon<<endl;
  cout<<histX2Quark<<endl;
  cout<<histXStrange<<endl;
  cout<<histXCharm<<endl;
  cout<<histXBottom<<endl;
  cout<<histMSS<<endl;
  cout<<histMCC<<endl;
  cout<<histMBB<<endl;

  // Print out end-of-run information.
  pythia.stat();

  cout<<endl;
  cout<<fixed;
  cout<< " <nGluonSplit> = "<<num2str(nQsum * normFac-1.0)<<endl;
  cout<< " <nG->SS>      = "<<num2str(nStrangeSum * normFac / 2.)<<endl;
  cout<< " <nG->CC>      = "<<num2str(nCharmSum * normFac / 2.)<<endl;
  cout<< " <nG->BB>      = "<<num2str(nBottomSum * normFac / 2.)<<endl;
  cout<< " <nPartons>    = "<<num2str(nPsum * normFac)<<endl;
  cout<< " <nPhotons>    = "<<num2str(nGammaSum * normFac)<<endl;
  cout<< " <nCharged>    = "<<num2str(nCsum * normFac)<<endl;
  cout<< " <nBaryons>    = "<<num2str(nBaryonSum * normFac)<<endl;
  cout<<endl;

  // Done.
  return 0;
}
