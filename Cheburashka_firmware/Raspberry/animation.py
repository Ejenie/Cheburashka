import tkinter as tk
from PIL import Image, ImageTk
import cv2
import numpy as np
import math
import time
import os
import sys
import random
import threading
from pydub import AudioSegment
from pydub.playback import play

class MouthEllipse:
    def __init__(self, canvas, center_x, center_y, a=80, b_min=20, b_max=60):
        self.canvas = canvas
        self.center_x = center_x
        self.center_y = center_y
        self.a = a                     # горизонтальная полуось
        self.b_min = b_min
        self.b_max = b_max
        self.b_current = b_min
        self.b_direction = 1            # 1 - увеличение, -1 - уменьшение
        self.b_speed = 4.0             # скорость изменения высоты
        self.color = "#3D0101"
        self.outline_color = "#2A0000"
        self.ellipse_id = None
        self.create_ellipse()

    def create_ellipse(self):
        left = self.center_x - self.a
        top = self.center_y - self.b_current
        right = self.center_x + self.a
        bottom = self.center_y + self.b_current
        self.ellipse_id = self.canvas.create_oval(
            left, top, right, bottom,
            fill=self.color, outline=self.outline_color, width=4,
            tags="mouth", state='hidden'
        )

    def update_size(self):
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

    def set_visible(self, visible):
        state = 'normal' if visible else 'hidden'
        self.canvas.itemconfig(self.ellipse_id, state=state)

    def set_center(self, x, y):
        self.center_x = x
        self.center_y = y
        self.update_size()


