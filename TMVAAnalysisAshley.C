void TMVAAnalysisAshley( )
{
TFile* outputFile = TFile::Open( "/mnt/c/Users/ashbat/Desktop/ROOT Files/TMVAOutputDets5Reg1.root", "RECREATE" );

TMVA::Factory* factory = new TMVA::Factory( "TMVA Analysis", outputFile);

TFile *input1 = TFile::Open( "/mnt/c/Users/ashbat/Desktop/ROOT Files/MVAvariables_mH125mS5_withMuons_trackpTCut.root"  );
TTree* sigTree = (TTree*)input1->Get( "MVAvariables" );
    //Filter for barrel or endcap
    TCut sigCut = "DetectorRegion == 1";
    // TTree *sigTreeFiltered = sigTree->CopyTree(cut);
TFile *input2 = TFile::Open("/mnt/c/Users/ashbat/Desktop/ROOT Files/MVAvariables_data_withMuons_trackpTCut.root" );
TTree* backTree = (TTree*)input2->Get( "MVAvariables" );
    //Filter for barrel or endcap
    TCut backCut = "DetectorRegion == 1";
    //TTree *backTreeFiltered = backTree->CopyTree(cut);

TMVA::DataLoader *dataloader=new TMVA::DataLoader("dataset");

dataloader->AddVariable( "nHits", 'F' );
dataloader->AddVariable( "Tracklet_avg_dR", 'F' );
dataloader->AddVariable( "Tracklet_RMS_dR", 'F' );
dataloader->AddVariable( "Segment_avg_dR", 'F' );
dataloader->AddVariable( "Segment_RMS_dR", 'F' );
dataloader->AddVariable( "Segment_sumpT", 'F' );
dataloader->AddVariable( "Segment_Sphericity", 'F' );

// dataloader->AddSignalTree ( sigTreeFiltered, 1.0 );
// dataloader->AddBackgroundTree ( backTreeFiltered, 1.0 );

dataloader->AddSignalTree ( sigTree, 1.0 );
dataloader->AddBackgroundTree ( backTree, 1.0 );

dataloader->PrepareTrainingAndTestTree( sigCut, backCut, 
                                           "nTrain_Signal=0:nTrain_Background=0:SplitMode=Random:NormMode=NumEvents:!V" );

//factory->BookMethod( dataloader, TMVA::Types::kFisher, "Fisher Default Settings");

//factory->BookMethod( dataloader, TMVA::Types::kBDT, "BDTF",
                           //"!H:!V:NTrees=50:MinNodeSize=2.5%:UseFisherCuts:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.5:SeparationType=GiniIndex:nCuts=20");


array<float,3> nTrees{ 10, 50, 100 };
array<float,3> AdaBoostBeta{ 0.1, 0.5, 1.0 };
string title="";
string params="";

for (int i = 0; i < nTrees.size(); ++i) {
  for (int j = 0; j < AdaBoostBeta.size(); ++j){
    title = "BDTF Ntrees=" + std::to_string(nTrees[i]) + ", AdaBoostBeta=" + std::to_string(AdaBoostBeta[j]);
    params =  "!H:!V:NTrees=" + std::to_string(nTrees[i]) + ":MinNodeSize=2.5%:UseFisherCuts:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=" + std::to_string(AdaBoostBeta[j]) + ":SeparationType=GiniIndex:nCuts=20";
    factory->BookMethod( dataloader, TMVA::Types::kBDT, title, params);
  }
}

//factory->OptimizeAllMethods();

factory->TrainAllMethods();

factory->TestAllMethods();

factory->EvaluateAllMethods();

outputFile->Close();

TMVA::TMVAGui("/mnt/c/Users/ashbat/Desktop/ROOT Files/TMVAOutputDets5Reg1.root");

delete factory;
 
}