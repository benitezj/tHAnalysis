#include <iostream>
#include <fstream>
#include <string>
#include <TFile.h>
#include <TDirectory.h>
#include <TList.h>

void updateHistNames(){

  TString fileName = "/data/tHAnalysis/July5/tH2017_tHqb.root";
  TFile *file = TFile::Open(fileName);
  TList *histoList = (TList*)file->GetListOfKeys(); 

  std::ofstream output("tHAnalysis/macros/listOfHistos.txt");

  std::cout<<"updating listOfHistos.txt"<<std::endl; 

  for(const auto&& name: *histoList) output<<name->GetName()<<std::endl; 
  
  file->Close();
  delete file;  
  return;
}
