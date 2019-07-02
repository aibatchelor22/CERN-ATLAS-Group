void TMVAAnalysisIsolationAshley( )
{
TFile* outputFile = TFile::Open( "/mnt/c/Users/ashbat/Desktop/ROOT Files/TMVAIsolationOptimizedOutput.root", "RECREATE" );

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






dataloader->AddVariable( "Isolation_Trk_pT4", 'F' );
dataloader->AddVariable( "Isolation_Jet", 'F' );
dataloader->AddVariable( "Isolation_Muon", 'F' );
dataloader->AddVariable( "Isolation_LowpT_lower10", 'F' );


dataloader->AddSignalTree ( sigTreeFiltered, 1.0 );
dataloader->AddBackgroundTree ( backTreeFiltered, 1.0 );

factory->BookMethod( dataloader, TMVA::Types::kFisher, "Fisher", "H:!V:Fisher:VarTransform=None:CreateMVAPdfs:PDFInterpolMVAPdf=Spline2:NbinsMVAPdf=50:NsmoothMVAPdf=10" );

factory->BookMethod( dataloader, TMVA::Types::kBDT, "BDTF", "!H:!V:NTrees=50:MinNodeSize=2.5%:UseFisherCuts:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.5:SeparationType=GiniIndex:nCuts=20" );

factory->OptimizeAllMethods();

factory->TrainAllMethods();

factory->TestAllMethods();

factory->EvaluateAllMethods();

outputFile->Close();

TMVA::TMVAGui("TMVAIsolationOptimizedOutput.root");

delete factory;
 
}