# other.py
# 决策树
import math
import numpy as np
from collections import Counter


class TreeNode:
    def __init__(self, nodetype):
        self.type = nodetype
        self.feature = []
        self.nextnode = {}


def C(label):
    return dict(Counter(label))


def Di(data, feature):
    return dict(Counter(data[:, feature]))


def Dik(data, label, feature, i, k,n):
    res = 0
    for p in range(n):
        if data[p, feature] == i and label[p] == k:
            res += 1
    return res


def H(label,n):
    res, c = 0, C(label)
    for i in c:
        res -= c[i] / n * math.log2(c[i] / n)
    return res


def Hda(data, label, feature,n):
    res, di, c = 0, Di(data, feature), C(label)
    for i in di:
        res1 = 0
        for k in c:
            temp = Dik(data, label, feature, i, k, n) / di[i]
            if temp == 0:
                res1 += 0
            else:
                res1 += temp * math.log2(temp)
        res -= di[i] / n * res1
    return res


def Gda(data, label, feature,n):
    return H(label,n) - Hda(data, label, feature,n)

def create_tree(data, label, A, threhold):
        n, m = data.shape
        c = C(label)
        if len(c) == 1 or len(A) == 0:
            node = TreeNode("leaf")
            node.mark = list(c.keys())[0]
        else:
            g = []
            for i in A:
                g.append((Gda(data,label,i,n), i))
            g.sort()
            maxg, bestfeature = g[-1][0], g[-1][1]
            if maxg < threhold:
                node = TreeNode("leaf")
                node.mark = list(c.keys())[0]
            else:
                Ddict = {}
                di = Di(data,bestfeature)
                for i in di:
                    Ddict[i] = []
                for j in range(n):
                    Ddict[data[j, bestfeature]].append(j)
                
                mostkind = list(di.keys())[0]
                node = TreeNode("node")
                node.feature = bestfeature
                node.value = mostkind
                node.mark = list(dict(Counter(label[Ddict[mostkind]])).keys())[0]
                node.nextnode = {}
                di.pop(mostkind)
                for i in di:
                    node.nextnode[i] = create_tree(data[Ddict[i],:], label[Ddict[i]],A, threhold)
        return node

def dt(trainset, trainlabel, testset):

    data = trainset.values
    label = trainlabel.values
    n,m = data.shape
    A = [i for i in range(m)]
    T = create_tree(data, label, A, 0.1)
    
    res = {}

    def search_tree(DT, test):
        if DT.type == "leaf":
            return DT.mark
        else:
            fe = DT.feature
            if test[fe] == DT.value or test[fe] not in DT.nextnode:
                return DT.mark
            else:
                return search_tree(DT.nextnode[test[fe]], test)

    for i in testset.index:
        #print(i)
        t = testset.loc[i].values
        #print(t)
        res[i] = search_tree(T, t)
    return res

    
    





