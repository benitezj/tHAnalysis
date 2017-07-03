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

#include "/nfs/home/benitezj/Notes/atlasstyle/AtlasStyle.C"
#include "/nfs/home/benitezj/Notes/atlasstyle/AtlasUtils.C"
#include "/nfs/home/benitezj/Notes/atlasstyle/AtlasLabels.C"

using namespace std;

#define LUMI 3000000 //3000/fb  but in /pb (crossections are in pb)


TString FitGausFormula("[0]*exp(-0.5*(x-[1])**2/[2]**2)");
TString FitGausText("Mean=%.2f+/-%.2f,  Sigma=%.2f+/-%.2f");


//////////////Load the crossections
std::map<std::string,float> getCrossections(){
 std:map<std::string,float> xS;

  std::ifstream infile("tHAnalysis/data/xSections.txt");
  std::string line;
  while (std::getline(infile,line)){
    std::istringstream iss(line);

    char B[100]="";
    iss>>B;
    float X=0;
    iss>>X;
    xS[B]=X;
  }

  return xS;
}



std::map<std::string,TFile*> getSamples(TString inputpath, std::vector<std::string>  samplelist){
  std::map<std::string,TFile*> files;

  for(int b=0;b<samplelist.size();b++){
    TFile*F=new TFile(inputpath+"/tH2017_"+samplelist[b].c_str()+".root","read");
    if(!F) continue;
    if(F->IsZombie()) continue;
    if(!(F->Get("events"))) continue;
    files[samplelist[b]]=F;
  }

  return files;
}



std::map<std::string,TH1F*> getHistos(TString inputpath,std::vector<std::string> bkgs, TString histoname){

  std::map<std::string,float> xs=getCrossections();
  std::map<std::string,TFile*> samples=getSamples(inputpath,bkgs);
  std::map<std::string,TH1F*> histos;
  for(int b=0;b<bkgs.size();b++){
    if(!samples[bkgs[b]]) continue;
    TH1F*Hevents=((TH1F*)(samples[bkgs[b]]->Get("events")));
    if(!Hevents) continue;
    TH1F*H=(TH1F*)samples[bkgs[b]]->Get(histoname.Data());
    if(!H) continue;
    //cout<<bkgs[b].c_str()<<" "<<xs[bkgs[b]]<<" "<<Hevents->GetBinContent(2)<<endl;
    H->Scale(LUMI*xs[bkgs[b].c_str()]/Hevents->GetBinContent(2));
    histos[bkgs[b]]=H;
  }

  return histos;
}




/////////////////////////Some tools 
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
