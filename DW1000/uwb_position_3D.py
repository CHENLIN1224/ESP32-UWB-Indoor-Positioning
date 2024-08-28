# import time
# import socket
# import json
# import numpy as np
# import matplotlib.pyplot as plt
# from mpl_toolkits.mplot3d import Axes3D

# hostname = socket.gethostname()
# UDP_IP = socket.gethostbyname(hostname)  # My IP is 172.20.10.7
# print("***Local ip:" + str(UDP_IP) + "***")
# UDP_PORT = 8085
# sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
# sock.bind((UDP_IP, UDP_PORT))
# sock.listen(1)  # 接收的连接数
# data, addr = sock.accept()

# # Anchor coordinates
# anchors = {
#     # "1782": (0, 0, 0),
#     # "1783": (1.46, 0, 0),
#     # "1784": (0.73, 0.9, 0),
#     # "1785": (0.73, 0.45, 0.73)    # test 1
#     # "1782": (0, 0, 0),
#     # "1783": (2.83, 0, 0),
#     # "1784": (1.42, 1.33, 0),
#     # "1785": (1.42, -0.44, 1.85)   # test 2
#     # "1782": (0, 0, 0),
#     # "1783": (3.00, 0, 0),
#     # "1784": (1.00, 1.00, 0),
#     # "1785": (1.80, 1.80, 0.66)    # test 3
#     # "1782": (0, 0, 0),
#     # "1783": (3.00, 0, 0),
#     # "1784": (1.50, 1.60, 0),
#     # "1785": (2.00, 1.50, 0.80)    # test 4
#     # "1782": (0.70, 0, 0),
#     # "1783": (3.20, 0, 0),
#     # "1784": (2.20, 2.43, 0),
#     # "1785": (3.66, 2.16, 0.91)    # test 5
#     # "1782": (0, 0, 0),
#     # "1783": (2.14, 1.81, 0),
#     # "1784": (1.20, -0.44, 1.16)   # test 6
#     # "1782": (0.5, 1.17, 0),
#     # "1783": (1.73, 2.64, 0.75),
#     # "1784": (3.09, 0, 1.18)       # test 7
#     # "1782": (0.5, 1.17, 0),
#     # "1783": (1.73, 2.36, 0),
#     # "1784": (3.09, 0, 1.18)       # test 8
#     "1782": (0.64, 0.56, 0),
#     "1783": (2.38, 1.94, 0),
#     "1784": (3.09, 0, 1.18)       # test 9
# }


# # Set up the figure and the 3D axis
# fig = plt.figure()
# ax = fig.add_subplot(111, projection='3d')
# plt.ion()

# # ax.set_xlim([0, 3])
# # ax.set_ylim([0, 3])
# # ax.set_zlim([0, 3])


# def draw_anchors(ax, anchors):
#     for anchor_id, (x, y, z) in anchors.items():
#         ax.scatter(x, y, z, c='green', marker='o')
#         ax.text(x, y, z, f'A{anchor_id} ({x:.2f}, {y:.2f}, {z:.2f})', color='black')


# def update_tag_position(ax, x, y, z):
#     ax.scatter(x, y, z, c='blue', marker='^')
#     ax.text(x, y, z, f'TAG: ({x:.2f}, {y:.2f}, {z:.2f})', color='black')


# def read_data():
#     line = data.recv(1024).decode('UTF-8')
#     uwb_list = []
#     try:
#         uwb_data = json.loads(line)
#         print(uwb_data)
#         uwb_list = uwb_data["links"]
#         for uwb_anchor in uwb_list:
#             print(uwb_anchor)
#     except:
#         print(line)
#     print("")
#     return uwb_list


# def trilateration(distances, anchors):
#     # Convert dictionary to lists
#     points = np.array([anchors[anchor_id] for anchor_id in distances.keys()])
#     distances = np.array([distances[anchor_id] for anchor_id in distances.keys()])

#     # Calculate using trilateration formula
#     P1, P2, P3 = points
#     d1, d2, d3 = distances

