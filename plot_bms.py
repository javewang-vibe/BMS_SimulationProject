# -*- coding: utf-8 -*-
import sys

import pandas as pd
import matplotlib.pyplot as plt


def configure_utf8_output():
    """让 Windows 终端尽量使用 UTF-8 输出中文。"""
    for stream in (sys.stdout, sys.stderr):
        if hasattr(stream, "reconfigure"):
            stream.reconfigure(encoding="utf-8")


configure_utf8_output()

# 1. 读取 CSV 文件，确保它和 py 文件在同一个目录
file_name = "bms_log.csv"
data = pd.read_csv(file_name, encoding="utf-8")

# 2. 提取数据列
time = data["time"]
soc = data["soc"]
voltage = data["voltage"]
temp = data["temp"]

# 3. 创建画布
plt.figure()

# 4. 绘制 SOC 曲线
plt.plot(time, soc, label="SOC (%)")

# 5. 设置图像
plt.title("BMS SOC vs Time")
plt.xlabel("Time (s)")
plt.ylabel("SOC (%)")
plt.grid(True)
plt.legend()

# 6. 显示图像
plt.show()
