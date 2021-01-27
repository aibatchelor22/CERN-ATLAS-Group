Int_t getDetectorRegion(double eta, double lxy, double lz){
  // ** Legend                                                                                                                                                      
  // 0 = MS BARREL                                                                                                                                                  
  // 1 = MS ENDCAPS                                                                                                                                                 
  // 2 = ID or calorimeters                                                                                                                                         
  // 3 = outside ATLAS                                                                                                                                              
  if      (fabs(eta) < 0.7 && lxy > 3000 && lxy < 8000) return 0;
  else if (fabs(eta) > 1.3 && fabs(eta) < 2.5 && lxy < 10000 && fabs(lz) > 5000 && fabs(lz) < 15000 ) return 1;
  else if (lxy < 3000 && fabs(lz) < 5000) return 2;
  else if (fabs(eta) < 1.0 && lxy > 8000) return 3;
  else if (fabs(eta) > 1.0 && ( (lxy > 10000 && fabs(lz) > 5000 && fabs(lz) < 15000) || fabs(lz) > 15000) ) return 3;
  else return -1;
}

double deltaPhi(double phi1, double phi2){
  double dPhi = phi1-phi2;
  static const double M_2PI = 2*M_PI;
  while (dPhi> M_PI) dPhi -= M_2PI;
  while (dPhi<-M_PI) dPhi += M_2PI;
  return dPhi;
}

double deltaEta(double eta1, double eta2){
  return eta1-eta2;
}

double deltaR(double dEta, double dPhi){
  return sqrt(dEta*dEta+dPhi*dPhi);
}

Int_t isVtxRoIMatched(double vtx_eta, double vtx_phi, vector<float> *roi_eta, vector<float> *roi_phi)
{
  
  bool isMatched=false;
  for (int idx=0;idx<roi_eta->size();idx++)
    {
      double dr = deltaR( deltaEta(vtx_eta,roi_eta->at(idx)) , deltaPhi(vtx_phi,roi_phi->at(idx)) );
      if (dr<0.4) isMatched=true;
    }
  if (isMatched) return 1;
  return 0;
}



