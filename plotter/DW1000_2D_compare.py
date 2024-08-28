import pandas as pd
import matplotlib.pyplot as plt
import numpy as np


def extract_coordinates(value):
    try:
        x, y = map(float, value.split())
        return x, y
    except:
        return None, None


file_path1 = '/Users/chenlin/Desktop/plotter/DW1000_2D_KF.csv'
file_path2 = '/Users/chenlin/Desktop/plotter/DW1000_2D_3m_t2.csv'
file_path3 = '/Users/chenlin/Desktop/plotter/DW1000_2D_3m_t3.csv'
df1 = pd.read_csv(file_path1, header=None)
df2 = pd.read_csv(file_path2, header=None)
df3 = pd.read_csv(file_path3, header=None)


coordinates1 = df1.iloc[::5, 0]
coordinates2 = df2.iloc[::5, 0]
coordinates3 = df3.iloc[::5, 0]


xy_coords1 = coordinates1.apply(lambda x: extract_coordinates(x)).apply(pd.Series).dropna()
xy_coords1.columns = ['x', 'y']
xy_coords2 = coordinates2.apply(lambda x: extract_coordinates(x)).apply(pd.Series).dropna()
xy_coords2.columns = ['x', 'y']
xy_coords3 = coordinates3.apply(lambda x: extract_coordinates(x)).apply(pd.Series).dropna()
xy_coords3.columns = ['x', 'y']


plt.figure(figsize=(10, 8))
plt.plot(xy_coords1['x'], xy_coords1['y'], c='blue', marker='o', linestyle='-', label='Actual route of the tag with Kalman filter')
plt.plot(xy_coords2['x'], xy_coords2['y'], c='orange', marker='^', linestyle='-', label='Actual route of the tag without any filter')
plt.plot(xy_coords3['x'], xy_coords3['y'], c='green', marker='s', linestyle='-', label='Actual route of the tag without any filter')


plt.arrow(1, 1, 0, 1, head_width=0.1, head_length=0.2, fc='red', ec='red', linewidth=3)  
plt.plot([1, 1], [2, 3], 'red', linewidth=3)  

plt.arrow(1, 3, 1, 0, head_width=0.1, head_length=0.2, fc='red', ec='red', linewidth=3)  
plt.plot([2, 3], [3, 3], 'red', linewidth=3)  

plt.arrow(3, 3, 0, -1, head_width=0.1, head_length=0.2, fc='red', ec='red', linewidth=3)  
plt.plot([3, 3], [2, 1], 'red', linewidth=3, label='Ideal Route of Tag')  


plt.scatter([0], [0], color='blue', s=100)  # 在(0, 0)绘制Anchor1
plt.text(0, 0, 'Anchor1', fontsize=12, verticalalignment='bottom', horizontalalignment='right')  

plt.scatter([3], [0], color='blue', s=100)  # 在(3, 0)绘制Anchor2
plt.text(3, 0, 'Anchor2', fontsize=12, verticalalignment='bottom', horizontalalignment='left')  


plt.title('Comparison of ESP32 UWB DW1000 2D Indoor Positioning System with or without Kalman Filter')
plt.xlabel('X Coordinate')
plt.ylabel('Y Coordinate')
plt.xlim(-2, 5.5)
plt.ylim(-2, 5.5)
plt.xticks(np.arange(-2, 6, 0.5))
plt.yticks(np.arange(-2, 6, 0.5))
plt.grid(True)
plt.legend()
plt.gca().invert_yaxis()  # Keep the y-axis orientation consistent with previous plots
plt.show()
