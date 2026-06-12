import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import tensorflow as tf
import pickle

#===EKF误差===
df = pd.read_csv('bms_log.csv')
mae_ekf = np.mean(np.abs(df['soc_ekf'] - df['soc_true']))
rmse_ekf = np.sqrt(np.mean((df['soc_ekf'] - df['soc_true'])**2))

#===LSTM误差===
model = tf.keras.models.load_model('best_lstm_model.keras')
with open('scaler_Y.pkl','rb') as f:
    scaler_Y = pickle.load(f)
X_test = np.load('X_test.npy')
Y_test = np.load('Y_test.npy')
Y_pred_scaled = model.predict(X_test)
Y_pred = scaler_Y.inverse_transform(Y_pred_scaled)
Y_true = scaler_Y.inverse_transform(Y_test)
mae_lstm = np.mean(np.abs(Y_pred - Y_true))
rmse_lstm = np.sqrt(np.mean((Y_pred - Y_true)**2))


#===打印对比===
print(f"EKF  MAE={mae_ekf:.4f}%  RMSE={rmse_ekf:.4f}%")
print(f"LSTM  MAE={mae_lstm:.4f}%  RMSE={rmse_lstm:.4f}%")

#===柱状图对比===
labels = ['MAE','RMSE']
ekf_vals = [mae_ekf,rmse_ekf]
lstm_vals = [mae_lstm,rmse_lstm]
x = np.arange(len(labels))
plt.bar(x - 0.2, ekf_vals, 0.4, label='EKF')
plt.bar(x + 0.2, lstm_vals, 0.4, label='LSTM')
plt.xticks(x,labels)
plt.ylabel('Error (%)')
plt.title('EKF vs LSTM SOC Estimation Error')
plt.legend()
plt.savefig('ekf_vs_lstm.png',dpi=150)
plt.show()