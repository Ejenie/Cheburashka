import cv2
import cv2
import threading
import serial
import time
import tkinter as tk
from pydub import AudioSegment
from pydub.playback import play
from animation import GlintDraw, play_sound_with_mouth_movement

STATE_IDLE = 0
STATE_REACT = 1

current_state = STATE_IDLE
reaction_end_time = 0
reaction_trigger = None

# ---------------------- Работа со звуком ----------------------
DEFAULT_IMAGE = '/home/ejenie/opencv/sources/samples/python/faces/actualEmotion/smile_forward.png'
REACT_IMAGE_GENA = '/home/ejenie/opencv/sources/samples/python/faces/actualEmotion/scream/scream_up.png'
REACT_IMAGE_ORANGE = '/home/ejenie/opencv/sources/samples/python/мордочкиБольшие/улыбка_прямо.png'
REACT_IMAGE_GREEN = '/home/ejenie/opencv/sources/samples/python/мордочкиБольшие/улыбка_прямо.png'
background = '/home/ejenie/opencv/sources/samples/python/faces/empty.png'

sound_greet1 = AudioSegment.from_wav('1.wav')
sound_greet2 = AudioSegment.from_wav('2.wav')
sound_prak1 = AudioSegment.from_wav('prak3.1.wav')
sound_prak2 = AudioSegment.from_wav('prak3.2.wav')
sound_orangeAdd = AudioSegment.from_wav('orangeAdd.wav')
sound_surprise = AudioSegment.from_wav('сюрприз.wav')
sound_oy = AudioSegment.from_wav('ойойой.wav')
sound_fall = AudioSegment.from_wav('упал.wav')
sound_rat = AudioSegment.from_wav('лариска.wav')
sound_silence = AudioSegment.from_wav('tish.wav')
sound_end = AudioSegment.from_wav('end.wav')
sound_orange = AudioSegment.from_wav('orange.wav')

def play_async(sound):
    def _play():
        play(sound)
    threading.Thread(target=_play, daemon=True).start()

trigger_states = [True, True, True, True]

# ---------------------- Работа с Arduino ----------------------
def send_trigger_to_arduino(trigger_number):
    if trigger_number == 1:# and trigger_states[0]:
        print('Распознал красный')
        for _ in range(5):
            serial_port.write(b'gena\n')
    elif trigger_number == 2:# and trigger_states[1]:
        print('Распознал синий')
        for _ in range(5):
            serial_port.write(b'blue\n')
    elif trigger_number == 3:# and trigger_states[2]:
        print('Распознал оранжевый')
        for _ in range(5):
            serial_port.write(b'orange\n')
    elif trigger_number == 4:# and trigger_states[3]:
        print('Распознал зелёный')
        for _ in range(5):
            serial_port.write(b'green\n')
    else:
        serial_port.write(b'nothing')
        print("nothing")
    if trigger_number != 4:
        trigger_states[trigger_number - 1] = False

# ---------------------- Работа со внешними картинками ----------------------
window_name = "FRONT_IMAGE"
def open_fullscreen_image_fast(image_path):
    image = cv2.imread(image_path)
    if image is None:
        print("Ошибка: изображение не найдено или не может быть загружено.")
        return
    
    cv2.namedWindow(window_name, cv2.WND_PROP_FULLSCREEN)
    cv2.setWindowProperty(window_name, cv2.WND_PROP_FULLSCREEN, cv2.WINDOW_FULLSCREEN)
    cv2.imshow(window_name, image)
    
    return image
    
# ---------------------- Поиск цвета ----------------------
def find_color_center(mask, img):
    moments = cv2.moments(mask, 255)
    area = moments['m00']
    if area == 0:
        return None
    x_center = int(moments['m10'] / area)
    y_center = int(moments['m01'] / area)
    cv2.circle(img, (x_center, y_center), 20, (50, 200, 78), -1)
    return {'x': x_center, 'y': y_center, 'area': area}

last_trigger_time = {1: 0, 2: 0, 3: 0, 4: 0}
TRIGGER_COOLDOWN = 3.0

app = None
def check_color(frame, hsv_settings, min_area, trigger_number, react_image_path, sounds_list, scenario_filename, random_flag = False):
    global current_state, reaction_end_time, reaction_trigger, last_trigger_time, app
    if current_state != STATE_IDLE:
        return False
    current_time = time.time()
    if current_time - last_trigger_time.get(trigger_number, 0) < TRIGGER_COOLDOWN:
        return False
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    lower = (hsv_settings[0], hsv_settings[1], hsv_settings[2])
    upper = (hsv_settings[3], hsv_settings[4], hsv_settings[5])
    mask = cv2.inRange(hsv, lower, upper)
    info = find_color_center(mask, frame)
    if info and info['area'] > min_area:
        last_trigger_time[trigger_number] = current_time
        current_state = STATE_REACT
        reaction_trigger = trigger_number
        reaction_end_time = time.time() + 7.0

        if (random_flag): app.execute_random_scenario()
        else: app.start_animation(scenario_filename)
        # Объединяем все звуки в один (если их несколько)
        if sounds_list:
            combined_sound = sounds_list[0]
            for s in sounds_list[1:]:
                combined_sound += s
            play_sound_with_mouth_movement(combined_sound, app)
        
        send_trigger_to_arduino(reaction_trigger)
        
        current_state = STATE_IDLE
        app.show_static_face()
        return True
    return False

# ---------------------- Функции состояний ----------------------
def colorGena(frame):
    settingsColor = [166, 66, 162, 180, 126, 255]
    sounds = []
    check_color(frame, settingsColor, 200000, 1,
                       REACT_IMAGE_GENA, sounds, "gena.txt") #анимации все

def colorOrange(frame):
    settingsColor = [0, 89, 90, 33, 252, 255]
    sounds = [sound_orange]
    return check_color(frame, settingsColor, 200000, 3,
                       REACT_IMAGE_ORANGE, sounds, "orange.txt")#апельсин

def colorBlue(frame):
    settingsColor = [104, 137, 47, 130, 239, 163]
    sounds = [sound_greet1, sound_greet2]
    return check_color(frame, settingsColor, 200000, 2,
                       None, sounds, "rat.txt")#приветствие

def colorGreen(frame):
    settingsColor = [67, 100, 81, 91, 248, 255]
    sounds = [sound_prak1, sound_prak2]
    detected = check_color(frame, settingsColor, 200000, 4,
                           REACT_IMAGE_GREEN, sounds, "green.txt", True)#практическая значимость
    return detected


# ---------------------- Основной код ----------------------
serial_port = serial.Serial('/dev/ttyACM0', 9600, timeout=0.1)
serial_port.flush()
root = tk.Tk()
app = GlintDraw(root, DEFAULT_IMAGE)
app.show_static_face()
root.update()
camera = cv2.VideoCapture(0)
while True:
    _, frame = camera.read()
    if cv2.waitKey(1) & 0xFF == ord('q'):
        app.quit_app()
        break
    if current_state == STATE_IDLE:
        colorGena(frame)
        colorOrange(frame)
        colorBlue(frame)
        colorGreen(frame)
    root.update()
    time.sleep(0.01)
camera.release()
root.destroy()
