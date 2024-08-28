import pandas as pd
import matplotlib.pyplot as plt
import numpy as np


file_path = '/Users/chenlin/Desktop/plotter/DW1000_2D_2.6m.csv'
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
# plt.scatter(xy_coords['x'], xy_coords['y'], c='blue', marker='o', label='Position of the moving Tag')
plt.plot(xy_coords['x'], xy_coords['y'], c='blue', marker='o', linestyle='-', label='Position of the moving Tag')

plt.title('ESP32 UWB DW1000 Indoor 2D Positioning System', pad=12)
plt.xlabel('X Coordinate')
plt.ylabel('Y Coordinate')


plt.xlim(-1, 4)
plt.ylim(-1, 4)


plt.xticks(np.arange(-1, 4.5, 0.5))  
plt.yticks(np.arange(-1, 4.5, 0.5))  


plt.arrow(0.5, 0.5, 0, 1, head_width=0.08, head_length=0.1, fc='red', ec='red', linewidth=2)  # 从(0.5, 0.5)到(0.5, 2.5)
plt.plot([0.5, 0.5], [1.5, 2.5], 'red', linewidth=2)

plt.arrow(0.5, 2.5, 1, 0, head_width=0.08, head_length=0.1, fc='red', ec='red', linewidth=2)  # 从(0.5, 2.5)到(2.5, 2.5)
plt.plot([1.5, 2.5], [2.5, 2.5], 'red', linewidth=2)  

plt.arrow(2.5, 2.5, 0, -1, head_width=0.08, head_length=0.1, fc='red', ec='red', linewidth=2)  # 从(2.5, 2.5)到(2.5, 0.5)
plt.plot([2.5, 2.5], [1.5, 0.5], 'red', linewidth=2, label='Ideal Route of Tag')  


plt.scatter([0], [0], color='blue', s=100)  
plt.text(0, 0, 'Anchor1', fontsize=12, verticalalignment='bottom', horizontalalignment='right')  

plt.scatter([2.6], [0], color='blue', s=100)  
plt.text(2.6, 0, 'Anchor2', fontsize=12, verticalalignment='bottom', horizontalalignment='left')  


plt.gca().invert_yaxis() 


ax = plt.gca()
ax.xaxis.set_ticks_position('top')   
ax.xaxis.set_label_position('top')   

plt.grid(True)
plt.legend()
plt.show()
