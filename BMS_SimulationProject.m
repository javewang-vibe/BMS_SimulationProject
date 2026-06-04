clear;clc;

CELL_NUM = 6;
CAPACITY = 100.0;
dt_sec = 1;
dt_hour = dt_sec/3600;
total_time = 3600;

R0 = 0.05;
R1 = 0.02;
C1 = 2000;
tau = R1*C1;

P = 1.0;
Q = 0.001;
R_noise = 0.01;

soc_axis = 0:10:100;
ocv_table = [2.800,3.300,3.520,3.650,3.720,3.780,3.830,3.890,3.960,4.060,4.200];

OV_LIMIT = 4.20;
UV_LIMIT = 2.80;
OT_LIMIT = 60.0;
UT_LIMIT = -5.0;
OC_LIMIT = 0.80;
IM_LIMIT = 0.20;

soc_cell = ones(1,CELL_NUM)*50.0;
v_rc = zeros(1,CELL_NUM);
x = 50.0;
temperature = 25.0;

soc_true_hist = zeros(1,total_time)
soc_ekf_hist = zeros(1,total_time)
voltage_hist = zeros(1,total_time)
current_hist = zeros(1,total_time)
temp_hist = zeros(1,total_time)
K_hist = zeros(1,total_time)
fault_hist = zeros(1,total_time)
state_hist = zeros(1,total_time)

state = 0;
fault = 0;
fault_latch = 0;
fault_timer = 0;

fprintf('=== BMS MATLAB仿真开始 ===')

for k = 1:total_time
    %1.模拟传感器数据
    current = 0.5 + randn * 0.01;
    temperature = temperature + 0.02 + randn * 0.005;
    if temperature > 70, temperature = 70; end
    if temperature < -10, temperature = -10; end
    cell_voltage = zeros(1,CELL_NUM);
    for i =1:CELL_NUM
        cell_current = current * (1 + (i-3) * 0.01);
        soc_cell(i) = soc_cell(i) - (cell_current/CAPACITY)*dt_hour*100;
        if soc_cell(i) >100,soc_cell(i) = 100;end
        if soc_cell(i) <0,soc_cell(i) = 0;end
        
        ocv = interp1(soc_axis,ocv_table,soc_cell(i),'linear','extrap');
        
        alpha = exp(-dt_sec/tau);
        v_rc(i) = v_rc(i)*alpha + cell_current*R1*(1-alpha);
        
        cell_voltage(i) = ocv - cell_current*R0 - v_rc(i);
        if cell_voltage(i) > 4.2, cell_voltage(i) = 4.2;end
        if cell_voltage(i) <2.8, cell_voltage(i) =2.8;end
    end
    
    pack_voltage = sum(cell_voltage);
    min_v = min(cell_voltage);
    max_v = max(cell_voltage);
    delta_v = max_v - min_v;
    
    x_pred = x - (current/CAPACITY)*dt_hour*100;
    P_pred = P + Q;
    
    avg_v = pack_voltage / CELL_NUM;
    avg_v_compensated = avg_v + current * R0;
    avg_v_compensated = max(min(avg_v_compensated,ocv_table(end)),ocv_table(1));
    z = interp1(ocv_table,soc_axis,avg_v_compensated,'linear');
    
    if k <= 5
        disp(k)
        disp(avg_v)
        disp(z)
        disp(mean(soc_cell))
    end
    
       
    
    Kk = P_pred / (P_pred + R_noise);
    x = x_pred + Kk * (z - x_pred);
    P = (1 - Kk) * P_pred;
    
    if x > 100, x = 100;end
    if x < 0, x = 0;end
    
    fault = 0;
    if max_v >= OV_LIMIT;
        fault = 1;
    elseif min_v <= UV_LIMIT;
        fault = 2;
    elseif temperature >= OT_LIMIT;
        fault = 3;
    elseif temperature <= UT_LIMIT;
        fault = 4;
    elseif current >= OC_LIMIT;
        fault = 5;
    elseif current <= -OC_LIMIT;
        fault = 6;
    elseif delta_v >= IM_LIMIT;
        fault =7;
    end
    
    if fault ~= 0
        fault_latch = fault;
        fault_timer = 0;
        state = 3;
    end
    
    if state ~= 3
        if current > 0.05
            state = 2;
        elseif current < -0.05
            state = 1;
        else
            state = 0;
        end
    end
    
    soc_true_hist(k) = mean(soc_cell);
    soc_ekf_hist(k) = x;
    voltage_hist(k) = pack_voltage;
    current_hist(k) = current;
    temp_hist(k) = temperature;
    K_hist(k) = Kk;
    fault_hist(k) = fault;
    state_hist(k) = state;
end

fprintf('仿真完成！共运行%d秒\n',total_time);

error_ekf = abs(soc_ekf_hist - soc_true_hist);
fprintf('EKF最大误差:%.4f%%\n',max(error_ekf));
fprintf('EKF平均误差:%.4f%%\n',mean(error_ekf));

time = 1:total_time;
figure('Name','BMS仿真结果');

subplot(3,2,1)
plot(time,soc_true_hist,'k-','Linewidth',1.5);hold on;
plot(time,soc_ekf_hist,'b--','Linewidth',1.5);
legend('True SOC','EKF SOC')
xlabel('Time (s)')
ylabel('SOC (%)')
title('SOC Estimation')
grid on

subplot(3,2,2)
plot(time,error_ekf,'r-','Linewidth',1.5)
xlabel('Time (s)')
ylabel('Error (%)')
title('EKF SOC Error')
grid on

subplot(3,2,3)
plot(time,voltage_hist,'g-','Linewidth',1.5)
xlabel('Time (s)')
ylabel('Voltage (V)')
title('Pack Voltage')
grid on

subplot(3,2,4)
plot(time,current_hist,'color',[1 0.5 0],'Linewidth',1.5)
hold on
plot([1 total_time],[0 0],'k--','linewidth',0.8)
xlabel('Time (s)')
ylabel('Current (A)')
title('Current')
grid on

subplot(3,2,5)
plot(time,temp_hist,'r-','Linewidth',1.5)
hold on
plot([1 total_time],[OT_LIMIT OT_LIMIT],'r--','linewidth',1)
plot([1 total_time],[UT_LIMIT UT_LIMIT],'b--','linewidth',1)
xlabel('Time (s)')
ylabel('Temp (℃)')
title('Temperature')
grid on

subplot(3,2,6)
plot(time,K_hist,'g-','Linewidth',1.5)
xlabel('Time (s)')
ylabel('K')
title('Kalman Gain')
grid on
fault_times = find(fault_hist ~= 0);
for i = 1:6
    subplot(3,2,i)
    hold on
    for ft = fault_times
        plot([ft ft],ylim,'r-','linewidth',0.5,'color',[1 0 0 0.2])
    end
end






    
    