#     # From P1 and P2
#     ex = (P2 - P1) / np.linalg.norm(P2 - P1)
#     i = np.dot(ex, P3 - P1)
#     ey = (P3 - P1 - i * ex) / np.linalg.norm(P3 - P1 - i * ex)
#     ez = np.cross(ex, ey)
#     d = np.linalg.norm(P2 - P1)
#     j = np.dot(ey, P3 - P1)

#     x = (d1 ** 2 - d2 ** 2 + d ** 2) / (2 * d)
#     y = (d1 ** 2 - d3 ** 2 + i ** 2 + j ** 2) / (2 * j) - (i / j) * x
#     z = np.sqrt(d1 ** 2 - x ** 2 - y ** 2)

#     tag_position = P1 + x * ex + y * ey + z * ez

#     return tag_position


# def main():
#     draw_anchors(ax, anchors)
#     ax.set_xlabel('X Axis')
#     ax.set_ylabel('Y Axis')
#     ax.set_zlabel('Z Axis')

#     ax.view_init(elev=20, azim=-45)

#     while True:
#         distances = {}
#         uwb_list = read_data()
#         for one in uwb_list:
#             anchor_id = one["A"]
#             if anchor_id in anchors:
#                 range_val = float(one["R"])
#                 distances[anchor_id] = range_val

#         if len(distances) == 3:
#             x, y, z = trilateration(distances, anchors)
#             print(x, y, z)
#             ax.cla()  # Clear the previous points
#             draw_anchors(ax, anchors)  # Redraw the anchors
#             update_tag_position(ax, x, y, z)

#             # # 绘制Tag的理想路线：从Anchor1到Anchor2再到Anchor3
#             # anchor_1782 = anchors["1782"]
#             # anchor_1783 = anchors["1783"]
#             # anchor_1784 = anchors["1784"]
#             # ax.plot([anchor_1782[0], anchor_1783[0]],
#             #         [anchor_1782[1], anchor_1783[1]],
#             #         [anchor_1782[2], anchor_1783[2]],
#             #         color='red', linewidth=2)
#             # ax.plot([anchor_1783[0], anchor_1784[0]],
#             #         [anchor_1783[1], anchor_1784[1]],
#             #         [anchor_1783[2], anchor_1784[2]],
#             #         color='red', linewidth=2, label='Ideal Route of Tag')
#             # ax.legend()

#             # 绘制Tag的理想路线
#             start_point = (0.93, 2.94, 0.74)
#             end_point = (1.93, 2.94, 0.74)

#             ax.plot([start_point[0], end_point[0]],
#                     [start_point[1], end_point[1]],
#                     [start_point[2], end_point[2]],
#                     color='red', linewidth=1.5, label='Ideal Route of Tag')

#             ax.scatter(*start_point, color='red', s=50, label='Start Point (0.93, 2.94, 0.74)')
#             ax.scatter(*end_point, color='red', s=50, label='End Point (1.93, 2.94, 0.74)')

#             ax.text(start_point[0], start_point[1], start_point[2],
#                     f'({start_point[0]}, {start_point[1]}, {start_point[2]})', color='black')
#             ax.text(end_point[0], end_point[1], end_point[2], f'({end_point[0]}, {end_point[1]}, {end_point[2]})',
#                     color='black')

#             ax.legend(loc='upper left', bbox_to_anchor=(0.55, 1.03), borderaxespad=0.)

#             plt.title('ESP32 UWB DW1000 3D System', pad=14)
#             plt.draw()
#             plt.pause(1)

#         time.sleep(0.1)


# if __name__ == '__main__':
#     main()









# Blue line tag track

import time
import socket
import json
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

hostname = socket.gethostname()
UDP_IP = socket.gethostbyname(hostname)  # My IP is 172.20.10.7
print("***Local ip:" + str(UDP_IP) + "***")
UDP_PORT = 8085
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.bind((UDP_IP, UDP_PORT))
sock.listen(1)  
data, addr = sock.accept()

# Anchor coordinates
anchors = {
    "1782": (0.64, 0.56, 0),
    "1783": (2.38, 1.94, 0),
    "1784": (3.09, 0, 1.18)       # test 9
}

