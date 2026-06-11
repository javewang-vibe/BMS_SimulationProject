import numpy as np
import pandas as pd
import tensorflow as tf
import pickle
import matplotlib.pyplot as plt

model = tf.keras.models.load_model('best_lstm_model.keras')

with open('scaler_X.pkl','rb') as f:
    scaler_X = pickle.load(f)
with open('scaler_Y.pkl','rb') as f:
    scaler_Y = pickle.load(f)

X_test = np.load('X_test.npy')
Y_test = np.load('Y_test.npy')

Y_pred_scaled = model.predict(X_test)

Y_pred = scaler_Y.inverse_transform(Y_pred_scaled)
Y_true = scaler_Y.inverse_transform(Y_test)

mae = np.mean(np.abs(Y_pred - Y_true))
rmse = np.sqrt(np.mean((Y_pred - Y_true)**2))
print(f"测试集 MAE: {mae:.4f}%")
print(f"测试集 RMSE: {rmse:.4f}%")

plt.figure(figsize=(12,5))
plt.plot(Y_true,label='True SOC',alpha=0.8)
plt.plot(Y_pred,label='LSTM Predicted SOC',alpha=0.8)
plt.xlabel('Sample')
plt.ylabel('SOC(%)')
plt.legend()
plt.title(f'LSTM SOC Prediction (MAE={mae:.3f}%)')
plt.savefig('prediction_result.png')
plt.show()

SEQ_LEN = 30

def predict_soc(recent_data: pd.DataFrame) ->float:
    assert len(recent_data) >= SEQ_LEN, f"至少需要{SEQ_LEN}行数据"
    features = ['voltage','current','temp']
    window = recent_data[features].values[-SEQ_LEN:]
    window_scaled = scaler_X.transform(window)
    X_input = window_scaled.reshape(1,SEQ_LEN,len(features))
    soc_scaled = model.predict(X_input,verbose=0)
    soc = scaler_Y.inverse_transform(soc_scaled)[0][0]
    return float(np.clip(soc,0,100))

df_test_example = pd.DataFrame(scaler_X.inverse_transform(X_test[0]),columns=['voltage','current','temp'])

predicted_soc = predict_soc(df_test_example)
print(f"\n推理示例 -> 预测SOC: {predicted_soc:.2f}%")
print(f"真实SOC: {Y_true[0][0]:.2f}%")


