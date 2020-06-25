#include <TString.h>
#include <TLegend.h>
#include <TFile.h>
#include <TNtuple.h>
#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TROOT.h>
#include <TCutG.h>
#include <TMath.h>
#include <TProfile.h>
#include <TObjArray.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <math.h>
#include <iomanip>
#include <fstream>
using namespace std;


void comp_yfp_xfp(Int_t nrun=1814,Int_t FileID=-2) {
  
  gROOT->Reset();
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(11);
  gStyle->SetTitleOffset(1.,"Y");
  gStyle->SetTitleOffset(.7,"X");
  gStyle->SetLabelSize(0.04,"XY");
  gStyle->SetTitleSize(0.06,"XY");
  gStyle->SetPadLeftMargin(0.14);
  const Int_t Number=3;
  Double_t Red[Number] = { 1.0,0.0,0.0};
  Double_t Blue[Number] = { 1.0,0.0,1.0};
  Double_t Green[Number] = { 0.0,1.0,0.0};
 Double_t Len[Number] = { 0.0,.5,1.0};
 Int_t nb=50;
 TColor::CreateGradientColorTable(Number,Len,Red,Green,Blue,nb);
  //  Get info for that optics run
 TString OpticsFile = "list_of_optics_run.dat";
   ifstream file_optics(OpticsFile.Data());
 TString opticsline;
 vector<TString > OpticsID;
  TString temp;
 //
  if (file_optics.is_open()) {
    //
    cout << " Open file = " << OpticsFile << endl;
    while (!file_optics.eof()  ) {
      temp.ReadToDelim(file_optics,',');
      temp.ReadToDelim(file_optics,',');
      cout << temp;
      OpticsID.push_back(temp);
      file_optics >> temp;
      file_optics >> temp;
    }
  } else {
    cout << " No file = " << OpticsFile << endl;    
  }
  //
  TString inputroot;
   TFile *fhistroot;
   TString outputpdf;
   inputroot=Form("hist/Optics_%s_%d_hist.root",OpticsID[0].Data(),FileID);
   outputpdf = Form("plots/Optics__%s_%d_xfp_cuts",OpticsID[0].Data(),FileID);
     cout << " infile root = " << inputroot << endl;
   fhistroot =  new TFile(inputroot);
 //
}
