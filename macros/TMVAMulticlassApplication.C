/**********************************************************************************
 * Project   : TMVA - a Root-integrated toolkit for multivariate data analysis    *
 * Package   : TMVA                                                               *
 * Root Macro: TMVAMulticlassApplication                                          *
 *                                                                                *
 * This macro provides a simple example on how to use the trained multiclass      *
 * classifiers within an analysis module                                          *
 **********************************************************************************/

#include <cstdlib>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TStopwatch.h"
#include "TH1F.h"

#if not defined(__CINT__) || defined(__MAKECINT__)
#include "TMVA/Tools.h"
#include "TMVA/Reader.h"
#endif

using namespace TMVA;

void TMVAMulticlassApplication( TString myMethodList = "" )
{
#ifdef __CINT__
   gROOT->ProcessLine( ".O0" ); // turn off optimization in CINT
#endif

   TMVA::Tools::Instance();
   
   std::cout << std::endl;
   std::cout << "==> Start TMVAMulticlassApplication" << std::endl; 

   
   // create the Reader object
   TMVA::Reader *reader = new TMVA::Reader( "!Color:!Silent" );    

   // create a set of variables and declare them to the reader
   Float_t var1, var2, var3, var4;
   //reader->AddVariable( "var1", &var1 );
   //reader->AddVariable( "var2", &var2 );
   reader->AddVariable( "var3", &var3 );
   reader->AddVariable( "var4", &var4 );

   // book the MVA methods
   TString dir    = "weights/";
   TString prefix = "TMVAClassification";// TMVAClassification_BDT.weights.xml

   TString methodName = TString("BDT") + TString(" method");
   TString weightfile = dir + prefix + TString("_") + TString("BDT") + TString(".weights.xml"); 
   //reader->BookMVA( methodName, weightfile ); 
   reader->BookMVA("BDT", "./weights/TMVAClassification_BDT.weights.xml");

   // book output histograms
   UInt_t nbin = 100;
   TH1F *histMLP_signal(0), *histBDTG_signal(0), *histFDAGA_signal(0), *histPDEFoam_signal(0);
   histBDTG_signal  = new TH1F( "MVA_BDTG_signal",   "MVA_BDTG_signal",   nbin, 0., 1.1 );


   TFile *input(0); 
   TString fname = "./test.root";
   if (!gSystem->AccessPathName( fname )) {
      input = TFile::Open( fname ); // check if file in local directory exists
   }
   if (!input) {
      std::cout << "ERROR: could not open data file, please generate example data first!" << std::endl;
      exit(1);
   }
   std::cout << "--- TMVAMulticlassApp : Using input file: " << input->GetName() << std::endl;
   
   // prepare the tree
   // - here the variable names have to corresponds to your tree
   // - you can use the same variables as above which is slightly faster,
   //   but of course you can use different ones and copy the values inside the event loop
  
   TTree* theTree = (TTree*)input->Get("TreeS");
   std::cout << "--- Select signal sample" << std::endl;
   //theTree->SetBranchAddress( "var1", &var1 );
   //theTree->SetBranchAddress( "var2", &var2 );
   theTree->SetBranchAddress( "var3", &var3 );
   theTree->SetBranchAddress( "var4", &var4 );

   std::cout << "--- Processing: " << theTree->GetEntries() << " events" << std::endl;
   //TStopwatch sw;
   //sw.Start();

   for (Long64_t ievt=0; ievt<theTree->GetEntries();ievt++) {
      if (ievt%1000 == 0){
         std::cout << "--- ... Processing event: " << ievt << std::endl;
      }
      theTree->GetEntry(ievt);
      
      histBDTG_signal->Fill((reader->EvaluateMulticlass( "BDT" ))[0]);
   }
   
   // get elapsed time
   //sw.Stop();
   //std::cout << "--- End of event loop: "; sw.Print();
   
   TFile *target  = new TFile( "TMVAMulticlassApp.root","RECREATE" );
   histBDTG_signal->Write(); 
   
   target->Close();
   std::cout << "--- Created root file: \"TMVMulticlassApp.root\" containing the MVA output histograms" << std::endl;

   delete reader;
   
   std::cout << "==> TMVAClassificationApplication is done!" << std::endl << std::endl;
}
