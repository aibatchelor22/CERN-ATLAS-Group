# Boosted Decision Trees for Separating Signal from Background Measurements in Search for Long Lived, Weakly Interacting Particles in pp Collisions at sqrt(s) = 13 TeV 
## Ashley Batchelor, University of Washington Department of Physics 
## Final Project for M.S. Physics


I used boosted decision trees to develop models for separating signal and background data of measurements of pp collision events taken by the ATLAS detector of the Large Hadron Collider. Such models are necessary in the search for long lived, weakly interacting decay products of pp collisions. In order to train the weights of the models, signal data was provided by Monte Carlo simulations of scalar particle (e.g. Higgs Boson) decays, and background data was provided by ATLAS detector data with feature selection based on missing energy values of less than 40 GeV. 



## Built With

Visual Studio Code, CERN ROOT, Toolkit for Multivariable Analysis (TMVA), Geant4, Pythia 8.210, MadGraph5

Please note, the *.C files in this repository cannot be compiled and executed as standalone programs.  Rather, they must be executed as macros within the ROOT software used by CERN.

https://root.cern/

https://root.cern/manual/tmva/


## Files

AddBDTFVariables.C

	Feature engineering from raw data output by Monte Carlo simulations

Ashley Batchelor Final Paper.pdf 

	Capstone project paper submitted to advisory committee for M.S. Physics.

TMVAAshleyGrid.C

	Boosted Decision Tree models with hyperparameter tuning by grid search.


## Author

Ashley Batchelor
https://github.com/aibatchelor22/


## Acknowledgments

This work was prepared under the supervision of Henry Lubatti of the University of Washington ATLAS Group, with much help from Cristiano Alpigiani at CERN in Geneva. 
