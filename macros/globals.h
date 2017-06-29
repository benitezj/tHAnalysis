#include <iostream>
#include <iomanip>
#include <sys/stat.h>
#include <fstream>
#include <string>
#include <vector>
#include <stdlib.h> 
#include <stddef.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
using namespace std;

#include <TString.h>
#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TH1F.h>
#include <TDirectory.h>
#include <TLegend.h>
#include <TProfile.h>
#include <TLatex.h>
#include <TLine.h>
#include <TH2F.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TStyle.h>
#include <TROOT.h>
#include <TEllipse.h>
#include <TLorentzVector.h>
#include <TPaveStats.h>
#include <TFrame.h>
#include <TGraph.h>

#define NMAXINPUTFILES 100

TString FitGausFormula("[0]*exp(-0.5*(x-[1])**2/[2]**2)");
TString FitGausText("Mean=%.2f+/-%.2f,  Sigma=%.2f+/-%.2f");

TChain* TREE = NULL; ///Chain with the cells,jets,...
TH1F* HNCELLvsR = NULL; ///histogram containing the definition of the cells

void loadChain(TString inpath, TChain * TREE, TString sample){
  if(!TREE) return;

  int nonExistentCounter=0;
  for(long i=0;i<=NMAXINPUTFILES;i++){
    if(nonExistentCounter==10)      break;//stop adding files

    TString fname =inpath+"/mytuple"+(long)i;

    struct stat st;
    if(stat((fname+".root").Data(),&st) != 0){
      if(i==0) cout<<(fname+".root")<<" Not found"<<endl;      nonExistentCounter++;       continue;
    }  
    TFile file((fname+".root").Data(),"read");
    if(file.IsZombie()){
      cout<<(fname+".root")<<" is Zombie"<<endl;  continue;
    }
    if(!file.GetListOfKeys()){
      cout<<(fname+".root")<<" has no Keys"<<endl;      continue;
    }
    if(file.GetListOfKeys()->GetSize()==0){
      cout<<(fname+".root")<<" KeysSize = 0"<<endl;      continue;            
    }

    TREE->Add((fname+".root").Data());    
  }
  cout<<"Number of input events "<<TREE->GetEntries()<<endl;
}

void loadHistos(TString inpath){
  for(long i=0;i<=NMAXINPUTFILES;i++){

    TString fname=inpath+"/mytuple"+(long)i;
    struct stat st;

    if(stat((fname+".root").Data(),&st) != 0){
      continue;
    }  
    TFile file((fname+".root").Data(),"read");
    if(file.IsZombie()){
      continue;
    }
    if(!file.GetListOfKeys()){
      continue;
    }
    if(file.GetListOfKeys()->GetSize()==0){
      continue;            
    }

    HNCELLvsR = (TH1F*) file.Get("HNCell");
    break;//Use the first good file
  }

  if(!HNCELLvsR){
    cout<<" No file was found for loading histograms"<<endl;
  }
}


TH1F* makeHistRatio(TH1* HNum,TH1* HDenom,float maxerr=0.5){
  if(HNum==NULL || HDenom == NULL){ 
    cout<<"Histograms are NULL"<<endl;
    return 0;
  }
  if(HNum->GetNbinsX() != HDenom->GetNbinsX()){ 
    cout<<"Histograms bins don't match"<<endl;
    return 0;
  }
  
  TH1F* HRatio=(TH1F*)HNum->Clone("HRatio");
  for(int b=0;b<=HNum->GetNbinsX()+1;b++){
    if(HDenom->GetBinContent(b)>0. &&
       HNum->GetBinError(b)/HNum->GetBinContent(b)<maxerr
       ){
      HRatio->SetBinContent(b,HNum->GetBinContent(b)/HDenom->GetBinContent(b));
      HRatio->SetBinError(b,HNum->GetBinError(b)/HDenom->GetBinContent(b));
    }else {
      HRatio->SetBinContent(b,0.);
      HRatio->SetBinError(b,0.);
    }
  }

  return HRatio;
}

double dR_fn(float eta1, float eta2, float phi1, float phi2){
  double deta= fabs(eta1 - eta2);      double dphi= fabs(phi1 - phi2);
  if (dphi > TMath::Pi() ) dphi = 2*(TMath::Pi()) - dphi;
  return sqrt((dphi*dphi)+(deta*deta));
}
