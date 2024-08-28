import pandas as pd
import matplotlib.pyplot as plt


file_path = '/Users/chenlin/Desktop/plotter/DW3000_1D_KF.csv'
df = pd.read_csv(file_path)


measured_distances = df.iloc[:, 7].dropna()  # The 8th column is the ordinate
ideal_distances = df.iloc[:, 10].dropna()  # The 11th column is the horizontal coordinate


measured_distances = measured_distances[:len(ideal_distances)]


group_size = 200
measured_groups = [measured_distances[i:i + group_size] for i in range(0, len(measured_distances), group_size)]


measured_averages = [group.mean() for group in measured_groups]


for i, avg in enumerate(measured_averages, start=1):
    print(f"Group {i}: Measured Distance Average = {avg:.4f}")


plt.figure(figsize=(8, 8))  


plt.scatter(ideal_distances, measured_distances, color='blue', marker='o', s=10, label='Measured Data')  


group_averages = [ideal_distances[i:i + group_size].mean() for i in range(0, len(ideal_distances), group_size)]
plt.scatter(group_averages, measured_averages, color='red', marker='o', s=50, label='Data Averages')


plt.plot(group_averages, measured_averages, color='red', linestyle='-', linewidth=2, label='Average Trend Line')


plt.plot([0, 2.5], [0, 2.5], color='green', linestyle='--', linewidth=2, label='Ideal Line')


plt.title('ESP32 UWB DW3000 1D System with Kalman Filter')
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



