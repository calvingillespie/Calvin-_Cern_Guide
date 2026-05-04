#!/bin/bash

# Activate the Enviroment
source /cvmfs/cms.cern.ch/cmsset_default.sh
cd CMSSW_14_1_4/src
cmsenv
cd

echo "Path to Home Directory"
pwd

echo "Here Is the input data set"

# Define the directory containing the ROOT files

File_NAME=$2

#B
#BASE_DIR="root://cmsxrootd.fnal.gov//store/data/Run2016B/DoubleEG/NANOAOD/ver1_HIPM_UL2016_MiniAODv2_NanoAODv9-v2/"
#C-F
#BASE_DIR="root://cmsxrootd.fnal.gov//store/data/Run2016F/DoubleEG/NANOAOD/HIPM_UL2016_MiniAODv2_NanoAODv9-v2/"
#G-H
#BASE_DIR="root://cmsxrootd.fnal.gov//store/data/Run2016H/DoubleEG/NANOAOD/UL2016_MiniAODv2_NanoAODv9-v1/"
#MC
BASE_DIR="root://cmsxrootd.fnal.gov//store/mc/RunIISummer20UL16NanoAODv9/DiPhotonJets_MGG-80toInf_TuneCP5_13TeV-amcatnloFXFX-pythia8/NANOAODSIM/106X_mcRun2_asymptotic_v17-v1/"

File_NAME=$(echo "$File_NAME" | sed 's/^param=//')

ROOT_FILE="${BASE_DIR}/${File_NAME}.root"

echo "ROOT_FILE: $ROOT_FILE"

echo "================= Dumping PSet ===================="
#python -c "import PSet; print PSet.process.dumpPython()"


#Run the program
echo "================ Run the File ==============="

root <<EOF
.L Emobject.cpp
load_AFCp("$ROOT_FILE")
.q
EOF

    
echo "All files processed."

ls


