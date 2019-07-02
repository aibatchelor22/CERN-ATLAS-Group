import pandas as pd
import uproot as up
import seaborn as sns

rootfile = up.open(r"C:\Users\ashbat\Desktop\ROOT Files\MVAvariables_data_withMuons_trackpTCut.root")
tree2 = rootfile["MVAvariables"]
dfsignal = tree2.pandas.df()
dfsignal = dfsignal[dfsignal['DetectorRegion']==0]
corr = dfsignal.corr()
import matplotlib.pyplot as plt
fig, ax = plt.subplots(figsize=(30,30))
sns.heatmap(corr, annot=True,  
            cmap=plt.get_cmap('coolwarm'), cbar=False, ax=ax, xticklabels=True, yticklabels=True)
ax.set_yticklabels(ax.get_yticklabels(), rotation="horizontal")
plt.savefig('BackgroundCorrelation_Barrel.png', bbox_inches='tight', pad_inches=0.0)