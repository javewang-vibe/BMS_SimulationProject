import numpy as np
import tensorflow as tf
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import LSTM,Dense,Dropout
from tensorflow.keras.callbacks import EarlyStopping,ModelCheckpoint
import matplotlib.pyplot as plt

X_train = np.load('X_train.npy')
Y_train = np.load('Y_train.npy')
X_val = np.load('X_val.npy')
Y_val = np.load('Y_val.npy')

model = Sequential([LSTM(64,return_sequences=True,input_shape=(X_train.shape[1],X_train.shape[2])),
        Dropout(0.2),LSTM(32,return_sequences=False),Dropout(0.2),Dense(16,activation='relu'),Dense(1)])
model.compile(optimizer=tf.keras.optimizers.Adam(learning_rate=0.001),loss='mse',metrics=['mae'])
model.summary()
callbacks=[EarlyStopping(patience=10,restore_best_weights=True),ModelCheckpoint('best_lstm_model.keras',save_best_only=True)]
history = model.fit(X_train,Y_train,validation_data=(X_val,Y_val),epochs=100,batch_size=32,callbacks=callbacks,verbose=1)

plt.figure(figsize=(10,4))
plt.plot(history.history['loss'],label='Train Loss')
plt.plot(history.history['val_loss'],label='Val Loss')
plt.xlabel('Epoch')
plt.ylabel('MSE Loss')
plt.legend()
plt.title('LSTM Training History')
plt.savefig('training_history.png')
plt.show()

print("训练完成！模型已保存为 best_lstm_model.keras")