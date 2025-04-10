import tkinter as tk
from tkinter import ttk
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from mpl_toolkits.mplot3d import Axes3D
from mpl_toolkits.mplot3d.art3d import Poly3DCollection
import serial
import serial.tools.list_ports
import threading



class DroneAttitudeGUI:
    def __init__(self, master):
        self.master = master
        master.title("无人机姿态显示")

        # 初始化无人机姿态参数
        self.pitch = 0.0  # 俯仰角
        self.roll = 0.0   # 横滚角
        self.yaw = 0.0    # 偏航角
        self.port = ""
        self.baudrate = 115200
        self.ser = None
        # 创建 Matplotlib 图形
        self.fig = plt.figure(figsize=(15, 9))
        self.ax = self.fig.add_subplot(111, projection='3d')
        self.ax.set_xlim([-5, 5])
        self.ax.set_ylim([-5, 5])
        self.ax.set_zlim([-5, 5])
        self.ax.set_xlabel('X')
        self.ax.set_ylabel('Y')
        self.ax.set_zlabel('Z')

        # 将 Matplotlib 图形嵌入到 Tkinter 窗口
        self.canvas = FigureCanvasTkAgg(self.fig, master=master)
        self.canvas.get_tk_widget().grid(row=0, column=0, rowspan=5)

        # 创建控制滑块
        self.create_sliders()

        # 初始化无人机模型
        self.drone_body = None
        self.update_drone()

        # 串口相关
        
        self.create_serial_widgets()
        


    def close_serial(self):
        """关闭串口"""
        if self.ser and self.ser.is_open:
            self.ser.close()
            print(f"串口 {self.port} 已成功关闭")

    def send_data(self, data):
        """发送数据到串口"""
        if self.ser and self.ser.is_open:
            self.ser.write(data.encode())  # 将字符串编码为字节后发送
            print(f"已发送数据: {data}")
        else:
            print("串口未打开，无法发送数据")

    def read_data(self):
        """从串口读取数据"""
        while self.ser and self.ser.is_open:
            try:
                data = self.ser.readline().decode('utf-8').strip()  # 读取一行数据并解码
                if ("pitch" in data) and ("roll" in data) and ("yall" in data):
                    datarr  = data.split(",")
                    pitch = datarr[0].split(":")[1]
                    roll = datarr[1].split(":")[1]
                    yall = datarr[2].split(":")[1]
                    print(f"{pitch,roll,yall}")
                    self.update_angle('pitch', float(pitch))
                    self.update_angle('roll', float(pitch))
                    self.update_angle('yaw', float(pitch))
            except Exception as e:
                print(f"读取数据时发生错误: {e}")
                break

    def start_reading(self):
        """启动一个线程来持续读取数据"""
        self.read_thread = threading.Thread(target=self.read_data, daemon=True)
        self.read_thread.start()
        
    def create_sliders(self):
        # 俯仰角滑块
        self.pitch_slider = ttk.Scale(
            self.master, from_=-90, to=90, orient=tk.HORIZONTAL,
            command=lambda val: self.update_angle('pitch', float(val))
        )
        self.pitch_slider.grid(row=0, column=1, padx=10, pady=5)
        ttk.Label(self.master, text="俯仰 (Pitch)").grid(row=0, column=2)

        # 横滚角滑块
        self.roll_slider = ttk.Scale(
            self.master, from_=-90, to=90, orient=tk.HORIZONTAL,
            command=lambda val: self.update_angle('roll', float(val))
        )
        self.roll_slider.grid(row=1, column=1, padx=10, pady=5)
        ttk.Label(self.master, text="横滚 (Roll)").grid(row=1, column=2)

        # 偏航角滑块
        self.yaw_slider = ttk.Scale(
            self.master, from_=-180, to=180, orient=tk.HORIZONTAL,
            command=lambda val: self.update_angle('yaw', float(val))
        )
        self.yaw_slider.grid(row=2, column=1, padx=10, pady=5)
        ttk.Label(self.master, text="偏航 (Yaw)").grid(row=2, column=2)

        # 重置按钮
        ttk.Button(
            self.master, text="重置姿态",
            command=self.reset_attitude
        ).grid(row=3, column=1, columnspan=2)
        

    def update_angle(self, angle_type, value):
        # 更新角度值
        if angle_type == 'pitch':
            self.pitch = np.deg2rad(value)
        elif angle_type == 'roll':
            self.roll = np.deg2rad(value)
        elif angle_type == 'yaw':
            self.yaw = np.deg2rad(value)

        self.update_drone()

    def reset_attitude(self):
        # 重置所有角度
        self.pitch = 0.0
        self.roll = 0.0
        self.yaw = 0.0
        self.pitch_slider.set(0)
        self.roll_slider.set(0)
        self.yaw_slider.set(0)
        self.update_drone()

    def rotation_matrix(self):
        # 计算旋转矩阵（Z - Y - X 顺序）
        cy = np.cos(self.yaw)
        sy = np.sin(self.yaw)
        cp = np.cos(self.pitch)
        sp = np.sin(self.pitch)
        cr = np.cos(self.roll)
        sr = np.sin(self.roll)

        return np.array([
            [cy * cp, cy * sp * sr - sy * cr, cy * sp * cr + sy * sr],
            [sy * cp, sy * sp * sr + cy * cr, sy * sp * cr - cy * sr],
            [-sp, cp * sr, cp * cr]
        ])

    def update_drone(self):
        # 清除旧图形
        if self.drone_body is not None:
            self.drone_body.remove()

        # 定义无人机基本形状（长方体）
        vertices = np.array([
            [-2, -2, -1],
            [2, -2, -1],
            [2, 2, -1],
            [-2, 2, -1],
            [-2, -2, 1],
            [2, -2, 1],
            [2, 2, 1],
            [-2, 2, 1]
        ])

        # 应用旋转矩阵
        R = self.rotation_matrix()
        rotated_vertices = np.dot(vertices, R.T)

        # 定义长方体的面
        faces = [
            [rotated_vertices[0], rotated_vertices[1], rotated_vertices[2], rotated_vertices[3]],
            [rotated_vertices[4], rotated_vertices[5], rotated_vertices[6], rotated_vertices[7]],
            [rotated_vertices[0], rotated_vertices[1], rotated_vertices[5], rotated_vertices[4]],
            [rotated_vertices[2], rotated_vertices[3], rotated_vertices[7], rotated_vertices[6]],
            [rotated_vertices[0], rotated_vertices[3], rotated_vertices[7], rotated_vertices[4]],
            [rotated_vertices[1], rotated_vertices[2], rotated_vertices[6], rotated_vertices[5]]
        ]

        # 绘制无人机主体
        self.drone_body = self.ax.add_collection3d(Poly3DCollection(faces, facecolors='skyblue', linewidths=1, edgecolors='k', alpha=0.8))

        # 更新画布
        self.canvas.draw()

    def create_serial_widgets(self):
        # 串口选择下拉框
        ports = [port.device for port in serial.tools.list_ports.comports()]
        self.port_var = tk.StringVar()
        self.port_var.set(ports[0] if ports else "")
        self.port_combobox = ttk.Combobox(self.master, textvariable=self.port_var, values=ports)
        self.port_combobox.grid(row=4, column=1, padx=10, pady=5)
        #ttk.Label(self.master, text="选择串口").grid(row=4, column=2)
        # 重置按钮
        ttk.Button(
            self.master, text="选择串口",
            command=self.selectCOM
        ).grid(row=3, column=1, columnspan=2)
        # 断开按钮
        ttk.Button(
            self.master, text="关闭串口",
            command=self.disconnectCOM
        ).grid(row=3, column=2, columnspan=2)

    def selectCOM(self):
        try:
            # 配置串口参数
            self.port = self.port_var.get()
            self.baudrate = 115200

            """打开串口"""
            try:
                self.ser = serial.Serial(self.port, self.baudrate, timeout=1)
                if self.ser.is_open:
                    print(f"串口 {self.port} 已成功打开")
            except serial.SerialException as e:
                print(f"无法打开串口: {e}")

            self.start_reading()

            print("串口打开成功",self.port)
            #self.master.after(1, self.read_serial_data)  # 每秒读取一次
        except serial.SerialException as e:
            print(f"串口打开失败: {e}")
        
    def disconnectCOM(self):
        self.close_serial()

    def read_serial_data(self):
        if self.ser and self.ser.is_open:
            try:
                line = self.ser.readline().decode('utf-8').strip()
                if line:
                    try:
                        pitch, roll, yaw = map(float, line.split())  # 解析 pitch roll yaw 数据
                        self.pitch = np.deg2rad(pitch)
                        self.roll = np.deg2rad(roll)
                        self.yaw = np.deg2rad(yaw)
                        self.pitch_slider.set(pitch)
                        self.roll_slider.set(roll)
                        self.yaw_slider.set(yaw)
                        self.update_drone()
                    except ValueError:
                        print(f"无效的串口数据: {line}")
            except serial.SerialException as e:
                print(f"串口读取错误: {e}")
        self.master.after(1, self.read_serial_data)  # 每秒读取一次


if __name__ == "__main__":
    root = tk.Tk()
    app = DroneAttitudeGUI(root)
    root.mainloop()
