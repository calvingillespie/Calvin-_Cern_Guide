#include "TLegend.h"


void HIST(){
  
  
  /*//Read File
  TFile *f1 = TFile::Open("outfileelectronFinalJet2.root");
  TFile *f2 = TFile::Open("outfilephotonFinalJet2.root");
  */
  /*
  TFile *f1 = TFile::Open("outfileelectronFinal.root");
  TFile *f2 = TFile::Open("outfilephotonFinal.root");
  TFile *outfile = new TFile("comparisonFinal.root", "RECREATE");
  */
  /*
  TFile *f1 = TFile::Open("outfileelectronFinalMaGa.root");
  TFile *f2 = TFile::Open("outfilephotonFinal.root");  
  TFile *outfile = new TFile("comparisonFinalMaGa.root", "RECREATE");
  */
  ///*
  TFile *f1 = TFile::Open("outfileelectronFinalJet20_2.root");
  TFile *f2 = TFile::Open("outfilephotonFinal20Jet2.root");
  TFile *outfile = new TFile("comparisonFinalJet20_2.root", "RECREATE");
  //*/
  
  //Make Canvas
  
  TCanvas *c = new TCanvas("c", "DIEM OBJ", 800 , 800);
  
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
  //System PT

  TH1F *hesyspt; f1->GetObject("hpSyspt", hesyspt);
  hesyspt->SetLineColor(kRed);
  hesyspt->Sumw2();

  TH1F *hpsyspt; f2->GetObject("hpSyspt", hpsyspt);
  hpsyspt->SetLineColor(kBlue);
  hpsyspt->Sumw2();
  
  hpsyspt->SetLineColor(kBlue);
  hesyspt->SetLineColor(kRed);
  
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
  //Plot Ratio System PT
  TH1F *hratio = (TH1F*)hpsyspt->Clone("hratio");
  TH1F *hratio1 = (TH1F*)hesyspt->Clone("hratio1");
  
  hratio->Scale(1./hratio->Integral());
  hratio1->Scale(1./hratio1->Integral());
  TH1F *hratio2 = (TH1F*)hratio->Clone("hratio2");
  
  hratio->SetLineWidth(2);
  hratio1->SetLineWidth(2);
  hratio2->SetLineWidth(2);
  
  hratio1->SetXTitle("GeV");
  hratio1->SetYTitle("Probabilty");
  hratio->SetXTitle("GeV");
  hratio->SetYTitle("yy/ee ratio");
  
  hratio->SetTitleSize(0.05, "X");  // A more reasonable title size
  hratio->SetTitleSize(0.05, "Y");
  hratio1->SetTitleSize(0.05, "X");  // A more reasonable title size
  hratio1->SetTitleSize(0.05, "Y");
  
  hratio->Divide(hratio1);
  //hrsyspt->Rebin(2);
  hratio->SetTitle("System Momentum Ratio between Photon and Electron Candidates");
  //TLegend *legend = new TLegend(0.7, 0.7, 0.9, 0.9);
  
  
  
  outfile->WriteObject(hratio,"Electron to Photon SysP Ratio");
  
  
  
  //TCanvas *c = new TCanvas("c1", "Histogram Ratio", 800, 600);
  
  c->Divide(1, 2);
  c->cd(2);
  hratio->GetXaxis()->SetRangeUser(0,400);  
  hratio->Draw();
  c->cd(1);
  
  hratio1->SetTitle("System Momentum of Photon and Electron Candidates");
  
  hratio1->GetXaxis()->SetRangeUser(0,400);
  hratio1->Draw("HIST");
  hratio2->GetXaxis()->SetRangeUser(0,400);
  hratio2->Draw("HIST SAME");
  
  TLegend *legendSYS = new TLegend(0.5, 0.7, 0.7, 0.9);
  legendSYS ->AddEntry(hratio1, "Dielectron", "f");
  legendSYS ->AddEntry(hratio2, "Diphoton", "f");
  legendSYS->Draw();
  
  outfile->WriteObject(c, "SystemPT of Di-E and Di-P and Ratio");
  
  delete hratio;
  c-> Clear();
  

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
  //Missing Energy
  
  TH1F *heMET; f1->GetObject("hpMET", heMET);
  heMET->SetLineColor(kRed);

  TH1F *hpMET; f2->GetObject("hpMET", hpMET);
  hpMET->SetLineColor(kBlue);
  
  
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
  //Get ratio between Electron MET to Photon MET
  
  
  

  
  c-> Clear();
  
  //////////////////////////////////////////////////////////////////////////////////////////
  
  heMET->SetLineColor(kBlue);
  hpMET->SetLineColor(kRed);
  
  
  heMET->Scale(1./heMET->Integral());
  hpMET->Scale(1./hpMET->Integral());
  
  heMET->SetLineWidth(2);
  hpMET->SetLineWidth(2);
  
  
  heMET->SetXTitle("GeV");
  heMET->SetYTitle("Probability");  // Corrected typo: "Probabilty" -> "Probability"
  heMET->SetTitleSize(0.05, "X");  // A more reasonable title size
  heMET->SetTitleSize(0.05, "Y");

  
  
  
  TH1F *hrMET = (TH1F*)hpMET->Clone("hrMET");
  hrMET->SetTitle("Missing Transverse Energy Ratio between Diphoton and Dielectron Candidates");
  hrMET->SetLineWidth(2);
  
  hrMET->SetXTitle("GeV");
  hrMET->SetYTitle("yy/ee ratio");  // Corrected typo: "Probabilty" -> "Probability"
  hrMET->SetTitleSize(0.05, "X");  // A more reasonable title size
  hrMET->SetTitleSize(0.05, "Y");

  
  
  hrMET->Divide(heMET);

  outfile->WriteObject(hrMET, "Photon to Electon MET Ratio");
  hrMET->SetLineColor(kBlack);
  
  c->Divide(1, 2);
  c->cd(1);
  hrMET->Draw();
  c->cd(2);
  
  heMET->SetTitle("Missing Transverse Energy of Diphoton and Dielectron Candidates");
  heMET->Draw("HIST");
  hpMET->Draw("HIST SAME");
  
  TLegend *legendMET = new TLegend(0.5, 0.7, 0.7, 0.9);
  legendMET->AddEntry(heMET, "Dielectron", "f");
  legendMET->AddEntry(hpMET, "Diphoton", "f");
  
  heMET->Draw("E");
  hpMET->Draw("same E");
  
  legendMET->Draw();
  
  c->SetTitle("MET Plot and Ratio");
  
  outfile->WriteObject(c, "MET Ratio of DiPhoton/DiElectron");
  
  c-> Clear();
  
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
  //h21->Scale(1/h21->Integral("width"));
  //h22->Scale(1/h22->Integral("width"));
  
  /*
  heMET->Draw("E");
  hpMET->Draw("same E");
  
  c->SetTitle("MET Plot; System Momentum");
  outfile->WriteObject(c, "SystemMET of Di-E and Di-P");
  
  
  c-> Clear();
  */
  

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
  //Transverse momentum
  
  TH1F *hept; f1->GetObject("hpPT", hept);
  hept->SetLineColor(kRed);
  hept->Sumw2();

  TH1F *hppt; f2->GetObject("hpPT", hppt);
  hppt->SetLineColor(kBlue);
  hppt->Sumw2();
  
  TH1F *hrpt = (TH1F*)hppt->Clone("hrpt");
  hrpt->Divide(hept);
  outfile->WriteObject(hrpt, "Photon to Electon PT Ratio");
  
  TH1F *hPVZ; f1->GetObject("PVZ", hPVZ);
  
  
  
  
  TF1 *gaussFit = new TF1("gaussFit", "gaus", -15, 15);
  
  hPVZ->Fit(gaussFit, "R");  // "R" restricts fit to the range of the function

    // Retrieve the fit parameters
  double mean = gaussFit->GetParameter(1);  // Mean (mu)
  double sigma = gaussFit->GetParameter(2); // Standard deviation (sigma)

    // Print the results
  cout << "Fitted Mean: " << mean << endl;
  cout << "Fitted Sigma: " << sigma << endl;

    // Draw the histogram and the fit
	
  hPVZ->SetXTitle("cm");
  hPVZ->SetYTitle("ee events");  // Corrected typo: "Probabilty" -> "Probability"
  hPVZ->SetTitleSize(0.05, "X");  // A more reasonable title size
  hPVZ->SetTitleSize(0.05, "Y");
	
  hPVZ->Draw();
  gaussFit->Draw("same");  // Overlay fit on histogram
  
  TLegend *legendPVZ = new TLegend(0.5, 0.7, 0.7, 0.9);
  legendPVZ->AddEntry(hPVZ, "Primary Vertex Info", "f");
  legendPVZ->AddEntry(gaussFit, "Gauss Fit", "f");
  legendPVZ->Draw();
  
  
  outfile->WriteObject(c, "Gauss FIT of PVZ with PVZ");  // Save as an image
  
  c-> Clear();
  
  TH1F *heNJ; f1->GetObject("hNj", heNJ);
  TH1F *hpNJ; f2->GetObject("hjNID",hpNJ);
  
  heNJ->Scale(1.0/heNJ->Integral());
  hpNJ->Scale(1.0/hpNJ->Integral());
  
  hpNJ->SetLineWidth(3);
  heNJ->SetLineWidth(3);
  
  hpNJ->SetLineColor(kRed);
  heNJ->SetLineColor(kBlue);
  
  heNJ->Draw("E");
  hpNJ->Draw("SAME E");
  
  TLegend *legendJET = new TLegend(0.5, 0.7, 0.7, 0.9);
  legendJET->AddEntry(heNJ, "DiElectron", "f");
  legendJET->AddEntry(hpNJ, "DiPhoton", "f");
  legendJET->Draw();
  
  outfile->WriteObject(c, "Number of Jets Comparisons between DiElectron and Diphoton");  // Save as an image
  
  c-> Clear();
  
  
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////









////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void weighted (){
  
  
  /*
  //Read File
  TFile *f1 = TFile::Open("outfileelectronFinal.root");
  TFile *outfile = new TFile("comparison_weighted_Final.root", "RECREATE");
  
  TFile *f2 = TFile::Open("outfilephotonFinal.root"); 
  
  TFile *f3 = TFile::Open("weightedDielectronFinal.root");
  */
  ////////////////////////////////////////////////////////////////////////////////
  
  /*
  TFile *f1 = TFile::Open("outfileelectronFinalMaGa.root");
  TFile *outfile = new TFile("comparison_weighted_FinalMaGa.root", "RECREATE");
  
  TFile *f2 = TFile::Open("outfilephotonFinal.root"); 
  
  TFile *f3 = TFile::Open("weightedDielectronFinalMaGa.root");
  */
  ////////////////////////////////////////////////////////////////////////////////
  ///*
  TFile *f1 = TFile::Open("outfileelectronFinalJet.root");
  TFile *outfile = new TFile("comparison_weighted_FinalJet.root", "RECREATE");
  
  TFile *f2 = TFile::Open("outfilephotonFinalJet.root"); 
  
  TFile *f3 = TFile::Open("weightedDielectronFinalJet.root");
  //*/
  //Make Canvas
  TCanvas *c = new TCanvas("c", "DIEM OBJ", 800 , 800);
  
  /////////////////////////////////////////////////////////////////////////////////
  
  //Make Weighted Graphs and prepare
  
  TH1F *heMET; f1->GetObject("hpMET", heMET);
  heMET->SetLineColor(kRed);

  TH1F *hewMET; f3->GetObject("hpMET", hewMET);
  hewMET->SetLineColor(kBlue);
  
  TH1F *hpMET; f2->GetObject("hpMET", hpMET);
  hewMET->SetLineColor(kGreen);
  
  double binEdges[] = {0, 3.571,7.142,10.713,14.284,17.855,21.426,24.997,28.568,32.139,35.710,39.281,42.852,46.423,50,62.5,75,87.5,100};
  int nBins = sizeof(binEdges) / sizeof(binEdges[0]) - 1;
  
  TH1F *hpMETrb = new TH1F("hpMETrb", "Rebinned DiPhoton Hist MET", nBins, binEdges);
  TH1F *heMETrb = new TH1F("heMETrb", "Rebinned DiElectron Hist MET", nBins, binEdges);
  TH1F *hewMETrb = new TH1F("hewMETrb", "Rebinned Weighted DiElectron Hist MET", nBins,binEdges);
  
  
  for (int i = 1; i <= hpMET->GetNbinsX(); ++i) {
    double binContent = hpMET->GetBinContent(i);
    double binCenter = hpMET->GetBinCenter(i);
    hpMETrb->Fill(binCenter, binContent);
  }
  
  for (int i = 1; i <= heMET->GetNbinsX(); ++i) {
    double binContent = heMET->GetBinContent(i);
    double binCenter = heMET->GetBinCenter(i);
    heMETrb->Fill(binCenter, binContent);
  }
  
  for (int i = 1; i <= hewMET->GetNbinsX(); ++i) {
    double binContent = hewMET->GetBinContent(i);
    double binCenter = hewMET->GetBinCenter(i);
    hewMETrb->Fill(binCenter, binContent);
  }
  
  //Normalized MET of Photon, electron and weighted electron
  
  TH1F *hpMETc = (TH1F*)hpMET->Clone("hpMETc");
  hpMETc ->Scale(1./hpMETc->Integral());
  TH1F *heMETc = (TH1F*)heMET->Clone("heMETc");  
  heMETc ->Scale(1./heMETc->Integral());
  TH1F *hewMETc = (TH1F*)hewMET->Clone("hewMETc");  
  hewMETc ->Scale(1./hewMETc->Integral());
  ////////////////////////////////////////////////////////////////////////////////////////////////////
  
  heMETc->SetLineColor(kRed);
  hewMETc->SetLineColor(kBlue);
  
  heMETc->Draw("E");
  hewMETc->Draw("same E");
  
  TLegend *legendMET = new TLegend(0.5, 0.7, 0.7, 0.9);
  legendMET->AddEntry(heMETc, "DiElectron MET", "f");
  legendMET->AddEntry(hewMETc, "Weighted DiElectron MET", "f");
  legendMET->Draw();
	
  c->SetTitle("Weighted Dielectron MET vs Unweighted Dielectron");
  c->Update();
  outfile->WriteObject(c, "Electron to Weighted Electron Plot");
  
  c-> Clear();
  
  /////////////////////////////////////////////////////////////////////////////////////////////////////
  
  //Weighted DiElectron vs DiPhoton MET
  
  c->Divide(1, 2);
  c->cd(1);
  
  hpMETc->SetLineColor(kRed);
  hewMETc->SetLineColor(kBlue);
  
  hewMETc->SetLineWidth(2);
  hpMETc->SetLineWidth(2);
  
  hewMETc->SetTitle("Missing Transverse Energy of Diphoton and and Weighted Dielectron Candidates");
  
  hewMETc->SetXTitle("GeV");
  hewMETc->SetYTitle("Probability");  // Corrected typo: "Probabilty" -> "Probability"
  hewMETc->SetTitleSize(0.05, "X");  // A more reasonable title size
  hewMETc->SetTitleSize(0.05, "Y");

  
  TH1F *hrMET = (TH1F*)hpMET->Clone("hrMET");
  hrMET->SetTitle("Missing Transverse Energy Ratio between Diphoton and Dielectron Candidates");
  hrMET->SetLineWidth(2);
  
  TH1F *hrpMET = (TH1F*)hpMETc->Clone("hrpMET");
  
  hrpMET->SetLineWidth(2);

  hrpMET->SetXTitle("GeV");
  hrpMET->SetYTitle("yy/ee ratio");  // Corrected typo: "Probabilty" -> "Probability"
  hrpMET->SetTitleSize(0.05, "X");  // A more reasonable title size
  hrpMET->SetTitleSize(0.05, "Y");

  
  hewMETc->Draw("E");
  hpMETc->Draw("same E"); 
  
  TLegend *legendMETP = new TLegend(0.5, 0.7, 0.7, 0.9);
  legendMETP->AddEntry(hpMETc, "Diphoton", "f");
  legendMETP->AddEntry(hewMETc, "Weighted Dielectron", "f");
  legendMETP->Draw();

  c->cd(2);
  
  hrpMET->Divide(hewMETc);
  hrpMET->SetTitle("Missing Transverse Energy Ratio between Diphoton and Weighted Dielectron Candidates");
  c->Update();
  outfile->WriteObject(hrpMET, "Photon to Electon Control");
  hrpMET->Draw();
  
  outfile->WriteObject(c, "Photon to Weighted Electron Plot");
  
  c-> Clear();
  ////////////////////////////////////////////////////////////////////////////////
  
  //Normalized Distribution of Unweighted Dielectron and Diphoton System PT
  
  TH1F *hewSyspt; f3->GetObject("hpSyspt", hewSyspt);
  TH1F *hpSyspt; f2->GetObject("hpSyspt", hpSyspt);
  TH1F *heSyspt; f1->GetObject("hpSyspt", heSyspt);
  
  TH1F *hr1 = (TH1F*)hpSyspt->Clone("hr1");
  TH1F *hr2 = (TH1F*)heSyspt->Clone("hr2");
  TH1F *hr3 = (TH1F*)hpSyspt->Clone("hr3");
  
  hr1->SetLineColor(kRed);
  hr2->SetLineColor(kBlue);
  
  hr1->Scale(1./hr1->Integral());
  hr2->Scale(1./hr2->Integral());
  hr3->Scale(1./hr3->Integral());
  
  hr1->Draw("E");
  hr2->Draw("same E");
  
  TLegend *legendSYS = new TLegend(0.5, 0.7, 0.7, 0.9);
  legendSYS->AddEntry(hr1, "Diphoton", "f");
  legendSYS->AddEntry(hr2, "Dielectron", "f");
  legendSYS->Draw();
  
  
  c->SetTitle("Normalized System PT for Unweighted Dielectron and diphoton");
  c->Update();
  outfile->WriteObject(c, "Normalized System PT");
  c-> Clear();
  
  ///////////////////////////////////////////////
  
  
  hr1->Draw("E");
  hr3->Draw("same E");
  
  TLegend *legendSYSN = new TLegend(0.5, 0.7, 0.7, 0.9);
  legendSYSN->AddEntry(hr1, "Photon System PT", "f");
  legendSYSN->AddEntry(hr3, "Weighted Electron System PT", "f");
  legendSYSN->Draw();
  
  outfile->WriteObject(c, "Weighted Electron And Photon System PT");
  c-> Clear();
  
  cout << hpSyspt->Integral()/heSyspt->Integral() << endl;
  
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}  
  
