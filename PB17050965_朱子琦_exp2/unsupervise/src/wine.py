# unsupervise ml
import numpy as np
import matplotlib.pyplot as plt
import csv

def evaluate_RI(C, K):
    """
    evaluate_RI [使用兰德系数评价结果]

    Args:
        C ([dict]): [真实结果]
        K ([dict]): [聚类结果]

    Returns:
        [num]: [兰德系数值]
    """
    a, b, c, d = 0, 0, 0, 0

    for i in range(len(C)):
        for j in range(len(C)):
            if i == j:
                continue
            else:
                if C[i] == C[j] and K[i] == K[j]:
                    a += 1
                elif C[i] == C[j] and K[i] != K[j]:
                    b += 1
                elif C[i] != C[j] and K[i] == K[j]:
                    c += 1
                else:
                    d += 1
    RI = (a + d) / (a + b + c + d)
    return RI


def evaluate_SI(k,label, data, center):
    def distance(v1, v2):
        return (v2.T @ v2 - 2 * v1.T @ v2 + v1.T @ v1) ** 0.5
    
    n,m = data.shape
    board = np.zeros([n,n])

    for i in range(n):
        for j in range(n):
            board[i,j] = distance(data[i],data[j])
            

    def a(i):
        avr,count = 0,0
        for j in range(n):
            if j != i and label[i] == label[j]:
                avr += board[i,j]
                count += 1
        return avr/count
    def b(i):
        d = np.zeros([k])
        count = np.zeros([k])
        for j in range(n):
            d[label[j]] += board[i,j]
            count[label[j]] += 1
        for j in range(k):
            d[j] = d[j] / count[j]
        mind = np.inf
        for j in range(k):
            if j != label[i] and d[j] < mind:
                mind = d[j]
        return mind
    
    def s(i):
        ai,bi = a(i),b(i)
        return (bi-ai)/max(ai,bi)

    slist = []
    for i in range(n):
        slist.append(s(i))
    return sum(slist)/n


def PCA(X, threhold=0.8):
    """
    PCA [使用PCA算法对数据进行降维处理]

    Args:
        X ([np.array([n,m])]): [待降维的数据集np.array([n,m])]]
        threhold (float, optional): [PCA 降维阈值，选取特征累加贡献率达到该阈值的前k个特征]. Defaults to 0.8.

    Returns:
        [np.array([n,k])]: [返回降低至k维后的数据集]
    """
    if threhold < 0 or threhold > 1:
        print("threhold should be between 0 and 1")
        return X

    # 计算输入数据矩阵的特征向量
    n, m = X.shape
    scatter_matrix = X.T @ X
    eig_val, eig_vec = np.linalg.eig(scatter_matrix)
    eig_pairs = [(np.abs(eig_val[i]), eig_vec[:, i]) for i in range(m)]
    eig_pairs.sort(reverse=True)

    # 计算需要达到threhold的k
    base, top, k = sum(eig_val), 0, m
    '''
    te = []
    for i in range(k):
        print("|"+str(sum(eig_val[:i+1])/base)+"|"+str(i+1)+"|")
    for i in range(k):
        print(str(sum(eig_val[:i+1])/base)+",")
    '''
    for i in range(m):
        top += eig_pairs[i][0]
        if top/base > threhold:
            k = i + 1
            break
    # 选取前k个特征向量
    feature = np.array([ele[1] for ele in eig_pairs[:k]])
    data = X @ feature.T
    return data


def k_means(k, data):
    def distance(v1, v2):
        return (v2.T @ v2 - 2 * v1.T @ v2 + v1.T @ v1) ** 0.5

    def avrcenter(v1, v2, k1, k2):
        return (v1 + v2) / (k1 + k2)
    
    n, m = data.shape
    center = np.zeros([k, m])
    num = np.zeros([k])
    label = [1]
    next_label = []
    for i in range(k):
        center[i] = data[int(n / k * i)]
    t = 0
    while next_label != label:
        label = next_label.copy()
        next_label = [None]*n

        for i in range(n):
            d = []
            for j in range(k):
                d.append((distance(center[j], data[i]), j))
            d.sort()
            cluster = d[0][1]
            next_label[i] = cluster
            num[cluster] += 1
        for i in range(k):
            center[i] = np.zeros(m)
        for i in range(n):
            center[next_label[i]] += data[i]
        for i in range(k):
            center[i] = center[i] / num[i]
        num = np.zeros([k])
        t += 1

    def draw_plt():
        color = ['gold', 'deepskyblue',  'green','red','b','k']
        for i in range(n):
            plt.scatter(data[i, 0], data[i, 1], s=10, c=color[label[i]])
        for i in range(k):
            plt.scatter(center[i, 0], center[i, 1], s=150, marker="*", c='purple')
    draw_plt()

    return k,label,center


def data_preprocess(data):
    n, m = data.shape

    for i in range(1, m):
        max_value = data[:, i].max()
        min_value = data[:, i].min()
        data[:, i] = ((data[:, i] - min_value) /
                      (max_value - min_value) - 0.5) * 2

    label = data[:, 0]
    data = data[:, 1:]
    # print(label)
    # print(data)
    return data, label


def read_data(data_path):
    data = []
    with open(data_path) as f:
        for line in f:
            line = line.strip("\n").split(',')
            line[0] = int(line[0])
            for i in range(1, len(line)):
                line[i] = float(line[i])
            data.append(line)
    return np.array(data)


def listed(label):
    (n,) = label.shape
    res = []
    for i in range(n):
        res.append(label[i])
    return res


def to_csv(label):
    for i in range(len(label)):
        label[i] += 1
    with open('unsupervise\\output\\result.csv', 'w', newline='') as csvfile:
        writer  = csv.writer(csvfile)
        writer.writerow(label)
    


if __name__ == "__main__":
    data_path = "unsupervise\\input\\wine.data"
    
    A = [0.5]
    for i in range(3,4):
        data, label = data_preprocess(read_data(data_path))
        data = PCA(data, 0.5)
        k,predict_label,center = k_means(i, data)
        #draw_plt(predict_label)
        label = listed(label)
        RI = evaluate_RI(label, predict_label)
        SI = evaluate_SI(k, predict_label, data, center)
        to_csv(predict_label)
        print("Rand Index:"+str(RI))
        print("Silhouette Coefficient:" + str(SI))
        #print("|"+str(i)+"|"+str(RI)+"|"+str(SI)+"|")
    plt.show()
