/*--------------------------------------------------------------/ 
  Read out the branched (CHMspx, CHMspy) and
  sort out evtit==47 and Fit circe to data. 
/--------------------------------------------------------------*/


#include <iostream>
#include "TROOT.h"
#include "TObject.h"
#include "TFile.h"
#include "TTree.h"
#include "TSystem.h"
#include "TBranch.h"
#include "TCanvas.h"
#include "TRandom3.h"
#include "TGraph.h"
#include "TMath.h"
#include "TArc.h"
#include "TVirtualFitter.h"
#include "TLegend.h"

using namespace std;

TGraph* CHMsp;

void myfcn(Int_t &, Double_t *, Double_t &f, Double_t *par, Int_t) {
   //minimisation function computing the sum of squares of residuals
Int_t np = CHMsp->GetN();
f = 0;
Double_t *x = CHMsp->GetX();
Double_t *y = CHMsp->GetY();
   for (Int_t i=0;i<np;i++) {
      Double_t u = x[i] - par[0];
      Double_t v = y[i] - par[1];
      Double_t dr = par[2] - TMath::Sqrt(u*u+v*v);
      f += dr*dr;
   }
}

void Anal(){
TFile f("data.root");
TTree* t;
f.GetObject("trdata", t);

Int_t n_ent=0;
Float_t CHMspx[10000] = {0};
Float_t CHMspy[10000] = {0};
Int_t nspCHM=0;
Int_t evtid=0;
Bool_t CHMspprim[10000]={0};

n_ent = t->GetEntries();
t->SetBranchAddress("CHMspx",CHMspx);
t->SetBranchAddress("CHMspy",CHMspy);
t->SetBranchAddress("nspCHM", &nspCHM);
t->SetBranchAddress("evtid", &evtid);
t->SetBranchAddress("CHMspprim", CHMspprim);

Int_t evtid47_pt; 
Int_t evtid47_ptnum;
Int_t evtid47_ptprim=0;

for (Int_t i=0; i<n_ent; i++){
	for (Int_t ii=0; ii<10000; ii++){
	   CHMspx[ii]=0;  
	}
t->GetEntry(i);
if (evtid==47){
evtid47_pt = i;
evtid47_ptnum = nspCHM;

cout << "Event ID " << evtid <<endl;
cout << "*----------------------------------*" <<endl;
	for (Int_t ii=0; ii<nspCHM; ii++){
	  if (CHMspprim[ii]==1){
	  evtid47_ptprim+=1;
	  cout << CHMspx[ii]<< "	" << CHMspy[ii] << endl;}
	}
cout << "Number of primary particle step points is " << evtid47_ptprim << endl;
cout << "*----------------------------------*" <<endl;
}
}

/*----Fitting Part----*/
t->GetEntry(evtid47_pt);

CHMsp = new TGraph(evtid47_ptprim);
TCanvas* c1 = new TCanvas("c1","c1", 600, 600);	

//Int_t prim_counter=0;

for (Int_t i=0; i<evtid47_ptnum; i++){
		CHMsp->SetPoint(i,CHMspx[i],CHMspy[i]);
	
}
c1->SetFrameFillColor(0);
c1->DrawFrame(-1000,-1000,1000,1000);
CHMsp->SetMarkerStyle(20); 
CHMsp->SetMarkerSize(0.2);
CHMsp->Draw("p");


TVirtualFitter::SetDefaultFitter("Minuit");  //default is Minuit
TVirtualFitter *fitter = TVirtualFitter::Fitter(0, 3);
fitter->SetFCN(myfcn);

fitter->SetParameter(0, "x0",   0, 0.1, 0,0);
fitter->SetParameter(1, "y0",   0, 0.1, 0,0);
fitter->SetParameter(2, "R",    500, 0.1, 0,0);

Double_t arglist[1] = {0};
fitter->ExecuteCommand("MIGRAD", arglist, 0);

//Draw the circle on top of the points
TArc *arc = new TArc(fitter->GetParameter(0), fitter->GetParameter(1), fitter->GetParameter(2));
arc->SetLineColor(kRed); arc->SetFillColor(kWhite); arc->SetFillStyle(4050); arc->SetLineWidth(1);
arc->Draw();

Double_t Rad = fitter->GetParameter(2);
const Double_t *R = &Rad;
TLegend* leg = new TLegend(0.2,0.8,0.35,0.85);
leg->SetHeader("The Legend Title");
leg->AddEntry(arc,"Fitted Circle","L");
leg->Draw();

/*
TCanvas* c2 = new TCanvas("c2","c2", 600, 600);	
c2->SetFrameFillColor(0);
c2->DrawFrame(-1000,-1000,1000,1000);
t->Draw("CHMspy:CHMspx","evtid==47 && CHMspprim==1");
*/
}


