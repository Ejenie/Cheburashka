import tkinter as tk
from PIL import Image, ImageTk
import cv2
import numpy as np
import math
import time
import os


class PupilMovement:
    def __init__(self, canvas, eye_center_x, eye_center_y, pupil_radius=15, eye_radius=85):
        self.canvas = canvas
        self.eye_center_x = eye_center_x
        self.eye_center_y = eye_center_y
        self.pupil_radius = pupil_radius
        self.eye_radius = eye_radius
        
        self.current_x = eye_center_x
        self.current_y = eye_center_y
        self.amplitude = eye_radius - pupil_radius - 5  
        self.speed = 2.0 
        self.direction = 1 
        self.movement_progress = 0.0
        
        self.pupil_color = "#000000"  
        self.pupil_outline = "#2A1610" 
        
        self.pupil_id = None
        self.is_moving = True
        
        self.create_pupil()
    
    def create_pupil(self):
        left = self.current_x - self.pupil_radius
        top = self.current_y - self.pupil_radius
        right = self.current_x + self.pupil_radius
        bottom = self.current_y + self.pupil_radius
        
        self.pupil_id = self.canvas.create_oval(
            left, top, right, bottom,
            fill=self.pupil_color, 
            outline=self.pupil_outline, 
            width=2, 
            tags="pupil"
        )
    
    def update_position(self, progress=None):
        if progress is not None:
            movement = -self.amplitude + 2 * self.amplitude * progress
        else:
            movement = self.amplitude * math.sin(self.movement_progress * math.pi)
        
        self.current_x = self.eye_center_x + movement
        
        left = self.current_x - self.pupil_radius
        top = self.current_y - self.pupil_radius
        right = self.current_x + self.pupil_radius
        bottom = self.current_y + self.pupil_radius
        
        self.canvas.coords(self.pupil_id, left, top, right, bottom)
    
    def animate_horizontal(self, dt):
        if not self.is_moving:
            return
        
        self.movement_progress += self.speed * dt
        
        if self.movement_progress >= 1.0:
            self.movement_progress = 1.0
            self.speed = -abs(self.speed)  
        elif self.movement_progress <= 0.0:
            self.movement_progress = 0.0
            self.speed = abs(self.speed)  
        
        self.update_position()
    
    def animate_linear(self, dt):
        if not self.is_moving:
            return
        
        self.movement_progress += self.speed * dt
        
        if self.movement_progress >= 1.0:
            self.movement_progress = 1.0
            self.direction = -1
        elif self.movement_progress <= 0.0:
            self.movement_progress = 0.0
            self.direction = 1
        
        start_x = self.eye_center_x - self.amplitude
        end_x = self.eye_center_x + self.amplitude
        
        if self.direction == 1:
            current_movement = start_x + (end_x - start_x) * self.movement_progress
        else:
            current_movement = end_x - (end_x - start_x) * self.movement_progress
        
        self.current_x = current_movement
        self.update_position()
    
    def animate_smooth(self, dt):
        if not self.is_moving:
            return
        
        self.movement_progress += self.speed * dt
        
        sine_value = math.sin(self.movement_progress)
        
        progress_normalized = (sine_value + 1) / 2
        
        start_x = self.eye_center_x - self.amplitude
        end_x = self.eye_center_x + self.amplitude
        self.current_x = start_x + (end_x - start_x) * progress_normalized
        
        self.update_position()
    
    def set_target(self, target_x, target_y, speed=2.0):
        dx = target_x - self.eye_center_x
        dy = target_y - self.eye_center_y
        distance = math.sqrt(dx**2 + dy**2)
        
        if distance > self.amplitude:
            scale = self.amplitude / distance
            target_x = self.eye_center_x + dx * scale
            target_y = self.eye_center_y + dy * scale
        
        self.current_x += (target_x - self.current_x) * speed * 0.05
        self.current_y += (target_y - self.current_y) * speed * 0.05
        
        self.update_position()
    
    def stop_movement(self):
        self.is_moving = False
    
    def start_movement(self):
        self.is_moving = True
    
    def set_center(self, center_x, center_y):
        self.eye_center_x = center_x
        self.eye_center_y = center_y
        self.update_position()


class MouthEllipse:
    def __init__(self, canvas, center_x, center_y):
        self.canvas = canvas
        self.center_x = center_x
        self.center_y = center_y
        self.a = 80  
        self.b_min = 20
        self.b_max = 60
        self.b_current = self.b_min
        self.b_direction = 1
        self.b_speed = 1.5
        
        self.color = "#3D0101"
        self.outline_color = "#2A1610"  
        self.ellipse_id = None
        
        self.create_ellipse()
    
    def create_ellipse(self):
        left = self.center_x - self.a
        top = self.center_y - self.b_current
        right = self.center_x + self.a
        bottom = self.center_y + self.b_current
        
        self.ellipse_id = self.canvas.create_oval(
            left, top, right, bottom,
            fill=self.color, outline=self.outline_color, width=2, tags="mouth"
        )
    
    def update_size(self, progress=None):
        if progress is not None:
            self.b_current = self.b_min + (self.b_max - self.b_min) * progress
        
        left = self.center_x - self.a
        top = self.center_y - self.b_current
        right = self.center_x + self.a
        bottom = self.center_y + self.b_current
        
        self.canvas.coords(self.ellipse_id, left, top, right, bottom)
    
    def animate(self, dt):
        self.b_current += self.b_direction * self.b_speed * dt * 60
        
        if self.b_current >= self.b_max:
            self.b_current = self.b_max
            self.b_direction = -1
        elif self.b_current <= self.b_min:
            self.b_current = self.b_min
            self.b_direction = 1
        
        self.update_size()


