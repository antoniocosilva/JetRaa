#!/bin/bash
ANALYSIS_DIR=/home/antoniosilva/RivetWorkdir/Jets/Raa
REFERENCE_DIR=/home/antoniosilva/RivetWorkdir/Pythia8/pythia8243/examples
JEWEL_DIR=/home/antoniosilva/Dropbox/2357154.apollo-acf/2359154
rm *.so
export RIVET_ANALYSIS_PATH=$PWD
rivet-buildplugin RivetJet_Raa_Analysis.so Jet_Raa_Analysis.cc
rivet --pwd -a Jet_Raa_Analysis:coll=PP -o Rivet_pp.yoda $REFERENCE_DIR/hepmc_pp_MB_1.hepmc
rivet --pwd -p Rivet_pp.yoda -a Jet_Raa_Analysis:coll=PBPB -o Rivet_PbPb.yoda --ignore-beams $JEWEL_DIR/jewel.hepmc
rivet-mkhtml --pwd Rivet_PbPb.yoda 
