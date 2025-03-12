import sensor
import time
import pyb

#инициализация камеры
sensor.reset()
sensor.set_pixformat(sensor.GRAYSCALE)
sensor.set_framesize(sensor.HQVGA)
sensor.set_framerate(15)
sensor.set_windowing(500, 130)
sensor.skip_frames(time=2000)
sensor.set_auto_gain(False)  # must turn this off to prevent image washout...
sensor.set_auto_whitebal(False)  # must turn this off to prevent image washout...

#создание переменных
clock = time.clock()
time_in_cam = time.time()
tags_id = []    #список тегов
main_tag = -1   #номер установочного тега
main_tag_rect = []
last_tag_rect = [0, 0, 0, 0]
frames_skip = 0
tag_detected = False
x_out = 0
tag_out_cam = False

uart = pyb.UART(1, 115200, timeout_char = 1000)

#основной алгоритм
while True:
    clock.tick()
    img = sensor.snapshot()
    #прорисовка тегов
    for tag in img.find_apriltags():
        img.draw_rectangle(tag.rect, color=0)
        img.draw_cross(tag.cx, tag.cy, color=(255, 255, 255))
        tag_detected = True
        #проверка на соотвествие с установочным тегом
        if tag.id == main_tag:
            frames_skip += 1
            if tag_out_cam:
                #фильтр значения х
                x_out = (x_out * 1.075 + (tag.rect[0])) // 2
            else:
                x_out = tag.rect[0]
            if frames_skip > 4:
                tag_out_cam = False

            #задается список со старыми значениями положения и размеров тега
            for index, parametr in enumerate(tag.rect):
                last_tag_rect[index] = parametr
        tags_id.append(tag.id)

    #создание установочного тега
    if len(tags_id) != 0 and main_tag == -1:
        main_tag = tags_id[0]
    #проверка на отсутствие тега больше 4 сек
    if (not main_tag in tags_id) and (time.time() - time_in_cam > 4):
        x_out = 0  #человек вне камеры
        main_tag = -1
        tag_out_cam = True
        frames_skip = 0
    #проверка на остутствие тега; время < 4 сек
    elif not main_tag in tags_id and tag_detected:
        x_out = 1
        tag_out_cam = True
        frames_skip = 0
        if main_tag != -1:
            img.draw_rectangle(last_tag_rect, color=0)
    #обновление таймера при нахождении тега в камере
    else:
        time_in_cam = time.time()

    tags_id = []

    #отправка позиции тега по х
    x_out = int(x_out)
    print(x_out)
    uart.write(chr(x_out))
#конец
