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

void TMVAAshleyGrid() {
TFile* outputFile = TFile::Open( "TMVAVarGridDistBarrel_mS55.root", "RECREATE" );

TMVA::Factory* factory = new TMVA::Factory( "TMVA Analysis", outputFile);

TFile *input1 = TFile::Open( "mc16ade_13TeV.311315.MadGraphPythia8EvtGen_A14NNPDF31LO_HSS_LLP_mH125_mS55_lthigh_DIST_NEW.root"  );
TTree* sigTree = (TTree*)input1->Get( "BDTFVariables" );
    //Filter for barrel or endcap
    TTree* originalSigTree = sigTree;
    TCut sigCut = "((nHits !=0) && (DetectorRegion==1) && (!std::isnan(Tracklet_avg_dR)) && (!std::isnan(Segment_avg_dR)) && (!std::isinf(Tracklet_avg_dR)) && (!std::isinf(Segment_avg_dR)))";
    
TFile *input2 = TFile::Open("data15_13TeV.physics_Main_DIST_NEW.root" );
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

//Single Model

dataloader->PrepareTrainingAndTestTree( sigCut, backCut, 
    "nTrain_Signal=0:nTrain_Background=0:SplitMode=Random:NormMode=NumEvents:!V" );

// factory->BookMethod( dataloader, TMVA::Types::kBDT, "BDTF",
//      "!H:!V:NTrees=50:MinNodeSize=2.5%:UseFisherCuts:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.5");

//Multiple Model Grid Search

array<float,3> nTrees{ 30, 50, 100 };
array<float,3> AdaBoostBeta{ 0.2, 0.5, 1.0 };


string title="";
string params="";

for (int i = 0; i < nTrees.size(); ++i) {
  for (int j = 0; j < AdaBoostBeta.size(); ++j){
    title = "BDTF Ntrees=" + std::to_string(nTrees[i]) + ", AdaBoostBeta=" + std::to_string(AdaBoostBeta[j]);
    params =  "!H:!V:NTrees=" + std::to_string(nTrees[i]) + ":MinNodeSize=2.5%:UseFisherCuts:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=" + std::to_string(AdaBoostBeta[j]) + ":SeparationType=GiniIndex:nCuts=20";
    factory->BookMethod( dataloader, TMVA::Types::kBDT, title, params);
  }
}


factory->TrainAllMethods();
factory->TestAllMethods();
factory->EvaluateAllMethods();
originalSigTree->Write();
outputFile->Write();
outputFile->Close();

//TMVA::TMVAGui("TMVAVarDistBarrel_mS5.root");

delete factory;
 
}
