# main.py
import numpy as np
import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import LabelEncoder
import KNN
import SVM
import DT
from time import time

import warnings
warnings.filterwarnings("ignore")


def pre_process(data, drop_g1g2):
    print("=======pre-process of data=======")
    le = LabelEncoder()
    for col in data[["school", "sex", "address", "famsize", "Pstatus", "Mjob", "Fjob", "reason", "guardian", "schoolsup", "famsup",
                     "paid", "activities", "nursery", "higher", "internet", "romantic"]].columns:
        data[col] = le.fit_transform(data[col].values)
    for row in data.index:
        if data['G3'][row] >= 10:
            data['G3'][row] = 1
        else:
            data['G3'][row] = -1

    label = data.pop('G3')
    # 根据需求选择是否去除G1，G2
    if drop_g1g2 == 1:
        data = data.drop(columns=['G1', 'G2'])
    return train_test_split(data, label, test_size=0.9)


def running_knn(data, drop_g1g2=0):
    train_x, test_x, train_y, test_y = pre_process(data, drop_g1g2)
    # 通过k_list 输入一组knn中k的取值
    res_list, k_list = [], [21]
    k_res = KNN.knn(train_x, train_y, test_x, k_list)
    for k in k_res:
        P, R, F1 = evaluate_result(test_y, k_res[k])
        res_list.append([F1, k, P, R])
    res_list.sort()
    res_list.reverse()
    for F1, k, P, R in res_list:
        print("K = " +str(k))
        print("Precise:" + str(P))
        print("Recall:" + str(R))
        print("F1 Score:" + str(F1))
        print()
    return res_list[0]

def running_svm(data, drop_g1g2=0, kernel_type="line"):
    train_x, test_x, train_y, test_y = pre_process(data, drop_g1g2)
    res = SVM.svm(train_x, train_y, test_x, 1, kernel_type)
    P, R, F1 = evaluate_result(test_y, res)
    
    print("Precise:" + str(P))
    print("Recall:" + str(R))
    print("F1 Score:" + str(F1))
    print()
    return P,R,F1

def running_dt(data, drop_g1g2=0):
    train_x, test_x, train_y, test_y = pre_process(data, drop_g1g2)
    res = DT.dt(train_x, train_y, test_x)
    P, R, F1 = evaluate_result(test_y, res)
    print("Precise:" + str(P))
    print("Recall:" + str(R))
    print("F1 Score:" + str(F1))
    print()
    return P,R,F1

def evaluate_result(testlabel, predictlabel):
    TP, FP, TN, FN = 0, 0, 0, 0
    for index in predictlabel:
        predict_res = predictlabel[index]
        real_res = testlabel[index]
        if real_res == 1 and predict_res == 1:
            TP += 1
        elif real_res == 1 and predict_res == -1:
            FN += 1
        elif real_res == -1 and predict_res == 1:
            FP += 1
        elif real_res == -1 and predict_res == -1:
            TN += 1
    print("TP:" + str(TP)+"\nTN:" + str(TN) +
            "\nFP:" + str(FP)+"\nFN:" + str(FN))
    try:
        P = TP / (TP + FP)
    except:
        P = 0
    try:
        R = TP / (TP + FN)
    except:
        R = 0
    try:
        F1 = 2 * P * R / (P + R)
    except:
        F1 = 0
    return P, R, F1


if __name__ == "__main__":
    start_time = time()
    data_path = "supervise\\data\\student-por.csv"
    F1 = []
    P = []
    R = []
    for i in range(10):
        
        data = pd.read_csv(data_path, sep=";")
        #f,k,p,r = running_knn(data, drop_g1g2=1)
        p,r,f = running_svm(data, drop_g1g2=0, kernel_type="gauss")
        #p,r,f =running_dt(data, drop_g1g2=1)
        F1.append(f)
        P.append(p)
        R.append(r)
        
        end_time = time() - start_time
        
        print("time : " + str(end_time))
    print("F1:" + str(F1))
    print("P:" + str(P))
    print("R:"+str(R))
    print(sum(F1) / len(F1))
    print(sum(P) / len(P))
    print(sum(R)/len(R))
