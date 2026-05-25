import pandas as pd
import matplotlib.pyplot as plt

# 1. 读取CSV文件（确保和py文件在同一目录）
file_name = "bms_log.csv"
data = pd.read_csv(file_name)

# 2. 提取数据列
time = data["time"]
soc = data["soc"]
voltage = data["voltage"]
temp = data["temp"]

# 3. 创建画布
plt.figure()

# 4. 画SOC曲线
plt.plot(time, soc, label="SOC (%)")

# 5. 图像设置
plt.title("BMS SOC vs Time")
plt.xlabel("Time (s)")
plt.ylabel("SOC (%)")
plt.grid(True)
plt.legend()

# 6. 显示图像
plt.show()