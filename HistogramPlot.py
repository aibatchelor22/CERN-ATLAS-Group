import plotly.graph_objs as go
import pylab as plt
import plotly.plotly as py
import plotly.tools as tls

labeledData = pd.DataFrame(Xtesttransformdf)
labeledData['label']=predict
labeledData[labeledData['label']=='background']

signalpredict = labeledData[labeledData['label']=='signal'].drop('label',axis=1)
backgroundpredict = labeledData[labeledData['label']=='background'].drop('label',axis=1)

trace1 = go.Histogram(
    x=clf.transform(signal_test.drop('label', axis=1)),
    opacity=0.75,  name='Signal - Test', histnorm='probability'
)
trace2 = go.Histogram(
    x=clf.transform(background_test.drop('label', axis=1)),
    opacity=0.75, name='Background - Test', histnorm='probability'
)
trace3 = go.Histogram(
    x=signalpredict.values,
    opacity=0.75, name='Signal - Predicted',histnorm='probability'
)
trace4 = go.Histogram(
    x=backgroundpredict.values,
    opacity=0.75, name='Background - Predicted', histnorm='probability'
)


data = [trace1,trace2,trace3,trace4]
layout = go.Layout()
fig = go.Figure(data=data, layout=layout)

py.iplot(fig, filename='overlaid histogram')