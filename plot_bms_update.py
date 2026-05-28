# -*- coding: utf-8 -*-
import sys
import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches

def configure_utf8_output():
    for stream in (sys.stdout, sys.stderr):
        if hasattr(stream,"reconfigure"):
            stream.reconfigure(encoding="utf-8")

configure_utf8_output()

data = pd.read_csv("bms_log.csv", encoding="utf-8")

time = data["time"]
soc = data["soc"]
voltage = data["voltage"]
current = data["current"]
temp = data["temp"]
state = data["state"]
fault = data["fault"]

fault_times = time[fault != "NONE"]

fig,axes = plt.subplots(2,2,figsize=(14,8))
fig.suptitle("BMS Simulation Data", fontsize=14)

ax1 = axes[0][0]
ax1.plot(time,soc,color="blue",linewidth = 1.5)
ax1.set_title("SOC vs Time")
ax1.set_xlabel("Time (s)")
ax1.set_ylabel("SOC (%)")
ax1.grid(True)
for ft in fault_times:
    ax1.axvline(x=ft, color = "red",alpha = 0.3, linewidth = 0.8)

ax2 = axes[0][1]
ax2.plot(time,voltage,color="green",linewidth = 1.5)
ax2.set_title("Pack Voltage vs Time")
ax2.set_xlabel("Time (s)")
ax2.set_ylabel("Voltage (V)")
ax2.grid(True)
for ft in fault_times:
    ax2.axvline(x=ft, color = "red",alpha = 0.3, linewidth = 0.8)

ax3 = axes[1][0]
ax3.plot(time,current,color="orange",linewidth = 1.5)
ax3.axhline(y=0, color="gray", linewidth = 0.8, linestyle="--")
ax3.set_title("Current vs Time")
ax3.set_xlabel("Time (s)")
ax3.set_ylabel("Current (A)")
ax3.grid(True)
for ft in fault_times:
    ax3.axvline(x=ft, color = "red",alpha = 0.3, linewidth = 0.8)

ax4 = axes[1][1]
ax4.plot(time,temp,color="red",linewidth = 1.5)
ax4.axhline(y=60, color="red", linewidth = 0.8, linestyle="--", label="Over-temp limit (60℃)")
ax4.axhline(y=-5, color="blue", linewidth = 0.8, linestyle="--", label="Under_temp limit (-5℃)")
ax4.set_title("Temperature vs Time")
ax4.set_xlabel("Time (s)")
ax4.set_ylabel("Temperature (℃)")
ax4.grid(True)
ax4.legend(fontsize=8)

fault_path = mpatches.Patch(color="red", alpha=0.3, label="Fault event")
fig.legend(handles=[fault_path], loc="lower center", ncol=1, fontsize=9)
plt.tight_layout(rect=(0,0.03,1,1))
plt.savefig("bms_simulation.png",dpi=150)
plt.show()

print("图表已保存为 bms_simulation.png")

