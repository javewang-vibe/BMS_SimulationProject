import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("bms_log.csv")
print(df.head())
print(df.dtypes)
print(df.describe())
print(f"总行数:{len(df)}")

print("缺失值:\n",df.isnull().sum())
fig,axes = plt.subplots(4, 1, figsize=(12, 6))
df['soc'].plot(ax=axes[0],title='SOC')
df['voltage'].plot(ax=axes[1],title='Voltage (V)')
df['current'].plot(ax=axes[2],title='Current (A)')
df['temp'].plot(ax=axes[3],title='Temperature (℃)')
plt.tight_layout()
plt.savefig("bms_log.png")
plt.show()