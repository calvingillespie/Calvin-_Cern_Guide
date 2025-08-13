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
const Float_t Ecal_radius = 2.0;

const Float_t gauss_mean =  0.251021;
const Float_t gauss_width = 3.37119;

const bool jr30idCOND = true;
const bool massCOND = false;
const bool GaussCOND = false;
const bool jcondition = true;

TFile *weightinput = TFile::Open("/uscms/home/cgillesp/comparisonFinalJet2.root");
//TFile *weightinput = TFile::Open("/uscms/home/cgillesp/comparisonFinalMa.root");
//TFile *weightinput = TFile::Open("/uscms/home/cgillesp/comparisonFinalMaGaJet.root");

TFile *outfile = new TFile("weightedDielectronFinalJet2.root", "RECREATE");
//TFile *outfile = new TFile("weightedDielectronFinalMA.root", "RECREATE");
//TFile *outfile = new TFile("weightedDielectronFinalMaGaJet.root", "RECREATE");

const Float_t ecal_radi = 129;
const Float_t ecal_capZ = 317;

const Float_t integralWeight = 0.894608;

double binsAllie[] = {0, 
                 50.0/14, 2*50.0/14, 3*50.0/14, 4*50.0/14, 5*50.0/14, 6*50.0/14, 7*50.0/14, 8*50.0/14, 9*50.0/14, 
                 10*50.0/14, 11*50.0/14, 12*50.0/14, 13*50.0/14, 50, 
                 60, 75, 85, 100};

int nBinsAllie = sizeof( binsAllie)/sizeof( binsAllie[0]) - 1; // Number of bins

double binsSyS[] = {0, 
                 50.0/14, 2*50.0/14, 3*50.0/14, 4*50.0/14, 5*50.0/14, 6*50.0/14, 7*50.0/14, 8*50.0/14, 9*50.0/14, 
                 10*50.0/14, 11*50.0/14, 12*50.0/14, 13*50.0/14, 50, 
                 60, 70, 
                 85, 100,120,140,160,180,200,300,400,600};   

int nBinsSyS = sizeof(binsSyS)/sizeof(binsSyS[0]) - 1; // Number of bins

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

