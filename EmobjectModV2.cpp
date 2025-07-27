#include <iostream>
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TTree.h>
#include <cmath>



using namespace std;


const Float_t pi = 3.14159;

// functions

Float_t deltaR (Float_t xA , Float_t yA , Float_t xB , Float_t yB ){
	float_t range;
	Float_t deltaphi = abs(yA-yB);
	
	if(deltaphi > pi){
		deltaphi = 2*pi-deltaphi;
	}
	
	range = sqrt((xA-xB)*(xA-xB)+(deltaphi)*(deltaphi));
	return(range);
}

// Main Function

void load_AFCp(){
	
	const string eosDirectory[] = {
    "/store/data/Run2016C/DoubleEG/NANOAOD/22Aug2018-v1/70000",
    "/store/data/Run2016B/DoubleEG/NANOAOD/22Aug2018_ver2-v1/10000",
    "/store/data/Run2016B/DoubleEG/NANOAOD/22Aug2018_ver2-v1/60000",
    "/store/data/Run2016D/DoubleEG/NANOAOD/22Aug2018-v1/70000",
    "/store/data/Run2016D/DoubleEG/NANOAOD/22Aug2018-v1/80000",
    "/store/data/Run2016E/DoubleEG/NANOAOD/22Aug2018-v1/70000",
    "/store/data/Run2016F/DoubleEG/NANOAOD/22Aug2018-v1/10000",
    "/store/data/Run2016G/DoubleEG/NANOAOD/22Aug2018-v1/100000",
    "/store/data/Run2016G/DoubleEG/NANOAOD/22Aug2018-v1/110000",
    "/store/data/Run2016H/DoubleEG/NANOAOD/22Aug2018-v1/110000",
    "/store/data/Run2016H/DoubleEG/NANOAOD/22Aug2018-v1/60000"
	};
		

	
	// variable declaration
	
	Float_t pTotal;
	Float_t pT_sum;
	Float_t pX_sum;
	Float_t pY_sum;
	Float_t pZ_sum;
	Float_t mass;
	Float_t theta;
	Float_t sysPTsq;
	
	string line;
	int fileCounter = 0; // Counter to create unique tree names
	
	int holder;
	int amount = 1000;
	int swap;
	bool pass;
	int binsize = 100;
	int problem = 0;
	
	TFile *outfile = new TFile("outfileEmObject.root", "RECREATE");
	
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	//open files
	
	/*
	TFile *outfile = new TFile("outfileEmObject.root", "RECREATE");
	
	
	
	//store/data/Run2016D/DoubleEG/NANOAOD/22Aug2018-v1/70000/043B6A52-DFA8-E811-989D-009C02AABEB8.root
	TFile* sourceFile = TFile::Open("root://cmsxrootd.fnal.gov//store/data/Run2016D/DoubleEG/NANOAOD/22Aug2018-v1/70000/043B6A52-DFA8-E811-989D-009C02AABEB8.root", "READ");
	if (!sourceFile || sourceFile->IsZombie()) {
		cerr << "Error opening source file!" << endl;
		return;
	}

	// Access the "Events" tree
	TTree* sourceTree = static_cast<TTree*>(sourceFile->Get("Events"));
	if (!sourceTree) {
		cerr << "Tree 'Events' not found in source file!" << endl;
		sourceFile->Close();
		return;
	}
	*/
	//TTree* newTree = sourceTree->CloneTree(0);
	
	//newTree->SetBasketSize("*", 320000);
	
	//sourceFile -> Close();

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	//Open File for reading
	
	const string input_file = "/uscms/home/cgillesp/text_full.txt";
	
	ifstream infile(input_file);
	
    if (!infile.is_open()) {
        cerr << "Error opening file: " << input_file << endl;
        return;
    }
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
    while (getline(infile, line)) {
		
		//Process the line and make the path
		
		size_t first = line.find_first_not_of(" \t");
		size_t last = line.find_last_not_of(" \t");
		if (first == string::npos) continue; // Skip empty lines
		string trimmed_line = line.substr(first, (last - first + 1));
		
		
		cout << "Processing file: " << trimmed_line << endl;
		
		string fullPath = "root://cmsxrootd.fnal.gov/" + trimmed_line;
		cout << "Processing file: " << fullPath << endl;
		
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		// Open the ROOT file
		TFile* file = TFile::Open(fullPath.c_str(), "READ");
		if (!file || file->IsZombie()) {
			cerr << "Error opening file: " << fullPath << endl;
			continue;
		}

		// Access the tree named "Events"
		//TTree* tree = static_cast<TTree*>(file->Get("Events"));
		TTree* tree = nullptr;
		file->GetObject("Events",tree);
		if (!tree) {
			cerr << "Tree 'Events' not found in file: " << fullPath << endl;
			continue;
		}
		
		//file->Close();
		
		stringstream treeName;
		treeName << "newTree_" << fileCounter++; // Create unique name
		TTree* newTree = tree->CloneTree(0);
		newTree->SetName(treeName.str().c_str());
		
		ULong64_t entries = tree->GetEntries();
		cout << "Number of entries: " << entries << endl;

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		//reading the file branches and attaching them to variables	

		UInt_t photonN;
		Float_t photonPT[amount];
		Float_t eta[amount];
		Float_t phi[amount];
		Bool_t pconditionE;
		Bool_t pconditionP;
		Bool_t pixseed[amount];
		Float_t missingE;

		tree->SetBranchAddress("nPhoton", &photonN);
		tree->SetBranchAddress("Photon_pt", &photonPT);
		tree->SetBranchAddress("Photon_eta", &eta);
		tree->SetBranchAddress("Photon_phi", &phi);
		tree->SetBranchAddress("HLT_Diphoton30_18_R9Id_OR_IsoCaloId_AND_HE_R9Id_Mass90", &pconditionP);
		tree->SetBranchAddress("HLT_Diphoton30_18_R9Id_OR_IsoCaloId_AND_HE_R9Id_DoublePixelSeedMatch_Mass70", &pconditionE);
		tree->SetBranchAddress("Photon_pixelSeed", &pixseed);
		tree->SetBranchAddress("MET_pt", &missingE);

		//The previous trigger cut
		//HLT_Diphoton30_18_R9Id_OR_IsoCaloId_AND_HE_R9Id_DoublePixelSeedMatch_Mass70;


		// load the histogram with values from the tree

		for(int i = 0; i < entries; i++){
			
			tree -> GetEntry(i);
			
			//variable declaration
			
			vector<bool> pclist(photonN,true);
			vector<int> passlist(0);
			pass = true;
			pX_sum = 0;
			pY_sum = 0;
			pZ_sum = 0;
			pT_sum = 0;
			

			
			//to check if running
			if (i % 10000 == 0){
				cout <<endl << "Events passed " <<i << endl;
			}
			

			
			
			//Scan all photons if they pass photon and eta requirements, along with R9 and R9 Mass requirments for events with atleast 2 EM objects

			if ((pconditionP|| pconditionE) && photonN >= 2){
				
				
				for(int j = 0; j < photonN; j++){
					
					//EM object must have enough momentum
					
					if(photonPT[j]< 40){
						pclist[j] = false;
					}
					
					//EM object must be in barral
					
					if(abs(eta[j])>1.44){
						pclist[j] = false;
					}
					
									
				}
				

				// pass all positions in the pclist to the passlist if the EM object satisfied trigger requirements
				
				for(int j =0; j < pclist.size(); j++){
					
					if(pclist[j]){
						passlist.push_back(j);
					}				
					
				}			
				
			}
			else{
				pass = false;
			}
			
			//note, pass list holds the positions of each object that passes the requirements
			
			//Rank pass list from greatest to least in terms of transverse momentum
			
			if (passlist.size() >= 2){
				
				for(int j = 0; j < passlist.size()-1; j++){
					for(int k = j+1; k < passlist.size(); k++){
						
						if(photonPT[passlist[j]] < photonPT[passlist[k]]){
							swap = passlist[j];
							passlist[j] = passlist[k];
							passlist[k] = swap;
						}
						
					}				
				}
										
			}
			else{
				pass = false;
			}

			
			///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			
			//Determine if the first two events pass DeltaR and PixSeed Requirements for an Electron
			
			if(pass){
			
				if(deltaR(eta[passlist[0]],phi[passlist[0]],eta[passlist[1]],phi[passlist[1]])<.6){
					pass = false;
				
				}
			
				//if(pixseed[passlist[0]] == true || pixseed[passlist[1]] == true){
				//	pass = false;
				//}
			}	
			
				
			if (pass){
				
				try{
					newTree->Fill();
				}
				catch(int bug){
					problem += 1;
					cerr << "Error at entry: " << i << endl;
				}
							

					


			}
			
			
			
			
		}
		newTree->Write();
		delete newTree;
		cout << "Amount of stuff not written: " << problem << endl;
		
	}
	// save new tree and output the dataset
	
		
	outfile->cd();
	
	outfile->Close();
	
}