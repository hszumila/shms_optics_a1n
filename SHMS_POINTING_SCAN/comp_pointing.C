#include <TString.h>
#include <TLegend.h>
#include <TFile.h>
#include <TNtuple.h>
#include <TF1.h>
#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TROOT.h>
#include <TMath.h>
#include <TProfile.h>
#include <TObjArray.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include<math.h>
using namespace std;
void comp_pointing() {
 gROOT->Reset();
 gStyle->SetOptStat(0);
 gStyle->SetOptFit(11);
 gStyle->SetTitleOffset(1.,"Y");
 gStyle->SetTitleOffset(.7,"X");
 gStyle->SetLabelSize(0.04,"XY");
 gStyle->SetTitleSize(0.06,"XY");
 gStyle->SetPadLeftMargin(0.14);
     TString outputpdf;
    outputpdf="plots/comp_shms_pointing.pdf";
  //  Get info pointing scan
 TString PointingFileName = "SHMS_POINTING_SCAN/list_of_pointing_run.dat";
   ifstream file_Pointing(PointingFileName.Data());
  vector <Int_t> nrun;
  vector <Double_t> angles;
    cout << " Open file = " << PointingFileName << endl;
    if (file_Pointing.fail()) exit(1);
    while( !file_Pointing.eof()) {
      Int_t tn;
      Double_t tang;
      file_Pointing >> tn >> tang ;
      nrun.push_back(tn);
      angles.push_back(tang);
    }
    //
    vector <Double_t> x_mis(nrun.size());    
    vector <Double_t> y_mis(nrun.size());    
    for (Int_t n=0;n<nrun.size();n++) {
      y_mis[n] = 0.1*(-0.6); // cm
      x_mis[n] = 0.1*(-1.26); //cm
      //      cout << nrun[n] << " " << angles[n] << " "<< y_mis[n] << " "<< x_mis[n] << endl;
    }
    //
    vector<TH1F* > Hxbeam(nrun.size());
    vector<TH1F* > Hybeam(nrun.size());
    vector<TH1F* > Hxs(nrun.size());
    vector<TH1F* > Hys(nrun.size());
    vector<TH1F* > Hytar(nrun.size());
    vector <Double_t> xbeam(nrun.size());    
    vector <Double_t> ybeam(nrun.size());    
     //
    for (Int_t n=0;n<nrun.size();n++) {
      TString HistFileName = Form("hist/shms_coin_replay_production_all_%d_-1_hist.root",nrun[n]);
      TFile * HistRoot =  new TFile(HistFileName);
      Hxbeam[n] = (TH1F*)HistRoot->Get("hxbeam");
      Hybeam[n] = (TH1F*)HistRoot->Get("hybeam");
      Hxs[n] = (TH1F*)HistRoot->Get("hxsieve_cent");
      Hys[n] = (TH1F*)HistRoot->Get("hysieve_cent");
      Hytar[n] = (TH1F*)HistRoot->Get("hytar");
      xbeam[n] = Hxbeam[n]->GetMean();
       ybeam[n] = Hybeam[n]->GetMean();
   }    
    //
	TCanvas* canytar;
	TF1* fytar[nrun.size()];
	Double_t ytar_cent[nrun.size()];
	Double_t ytar_centerr[nrun.size()];
	Double_t ytar_sigma[nrun.size()];
	canytar = new TCanvas(Form("Canytar"),"ytar", 700,700);
	canytar->Divide(2,5);
	for  (Int_t nc=0;nc<nrun.size();nc++) {
	  Hytar[nc]->GetXaxis()->SetRangeUser(-1,1);
	  fytar[nc] = new TF1(Form("fytar_%d",nc),"gaus");
	      fytar[nc]->SetParameter(1,Hytar[nc]->GetBinCenter(Hytar[nc]->GetMaximumBin()));
	      fytar[nc]->SetParameter(0,Hytar[nc]->GetMaximum());
	      fytar[nc]->SetParameter(2,Hytar[nc]->GetRMS());
	  canytar->cd(nc+1);
	  Hytar[nc]->GetXaxis()->SetRangeUser(-3,3);
	  Hytar[nc]->Draw();
	  Hytar[nc]->Fit(Form("fytar_%d",nc),"Q","",-1,1);
	  ytar_cent[nc] = 10*fytar[nc]->GetParameter(1);
	  ytar_centerr[nc] = 10*fytar[nc]->GetParError(1);
	  ytar_sigma[nc] = 10*fytar[nc]->GetParameter(2);
	  Double_t YPred = 10*(-xbeam[nc]*cos(angles[nc]/180.*3.14159) -y_mis[nc]); 
	  cout << nrun[nc] << " " << angles[nc] << " "<< y_mis[nc] << " "<< xbeam[nc] << " " << ytar_cent[nc]<< " " << ytar_centerr[nc] << " " << YPred << " " << YPred-ytar_cent[nc] << " " << ytar_sigma[nc]<< endl;
	}
	
    
    //
}
