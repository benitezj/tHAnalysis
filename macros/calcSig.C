#include "globals.h"

void calcSig(TString inputpath="/data/tHAnalysis/August7/mu200_TC_HS0.70"){
 
  cout<<"calcSig path: "<<inputpath<<endl;

  int method=2;

  /// Include 3 b-tag region
  getHistos(inputpath,m_bkgs,"jfwd_eta_SRB3");
  if(m_histos[m_bkgs[0]]==0){cout<<" m_bkgs[0] = NULL for 3B"<<endl; return;}
  std::pair<float,float> signif3B = getSignificance("1-lep., 3 b-tag, 1-2 light-jets",method);
  std::cout<<"calcsig: "<<inputpath<<" 3btag "<<setprecision(3)<<signif3B.first<<"+/-"<<setprecision(2)<<signif3B.second<<std::endl;
  system("mv Fit.png Fit_3b.png");

  /// Include 4 b-tag region
  getHistos(inputpath,m_bkgs,"jfwd_eta_SRB4");
  if(m_histos[m_bkgs[0]]==0){cout<<" m_bkgs[0] = NULL for 4B"<<endl; return;}
  std::pair<float,float> signif4B = getSignificance("1-lep., 4 b-tag, 1-2 light-jets",method);
  std::cout<<"calcsig: "<<inputpath<<" 4btag: "<<setprecision(3)<<signif4B.first<<"+/-"<<setprecision(2)<<signif4B.second<<std::endl;
  system("mv Fit.png Fit_4b.png");

  /////////////////////////////
  /// Calculate total 
  /////////////////////////////
  std::pair<float,float> signif;
  signif.first  = sqrt( signif3B.first*signif3B.first + signif4B.first*signif4B.first );
  signif.second = sqrt( pow(signif3B.first*signif3B.second,2) + pow(signif4B.first*signif4B.second,2) ) / signif.first;
  std::cout<<"calcsig: "<<inputpath<<" total: "<<setprecision(3)<<signif.first<<"+/-"<<setprecision(2)<<signif.second<<std::endl;


  gROOT->ProcessLine(".q");
}
