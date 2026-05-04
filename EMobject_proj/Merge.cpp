#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TSystem.h>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

void CombineROOTFiles(const string& eosDirectory, const string& outputFileName, const string& treeName) {
    // Create a TChain to hold all the TTrees
    TFile outputFile(outputFileName.c_str(), "RECREATE");

    TChain chain(treeName.c_str());
    
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Command to list files in EOS directory
    string command = "eosls " + eosDirectory;
    FILE* pipe = gSystem->OpenPipe(command.c_str(), "r");
    if (!pipe) {
        cerr << "Failed to list files in EOS directory: " << eosDirectory << endl;
        return;
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Read the output of the command
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), pipe)) {
        string filePath = buffer;
        filePath.erase(filePath.find_last_not_of("\n") + 1); // Remove trailing newline

        // Construct the full xrootd path and add it to the TChain
        string fullPath = "root://cmsxrootd.fnal.gov/" + eosDirectory + "/" + filePath;
        if (chain.Add(fullPath.c_str())) {
            cout << "Chained file: " << filePath << endl;
        } else {
            cerr << "Failed to add file: " << filePath << endl;
        }
    }

    gSystem->ClosePipe(pipe);
    
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Check if the chain has entries
    if (chain.GetEntries() == 0) {
        cerr << "No entries found in the chain. Exiting." << endl;
        return;
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
    // Create a new ROOT file to save the combined TTree
    

    // Copy the chain into a new TTree in the output file
    TTree* combinedTree = chain.CloneTree();
    if (!combinedTree) {
        cerr << "Failed to clone the TChain into a TTree." << endl;
        return;
    }

    combinedTree->Write();
    outputFile.Close();

    cout << "Successfully combined all TTrees into: " << outputFileName << endl;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main() {
    cout << "Begin The Merging" << endl;
    // Set EOS directory containing ROOT files
    string eosDirectory = "/store/user/cgillesp/2016_Reduced_Dataset";

    // Set output file name
    string outputFileName = "CombinedTree_MC.root";

    // Set the name of the TTree to combine
    string treeName = "Events"; 

    // Combine the ROOT files
    CombineROOTFiles(eosDirectory, outputFileName, treeName);

    return 0;
}

