
#include <cstdlib>
#include <iostream>
#include <map>
#include <string>

#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TObjString.h"
#include "TSystem.h"
#include "TROOT.h"

#if not defined(__CINT__) || defined(__MAKECINT__)
// needs to be included when makecint runs (ACLIC)
#include "TMVA/Factory.h"
#include "TMVA/Tools.h"
#endif

void TMVAClassification(TString Production="/data/tHAnalysis/July10", TString Region = "Nbjets==3" )
{

  // Input paths
  TString SignalPath=Production+"/tH";
  TString BkgPath=Production+"/mc15_13TeV.410501.PowhegPythia8EvtGen_A14_ttbar_hdamp258p75_nonallhad.merge.DAOD_TRUTH3.e5458_p2949";
  
  
  // Default MVA methods to be trained + tested
  TString outfileName( "TMVA.root" );
  TFile* outputFile = TFile::Open( outfileName, "RECREATE" );
  
  
  TMVA::Tools::Instance();
  
  TMVA::Factory *factory = new TMVA::Factory( "TMVAClassification", outputFile,
					      "!V:!Silent:Color:DrawProgressBar:Transformations=I;D;P;G,D:AnalysisType=Classification" );
  
  factory->AddVariable( "met","met", "GeV", 'F' );
  factory->AddVariable( "top_m","top_m", "GeV", 'F' );
  factory->AddVariable( "h_pt","h_pt", "GeV", 'F' );
  factory->AddVariable( "lep_pt","lep_pt", "GeV", 'F' );
  factory->AddVariable( "b1_pt","b1_pt", "GeV", 'F' );

  factory->AddVariable( "dR_R1_H3","dR_R1_H3", "", 'F' );
  factory->AddVariable( "dR_lep_b1NotH","dR_lep_b1NotH", "", 'F' );
  factory->AddVariable( "dR_H3_b1NotH","dR_H3_b1NotH", "", 'F' );

  factory->AddVariable( "dEta_R1_H3","dEta_R1_H3", "", 'F' );
  factory->AddVariable( "dEta_lep_b1NotH","dEta_lep_b1NotH", "", 'F' );
  factory->AddVariable( "dEta_H3_b1NotH","dEta_H3_b1NotH", "", 'F' );
  

  //  factory->AddVariable( "","", "", 'F' );
  
  // // //Read training and test data
  // TString fnameS = "/data/tHAnalysis/July10/tH/tuple_0.root";
  // TString fnameB = "/data/tHAnalysis/July10/mc15_13TeV.410501.PowhegPythia8EvtGen_A14_ttbar_hdamp258p75_nonallhad.merge.DAOD_TRUTH3.e5458_p2949/tuple_0.root";      
  // TFile *inputS = TFile::Open( fnameS );
  // TFile *inputB = TFile::Open( fnameB );
  // TTree *signal     = (TTree*)inputS->Get("ntuple");
  // TTree *background = (TTree*)inputB->Get("ntuple");


  TChain signal("ntuple");
  TChain background("ntuple");
  for(long i=0;i<100;i++){
    TString fnameS = SignalPath+"/tuple_"+i+".root";
    TString fnameB = BkgPath+"/tuple_"+i+".root";
    
    if(!(gSystem->AccessPathName( fnameS )))
      signal.Add(fnameS);
    if(!(gSystem->AccessPathName( fnameB )))
      background.Add(fnameB);
  } 
  

  // global event weights per tree (see below for setting event-wise weights)
  Double_t signalWeight     = 1.0;
  Double_t backgroundWeight = 1.0;
   
  // You can add an arbitrary number of signal or background trees
  factory->AddSignalTree    ( &signal,     signalWeight     );
  factory->AddBackgroundTree( &background, backgroundWeight );
   

  // Set individual event weights (the variables must exist in the original TTree)
  // factory->SetBackgroundWeightExpression( "weight" );
  
  factory->PrepareTrainingAndTestTree(Region.Data(), "SplitMode=random:!V" );


  // ---- Book MVA methods
  factory->BookMethod( TMVA::Types::kBDT,"BDT",
		       "!H:!V:NTrees=200:MaxDepth=4:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100:PruneMethod=NoPruning" );


  // Train MVAs using the set of training events
  factory->TrainAllMethods();

  // ---- Evaluate all MVAs using the set of test events
  factory->TestAllMethods();

  // ----- Evaluate and compare performance of all configured MVAs
  factory->EvaluateAllMethods();

  // --------------------------------------------------------------

  // Save the output
  outputFile->Close();

  std::cout << "==> Wrote root file: " << outputFile->GetName() << std::endl;
  std::cout << "==> TMVAClassification is done!" << std::endl;

  delete factory;

}
