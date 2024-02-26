import numpy as np
from sklearn.neighbors import KNeighborsRegressor
from sklearn.metrics import mean_squared_error
from sklearn.preprocessing import MinMaxScaler

def normalize_data(data):
    data.iloc[:, -1] = np.where(data.iloc[:, -1] > 125, 125, data.iloc[:, -1])
    data_to_normalize = data.iloc[:, :-1]
    scaler = MinMaxScaler()
    normalized_data = scaler.fit_transform(data_to_normalize)
    data.iloc[:, :-1] = normalized_data
    return data

def build_knn_model() -> KNeighborsRegressor:
    model = KNeighborsRegressor(n_neighbors=15, weights='distance', p=2)  # 设置权重为距离的倒数
    return model

def process(train_data, test_data) -> dict:
    train_data = normalize_data(train_data)
    test_data = normalize_data(test_data)

    x_test, y_test = test_data.iloc[:, :-1], test_data.iloc[:, -1]
    x_train, y_train = train_data.iloc[:, :-1], train_data.iloc[:, -1]

    model = build_knn_model()

    model.fit(x_train, y_train)  # 拟合模型

    result = {}

    # 进行预测
    y_pred = model.predict(x_test)  # 使用测试数据进行预测

    # 计算均方误差
    rmse = np.sqrt(mean_squared_error(y_test, y_pred))

    result['mean_mse'] = float(rmse)

    return result