class GlintDraw:
    def __init__(self, root, background_image_path):
        self.root = root
        self.root.title("")
        self.screen_width = self.root.winfo_screenwidth()
        self.screen_height = self.root.winfo_screenheight()
        self.root.overrideredirect(True)
        self.root.geometry(f"{self.screen_width}x{self.screen_height}+0+0")
        self.root.update()
      #  try:
       #     self.root.title("Cheburashka")
        #    os.system("xdotool search --name 'CheburashkaAnimation' windowactivate")
         #   os.system(f"xdotool getactivewindow windowsize {self.screen_width} {self.screen_height}")
          #  os.system("xdotool getactivewindow windowmove 0 0")
        #except:
        #    pass
        self.root.attributes('-fullscreen', True)
        self.root.focus_force()
        self.root.bind('q', self.quit_app)

        self.SIZE = 1
        self.GLINT_COLOR = "white"
        self.RADIUS = 10
        self.FPS = 20

        self.rotation_center_left = [self.screen_width * 0.25 + 25, self.screen_height * 0.35 + 135]
        self.rotation_center_right = [self.screen_width * 0.75 - 5, self.screen_height * 0.35 + 135]

        self.LINEAR_GOAL = 100
        self.LEFT_TARGET = [self.rotation_center_left[0] - self.LINEAR_GOAL, self.rotation_center_left[1]]
        self.RIGHT_TARGET = [self.rotation_center_right[0] + self.LINEAR_GOAL, self.rotation_center_right[1]]
        self.linear_speed = 3.0
        self.linear_direction = "left"
        self.linear_progress = 0.0

        self.mouth_center_x = self.screen_width // 2
        self.mouth_center_y = int(self.screen_height * 0.75) + 152

        self.scenarios_dir = "/home/ejenie/opencv/sources/samples/python/myCodes/scenarios"

        image_path = background_image_path
        cv_image = cv2.imdecode(np.fromfile(image_path, dtype=np.uint8), cv2.IMREAD_COLOR)
        cv_image = cv2.cvtColor(cv_image, cv2.COLOR_BGR2RGB)
        pil_image = Image.fromarray(cv_image)
        resized_image = pil_image.resize((self.screen_width, self.screen_height), Image.Resampling.LANCZOS)
        self.face_image = resized_image

        self.canvas = tk.Canvas(root, highlightthickness=0, bg='black')
        self.canvas.pack(fill=tk.BOTH, expand=True)
        self.tk_face_image = ImageTk.PhotoImage(self.face_image)
        self.face_image_id = self.canvas.create_image(0, 0, anchor=tk.NW, image=self.tk_face_image)

        self.RADIUS = int(self.screen_width * 0.08)
        self.SIZE = int(self.RADIUS * 0.2)

        self.rotation_angle_left = 0
        self.rotation_angle_right = 0
        self.rotation_speed = 2 * math.pi / self.FPS

        self.left_eye_glint_pos = self.rotation_center_left.copy()
        self.right_eye_glint_pos = self.rotation_center_right.copy()

        self.left_glint_id = self.canvas.create_oval(
            self.left_eye_glint_pos[0] - self.SIZE, self.left_eye_glint_pos[1] - self.SIZE,
            self.left_eye_glint_pos[0] + self.SIZE, self.left_eye_glint_pos[1] + self.SIZE,
            fill=self.GLINT_COLOR, outline="", tags="glintL")
        self.right_glint_id = self.canvas.create_oval(
            self.right_eye_glint_pos[0] - self.SIZE, self.right_eye_glint_pos[1] - self.SIZE,
            self.right_eye_glint_pos[0] + self.SIZE, self.right_eye_glint_pos[1] + self.SIZE,
            fill=self.GLINT_COLOR, outline="", tags="glintR")

        mouth_a = int(self.screen_width * 0.08)
        mouth_b_min = int(mouth_a * 0.25)
        mouth_b_max = int(mouth_a * 0.6)
        self.mouth = MouthEllipse(self.canvas, self.mouth_center_x, self.mouth_center_y,
                                  a=mouth_a, b_min=mouth_b_min, b_max=mouth_b_max)

        self.flag_run = True
        self.current_animation = None
        self.animation_start_time = 0
        self.animation_time_animate = 0
        self.all_animations_complete = False

        # Для независимой анимации рта
        self.mouth_animation_active = False
        self.mouth_animation_end_time = 0
        self.mouth_animation_active = False
        self.mouth_animation_end_time = 0
        self.last_mouth_update_time = 0
        self.mouth_animation_loop_id = None
        self.mouth_animation_active = False
        self.mouth_animation_end_time = 0
        self.last_mouth_update_time = 0
        
        self.mouth_fps = 10             
        self.mouth_interval = int(1000 / self.mouth_fps)
        self.mouth_animation_after_id = None

        self.root.update()

    def reset_glints_to_center(self):
        self.left_eye_glint_pos = self.rotation_center_left.copy()
        self.right_eye_glint_pos = self.rotation_center_right.copy()
        self.update_pos(self.left_glint_id, self.left_eye_glint_pos)
        self.update_pos(self.right_glint_id, self.right_eye_glint_pos)

    def show_static_face(self):
        """Показывает статичное лицо (не трогает рот, если он активен)."""
        if not hasattr(self, 'canvas') or not self.canvas.winfo_exists():
            return
        # Не скрываем рот принудительно, если он анимируется
        if not self.mouth_animation_active:
            self.mouth.set_visible(False)
        self.reset_glints_to_center()
        self.root.update()
    
    def start_mouth_animation(self, duration_sec):
        if duration_sec <= 0:
            return
        # Отменить предыдущую анимацию, если была
        if self.mouth_animation_after_id is not None:
            self.root.after_cancel(self.mouth_animation_after_id)
            self.mouth_animation_after_id = None
        self.mouth.set_visible(True)
        self.mouth_animation_active = True
        self.mouth_animation_end_time = time.time() + duration_sec
        self.last_mouth_update_time = time.time()
        self._schedule_mouth_animation()

    def stop_mouth_animation(self):
        self.mouth_animation_active = False
        if self.mouth_animation_after_id is not None:
            self.root.after_cancel(self.mouth_animation_after_id)
            self.mouth_animation_after_id = None
        self.mouth.set_visible(False)

    def _schedule_mouth_animation(self):
        if not self.mouth_animation_active:
            return
        now = time.time()
        if now >= self.mouth_animation_end_time:
            self.stop_mouth_animation()
            return
        # Реальное время между кадрами
        dt = now - self.last_mouth_update_time
        self.last_mouth_update_time = now
        if dt > 0.2:        # защита от слишком большого скачка
            dt = 0.2
        self.mouth.animate(dt)
        # Планируем следующий кадр с фиксированным интервалом
        self.mouth_animation_after_id = self.root.after(self.mouth_interval, self._schedule_mouth_animation)


    def _animate_mouth_independent(self):
        """Независимый цикл анимации рта (вызывается через root.after)."""
        if not self.mouth_animation_active:
            self._mouth_animation_loop_running = False
            return
        now = time.time()
        if now >= self.mouth_animation_end_time:
            self.stop_mouth_animation()
            self._mouth_animation_loop_running = False
            return
        dt = 1.0 / self.FPS
        self.mouth.animate(dt)
        self._mouth_animation_loop_running = True
        self.root.after(int(1000 / self.FPS), self._animate_mouth_independent)

    def start_animation(self, scenario_name):
        """Запускает сценарий анимации без блокировки."""
        if not hasattr(self, 'canvas') or not self.canvas.winfo_exists():
            return
        self.execute_specific_scenario(scenario_name)

    def quit_app(self, event=None):
        print("Завершение программы...")
        self.all_animations_complete = True
        self.flag_run = False
        try:
            self.root.quit()
            self.root.destroy()
        except:
            pass
        sys.exit(0)

    def execute_specific_scenario(self, scenario_name):
        try:
            file_path = os.path.join(self.scenarios_dir, scenario_name)
            if not os.path.exists(file_path):
                print(f"Файл сценария не найден")
                self.all_animations_complete = True
                return
            with open(file_path, 'r', encoding='utf-8') as file:
                commands = file.read().strip().split('\n')
            if not commands or (len(commands) == 1 and commands[0] == ''):
                print(f"Сценарий пуст")
                self.all_animations_complete = True
                return
            self.scenery_commands = commands
            self.index_now = 0
            self.run_next_command()
        except Exception as e:
            print(f"Ошибка при чтении сценария")
            self.all_animations_complete = True
    def execute_random_scenario(self):
        try:            
            scenario_files = [f for f in os.listdir(self.scenarios_dir) 
                            if f.endswith('.txt')]
            if not scenario_files:
                self.all_animations_complete = True
                return
            
            random_file = random.choice(scenario_files)
            file_path = os.path.join(self.scenarios_dir, random_file)
            
            with open(file_path, 'r', encoding='utf-8') as file:
                commands = file.read().strip().split('\n')
            
            self.scenery_commands = commands
            self.index_now = 0
            self.run_next_command()

        except FileNotFoundError:
            print("Файл не найден")
            self.all_animations_complete = True

    def run_next_command(self):
        if self.index_now >= len(self.scenery_commands):
            print("Сценарий завершен")
            self.all_animations_complete = True
            self.stop_animation()
            return
        command_line = self.scenery_commands[self.index_now].strip()
        if not command_line:
            self.index_now += 1
            self.root.after(10, self.run_next_command)
            return
        if command_line.lower() == "stop":
            self.stop_animation()
            self.all_animations_complete = True
            return
        parts = command_line.split()
        command = parts[0]
        time_animate = int(parts[1])
        self.animation_start_time = time.time()
        self.animation_time_animate = time_animate
        self.current_animation = command
        self.flag_run = True
        if command == "animate_glint_linear":
            self.animate_glint_linear()
        elif command == "animate_glint_circling":
            self.animate_glint_circling()
        elif command == "animate_mouth":
            self.start_mouth_animation(time_animate)
            self.index_now += 1
            self.root.after(10, self.run_next_command)
            return
        else:
            self.index_now += 1
            self.root.after(10, self.run_next_command)
            return
        self.check_time()

    def check_time(self):
        if not self.flag_run or self.current_animation is None:
            return
        if time.time() - self.animation_start_time >= self.animation_time_animate:
            self.current_animation = None
            self.index_now += 1
            self.root.after(10, self.run_next_command)
        else:
            self.root.after(100, self.check_time)

    def stop_animation(self):
        self.flag_run = False
        self.current_animation = None

    def position_now(self, center, radius, angle):
        x = center[0] + radius * math.cos(angle)
        y = center[1] + radius * math.sin(angle)
        return [int(x), int(y)]

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
        new_left_pos = self.position_now(self.rotation_center_left, self.RADIUS, self.rotation_angle_left)
        new_right_pos = self.position_now(self.rotation_center_right, self.RADIUS, self.rotation_angle_right)
        self.update_pos(self.left_glint_id, new_left_pos)
        self.update_pos(self.right_glint_id, new_right_pos)
        self.left_eye_glint_pos = new_left_pos
        self.right_eye_glint_pos = new_right_pos
        if self.current_animation == "animate_glint_circling":
            self.root.after(int(1000 / self.FPS), self.animate_glint_circling)


def run_scenario(scenario_filename, background_path):
    root = tk.Tk()
    app = GlintDraw(root, background_path)
    app.execute_specific_scenario(scenario_filename)
    root.mainloop()
    root.destroy()

def run_gena_scenario(background_path):
    run_scenario("gena.txt", background_path)

def run_orange_scenario(background_path):
    run_scenario("orange.txt", background_path)

def run_rat_scenario(background_path):
    run_scenario("rat.txt", background_path)

def run_green_scenario(background_path):
    run_scenario("green.txt", background_path)

def run_random_scenario(background_path):
    root = tk.Tk()
    app = GlintDraw(root, background_path)
    app.execute_random_scenario()
    root.mainloop()
    root.destroy()
    
def play_sound_with_mouth_movement(sound, app, duration_sec=None, is_silence = False):
    if duration_sec is None:
        if hasattr(sound, 'duration_seconds'):
            duration_sec = sound.duration_seconds
        else:
            for _ in sound:
                duration_sec = len(_) / 1000.0
    # Воспроизводим звук в отдельном потоке
    def play_thread():
        play(sound)
    threading.Thread(target=play_thread, daemon=True).start()
    time.sleep(1.0)
    app.start_mouth_animation(duration_sec + 0.5)
    

