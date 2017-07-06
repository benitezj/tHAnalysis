#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

std::vector<std::string> getHistNames(std::string file){
 std:vector<std::string> histNames;

  std::ifstream infile(file);
  std::string line;
  while (std::getline(infile,line)){
    std::istringstream iss(line);

    std::string hist=""; 
    iss>>hist; 
    histNames.push_back(hist); 
  }

  return histNames;
}

void makeplots_all(){

  TString inpath="/data/tHAnalysis/July5";
  TString outpath="/afs/cern.ch/user/e/ehelfenb/www/tHAnalysis";
  std::vector<std::string> histNames=getHistNames("tHAnalysis/macros/listOfHistos.txt");  

  gROOT->ProcessLine(".L tHAnalysis/macros/plotSamples.C");

  for(auto const& hist: histNames){
    gROOT->ProcessLine("plotSamples(\""+hist+"\",\""+inpath+"\",\""+outpath+"\")");    
  }

  gROOT->ProcessLine(".q"); 
 
  return; 

}
