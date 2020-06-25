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


void comp_yfp_xfp(Int_t FileID=-2) {
  
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
 vector<Double_t > Angle;
 vector<vector<Double_t > > ztar;
  TString temp;
 //
  if (file_optics.is_open()) {
    //
    cout << " Open file = " << OpticsFile << endl;
    while (!file_optics.eof()  ) {
      temp.ReadToDelim(file_optics,',');
      temp.ReadToDelim(file_optics,',');
      //     cout << temp << endl;
      OpticsID.push_back(temp);
      temp.ReadToDelim(file_optics,',');
      //    cout << temp << endl;
      Angle.push_back(temp.Atof());
      temp.ReadToDelim(file_optics,',');
      //  cout << temp << endl;
      Int_t nftot = temp.Atoi();
      vector<Double_t > ftemp;
     file_optics >> temp;
     for (Int_t nf=0;nf<nftot;nf++) {
        temp.ReadToDelim(file_optics,',');
	//  cout << temp << endl;
 	ftemp.push_back(temp.Atof());
      }
      ztar.push_back(ftemp);
      file_optics >> temp;
      //      cout << temp << endl;
    }
  } else {
    cout << " No file = " << OpticsFile << endl;    
  }
  //
  Int_t nftot=OpticsID.size();
  cout << " n files = " << nftot << endl;
  TString inputroot;
   TFile *fhistroot;
   TH2F* hYFpXFp[nftot];
   TH2F* hYtarDelta[nftot];
   TString outputpdf;
   for (Int_t nf=0;nf<nftot;nf++) {
   inputroot=Form("hist/Optics_%s_%d_hist.root",OpticsID[nf].Data(),FileID);
   outputpdf = Form("plots/comp_shms_xfp_yfp_%d",FileID);
     cout << " infile root = " << inputroot << endl;
     fhistroot =  new TFile(inputroot);
     hYFpXFp[nf] = (TH2F*)fhistroot->Get("hYFpXFp");
     hYtarDelta[nf] = (TH2F*)fhistroot->Get("hYtarDelta");
   }
 //
   TCanvas* cFp = new TCanvas("cFp","cFp",700,700);
   cFp->Divide(2,2);
   for (Int_t nf=0;nf<nftot;nf++) {
     cFp->cd(nf+1);
 	gPad->SetGridx();
	gPad->SetGridy();
	gPad->SetLogz();    
    hYFpXFp[nf]->Draw("colz");
     hYFpXFp[nf]->SetMinimum(10);
   }  
   cFp->Print(outputpdf+".pdf("); 
 //
   TCanvas* cYtar = new TCanvas("cYtar","cYtar",700,700);
   cYtar->Divide(2,2);
   for (Int_t nf=0;nf<nftot;nf++) {
     cYtar->cd(nf+1);
     hYtarDelta[nf]->Draw("colz");
     hYtarDelta[nf]->SetMinimum(10);
     Int_t doff=1;
     for (Int_t nc=0;nc<ztar[nf].size();nc++) {
       Double_t ytar;
       if (nf!=0) ytar = -ztar[nf][nc]*sin(Angle[nf]/57.3)-0.34*cos(Angle[nf]/57.3)+0.06;
       if (nf==0)ytar = -ztar[nf][nc]*sin(Angle[nf]/57.3)+0.06;
       //      cout << doff << " " << Angle[nf] << " " << ztar[nf][nc] << " " << ytar << endl;
       TText* ytext ;
       if(doff >0) {
	 ytext = new TText(ytar,21,Form("%3.1f",ztar[nf][nc]));
       } else {
	 ytext = new TText(ytar,23,Form("%3.1f",ztar[nf][nc]));
       }
       doff=pow(-1,(nc+1));
       ytext->SetTextAlign(22);
       ytext->Draw("same");
       TLine* yline = new TLine(ytar,-15,ytar,20);
       yline->Draw("same");
   }
     
   }   
   cYtar->Print(outputpdf+".pdf)"); 
   //
}
