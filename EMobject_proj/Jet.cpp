#include <iostream>
#include <vector>
#include <string>
#include <filesystem> // Requires C++17 or later
#include <TFile.h>
#include <TTree.h>
#include <fstream>
#include <sstream>

using namespace std;
namespace fs = std::filesystem;

const Float_t pi = 3.14159;

const Float_t integralWeight = 0.894608;

class KeyRange {
public:
    string key;
    pair<int, int> range;

    // Constructor
    KeyRange(const string& k, const pair<int, int>& r) : key(k), range(r) {}

    // Display the object
    void display() const {
        cout << "Key: " << key << ", Range: [" << range.first << ", " << range.second << "]" << endl;
    }
	
	const string& getKey() const {
        return key;
    }

    bool contains(int number) const {
        return number >= range.first && number <= range.second;
    }
	
};

//////////////////////////////////////////////////////////////////////////////////////////////////

// functions

// Function to process a line and split it into key and ranges
vector<KeyRange> processLine(const string& line) {
    //cout << "Processing line: " << line << endl; // Debug output

    string noSpaces;
    for (char c : line) {
        if (!isspace(c)) noSpaces += c; // Remove spaces
    }

    // Find the key
    size_t keyStart = noSpaces.find('"') + 1;
    size_t keyEnd = noSpaces.find('"', keyStart);
    if (keyStart == string::npos || keyEnd == string::npos) {
        cerr << "Error: Key not found or improperly formatted in line: " << line << endl;
        return {}; // Return an empty vector for invalid lines
    }
    string key = noSpaces.substr(keyStart, keyEnd - keyStart);

    // Find the ranges
    size_t rangesStart = noSpaces.find('[', keyEnd) + 1;
    size_t rangesEnd = noSpaces.rfind(']');
    if (rangesStart == string::npos || rangesEnd == string::npos || rangesStart >= rangesEnd) {
        cerr << "Error: Ranges not found or improperly formatted in line: " << line << endl;
        return {}; // Return an empty vector for invalid lines
    }
    string rangesStr = noSpaces.substr(rangesStart, rangesEnd - rangesStart);

    // Extract individual ranges
    vector<KeyRange> objects;
    stringstream ss(rangesStr);
    string range;

    while (getline(ss, range, ']')) {
        if (range.empty()) continue;

        size_t start = range.find('[');
        size_t comma = range.find(',', start);
        if (start == string::npos || comma == string::npos || comma <= start) {
            cerr << "Skipping invalid range format: " << range << endl;
            continue;
        }

        try {
            int first = stoi(range.substr(start + 1, comma - start - 1));
            int second = stoi(range.substr(comma + 1));
            objects.emplace_back(key, make_pair(first, second));
        } catch (const invalid_argument& e) {
            cerr << "Invalid integer in range: " << range << endl;
        }
    }

    return objects;
}

//////////////////////////////////////////////////////////////////////////////////////////////////

bool hasKeyWithNumber(const vector<KeyRange>& objects, const int runNum, int lumiBlock) {
    for (const auto& obj : objects) {
        if (obj.getKey() == to_string(runNum) && obj.contains(lumiBlock)) {
            return true; // Found a match
        }
    }
    return false; // No matching key and range found
}


/////////////////////////////////////////////////////////////////////////////////////////////////

Float_t deltaR (Float_t xA , Float_t yA , Float_t xB , Float_t yB ){
	float_t range;
	Float_t deltaphi = abs(yA-yB);
	
	if(deltaphi > pi){
		deltaphi = 2*pi-deltaphi;
	}
	
	range = sqrt((xA-xB)*(xA-xB)+(deltaphi)*(deltaphi));
	return(range);
}

//////////////////////////////////////////////////////////////////////////////////////////////////