tag_positions = []  

# Set up the figure and the 3D axis
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
plt.ion()

def draw_anchors(ax, anchors):
    for anchor_id, (x, y, z) in anchors.items():
        ax.scatter(x, y, z, c='green', marker='o')
        ax.text(x, y, z, f'A{anchor_id} ({x:.2f}, {y:.2f}, {z:.2f})', color='black')

def update_tag_position(ax, positions):
    xs, ys, zs = zip(*positions)
    ax.plot(xs, ys, zs, '-o', color='blue', label='Actual Route of Tag')  
    # for x, y, z in positions:
    #     ax.text(x, y, z, f'TAG: ({x:.2f}, {y:.2f}, {z:.2f})', color='black')

def read_data():
    line = data.recv(1024).decode('UTF-8')
    uwb_list = []
    try:
        uwb_data = json.loads(line)
        print(uwb_data)
        uwb_list = uwb_data["links"]
        for uwb_anchor in uwb_list:
            print(uwb_anchor)
    except:
        print(line)
    print("")
    return uwb_list

def trilateration(distances, anchors):
    points = np.array([anchors[anchor_id] for anchor_id in distances.keys()])
    distances = np.array([distances[anchor_id] for anchor_id in distances.keys()])

    P1, P2, P3 = points
    d1, d2, d3 = distances

    ex = (P2 - P1) / np.linalg.norm(P2 - P1)
    i = np.dot(ex, P3 - P1)
    ey = (P3 - P1 - i * ex) / np.linalg.norm(P3 - P1 - i * ex)
    ez = np.cross(ex, ey)
    d = np.linalg.norm(P2 - P1)
    j = np.dot(ey, P3 - P1)
    x = (d1 ** 2 - d2 ** 2 + d ** 2) / (2 * d)
    y = (d1 ** 2 - d3 ** 2 + i ** 2 + j ** 2) / (2 * j) - (i / j) * x
    z = np.sqrt(d1 ** 2 - x ** 2 - y ** 2)

    tag_position = P1 + x * ex + y * ey + z * ez

    return tag_position

def main():
    draw_anchors(ax, anchors)
    ax.set_xlabel('X Axis')
    ax.set_ylabel('Y Axis')
    ax.set_zlabel('Z Axis')
    ax.view_init(elev=20, azim=-45)

    while True:
        distances = {}
        uwb_list = read_data()
        for one in uwb_list:
            anchor_id = one["A"]
            if anchor_id in anchors:
                range_val = float(one["R"])
                distances[anchor_id] = range_val

        if len(distances) == 3:
            x, y, z = trilateration(distances, anchors)
            print(x, y, z)
            tag_positions.append((x, y, z))  
            ax.cla() 
            draw_anchors(ax, anchors) 
            update_tag_position(ax, tag_positions) 

            # Draw the ideal route for Tag
            start_point = (0.93, 2.94, 0.74)
            end_point = (1.93, 2.94, 0.74)

            ax.plot([start_point[0], end_point[0]],
                    [start_point[1], end_point[1]],
                    [start_point[2], end_point[2]],
                    color='red', linewidth=1.5, label='Ideal Route of Tag')

            ax.scatter(*start_point, color='red', s=50, label='Start Point (0.93, 2.94, 0.74)')
            ax.scatter(*end_point, color='red', s=50, label='End Point (1.93, 2.94, 0.74)')

            ax.text(start_point[0], start_point[1], start_point[2],
                    f'({start_point[0]}, {start_point[1]}, {start_point[2]})', color='black')
            ax.text(end_point[0], end_point[1], end_point[2], f'({end_point[0]}, {end_point[1]}, {end_point[2]})',
                    color='black')

            ax.legend(loc='upper left', bbox_to_anchor=(0.55, 1.1), borderaxespad=0.)

            # plt.title('ESP32 UWB DW1000 3D System', pad=30)
            plt.title('ESP32 UWB DW1000 3D System with Kalman Filter', pad=30)
            plt.draw()
            plt.pause(0.5)

        time.sleep(0.1)

if __name__ == '__main__':
    main()
