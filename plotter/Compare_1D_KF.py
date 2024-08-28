# Compare DW1000 and DW3000 in 1D system with Kalman filter

import pandas as pd
import matplotlib.pyplot as plt

# Reading CSV file
file_path1 = '/Users/chenlin/Desktop/plotter/DW1000_1D_KF.csv'
df1 = pd.read_csv(file_path1)

file_path2 = '/Users/chenlin/Desktop/plotter/DW3000_1D_KF.csv'
df2 = pd.read_csv(file_path2)


measured_distances1 = df1.iloc[:, 5].dropna()  # DW1000_1D_KF.csv的第6列作为纵坐标
ideal_distances1 = df1.iloc[:, 7].dropna()  # DW1000_1D_KF.csv的第8列作为横坐标

measured_distances2 = df2.iloc[:, 7].dropna()  # DW3000_1D_KF.csv的第8列作为纵坐标
ideal_distances2 = df2.iloc[:, 10].dropna()  # DW3000_1D_KF.csv的第11列作为横坐标


measured_distances1 = measured_distances1[:len(ideal_distances1)]
measured_distances2 = measured_distances2[:len(ideal_distances2)]


group_size = 200
measured_groups1 = [measured_distances1[i:i + group_size] for i in range(0, len(measured_distances1), group_size)]
measured_groups2 = [measured_distances2[i:i + group_size] for i in range(0, len(measured_distances2), group_size)]


measured_averages1 = [group.mean() for group in measured_groups1]
measured_averages2 = [group.mean() for group in measured_groups2]


for i, avg in enumerate(measured_averages1, start=1):
    print(f"Group {i}: Measured Distance Average of DW1000 = {avg:.4f}")

for i, avg in enumerate(measured_averages2, start=1):
    print(f"Group {i}: Measured Distance Average of DW3000 = {avg:.4f}")


plt.figure(figsize=(8, 8))  


plt.scatter(ideal_distances1, measured_distances1, color='lightblue', marker='o', s=10, label='DW1000 Measured Data')  
plt.scatter(ideal_distances2, measured_distances2, color='pink', marker='o', s=10, label='DW3000 Measured Data') 


group_averages1 = [ideal_distances1[i:i + group_size].mean() for i in range(0, len(ideal_distances1), group_size)]
plt.scatter(group_averages1, measured_averages1, color='blue', marker='o', s=50, label='DW1000 Data Averages')

group_averages2 = [ideal_distances2[i:i + group_size].mean() for i in range(0, len(ideal_distances2), group_size)]
plt.scatter(group_averages2, measured_averages2, color='red', marker='o', s=50, label='DW3000 Data Averages')


plt.plot(group_averages1, measured_averages1, color='blue', linestyle='-', linewidth=2, label='DW1000 Average Trend Line')
plt.plot(group_averages2, measured_averages2, color='red', linestyle='-', linewidth=2, label='DW3000 Average Trend Line')


plt.plot([0, 2.5], [0, 2.5], color='green', linestyle='--', linewidth=2, label='Ideal Line')


plt.title('Comparison of indoor ranging between DW1000 and DW3000 in 1D System with Kalman Filter')
plt.xlabel('Ideal Distance (m)')
plt.ylabel('Measured Distance (m)')


ticks = [round(x * 0.1, 1) for x in range(1, 26)]
plt.xticks(ticks)
plt.yticks(ticks)


plt.xlim(0.1, 2.5)
plt.ylim(0.1, 2.5)


plt.grid(True)
plt.legend()
plt.show()