void load_jet() {
	
    // Directory containing ROOT files
    string eosDirectory = "/store/user/cgillesp/2016_Reduced_DatasetV2";
	
	Float_t jTotal;
	Float_t pTotal;
	Float_t pT_sum;
	Float_t pX_sum;
	Float_t pY_sum;
	Float_t pZ_sum;
	Float_t mass;
	Float_t theta;
	Float_t sysPTsq;
	int event_count = 0;
	
	int holder;
	int amount = 100;
	int swap;
	bool pass;
	int binsize = 100;
	
	
	//creating the histo, reading the file, ahpnd finding the number of entries
	
	TH1F *h9 = new TH1F("hpSyspt","Hard Met for DiElectron with Jets",400,0,400);
	TH2F *h10 = new TH2F("heMetvHard","Hard MET vs PF MET for DiElectrons",200,0,200,200,0,200);

	////////////////////////////////////////////////////////////////////////////////////////////////
	
	string filename = "/uscms/home/cgillesp/json.txt";
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return;
    }

    vector<KeyRange> allObjects;
    string line;

    while (getline(file, line)) {
        try {
            vector<KeyRange> objects = processLine(line);
            if (!objects.empty()) {
                allObjects.insert(allObjects.end(), objects.begin(), objects.end());
            }
        } catch (const exception& e) {
            cerr << "Error processing line: " << e.what() << endl;
        }
    }
	
	for (const auto& obj : allObjects) {
        obj.display();
    }
	
    file.close();
	
	///////////////////////////////////////////////////////////////////////////////////////////////
	
	TFile *weightinput = TFile::Open("/uscms/home/cgillesp/comparison.root");
	TH1F *hratio; weightinput->GetObject("Electron to Photon SysP Ratio", hratio);
	
	int nbins = hratio->GetNbinsX();
	float wbins = hratio->GetBinWidth(5);
	Float_t weight;
	int weightapply;
	
	vector<float_t> weights;

	for (int j = 1; j <= nbins; ++j) {
		weight = hratio->GetBinContent(j);
		if (weight <= 0 || isnan(weight) || isinf(weight)) {
            cerr << "Invalid bin content at bin " << j << ": " << weight << endl;
            weight = 1.0; // Default to 1.0 if invalid
        }
		weights.push_back(weight);
	}	
			
	weightinput->Close();
		
	
	//////////////////////////////////////////////////////////////////////////////////////////////
	
	string command = "eosls " + eosDirectory;
    FILE* pipe = gSystem->OpenPipe(command.c_str(), "r");
    if (!pipe) {
        cerr << "Failed to list files in EOS directory: " << eosDirectory << endl;
        return;
    }		
	
    
    // Loop over all ROOT files in the directory
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), pipe)) {
        string fileName = buffer;
        fileName.erase(fileName.find_last_not_of("\n") + 1); // Remove trailing newline

        // Construct the full xrootd path
        string fullPath = "root://cmsxrootd.fnal.gov/" + eosDirectory + "/" + fileName;
        cout << "Processing file: " << fullPath << endl;

        // Open the ROOT file
        TFile* file = TFile::Open(fullPath.c_str(), "READ");
        if (!file || file->IsZombie()) {
            cerr << "Error opening file: " << fullPath << endl;
            continue;
        }

        // Access the tree named "Events"
        TTree* tree = static_cast<TTree*>(file->Get("Events"));
        if (!tree) {
            cerr << "Tree 'Events' not found in file: " << fullPath << endl;
            file->Close();
            continue;
        }
		
		
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		
		// Define variables and set branch addresses
		UInt_t photonN;
		UInt_t jetN;
		Float_t photonPT[amount];
		Float_t eta[amount];
		Float_t phi[amount];
		Float_t jeta[amount];
		Float_t jphi[amount];
		Bool_t pcondition;
		Bool_t econdition;
		Bool_t pixseed[amount];
		Float_t missingE;
		UInt_t runNum;
		UInt_t lumiblock;
		Int_t photonID[amount];
		Float_t jetPT[amount];
		
		ULong64_t entries = tree->GetEntries();

		cout << "Number of entries: " << entries << endl;

		tree->SetBranchAddress("nPhoton", &photonN);
		tree->SetBranchAddress("Photon_pt", &photonPT);
		tree->SetBranchAddress("Photon_eta", &eta);
		tree->SetBranchAddress("Photon_phi", &phi);
		tree->SetBranchAddress("HLT_Diphoton30_18_R9Id_OR_IsoCaloId_AND_HE_R9Id_Mass90", &pcondition);
		tree->SetBranchAddress("HLT_Diphoton30_18_R9Id_OR_IsoCaloId_AND_HE_R9Id_DoublePixelSeedMatch_Mass70", &econdition);
		tree->SetBranchAddress("Photon_pixelSeed", &pixseed);
		tree->SetBranchAddress("MET_pt", &missingE);
		tree->SetBranchAddress("run", &runNum);
		tree->SetBranchAddress("luminosityBlock", &lumiblock);
		tree->SetBranchAddress("Photon_cutBased", &photonID);
		tree->SetBranchAddress("nJet", &jetN);
		tree->SetBranchAddress("Jet_pt", &jetPT);
		tree->SetBranchAddress("Jet_eta", &jeta);
		tree->SetBranchAddress("Jet_phi", &jphi);

		// Loop over entries in the tree
		
		for(int i = 0; i < entries; i++){
	
			tree -> GetEntry(i);
			
			//variable declaration
			
			vector<bool> pclist(photonN,true);
			vector<int> passlist(0);
			//Determine if it passes the conditions
			//pass = true;
			pass = hasKeyWithNumber(allObjects, runNum, lumiblock);
			pX_sum = 0;
			pY_sum = 0;
			pZ_sum = 0;
			pT_sum = 0;
			
			
			//to check if running
			if (i % 10000 == 0){
				cout <<endl << "Events passed " << i << endl;
			}
			

			
			//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			
			//Scan all photons if they pass photon and eta requirements, along with R9 and R9 Mass requirments for events with atleast 2 EM objects
		
			if (pass && econdition && photonN >= 2){
				
				
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
			
			/////////////////////////////////////////////////////////////////////////////////////////////////////////
			
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
			
			/////////////////////////////////////////////////////////////////////////////////////////////////////////
			
			//Determine if the first two events pass DeltaR and PixSeed Requirements for an Electron
			
			if(pass){
			
				if(deltaR(eta[passlist[0]],phi[passlist[0]],eta[passlist[1]],phi[passlist[1]])<.6){
					pass = false;
				
				}
				if(pixseed[passlist[0]] == false || pixseed[passlist[1]] == false){
					pass = false;
				}
			
			}	
				
				// calculate variables for invariant mass
			
			////////////////////////////////////////////////////////////////////////////////////////////////////////
			
			if (pass){
				
				// Calculate 
				
				for(int j = 0; j < 2; j++){
						
					holder = passlist[j];
					
					
					theta = 2*atan(exp(-eta[holder]));
					pTotal = photonPT[holder]/sin(theta);
					
					pX_sum += pTotal*sin(theta)*cos(phi[holder]);
					
					pY_sum += photonPT[holder]*sin(phi[holder]);
					
					pZ_sum += pTotal*cos(theta);
					pT_sum += pTotal;
					
								
					
				}	
				
				//Add Jet System Pt
				float_t weightapply;
				sysPTsq = (pX_sum*pX_sum)+(pY_sum*pY_sum);
				
				if (sqrt(sysPTsq) >= 300){
					float_t weightapply = integralWeight;
				}
				else{
					int weightpos = static_cast<int>(sqrt(sysPTsq) / wbins);
					weightpos = max(0, min(weightpos, nbins - 1));			
					weightapply = weights[weightpos];
				}
				
				for(int j = 0; j < jetN; j++){
					bool condiOne = deltaR(eta[passlist[0]],phi[passlist[0]],jeta[j],jphi[j])>.4;
					bool condiTwo = deltaR(eta[passlist[1]],phi[passlist[1]],jeta[j],jphi[j])>.4;
					if (condiOne && condiTwo && (jetPT[j] > 30.0)){
						
						theta = 2*atan(exp(-jeta[j]));
						jTotal = jetPT[j]/sin(theta);
						
						pX_sum += jTotal*sin(theta)*cos(jphi[j]);
						
						pY_sum += jetPT[j]*sin(jphi[j]);
						
						pZ_sum += jTotal*cos(theta);
						pT_sum += jTotal;
					}
										
				
				}
				sysPTsq = (pX_sum*pX_sum)+(pY_sum*pY_sum);
				
				
				//fill system momentum histo
				
				//h9->Fill(sqrt(sysPTsq), weightapply);
				h9->Fill(sqrt(sysPTsq));
				h10->Fill(sqrt(sysPTsq),missingE);
				event_count += 1;

				
			}		
				

		}
		
		//Allocate memory
		delete tree;
		file->Close();	
		
	}
	TFile *outfile = new TFile("outfilejete.root", "RECREATE");
	// save histograms to output file
	outfile->WriteObject(h9, "hpSyspt");
	outfile->WriteObject(h10, "heMetvHard");
	
			
	outfile->Close();
    
	cout << "event count: " << event_count << endl;
	
    return;
}

//Do this for both Photon and Di Electron Transverse momentum added to the Jet System Momentum. Follow the range delta r > .4
//Try the same binning