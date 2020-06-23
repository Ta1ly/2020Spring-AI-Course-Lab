# KNN.py



def get_distance(data1, data2, n):
    # Euclidean distance
    res = 0
    for i in range(n):
        d = data2[i] - data1[i]
        res += d * d
    return res ** 0.5


def knn(trainset, trainlabel, testset, k_list):
    n = testset.iloc[0].size
    k_res = {}
    for k in k_list:
        k_res[k] = {}
    for testrow in testset.index:
        d_list = []
        for trainrow in trainset.index:
            t1 = trainset.loc[trainrow].values
            t2 = testset.loc[testrow].values
            d = get_distance(t1, t2, n)
            d_list.append([d, trainrow])
        d_list.sort()
        for k in k_list:
            label_select = [0,0]
            for i in range(k):
                row = d_list[i][1]
                label = trainlabel.loc[row]
                label_select[label] += 1
            if label_select[0] > label_select[1]:
                k_res[k][testrow] = 0
            else:
                k_res[k][testrow] = 1
    return k_res