void load_AFCe() {
	
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
	Float_t hardmetsq;
	int event_count = 0;
	Float_t z_coord;
	float_t z_coordt;
	
	vector<string> badfiles;
	
	int holder;
	int amount = 100;
	int swap;
	bool pass;
	int binsize = 100;
	
	
	//creating the histo, reading the file, ahpnd finding the number of entries
	
	TH1F *h1 = new TH1F("hemass","DiElectron Invariant Mass after Cut",binsize,0,200);
	TH1F *h2 = new TH1F("hpPT","DiElectron Individual Momentum after Cut",binsize,0,200);
	TH1F *h3 = new TH1F("hpeta","DiElectron Eta after Cut",binsize,-2,2);
	TH1F *h5 = new TH1F("hpdist","Distance for DiElectron Events",binsize,0,7);
	TH1F *h8 = new TH1F("hpMET","Missing Transverse Energy from DiElectron events",nBinsAllie,binsAllie);
	TH1F *h9 = new TH1F("hpSyspt","Total Momentum from DiElectron Events",nBinsSyS,binsSyS);
	TH2F *h10 = new TH2F("hMeSpt","System Momentum vs Missing Energy",nBinsSyS,binsSyS,nBinsAllie,binsAllie);
	TH2F *h13 = new TH2F("heMetvHard","Hard MET vs PF MET for DiElectrons",nBinsAllie,binsAllie,nBinsAllie,binsAllie);
	TH1F *h14 = new TH1F("hpSysptj","Hard Met for DiElectron with Jets",nBinsAllie,binsAllie);

	
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
	
	//////////////////////////////////////////////////////////////////////////////////////////////
	
	TH1F *hratio; weightinput->GetObject("Electron to Photon SysP Ratio", hratio);
	
	int nbins = hratio->GetNbinsX();
	float wbins = hratio->GetBinWidth(5);
	Float_t weight;
	Float_t weightapply;
	
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
			badfiles.push_back(fullPath);
            continue;
        }

        // Access the tree named "Events"
        TTree* tree = static_cast<TTree*>(file->Get("Events"));
        if (!tree) {
            cerr << "Tree 'Events' not found in file: " << fullPath << endl;
            //file->Close();
			badfiles.push_back(fullPath);
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
		Float_t Pvz;
		
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
		tree->SetBranchAddress("HLT_Diphoton30_18_R9Id_OR_IsoCaloId_AND_HE_R9Id_DoublePixelSeedMatch_Mass70", &econdition);
		tree->SetBranchAddress("Photon_pixelSeed", &pixseed);
		
		//tree->SetBranchAddress("CaloMET_pt", &missingE);
		tree->SetBranchAddress("MET_pt", &missingE);
		//tree->SetBranchAddress("RawMET_pt", &missingE);
		
		tree->SetBranchAddress("run", &runNum);
		tree->SetBranchAddress("luminosityBlock", &lumiblock);
		tree->SetBranchAddress("Photon_cutBased", &photonID);
		tree->SetBranchAddress("nJet", &jetN);
		tree->SetBranchAddress("Jet_pt", &jetPT);
		tree->SetBranchAddress("Jet_eta", &jeta);
		tree->SetBranchAddress("Jet_phi", &jphi);
		tree->SetBranchAddress("PV_z", &Pvz);

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
			
			/*
			
			if (!pass){
				//cout << "Runnumber: " << runNum << endl << "Lumi: " << lumiblock << endl;
			}
			else{
				cout << "success" << endl;
			}
			*/
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
					
					if(photonID[j] < 2 && jr30idCOND){
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
				
				TRandom3 randGen(0);
				z_coord = randGen.Gaus(gauss_mean,gauss_width);
				
				
				for(int j = 0; j < 2; j++){
						
					holder = passlist[j];
					
					z_coordt = z_coord;
					
					theta = 2*atan(exp(-eta[holder]));
					pTotal = photonPT[holder]/sin(theta);
					
					if(GaussCOND){
						
						if(abs(eta[holder]) > 1.44){
							//Endcap
							continue;
						}
						else{
							//Barrel
							z_coordt = Pvz + ecal_radi/TMath::Tan(theta) - z_coord;
							theta = TMath::ATan2(ecal_radi,z_coordt);
						}
						
					}
					
					pX_sum += pTotal*sin(theta)*cos(phi[holder]);
					
					pY_sum += pTotal*sin(theta)*sin(phi[holder]);
					
					pZ_sum += pTotal*cos(theta);
					pT_sum += pTotal;
					
					
					h2->Fill(pTotal);
					h3->Fill(eta[holder]);	
								
					
				}	
				
				// calculate invariant mass and fill histogram and reset for next event
				
				sysPTsq = (pX_sum*pX_sum)+(pY_sum*pY_sum);
				
				mass = sqrt((pT_sum*pT_sum)-sysPTsq -(pZ_sum*pZ_sum));
				
				///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			
				//get weight
				
				if (sqrt(sysPTsq) >= 300){
					weightapply = integralWeight;
				}
				else{
					int weightpos = static_cast<int>(sqrt(sysPTsq) / wbins);
					weightpos = max(0, min(weightpos, nbins - 1));			
					weightapply = weights[weightpos];
				}
				//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				
				int jconditionApp = 0;
				
				for(int j = 0; j < jetN; j++){
						
					//reset Z_coor
					
					float_t z_temp;
					
					//Conditions to ensure that the jet is out of range of the 
					bool condiOne = deltaR(eta[passlist[0]],phi[passlist[0]],jeta[j],jphi[j])>.4;
					bool condiTwo = deltaR(eta[passlist[1]],phi[passlist[1]],jeta[j],jphi[j])>.4;

					if (condiOne && condiTwo && (jetPT[j] > 30.0)){
						
						jconditionApp +=1;
						
					}
					
				}
				
				
				//cout << sqrt(sysPTsq) << " " << weightapply << endl;
				
				//Window Cut
				if(((mass>=80 && mass<=100) || !massCOND) && jconditionApp>=2){
					
					//fill mass histo
					h1->Fill(mass,weightapply);
					//fill system momentum histo
					h9->Fill(sqrt(sysPTsq),weightapply);
					h5->Fill(deltaR(eta[passlist[0]],phi[passlist[0]],eta[passlist[1]],phi[passlist[1]]));
					
					h8->Fill(missingE,weightapply);
					
					h10->Fill(sqrt(sysPTsq),missingE,weightapply);
					
					
					//JET calculation
					for(int j = 0; j < jetN; j++){
						
						
						
						float_t z_temp;
						
						//Conditions to ensure that the jet is out of range of the 
						bool condiOne = deltaR(eta[passlist[0]],phi[passlist[0]],jeta[j],jphi[j])>.4;
						bool condiTwo = deltaR(eta[passlist[1]],phi[passlist[1]],jeta[j],jphi[j])>.4;
						//condiOne = true;
						//condiTwo = true;
						if (condiOne && condiTwo && (jetPT[j] > 30.0)){
							
							theta = 2*atan(exp(-1.*jeta[j]));
							jTotal = jetPT[j]/sin(theta);
							
							
							
							if(GaussCOND){								
								
								if(abs(jeta[j]) > 1.48){
									//Endcap
									
									z_coordt = z_coord;
									if(jeta[j]>0){
										z_coordt = -z_coord + ecal_capZ;
										
										z_temp = -Pvz + ecal_capZ;
									}
									else{
										z_coordt = - z_coord - ecal_capZ;
										
										z_temp = -Pvz - ecal_capZ;
									}
									
									
									
									theta = TMath::ATan2(abs(z_temp)*TMath::Tan(theta),z_coordt);
									
									
								}
								else{
									//Barrel
									z_coordt = Pvz + ecal_radi/TMath::Tan(theta) - z_coord;
									theta = TMath::ATan2(ecal_radi,z_coordt);
								}
								
							}
							
							pX_sum += jTotal*sin(theta)*cos(jphi[j]);
							
							pY_sum += jTotal*sin(theta)*sin(jphi[j]);
							
							
						}
											
					
					}
					
					hardmetsq = (pX_sum*pX_sum)+(pY_sum*pY_sum);
					
					h14->Fill(sqrt(hardmetsq),weightapply);
					h13->Fill(sqrt(hardmetsq),missingE,weightapply);
					
					event_count += 1;

				}
			}		
				

		}
		
		//Allocate memory
		delete tree;
		file->Close();	
		
	}
	//TFile *outfile = new TFile("weightedDielectron.root", "RECREATE");
	//TFile *outfile = new TFile("weightedDielectronIDMA.root", "RECREATE");
	//TFile *outfile = new TFile("weightedDielectronIDMA_gauss.root", "RECREATE");
	
	h1->SetTitle("Events; Mass GeV/C^2");
	h2->SetTitle("Events; Tranverse Momentum");
	h3->SetTitle("Events; Eta");
	h5->SetTitle("Events; DeltaR");
	h8->SetTitle("Events; Missing Transverse Energy");
	h9->SetTitle("Events; System Momentum");
	h10->SetTitle("Events; System Momentum vs Missing Transverse Energy");
	h13->SetTitle("Events; HardMet Vs MET for DiElectron Events");
	h14->SetTitle("Events; Hard MET for DiElectron Events");
	
	
	//h->GetXaxis()->SetRangeUser(0, 400); // Set X-axis range from 10 to 50
	//h->GetYaxis()->SetRangeUser(0, 200000); // Set Y-axis range from 0 to 100
	
	// save histograms to output file
		
	outfile->WriteObject(h1, "hpmass");
	outfile->WriteObject(h2, "hpPT");
	outfile->WriteObject(h3, "hpeta");
	outfile->WriteObject(h5, "hpdist");
	outfile->WriteObject(h8, "hpMET");
	outfile->WriteObject(h9, "hpSyspt");
	outfile->WriteObject(h10, "hMeSpt");
	outfile->WriteObject(h13, "heMETvHard");
	outfile->WriteObject(h14, "hpSysptJ");
			
	outfile->Close();
    
	cout << "event count: " << event_count << endl;
	
	
	cout << "Bad Files: " << endl;
    for (const string& str : badfiles) {
        cout << str << " ";
    }
    cout << endl;
	return;
}

//Gausian of 3cm since the electrons are better than the photons
//(Written)              We will make a TH1F concerning PV_Z, apply a gausian to that histogram, then use that gaussian to apply a z primary vertex to the dielectron samples

//(Check?)Calculate the new theta prime from original theta which will be used in the calculation.
//Hard MET of messed DiElectron and Photo should be correlated.
//Ratio of the DiPhoton to DiElectron System PT

//Normalize graphs, and then get the ratio's.

