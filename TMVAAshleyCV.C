#include <cstdlib>
#include <vector>
#include <iostream>
#include <map>
#include <string>

#include "TFile.h"
#include "TCanvas.h"
#include "TTree.h"
#include "TString.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TStopwatch.h"
#include "TH1.h"
#include "TH2.h"

#include "TMVA/Tools.h"
#include "TMVA/Reader.h"
#include "TMVA/MethodCuts.h"
#include "TMVA/Types.h"
#include "TMVA/CrossValidation.h"
#include "TMVA/DataLoader.h"

void TMVAAshleyCV() {
TFile* outputFile = TFile::Open( "TMVACVBarrel_mS55.root", "RECREATE" );

TFile *input1 = TFile::Open( "/afs/cern.ch/user/c/calpigia/eos/public/forAshley/mc16ade_13TeV.311315.MadGraphPythia8EvtGen_A14NNPDF31LO_HSS_LLP_mH125_mS55_lthigh_DIST_NEW.root"  );
TTree* sigTree = (TTree*)input1->Get( "BDTFVariables" );
    //Filter for barrel or endcap
    TTree* originalSigTree = sigTree;
    TCut sigCut = "((nHits !=0) && (DetectorRegion==1) && (!std::isnan(Tracklet_avg_dR)) && (!std::isnan(Segment_avg_dR)) && (!std::isinf(Tracklet_avg_dR)) && (!std::isinf(Segment_avg_dR)))";
    
TFile *input2 = TFile::Open("/afs/cern.ch/user/c/calpigia/eos/public/forAshley/data15_13TeV.physics_Main_DIST_NEW.root" );
TTree* backTree = (TTree*)input2->Get( "BDTFVariables" );
    //Filter for barrel or endcap, and set validation region
    TCut backCut = "(nHits !=0) && (DetectorRegion==1)  && (!std::isnan(Segment_avg_dR)) && (!std::isnan(Tracklet_avg_dR)) && (!std::isinf(Tracklet_avg_dR)) && (!std::isinf(Segment_avg_dR))";

TMVA::DataLoader *dataloader=new TMVA::DataLoader("dataset");

dataloader->AddVariable( "nHits", 'F' );
dataloader->AddVariable( "Tracklet_avg_dR", 'F' );
dataloader->AddVariable( "Segment_avg_dR", 'F' );
dataloader->AddVariable( "Segment_sumpT", 'F' );
dataloader->AddVariable( "Segment_Sphericity", 'F' );
dataloader->AddVariable( "Segment_avg_dist", 'D' );

dataloader->AddSignalTree ( sigTree, 1.0 );
dataloader->AddBackgroundTree ( backTree, 1.0 );

dataloader->PrepareTrainingAndTestTree( sigCut, backCut, 
    "nTrain_Signal=0:nTrain_Background=0:SplitMode=Random:NormMode=NumEvents:!V" );

UInt_t numFolds = 5;
   TString analysisType = "Classification";
   TString splitType = "Random";
   TString splitExpr = "";

string optionsString = "!H:!V:NTrees=50:MinNodeSize=2.5%:UseFisherCuts:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.5";
//TMVA::CrossValidation * CV = new TMVA::CrossValidation(dataloader);
   TString cvOptions = Form("!V"
                            ":!Silent"
                            ":ModelPersistence"
                            ":AnalysisType=%s"
                            ":SplitType=%s"
                            ":NumFolds=%i"
                            ":SplitExpr=%s",
                            analysisType.Data(), splitType.Data(), numFolds,
                            splitExpr.Data());
TMVA::CrossValidation CV{"TMVACrossValidation", dataloader, outputFile, cvOptions};

//TMVA::CrossValidation CV(dataloader);

CV.BookMethod(TMVA::Types::kBDT,"BDTF",optionsString);
CV.SetNumFolds(numFolds);
CV.Evaluate();

size_t iMethod = 0;
   for (auto && result : CV.GetResults()) {
      std::cout << "Summary for method " << CV.GetMethods()[iMethod++].GetValue<TString>("MethodName")
                << std::endl;
      for (UInt_t iFold = 0; iFold<CV.GetNumFolds(); ++iFold) {
         std::cout << "\tFold " << iFold << ": "
                   << "ROC int: " << result.GetROCValues()[iFold]
                   << ", "
                   << "BkgEff@SigEff=0.3: " << result.GetEff30Values()[iFold]
                   << std::endl;
      }
   }

originalSigTree->Write();
outputFile->Write();
outputFile->Close();

 
}