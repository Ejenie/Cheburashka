import tkinter as tk
from PIL import Image, ImageTk
import cv2
import numpy as np
import math
import time

class GlintDraw:
    def __init__(self, root):
        self.root = root
        self.root.title("window")
        self.root.attributes('-fullscreen', True)
        
        self.SIZE = 22
        self.TIME_LOOP = 15
        self.GLINT_COLOR = "white"
        self.RADIUS = 85
        self.FPS = 60
        
        self.rotation_center_left = [335, 350]
        self.rotation_center_right = [945, 350]
        
        self.LINEAR_GOAL = 100
        self.LEFT_TARGET = [self.rotation_center_left[0] - self.LINEAR_GOAL, self.rotation_center_left[1]]
        self.RIGHT_TARGET = [self.rotation_center_right[0] + self.LINEAR_GOAL, self.rotation_center_right[1]] 
        self.linear_speed = 3.0 
        self.linear_direction = "left"
        self.linear_progress = 0.0
        
        image_path = "C:/Users/DELL/Desktop/мордочки/face_empty.png"
            
        cv_image = cv2.imdecode(np.fromfile(image_path, dtype=np.uint8), cv2.IMREAD_COLOR)
            
        #tkinter part
        cv_image = cv2.cvtColor(cv_image, cv2.COLOR_BGR2RGB)
        self.face_image = Image.fromarray(cv_image)
        self.canvas = tk.Canvas(root, highlightthickness=0)
        self.canvas.pack(fill=tk.BOTH, expand=True)
        self.tk_face_image = ImageTk.PhotoImage(self.face_image)
        self.face_image_id = self.canvas.create_image(0, 0, anchor=tk.NW, image=self.tk_face_image)
        
        self.rotation_angle_left = 0
        self.rotation_angle_right = 0
        self.flag_run = True
        self.rotation_speed = 2 * math.pi / self.FPS
        self.rotation_speed_2 = 2 * math.pi / self.FPS
        
        self.left_eye_glint_pos = self.position_now(
            self.rotation_center_left, 
            self.RADIUS, 
            self.rotation_angle_left)
        self.right_eye_glint_pos = self.position_now(
            self.rotation_center_right,
            self.RADIUS,
            self.rotation_angle_right)
        
        self.left_glint_id = self.canvas.create_oval(
            self.left_eye_glint_pos[0] - self.SIZE,
            self.left_eye_glint_pos[1] - self.SIZE,
            self.left_eye_glint_pos[0] + self.SIZE,
            self.left_eye_glint_pos[1] + self.SIZE,
            fill=self.GLINT_COLOR, outline="", tags="glintL")
        
        self.right_glint_id = self.canvas.create_oval(
            self.right_eye_glint_pos[0] - self.SIZE,
            self.right_eye_glint_pos[1] - self.SIZE,
            self.right_eye_glint_pos[0] + self.SIZE,
            self.right_eye_glint_pos[1] + self.SIZE,
            fill=self.GLINT_COLOR, outline="", tags="glintR")
        
        self.SIZE_CENTER = 1
        self.left_centre = self.canvas.create_oval(
            self.rotation_center_left[0] - self.SIZE_CENTER,
            self.rotation_center_left[1] - self.SIZE_CENTER,
            self.rotation_center_left[0] + self.SIZE_CENTER,
            self.rotation_center_left[1] + self.SIZE_CENTER,
            fill="red", outline="", tags="center")
        
        self.right_centre = self.canvas.create_oval(
            self.rotation_center_right[0] - self.SIZE_CENTER,
            self.rotation_center_right[1] - self.SIZE_CENTER,
            self.rotation_center_right[0] + self.SIZE_CENTER,
            self.rotation_center_right[1] + self.SIZE_CENTER,
            fill="red", outline="", tags="center")        
        
        self.root.bind('<Configure>', self.on_resize)
        
    def position_now(self, center, radius, angle):
        x = center[0] + radius * math.cos(angle)
        y = center[1] + radius * math.sin(angle)
        return [int(x), int(y)]
    
    def stop_animation(self):
        self.flag_run = False

    def animate(self):
        time_now = time.time()
        if time_now - self.start_time >= self.TIME_LOOP:
            self.stop_animation()
        elif cv2.waitKey(1) == ord('q'):
            quit()
            
        if not self.flag_run:
            return
            
        self.rotation_angle_left += self.rotation_speed
        self.rotation_angle_right += self.rotation_speed
        new_left_pos = self.position_now(
            self.rotation_center_left, self.RADIUS, self.rotation_angle_left)
        new_right_pos = self.position_now(
            self.rotation_center_right, self.RADIUS, self.rotation_angle_right)
        self.update_pos(self.left_glint_id, new_left_pos)
        self.update_pos(self.right_glint_id, new_right_pos)
        self.left_eye_glint_pos = new_left_pos
        self.right_eye_glint_pos = new_right_pos
        
        self.root.after(int(1000 / self.FPS), self.animate)
    
    def animate_linear(self):
        time_now = time.time()
        if time_now - self.start_time >= self.TIME_LOOP:
            self.stop_animation()
        elif cv2.waitKey(1) == ord('q'):
            quit()
            
        if not self.flag_run:
            return
        self.linear_progress += self.linear_speed / self.FPS
        
        if self.linear_progress >= 1.0:
            self.linear_progress = 0.0
            
            if self.linear_direction == "left":
                self.linear_direction = "center_from_left"
            elif self.linear_direction == "center_from_left":
                self.linear_direction = "right"
            elif self.linear_direction == "right":
                self.linear_direction = "center_from_right"
            elif self.linear_direction == "center_from_right":
                self.linear_direction = "left"
        
        if self.linear_direction == "left":
            new_left_pos = self.inter(self.rotation_center_left, self.LEFT_TARGET, self.linear_progress)
            new_right_pos = self.inter(self.rotation_center_right, 
                                     [self.rotation_center_right[0] - 100, self.rotation_center_right[1]], 
                                     self.linear_progress)
        
        elif self.linear_direction == "center_from_left":
            new_left_pos = self.inter(self.LEFT_TARGET, self.rotation_center_left, self.linear_progress)
            new_right_pos = self.inter([self.rotation_center_right[0] - 100, self.rotation_center_right[1]], 
                                     self.rotation_center_right, 
                                     self.linear_progress)
        
        elif self.linear_direction == "right":
            new_left_pos = self.inter(self.rotation_center_left, 
                                    [self.rotation_center_left[0] + 100, self.rotation_center_left[1]], 
                                    self.linear_progress)
            new_right_pos = self.inter(self.rotation_center_right, self.RIGHT_TARGET, self.linear_progress)
        
        elif self.linear_direction == "center_from_right":
            new_left_pos = self.inter([self.rotation_center_left[0] + 100, self.rotation_center_left[1]], 
                                    self.rotation_center_left, 
                                    self.linear_progress)
            new_right_pos = self.inter(self.RIGHT_TARGET, self.rotation_center_right, self.linear_progress)
        
        self.update_pos(self.left_glint_id, new_left_pos)
        self.update_pos(self.right_glint_id, new_right_pos)
        self.left_eye_glint_pos = new_left_pos
        self.right_eye_glint_pos = new_right_pos
        
        self.root.after(int(1000 / self.FPS), self.animate_linear)
    
    def inter(self, start, end, progress):
        x = start[0] + (end[0] - start[0]) * progress
        y = start[1] + (end[1] - start[1]) * progress
        return [int(x), int(y)]
        
    def update_pos(self, glint_id, new_position):
        self.canvas.coords(
            glint_id,
            new_position[0] - self.SIZE,
            new_position[1] - self.SIZE,
            new_position[0] + self.SIZE,
            new_position[1] + self.SIZE)
    
    def on_resize(self, event):
        self.resize_image()

    def resize_image(self):
        width = self.root.winfo_width()
        height = self.root.winfo_height()
        resized_image = self.face_image.resize((width, height), Image.Resampling.LANCZOS)
        self.tk_face_image = ImageTk.PhotoImage(resized_image)
        self.canvas.itemconfig(self.face_image_id, image=self.tk_face_image)

root = tk.Tk()
while True:
    app = GlintDraw(root)
    app.start_time = time.time()
    app.TIME_LOOP = 15  

    
    # app.animate()        
    app.animate_linear()  
    
    root.mainloop()