void Jet(){
  
  //Read File
  
  ///*
  TFile *f1 = TFile::Open("outfileelectronFinalJet.root");
  TFile *outfile = new TFile("comparison_jet30_FinalJet.root", "RECREATE");
  
  TFile *f2 = TFile::Open("outfilephotonFinalJet.root"); 
  
  TFile *f3 = TFile::Open("weightedDielectronFinalJet.root");
  //*/
  ////////////////////////////////////////////////////////////////////////////////
  
  
  ////////////////////////////////////////////////////////////////////////////////
  /*
  TFile *f1 = TFile::Open("outfileelectronFinal.root");
  TFile *outfile = new TFile("comparison_jet30_Final.root", "RECREATE");
  
  TFile *f2 = TFile::Open("outfilephotonFinal.root"); 
  
  TFile *f3 = TFile::Open("weightedDielectronFinal.root");
  */
  /*
  TFile *f1 = TFile::Open("outfileelectronFinalMaGa.root");
  TFile *outfile = new TFile("comparison_jet30_FinalMaGa.root", "RECREATE");
  
  TFile *f2 = TFile::Open("outfilephotonFinal.root"); 
  
  TFile *f3 = TFile::Open("weightedDielectronFinalMaga.root");
   */
  
  TCanvas *c = new TCanvas("c", "DIEM OBJ", 800 , 800);
  
  TH1F *heMEThard; f1->GetObject("hpSysptJ", heMEThard);
  TH1F *heMET; f1->GetObject("hpMET", heMET);
  TH2F *heMETvHard; f1->GetObject("heMETvHard", heMETvHard);
  
  TH1F *hpMEThard; f2->GetObject("hpSysptJ",hpMEThard);
  
  TH1F *heMETF; f1->GetObject("hMETF", heMETF);
  TH1F *heMETP; f1->GetObject("hMETP", heMETP);
  TH1F *hpMETF; f2->GetObject("hMETF", hpMETF);
  TH1F *hpMETP; f2->GetObject("hMETP", hpMETP);
  
  TH1F *hewMEThard; f3->GetObject("hpSysptJ", hewMEThard);
  
  TH1F *heMass; f1->GetObject("hpmass", heMass);
  
  ////////////////////////////////////////////////////////////////////////////////////////////////
  /*
  
  TH1F *heMassG; f4->GetObject("hpmass", heMassG);
  
  heMass->Scale(1/heMass->Integral());
  heMassG->Scale(1/heMassG->Integral());
  
  heMass->SetLineColor(kBlue);
  heMassG->SetLineColor(kRed);
  
  heMass->Draw();
  heMassG->Draw("Same");
  
  TLegend *legendMass = new TLegend(0.5, 0.7, 0.7, 0.9);
  legendMass->AddEntry(heMass, " Electron Invariant Mass without Distribution", "f");
  legendMass->AddEntry(heMassG, " Electron Invariant Mass with Distribution", "f");
  legendMass->Draw();
  
  
  outfile->WriteObject(c, "Mass Comparison with or without Distribution");
  
  c->Clear();
  */
  ///////////////////////////////////////////////////////////////////////////////////////////////
  heMEThard->SetLineColor(kBlue);
  heMET->SetLineColor(kRed);
  
  heMET->Scale(1./heMET->Integral());
  heMEThard->Scale(1./heMEThard->Integral());
  
  c->Divide(1, 2);
  c->cd(1);
  
  heMEThard->Draw("E");
  heMET->Draw("Same E");
  
  c->cd(2);
  
  TH1F *hrMET1 = (TH1F*)heMET->Clone("hrMET1");
  hrMET1 -> Divide(heMEThard);
  hrMET1 ->Draw();
  
  c->cd(1);
  
  TLegend *legendSYSMET = new TLegend(0.5, 0.7, 0.7, 0.9);
  legendSYSMET->AddEntry(heMEThard, " Electron Hard MET", "f");
  legendSYSMET->AddEntry(heMET, " Electron MET", "f");
  legendSYSMET->Draw();
  
  outfile->WriteObject(c, "Hard MET vs MET for weighted electron");
  
  c->Clear();
///////////////////////////////////////////////////////////////////////////////////////////////////

//Profile Plot
  
  TProfile *phardMET = heMETvHard->ProfileX("Hard Met vs Pf MET");
  phardMET->SetLineColor(kBlue);
  phardMET->SetMarkerStyle(21);
  
  outfile->WriteObject(phardMET, "Average PF MET vs Hard MET");
  
 //////////////////////////////////////////////////////////////////////////////////////////////////
 
 c->Clear();
 
 heMETF->Scale(1./heMETF->Integral());
 heMETP->Scale(1./heMETP->Integral());
 
 heMETP->SetLineColor(kRed);
 heMETF->SetLineColor(kBlue);
 
 heMETP->Draw("E");
 heMETF->Draw("SAME E");
 
 TLegend *legendIDMET = new TLegend(0.5, 0.7, 0.7, 0.9);
 legendIDMET->AddEntry(heMETP, " Passed ID Electron MET", "f");
 legendIDMET->AddEntry(heMETF, " Failed ID Electron MET", "f");
 legendIDMET->Draw();
 
 outfile->WriteObject(c, "ID Electron MET comparison");
 c->Clear();
 
 ////////////////////////////////////////////////////////////////////////////////////////////////////
 
 c->Clear();
 
 hpMETF->Scale(1.0/hpMETF->Integral());
 hpMETP->Scale(1.0/hpMETP->Integral());
 
 hpMETP->SetLineColor(kRed);
 
 hpMETP->Draw("E");
 hpMETF->Draw("SAME E");
 
 TLegend *legendIDMETP = new TLegend(0.5, 0.7, 0.7, 0.9);
 legendIDMETP->AddEntry(hpMETP, " Passed ID Photon MET", "f");
 legendIDMETP->AddEntry(hpMETF, " Failed ID Photon MET", "f");
 legendIDMETP->Draw();
 
 outfile->WriteObject(c, "ID Photon MET comparison");
 
 /////////////////////////////////////////////////////////////////////////////////////////////////////
 
 c->Clear();
 
 TH1F *hemassP; f1->GetObject("hemassP", hemassP);
 TH1F *hemassF; f1->GetObject("hemassF", hemassF);
 
 hemassF->SetLineColor(kRed);
 
 hemassF->Scale(1.0/hemassF->Integral());
 hemassP->Scale(1.0/hemassP->Integral());
 
 hemassP->Draw("LF");
 hemassF->Draw("SAME LF");
 
 TLegend *legendIDMETPf = new TLegend(0.5, 0.7, 0.7, 0.9);
 legendIDMETPf->AddEntry(hemassF, " Pass Electron Invariant Mass", "f");
 legendIDMETPf->AddEntry(hemassP, " Failed ID Electron Invariant Mass", "f");
 legendIDMETPf->Draw();

 outfile->WriteObject(c, "Fail and Pass comparison");	
	
 c->Clear();
 
 //////////////////////////////////////////////////////////////////////////////////////////////////////////
 
 c->Divide(1, 2);
 c->cd(1);
 
 hewMEThard->SetLineColor(kRed);
 hpMEThard->SetLineColor(kBlue);
 
 hewMEThard->Scale(1.0/hewMEThard->Integral());
 hpMEThard->Scale(1.0/hpMEThard->Integral());
 
 TH1F *hpMEThardR = (TH1F*)hpMEThard->Clone("hpMEThardR");
 
 hpMEThard->SetTitle("Hard Missing Transverse Energy of Diphoton and Weighted Dielecton Candidates");
 
  hewMEThard->SetLineWidth(2);
  hpMEThard->SetLineWidth(2);
 
  hpMEThard->SetXTitle("GeV");
  hpMEThard->SetYTitle("Probability");  // Corrected typo: "Probabilty" -> "Probability"
  hpMEThard->SetTitleSize(0.05, "X");  // A more reasonable title size
  hpMEThard->SetTitleSize(0.05, "Y");

  
  hpMEThardR->SetTitle("Missing Transverse Energy Ratio between Diphoton and Dielectron Candidates");
  hpMEThardR->SetLineWidth(2);
  
  hpMEThardR->SetXTitle("GeV");
  hpMEThardR->SetYTitle("yy/ee ratio");  // Corrected typo: "Probabilty" -> "Probability"
  hpMEThardR->SetTitleSize(0.05, "X");  // A more reasonable title size
  hpMEThardR->SetTitleSize(0.05, "Y");


 
 
 
 hpMEThard->Draw("E");
 hewMEThard->Draw("SAME E");
 
 TLegend *legendMEThard = new TLegend(0.5, 0.7, 0.7, 0.9);
 legendMEThard->AddEntry(hewMEThard, "Weighted Dielectron", "f");
 legendMEThard->AddEntry(hpMEThard, " Diphoton", "f");
 legendMEThard->Draw();
 
 c->cd(2);
 hpMEThardR->Divide(hewMEThard);
 
 hpMEThardR->SetTitle("Hard Missing Transverse Energy Ratio between Diphoton and Weighted Dielecton Candidates");
 hpMEThardR->Draw();
 
 
 outfile->WriteObject(c, "Weighted vs Photon Hard MET");
 
 c->Clear();
 
		
}
