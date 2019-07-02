import uproot as up
import seaborn as sns
import numpy as np
from sklearn.discriminant_analysis import LinearDiscriminantAnalysis
from sklearn.model_selection import train_test_split
from sklearn.ensemble import AdaBoostClassifier as ada
from sklearn.tree import DecisionTreeClassifier
from sklearn.metrics import confusion_matrix

rootfile1 = up.open(r"C:\Users\ashbat\Desktop\ROOT Files\MVAvariables_data_withMuons_trackpTCut.root")
rootfile2 = up.open(r"C:\Users\ashbat\Desktop\ROOT Files\MVAvariables_mH125mS5_withMuons_trackpTCut.root")
tree1 = rootfile1["MVAvariables"]
tree2 = rootfile2["MVAvariables"]
dfbackground = tree1.pandas.df()
dfbackground = dfbackground[dfbackground['DetectorRegion']==0]
dfsignal = tree2.pandas.df()
dfsignal = dfsignal[dfsignal['DetectorRegion']==0]

signal = dfsignal[['nHits','Tracklet_avg_dR','Tracklet_RMS_dR','Segment_avg_dR','Segment_RMS_dR','Segment_sumpT','Segment_Sphericity']]
background = dfbackground[['nHits','Tracklet_avg_dR','Tracklet_RMS_dR','Segment_avg_dR','Segment_RMS_dR','Segment_sumpT','Segment_Sphericity']]
frames = [signal,background]
X = pd.concat(frames)
signal['label']='signal'
background['label']='background'
frames = [signal,background]
data=pd.concat(frames)
y=data['label']

y=y.values
y=pd.DataFrame(y)
cmtotal=np.zeros((2,2))

#X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2)
for i in range(20):
    #select 1000 signal and 1000 background data entries
    signalindices = np.random.choice(signal.index,size=2000)
    training_idx, test_idx = signalindices[:1000], signalindices[1000:]
    signal_train = signal.loc[training_idx,:]
    signal_test = signal.loc[test_idx,:]
    backgroundindices = np.random.choice(background.index,size=2000)
    training_idx, test_idx = backgroundindices[:1000], backgroundindices[1000:]
    background_train = background.loc[training_idx,:]
    background_test = background.loc[test_idx,:]
    X_train = pd.concat([signal_train,background_train])
    X_test = pd.concat([signal_test,background_test])

    #populate y with 1000 'signal' labels and 1000 'background labels'
    y_train = X_train['label']
    y_test = X_test['label']
    X_train = X_train.drop('label',axis=1)
    X_test = X_test.drop('label',axis=1)

    clf = LinearDiscriminantAnalysis()
    
    Xtransform = clf.fit_transform(X_train, y_train)
    Xtransformdf = pd.DataFrame(Xtransform)
    Xtesttransformdf = clf.transform(X_test)
   

    #Default parameters: n_estimators=50, learning_rate=1
    paramgrid = [ {'n_estimators': [1000], 'learning_rate':[0.01,0.1,1] } ]

    bdt = ada(DecisionTreeClassifier())
    bdt.fit(Xtransformdf, y_train)
    predict = bdt.predict(Xtesttransformdf)
    cm=confusion_matrix(predict,y_test)
    cmtotal=np.add(cmtotal,cm)
    
cmtotal

#make a plot of scores for each combo of estimators and learning ratew