class GlintDraw:
    def __init__(self, root):
        self.root = root
        self.root.title("window")
        self.root.attributes('-fullscreen', True)
        
        self.SIZE = 22
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
        
        self.mouth_center_x = 640
        self.mouth_center_y = 620  
        self.mouth_animation_speed = 2.0
        self.mouth_animation_progress = 0.0
        self.mouth_animation_direction = 1
        self.mouth_ellipse = None
        
        image_path = "C:/Users/DELL/reposGitHub/Cheburashka/Cheburashka_firmware/Raspberry/мордочка1.png"
            
        cv_image = cv2.imdecode(np.fromfile(image_path, dtype=np.uint8), cv2.IMREAD_COLOR)
            
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
        
        self.left_pupil = PupilMovement(
            self.canvas, 
            self.rotation_center_left[0], 
            self.rotation_center_left[1],
            pupil_radius=15,
            eye_radius=self.RADIUS
        )
        
        self.right_pupil = PupilMovement(
            self.canvas, 
            self.rotation_center_right[0], 
            self.rotation_center_right[1],
            pupil_radius=15,
            eye_radius=self.RADIUS
        )

        self.create_mouth()
        
        self.current_animation = None
        self.animation_start_time = 0
        self.animation_time_animate = 0
        
        self.root.bind('<Configure>', self.on_resize)
    
    def create_mouth(self):
        self.mouth_ellipse = MouthEllipse(self.canvas, self.mouth_center_x, self.mouth_center_y)
    
    def animate_pupils_linear(self):
        if not self.flag_run:
            return
        
        dt = 1.0 / self.FPS
        self.left_pupil.animate_linear(dt)
        self.right_pupil.animate_linear(dt)
        
        if self.current_animation == "animate_pupil_linear":
            self.root.after(int(1000 / self.FPS), self.animate_pupils_linear)
    
    def animate_glint_linear(self):
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
        
        if self.current_animation == "animate_glint_linear":
            self.root.after(int(1000 / self.FPS), self.animate_glint_linear)
    
    def animate_glint_circling(self):
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
        
        if self.current_animation == "animate_glint_circling":
            self.root.after(int(1000 / self.FPS), self.animate_glint_circling)
    
    def animate_mouth(self):
        if not self.flag_run:
            return
        
        dt = 1.0 / self.FPS
        self.mouth_ellipse.animate(dt)
        
        if self.current_animation == "animate_mouth":
            self.root.after(int(1000 / self.FPS), self.animate_mouth)
    
    def position_now(self, center, radius, angle):
        x = center[0] + radius * math.cos(angle)
        y = center[1] + radius * math.sin(angle)
        return [int(x), int(y)]
    
    def stop_animation(self):
        self.flag_run = False
        self.current_animation = None
    
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
        
        self.mouth_center_x = width // 2
        self.mouth_center_y = int(height * 0.8)
        if self.mouth_ellipse:
            self.mouth_ellipse.center_x = self.mouth_center_x
            self.mouth_ellipse.center_y = self.mouth_center_y
            self.mouth_ellipse.update_size()
    
    def execute_scenery(self):       
        try:
            with open("C:/Users/DELL/reposGitHub/Cheburashka/Cheburashka_firmware/Raspberry/scenery.txt", 
                      'r', encoding='utf-8') as file:
                commands = file.read().strip().split('\n')
            
            self.scenery_commands = commands
            self.index_now = 0
            self.run_next_command()

        except FileNotFoundError:
            print(f"Файл не найден")
    
    def run_next_command(self):
        command_line = self.scenery_commands[self.index_now].strip()
        
        if command_line.lower() == "stop":
            self.stop_animation()
            return
        
        parts = command_line.split()
        if len(parts) != 2:
            print("error", command_line)
            self.index_now += 1
            return
        
        command = parts[0]
        time_animate = int(parts[1])
        
        self.animation_start_time = time.time()
        self.animation_time_animate = time_animate
        self.current_animation = command
        self.flag_run = True
        
        if command == "animate_pupil_linear":
            self.animate_pupils_linear()
        elif command == "animate_glint_linear":
            self.animate_glint_linear()
        elif command == "animate_glint_circling":
            self.animate_glint_circling()
        elif command == "animate_mouth":
            self.animate_mouth()
        else:
            print("error", command)
            self.index_now += 1
            return
        
        self.check_time()
    
    def check_time(self):
        if not self.flag_run or self.current_animation is None:
            return
        
        current_time = time.time()
        elapsed_time = current_time - self.animation_start_time
        
        if elapsed_time >= self.animation_time_animate:
            self.current_animation = None
            self.index_now += 1
            self.root.after(10, self.run_next_command)
        else:
            self.root.after(100, self.check_time)


while True:
    root = tk.Tk()
    app = GlintDraw(root)
    app.execute_scenery()
    root.mainloop()