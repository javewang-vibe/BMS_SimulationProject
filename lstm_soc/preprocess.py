import pandas as pd
import numpy as np
from sklearn.preprocessing import MinMaxScaler
import pickle

df = pd.read_csv('bms_log.csv')

features = ['voltage','current','temp']
target = 'soc'

df = df.dropna()
df = df[df['soc'].between(0,100)]
df = df[df['voltage'].between(15.0,25.2)]
print("过滤后行数:",len(df))

scaler_X = MinMaxScaler()
scaler_Y = MinMaxScaler()
X_scaled = scaler_X.fit_transform(df[features])
Y_scaled = scaler_Y.fit_transform(df[[target]])

with open('scaler_X.pkl','wb') as f:
    pickle.dump(scaler_X,f)
with open('scaler_Y.pkl','wb') as f:
    pickle.dump(scaler_Y,f)

SEQ_LEN = 30

def create_sequences(X,Y,seq_len):
    X_seq,Y_seq = [],[]
    for i in range(seq_len,len(X)):
        X_seq.append(X[i-seq_len:i])
        Y_seq.append(Y[i])
    return np.array(X_seq),np.array(Y_seq)

X_seq,Y_seq = create_sequences(X_scaled,Y_scaled,SEQ_LEN)
print(f"序列数据形状:X={X_seq.shape},Y={Y_seq.shape}")

n = len(X_seq)
train_end = int(n * 0.7)
val_end = int(n * 0.85)

X_train,Y_train = X_seq[:train_end],Y_seq[:train_end]
X_val,Y_val = X_seq[train_end:val_end],Y_seq[train_end:val_end]
X_test,Y_test = X_seq[val_end:],Y_seq[val_end:]

np.save('X_train.npy',X_train)
np.save('Y_train.npy',Y_train)
np.save('X_val.npy',X_val)
np.save('Y_val.npy',Y_val)
np.save('X_test.npy',X_test)
np.save('Y_test.npy',Y_test)

print("预处理完成，文件已保存")