#main.py
import numpy as np
import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import LabelEncoder
import KNN
from time import time

import warnings
warnings.filterwarnings("ignore")

def read_data(path):
    data = pd.read_csv(path,sep=";")
    return data

def evaluate_result(testlabel, predictlabel):
    TP,FP,TN,FN = 0,0,0,0
    for index in predictlabel:
        predict_res = predictlabel[index]
        real_res = testlabel[index]
        if real_res == 1 and predict_res == 1:
            TP += 1
        elif real_res == 1 and predict_res == 0:
            FN += 1
        elif real_res == 0 and predict_res == 1:
            FP += 1
        elif real_res == 0 and predict_res == 0:
            TN += 1
    P = TP / (TP + FP)
    R = TP / (TP + FN)
    F1 = 2 * P * R / (P + R)
    return P, R, F1
    
def print_result(P, R, F1):
    print("======result=======")
    print("Precise:" + str(P))
    print("Recall:" + str(R))
    print("F1 Score:" + str(F1))
    print()



def pre_process(data,drop_g1g2):
    le = LabelEncoder()
    for col in data[["school", "sex", "address", "famsize", "Pstatus", "Mjob", "Fjob", "reason", "guardian", "schoolsup", "famsup",
         "paid", "activities", "nursery", "higher", "internet", "romantic"]].columns:
        data[col] = le.fit_transform(data[col].values)
    for row in data.index:
        #print(row)
        if data['G3'][row] >= 10:
            data['G3'][row] = 1
        else:
            data['G3'][row] = 0
    label = data.pop('G3')
    if drop_g1g2 == 1:
        data = data.drop(columns='G1')
        data = data.drop(columns='G2')
    return data,label


def partial_data(x, y):
    train_x, test_x, train_y, test_y = train_test_split(x, y, test_size=0.3, random_state=1)
    return train_x, test_x, train_y, test_y
    
def using_knn(data,drop_g1g2=0):
    x,y = pre_process(data,drop_g1g2)
    start_time = time()
    train_x, test_x, train_y, test_y = partial_data(x, y)
    find_k = []
    k_list = range(1,100,2)
    k_res = KNN.knn(train_x, train_y, test_x, k_list)
    for k in k_res:
        P, R, F1 = evaluate_result(test_y, k_res[k])
        find_k.append([F1, k, P, R])
    find_k.sort()
    find_k.reverse()
    end_time = time() - start_time
    print(end_time)
    print(find_k)

if __name__ == "__main__":
    data_path = "supervise\\data\\student-mat.csv"
    data = read_data(data_path)
    using_knn(data,drop_g1g2=0)

