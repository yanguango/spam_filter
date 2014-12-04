from __future__ import print_function
from numpy.linalg import *
from numpy import *
from collections import OrderedDict

def mvnpdf(x, mu, sigma):
    if not sigma.shape:
        sigma = reshape(sigma, (1, 1))
    det = linalg.det(sigma)
    print("det------%.256f" % det)
    if det == 0:
        return 0
    k = x.shape[0]
    dev = x - mu
    inv_sigma = inv(sigma)
    desc = dot(dev, dot(inv_sigma, transpose(dev)))
    c0 = (2*pi)**(-0.5*k)
    c1 = 8.81852703658386e+23
    print("c0----%.256f" % c0)
    print("c1----%.256f" % c1)
    return c0*c1*exp(-0.5*desc)

def classify(message):
    features_file = open('data/features.txt', 'r')
    features = features_file.read().split(',')
    feature_dict = OrderedDict((el,0) for el in features)
    words = message.split(' ')
        
    for word in words:
        if(word in features):
            feature_dict[word] = feature_dict[word] + 1

    for k in feature_dict:
        feature_dict[k] = feature_dict[k] * 100.0 / len(words)
    
    pf = open('data/P.txt', 'r')
    p = matrix([map(float, line.split(',')) for line in pf ])
    mean1f = open('data/mean1.txt' , 'r')
    mean1 = matrix([map(float, line.split(',')) for line in mean1f][0])
    # mean1 = asarray(mean1).reshape(-1)
    mean2f = open('data/mean2.txt' , 'r')
    mean2 = matrix([map(float, line.split(',')) for line in mean2f][0])
    # mean2 = asarray(mean2).reshape(-1)

    covm1f = open('data/covm1.txt' , 'r')
    covm1 = matrix([map(float,line.split(',')) for line in covm1f])
    covm2f = open('data/covm2.txt' , 'r')
    covm2 = matrix([map(float,line.split(',')) for line in covm2f])
    pf.close()
    mean1f.close()
    mean2f.close()
    covm1f.close()
    covm2f.close()

    x = matrix.transpose(matrix(feature_dict.values()))
    
    y = transpose(dot(p, x))
    y = asarray(y).reshape(-1)
    prior1 = 3672.0 / (3672 + 1500)
    prior2 = 1500.0 / (3672 + 1500)

    lkl1 = mvnpdf(y, mean1, covm1)
    lkl2 = mvnpdf(y, mean2, covm2)
    print(lkl1)
    print(lkl2)
    post1 = lkl1 * prior1 
    post2 = lkl2 * prior2
   
    if post1 > post2:
        return 0
    else:
        return 1
