from pymatbridge import Matlab
mlab = Matlab(matlab='/Applications/MATLAB_R2014a.app/bin/matlab')
mlab.start()
res = mlab.run('/Users/yanguango/Lab/SpamFilter/Project/src/PCA.m', {'arg1': 3, 'arg2': 5})
print res['result']
