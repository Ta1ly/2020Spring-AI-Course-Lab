# SVM.py

import cvxpy as cp
import numpy as np
from scipy.optimize import minimize
from scipy.optimize import Bounds


def svm(trainset, trainlabel, testset, C=1, kernel='line'):
    """
    svm [svm 主函数]

    Args:
        trainset ([type]): [输入的训练集]
        trainlabel ([type]): [训练集对应的标签]]
        testset ([type]): [待预测的测试集]]
        C ([num]): [软间隔的参数]. Defaults to 1.
        kernel ([string]): [svm采用的核函数的类型]. Defaults to 'line'.

    Returns:
        [dict]: [返回一个字典，形式为{测试编号:预测结果,...}]
    """

    def K(x, z, type):
        """
        K [核函数]

        Args:
            x ([np.array]): [一维向量x]
            z ([np.array]): [一维向量z]
            type (str, optional): [选取的核函数类型].

        Returns:
            [num]: [两个向量在核函数下的乘积]
        """

        res = 0
        if type == 'line':
            res = x.T @ z
        elif type == 'polynomial':
            temp = x.T @ z
            res = (temp+1)**3
        elif type == 'gauss':
            res =  np.exp(-np.sqrt(np.linalg.norm(x-z) ** 2 / (2 * 0.5 ** 2)))
        return res

    # solve problem
    # min 1/2 x^T * P * x + q^T * x
    # G * x <= h
    # A * x == b
    (n, m) = trainset.shape
    S = np.zeros([n, m])
    P = np.zeros([n, n])
    y = np.zeros([n])

    print("======calculating matrix P=======")
    for i in range(n):
        xi = trainset.iloc[i].values
        yi = trainlabel.iloc[i]
        S[i] = xi
        y[i] = yi

    for i in range(n):
        for j in range(n):
            P[i][j] = K(S[i], S[j], kernel) * y[i] * y[j]

    print("=======solving QP problem========")

    def fun(x):
        n = P.shape[0]
        res = 0
        for i in range(n):
            for j in range(n):
                res += P[i][j] * x[i] * x[j]
            res += - x[i]
        return res

    def eqcons(x):
        n = P.shape[0]
        res = 0
        for i in range(n):
            res += y[i] * x[i]
        return res

    bounds = Bounds([0] * n, [C] * n)
    constraint = {'type': 'eq', 'fun': eqcons}
    x0 = np.zeros(n)

    res = minimize(fun, x0, method='SLSQP', constraints=[
                   constraint], tol=0.001, bounds=bounds)
    print("QPproblem status:" + str(res.success))
    alpha = res.x
    
    # 由b = y[j] - \sum a_i * y_i * K(x_i,x_j) , P[i][j] = y_i*y_j*K(x_i,x_j) 而 *y_j 与 /y_j 作用一样
    # 取j = 0
    b = y[0]
    for i in range(n):
        b -= (alpha[i]*P[i][0] * y[0])

    def sign(x):
        """
        sign [用于判别testset中的label]

        Args:
            x ([np.array]): [输入测试例]

        Returns:
            [num]: [返回计算得到的标签{1，-1}]
        """
        res = b
        for i in range(n):
            res += alpha[i] * K(x, S[i], kernel) * y[i]
        if res >= 0:
            return 1
        else:
            return - 1

    result = {}
    for i in testset.index:
        x = testset.loc[i].values
        result[i] = sign(x)
    return result
