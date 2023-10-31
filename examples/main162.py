# main162.py is a part of the PYTHIA event generator.
# Copyright (C) 2023 Torbjorn Sjostrand.
# PYTHIA is licenced under the GNU GPL v2 or later, see COPYING for details.
# Please respect the MCnet Guidelines, see GUIDELINES for details.

# Authors: Philip Ilten <philten@cern.ch>,
#          Marius Utheim <marius.m.utheim@jyu.fi>.

# Keywords: parallelism; charged multiplicity; python;

# This is a simple test program to illustrate the usage of
# PythiaParallel in Python. The program is equivalent to main01, but in
# parallel. This program contains detailed comments about how the
# code works. For a shorter version that fits on a single slide, see
# main161.

# To set the path to the Pythia 8 Python interface do either
# (in a shell prompt):
#      export PYTHONPATH=$(PREFIX_LIB):$PYTHONPATH
# or the following which sets the path from within Python.
import sys
cfg = open("Makefile.inc")
lib = "../lib"
for line in cfg:
    if line.startswith("PREFIX_LIB="): lib = line[11:-1]; break
sys.path.insert(0, lib)

#==========================================================================

# Use the PythiaParallel class instead of Pythia for parallel generation.
# It will create multiple underlying Pythia instances to do the actual
# generation; one instance per thread.
import pythia8
pythia = pythia8.PythiaParallel()

# PythiaParallel reads settings the same way as the normal Pythia does.
# The settings will be copied for each Pythia instance.
pythia.readString("Beams:eCM = 8000.")
pythia.readString("HardQCD:all = on")
pythia.readString("PhaseSpace:pTHatMin = 20.")
pythia.readString("Main:numberOfEvents = 10000")

# The maximum degree of parallelism. If set to 0 (default), the program
# will use the maximum number of threads supported by the hardware.
pythia.readString("Parallelism:numThreads = 4")

# This tells PythiaParallel to not process events asynchronously
# (default behaviour). In Python, this should never be switched on
# because of the Python global interpreter lock (as is done in
# main163.cc for C++ code).
pythia.readString("Parallelism:processAsync = off")

# This defines the number of events generated by PythiaParallel::run.
pythia.readString("Main:numberOfEvents = 10000")

# Define the histogram to fill.
mult = pythia8.Hist("charged multiplicity", 100, -0.5, 799.5)

# This will create and initialize each underlying Pythia instance.
def init(pythiaNow):
    print("Initializing Pythia with index %i."
          % pythiaNow.settings.mode("Parallelism:index"))
    return pythiaNow.init()
pythia.init(init)

# Generate events. Note, any type of function can be passed to this method,
# as long as the function takes an 'Pythia' object as the argument.
def analyze(pythiaNow):
    mult.fill(pythiaNow.event.nFinal(True))
pythia.run(analyze)

# PythiaParallel::stat combines statistics for each Pythia instance.
pythia.stat()

# Output histogram.
print(mult)