void AddBDTFVariables(){

// const int cp_err = gSystem->Exec("cp data15_13TeV.physics_Main.root data15_13TeV.physics_Main_DIST_BAR_NEW.root");
//   if (-1 == cp_err){ printf("Error copying input file!n"); exit(1); }

//   TFile *input1 = TFile::Open( "data15_13TeV.physics_Main_DIST_BAR_NEW.root", "UPDATE" );

const int cp_err = gSystem->Exec("cp mc16ade_13TeV.311310.MadGraphPythia8EvtGen_A14NNPDF31LO_HSS_LLP_mH125_mS5_lthigh.root mc16ade_13TeV.311310.MadGraphPythia8EvtGen_A14NNPDF31LO_HSS_LLP_mH125_mS5_lthigh_DIST_EC_NEW.root");
  if (-1 == cp_err){ printf("Error copying input file!n"); exit(1); }

     TFile *input1 = TFile::Open( "mc16ade_13TeV.311310.MadGraphPythia8EvtGen_A14NNPDF31LO_HSS_LLP_mH125_mS5_lthigh_DIST_EC_NEW.root", "UPDATE" );
     TTree* inputTree = (TTree*)input1->Get( "trees_MS_" );
     TTree* outputTree = new TTree("BDTFVariables","BDTFVariables");

  gROOT->ProcessLine("#include <vector>");

  //Declare all variables
  Double_t nHits = 0;
  Double_t Tracklet_avg_dR = 0;
  Double_t Segment_avg_dR = 0;
  Double_t Segment_Sphericity = 0;
  vector<double>* MSVtx_eta=0; 
  vector<double>* MSVtx_phi=0;
  vector<double>* MSVtx_R=0;
  vector<double>* MSVtx_z=0; 
  Double_t Tracklet_dEta;
  vector<double>* MSTracklet_phi=0;
  Double_t Tracklet_dPhi;
  Double_t Tracklet_dR;
  vector<double>* MSTracklet_eta=0;
  Double_t Segment_dEta;
  vector<double>* MSeg_phiPos=0;
  Double_t Segment_dPhi;
  Double_t Segment_dR;
  vector<float>* MSeg_px=0;
  vector<float>* MSeg_py=0;
  vector<float>* MSeg_pz=0;
  vector<double>* MSeg_etaPos=0;
  vector<int>* MSVtx_nMDT=0;
  vector<int>* MSVtx_nRPC=0;
  vector<int>* MSVtx_nTGC=0;
  Double_t Segment_Aplanarity;
  Double_t Segment_TransverseSphericity;
  Double_t Segment_sumpT=0;
  Int_t DetectorRegion=0;
  vector<float>* L1Muon_eta=0;
  vector<float>* L1Muon_phi=0;
  Bool_t isGoodLAr;
  Bool_t isGoodSCT;
  Bool_t isGoodTile;
  Char_t hasGoodPV;
  Bool_t isCompleteEvent;
  Int_t pass_HLT_j30_muvtx_noiso;
  Bool_t ismatched;
  Double_t met_met;
  Double_t Tracklet_den;
  Double_t Segment_den;
  Double_t Segment_avg_dist;
  Double_t MSVtx_x;
  Double_t MSVtx_y;
  vector<double>* MSeg_x = 0;
  vector<double>* MSeg_y = 0;
  vector<double>* MSeg_z = 0;
  
 
  //Create new branches for calculated variables
  TBranch *bTavg = outputTree->Branch("Tracklet_avg_dR",&Tracklet_avg_dR,"Tracklet_avg_dR/D");
  TBranch *bSavg = outputTree->Branch("Segment_avg_dR",&Segment_avg_dR,"Segment_avg_dR/D");
  TBranch *bnHits = outputTree->Branch("nHits",&nHits,"nHits/D");
  TBranch *bsegspher = outputTree->Branch("Segment_Sphericity",&Segment_Sphericity,"Segment_Sphericity/D");
  TBranch *bsegsumpt = outputTree->Branch("Segment_sumpT",&Segment_sumpT,"Segment_sumpT/D");
  TBranch *bdr = outputTree->Branch("DetectorRegion",&DetectorRegion,"DetectorRegion/I");
  TBranch *bsegdist = outputTree->Branch("Segment_avg_dist",&Segment_avg_dist,"Segment_avg_dist/D");

  //Create branch addresses
  outputTree->SetBranchAddress("Tracklet_avg_dR",&Tracklet_avg_dR); //calculated
  outputTree->SetBranchAddress("Segment_avg_dR",&Segment_avg_dR); //calculated
  inputTree->SetBranchAddress("MSVtx_eta",&MSVtx_eta); //In root file
  inputTree->SetBranchAddress("MSVtx_phi",&MSVtx_phi); //In root file
  inputTree->SetBranchAddress("MSVtx_R",&MSVtx_R); //In root file
  inputTree->SetBranchAddress("MSVtx_z",&MSVtx_z); //In root file
  inputTree->SetBranchAddress("MSTracklet_phi",&MSTracklet_phi); //In root file
  inputTree->SetBranchAddress("MSTracklet_eta",&MSTracklet_eta); //In root file
  inputTree->SetBranchAddress("MSeg_phiPos",&MSeg_phiPos); //In root file
  inputTree->SetBranchAddress("MSeg_px",&MSeg_px); //In root file
  inputTree->SetBranchAddress("MSeg_py",&MSeg_py); //In root file
  inputTree->SetBranchAddress("MSeg_pz",&MSeg_pz); //In root file
  inputTree->SetBranchAddress("MSeg_etaPos",&MSeg_etaPos); //In root file
  inputTree->SetBranchAddress("MSVtx_nMDT",&MSVtx_nMDT); //In root file
  inputTree->SetBranchAddress("MSVtx_nRPC",&MSVtx_nRPC); //In root file
  inputTree->SetBranchAddress("MSVtx_nTGC",&MSVtx_nTGC); //In root file
  outputTree->SetBranchAddress("Segment_Sphericity",&Segment_Sphericity); //calculated
  outputTree->SetBranchAddress("nHits",&nHits); //calculated
  outputTree->SetBranchAddress("DetectorRegion",&DetectorRegion); //calculated
  outputTree->SetBranchAddress("Segment_sumpT",&Segment_sumpT); //calculated
  inputTree->SetBranchAddress("L1Muon_eta",&L1Muon_eta); //In root file
  inputTree->SetBranchAddress("L1Muon_phi",&L1Muon_phi); //In root file
  inputTree->SetBranchAddress("isGoodLAr",&isGoodLAr); //In root file
  inputTree->SetBranchAddress("isGoodSCT",&isGoodSCT); //In root file
  inputTree->SetBranchAddress("isGoodTile",&isGoodTile); //In root file
  inputTree->SetBranchAddress("hasGoodPV",&hasGoodPV); //In root file
  inputTree->SetBranchAddress("isCompleteEvent",&isCompleteEvent); //In root file
  inputTree->SetBranchAddress("pass_HLT_j30_muvtx_noiso",&pass_HLT_j30_muvtx_noiso); //In root file
  inputTree->SetBranchAddress("met_met",&met_met); //In root file
  outputTree->SetBranchAddress("Segment_avg_dist",&Segment_avg_dist); //calculated
  inputTree->SetBranchAddress("MSeg_x",&MSeg_x); //In root file
  inputTree->SetBranchAddress("MSeg_y",&MSeg_y); //In root file
  inputTree->SetBranchAddress("MSeg_z",&MSeg_z); //In root file
  

  Long64_t nentries = inputTree->GetEntries();
   
  //Populate tree with calculated variables
  for (Long64_t i_entries=0;i_entries<nentries;i_entries++) {

    inputTree->GetEntry(i_entries);

    //Select even or odd events
    //if (i_entries%2==1) continue;

    //Conditions to be met, otherwise skip these values
    //if ( !(isGoodLAr && isGoodSCT && isGoodTile && hasGoodPV && isCompleteEvent && pass_HLT_j30_muvtx_noiso) ) continue;
    if (MSVtx_eta->size()!=1) continue;

    if ( isVtxRoIMatched(MSVtx_eta->at(0),MSVtx_phi->at(0),L1Muon_eta,L1Muon_phi)==0 ) continue;

    //Cut ONLY for background
    //if (met_met > 40000) continue;

    //Calculate Tracklet variables
    Tracklet_avg_dR = 0;
    Tracklet_den = 0;
     for (int i_traklet=0;i_traklet<MSTracklet_eta->size();i_traklet++)
     {
        for (int i_vtx=0;i_vtx<MSVtx_eta->size();i_vtx++){
            Tracklet_dEta = fabs(deltaEta(MSVtx_eta->at(i_vtx),MSTracklet_eta->at(i_traklet)));
            Tracklet_dPhi = fabs(deltaPhi(MSVtx_phi->at(i_vtx),MSTracklet_phi->at(i_traklet)));
            Tracklet_dR = deltaR(Tracklet_dEta,Tracklet_dPhi);
            if ( fabs(Tracklet_dEta)<0.7 && fabs(Tracklet_dPhi)<1.0){ 
                Tracklet_avg_dR+=Tracklet_dR;
                Tracklet_den++;
                }
        }
     }
    Tracklet_avg_dR/=Tracklet_den; 
                                                                                                                                                                                                                             
    //Calculate Segment variables 
    
    vector<double> Segment_p(3,0.0);
    vector<double> Segment_sphericity_num(9,0.0);
    double Segment_sphericity_den=0;
    Segment_avg_dR = 0;
    Segment_den=0;
    double Segment_dist = 0;
        for (int i_segment=0;i_segment<MSeg_etaPos->size();i_segment++)
     {
        for (int i_vtx=0;i_vtx<MSVtx_eta->size();i_vtx++){ 
            Segment_dEta = fabs(deltaEta(MSVtx_eta->at(i_vtx),MSeg_etaPos->at(i_segment)));
            Segment_dPhi = fabs(deltaPhi(MSVtx_phi->at(i_vtx),MSeg_phiPos->at(i_segment)));
            Segment_dR = deltaR(Segment_dEta,Segment_dPhi);
            MSVtx_x = MSVtx_R->at(i_vtx) * cos(MSVtx_phi->at(i_vtx));
            MSVtx_y = MSVtx_R->at(i_vtx) * sin(MSVtx_phi->at(i_vtx));
            //Segment_dist = std::sqrt((MSeg_x->at(i_segment)-MSVtx_x)*(MSeg_x->at(i_segment)-MSVtx_x) + (MSeg_y->at(i_segment)-MSVtx_y)*(MSeg_y->at(i_segment)-MSVtx_y) );
            Segment_dist = fabs((MSeg_z->at(i_segment)-MSVtx_z->at(i_vtx))*(MSeg_z->at(i_segment)-MSVtx_z->at(i_vtx)));
            
            if (fabs(Segment_dEta)<0.7 && fabs(Segment_dPhi)<1.0){ 
                Segment_avg_dR+=Segment_dR;
                Segment_avg_dist+=Segment_dist;
                Segment_den++;
            }
        }
                                                                                                                                                                                                                                
        Segment_p[0]=MSeg_px->at(i_segment); Segment_p[1]=MSeg_py->at(i_segment); Segment_p[2]=MSeg_pz->at(i_segment);
        int m_idx=0;
        for (int i=0;i<3;i++)
        for (int j=0;j<3;j++){
            Segment_sphericity_num[m_idx]+=Segment_p[i]*Segment_p[j];
            m_idx++;
        }
        Segment_sphericity_den+=(Segment_p[0]*Segment_p[0]+Segment_p[1]*Segment_p[1]+Segment_p[2]*Segment_p[2]);
     
    }
    Segment_avg_dR/=Segment_den; 
    Segment_avg_dist/=Segment_den;
                                                                                                                                                                                                                                     
    // Calculate Segment sphericity                                                                                                                                                                                                     
    double Segment_array[9];
    for (int i=0;i<9;i++) Segment_array[i]=Segment_sphericity_num[i]/Segment_sphericity_den;
    TMatrixDSym Segment_matrix(3,Segment_array);
    TMatrixDSymEigen Segment_eigen(Segment_matrix);
    TVectorD Segment_eigenVal = Segment_eigen.GetEigenValues();

    double Segment_lambda_3=99;
    double Segment_lambda_2=99;
    double Segment_lambda_1=99;
    for (int i=0;i<3;i++) if (Segment_eigenVal[i]<Segment_lambda_3) Segment_lambda_3=Segment_eigenVal[i];
    for (int i=0;i<3;i++) if (Segment_eigenVal[i]<Segment_lambda_2 && Segment_eigenVal[i]!=Segment_lambda_3) Segment_lambda_2=Segment_eigenVal[i];
    for (int i=0;i<3;i++) if (Segment_eigenVal[i]<Segment_lambda_1 && Segment_eigenVal[i]!=Segment_lambda_3 && Segment_eigenVal[i]!=Segment_lambda_2) Segment_lambda_1=Segment_eigenVal[i];
    Segment_Sphericity = (Segment_lambda_3+Segment_lambda_2)*3/2;
    Segment_TransverseSphericity = 2*Segment_lambda_2/(Segment_lambda_3+Segment_lambda_2);
    Segment_Aplanarity = 3/2*Segment_lambda_3;

    //Calculate Segment_sumpT
    Segment_sumpT = 0;
    for (int i_pt=0;i_pt<MSeg_px->size();i_pt++){
      Segment_sumpT += std::sqrt(MSeg_px->at(i_pt)*MSeg_px->at(i_pt) + MSeg_py->at(i_pt)*MSeg_py->at(i_pt));
    }

    //Get rid of any zero, +inf or -inf values
    if (std::isinf(Segment_avg_dR)) continue;
    if (std::isinf(Tracklet_avg_dR)) continue;
    if (std::isinf(Segment_avg_dist)) continue;
    if (std::isnan(Segment_avg_dR)) continue;
    if (std::isnan(Tracklet_avg_dR)) continue;
    if (std::isnan(Segment_avg_dist)) continue;

    //Assign DetectorRegion
    DetectorRegion=getDetectorRegion(MSVtx_eta->at(0), MSVtx_R->at(0), MSVtx_z->at(0));

    //Calculate nHits
    nHits = accumulate(MSVtx_nMDT->begin(),MSVtx_nMDT->end(),0) + accumulate(MSVtx_nRPC->begin(),MSVtx_nRPC->end(),0)+ accumulate(MSVtx_nTGC->begin(),MSVtx_nTGC->end(),0); 

    outputTree->Fill();
  }

  outputTree->Print();
  outputTree->Write();
  delete input1;

}
