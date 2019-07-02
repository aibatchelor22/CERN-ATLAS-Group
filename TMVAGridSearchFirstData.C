#include <string>

void TMVAGridSearchFirstData()
{
TFile* outputFile = TFile::Open( "/mnt/c/Users/ashbat/Desktop/ROOT Files/TMVAOutput.root", "RECREATE" );

TMVA::Factory* factory = new TMVA::Factory( "TMVA Analysis", outputFile);

TFile *input1 = TFile::Open( "MVAvariables_data_withMuons_trackpTCut.root" );
TTree* sigTree = (TTree*)input1->Get( "MVAvariables" );
    //Filter for barrel or endcap
    TCut cut = "DetectorRegion == 0";
    TTree *sigTreeFiltered = sigTree->CopyTree(cut);
TFile *input2 = TFile::Open( "MVAvariables_mH125mS40_withMuons_trackpTCut.root" );
TTree* backTree = (TTree*)input2->Get( "MVAvariables" );
    //Filter for barrel or endcap
    TTree *backTreeFiltered = backTree->CopyTree(cut);

TMVA::DataLoader *dataloader=new TMVA::DataLoader("dataset");

dataloader->AddVariable( "nHits", 'F' );
dataloader->AddVariable( "Tracklet_avg_dR", 'F' );
dataloader->AddVariable( "Tracklet_RMS_dR", 'F' );
dataloader->AddVariable( "Segment_avg_dR", 'F' );
dataloader->AddVariable( "Segment_RMS_dR", 'F' );
dataloader->AddVariable( "Segment_sumpT", 'F' );
dataloader->AddVariable( "Segment_Sphericity", 'F' );

dataloader->AddSignalTree ( sigTreeFiltered, 1.0 );
dataloader->AddBackgroundTree ( backTreeFiltered, 1.0 );

//factory->BookMethod( dataloader, TMVA::Types::kFisher, "Fisher Default Settings");

//factory->BookMethod( dataloader, TMVA::Types::kBDT, "BDTF",
//                           "!H:!V:NTrees=50:MinNodeSize=2.5%:UseFisherCuts:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.5:SeparationType=GiniIndex:nCuts=20")

//"H:!V:Fisher:VarTransform=None:CreateMVAPdfs:PDFInterpolMVAPdf=Spline2:NbinsMVAPdf=50:NsmoothMVAPdf=10")
array<float,3> nTrees{ 10, 50, 100 };
array<float,3> AdaBoostBeta{ 0.1, 0.5, 1.0 };
string params="";
string title="";

for (int i = 0; i < nTrees.size(); ++i) {
  for (int j = 0; j < AdaBoostBeta.size(); ++j) {
    params="";
    params = "NTrees=" + std::to_string(nTrees[i]) + ":UseFisherCuts:BoostType=AdaBoost:AdaBoostBeta=" + std::to_string(AdaBoostBeta[j]) + "\"";
    title = "NTrees = " + std::to_string(nTrees[i]) + " AdaBoostBeta=" + std::to_string(AdaBoostBeta[j]);
    factory->BookMethod( dataloader, TMVA::Types::kBDT, title, params);
  }
}

//factory->OptimizeAllMethods();

factory->TrainAllMethods();

factory->TestAllMethods();

factory->EvaluateAllMethods();

outputFile->Close();

TMVA::TMVAGui("TMVAOutput.root");

delete factory;
 
}