import pandas as pd
import matplotlib.pyplot as plt
import numpy as np


file_path = '/Users/chenlin/Desktop/plotter/DW1000_2D_3m.csv'
df = pd.read_csv(file_path, header=None)


coordinates = df.iloc[::5, 0]
print(coordinates)


def extract_coordinates(value):
    try:
        x, y = map(float, value.split())
        return x, y
    except:
        return None, None


xy_coords = coordinates.apply(lambda x: extract_coordinates(x))


xy_coords = xy_coords.apply(pd.Series).dropna()
xy_coords.columns = ['x', 'y']


plt.figure(figsize=(8, 8))
plt.scatter(xy_coords['x'], xy_coords['y'], c='blue', marker='o', label='Position of the moving Tag')
plt.plot(xy_coords['x'], xy_coords['y'], c='blue', marker='o', linestyle='-', label='Position of the moving Tag')

plt.title('ESP32 UWB DW1000 Indoor 2D Positioning System', pad=12)
plt.xlabel('X Coordinate')
plt.ylabel('Y Coordinate')


plt.xlim(-1, 5)
plt.ylim(-1, 5)


plt.xticks(np.arange(-1, 5.5, 0.5))  
plt.yticks(np.arange(-1, 5.5, 0.5))  


plt.arrow(1, 1, 0, 1, head_width=0.08, head_length=0.1, fc='red', ec='red', linewidth=2)  
plt.plot([1, 1], [2, 3], 'red', linewidth=2) 

plt.arrow(1, 3, 1, 0, head_width=0.08, head_length=0.1, fc='red', ec='red', linewidth=2)  
plt.plot([2, 3], [3, 3], 'red', linewidth=2)  

plt.arrow(3, 3, 0, -1, head_width=0.08, head_length=0.1, fc='red', ec='red', linewidth=2)  
plt.plot([3, 3], [2, 1], 'red', linewidth=2, label='Ideal Route of Tag')  


plt.scatter([0], [0], color='blue', s=100)  # 在(0, 0)绘制Anchor1
plt.text(0, 0, 'Anchor1', fontsize=12, verticalalignment='bottom', horizontalalignment='right')  

plt.scatter([3], [0], color='blue', s=100)  # 在(3, 0)绘制Anchor2
plt.text(3, 0, 'Anchor2', fontsize=12, verticalalignment='bottom', horizontalalignment='left')  


plt.gca().invert_yaxis()  


ax = plt.gca()
ax.xaxis.set_ticks_position('top')  
ax.xaxis.set_label_position('top')  

plt.grid(True)
plt.legend()
plt.show()
