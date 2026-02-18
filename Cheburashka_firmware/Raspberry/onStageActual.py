import cv2
import time
import os
import numpy as np
from animations import main

window_name = "Fullscreen Image"

def open_fullscreen_image(image_path):
    normalized_path = os.path.normpath(image_path)
    
    if not os.path.exists(normalized_path):
        print(f"Ошибка: файл не найден: {normalized_path}")
        return
    image = cv2.imdecode(np.fromfile(normalized_path, dtype=np.uint8), cv2.IMREAD_COLOR)
    
    if image is None:
        print("Ошибка загрузки изображения")
        return
        
    cv2.namedWindow(window_name, cv2.WND_PROP_FULLSCREEN)
    cv2.setWindowProperty(window_name, cv2.WND_PROP_FULLSCREEN, cv2.WINDOW_FULLSCREEN)
    cv2.imshow(window_name, image)


conditionFlag = [True, True, True, True]
condGreen = [False]
def sendToArduino(n):
    #if (n == 1): nOld+=1
    if n == 1 and conditionFlag[0]:        
        print('gena')
    elif n == 2 and conditionFlag[1]:
        print('rat')
    elif n == 3 and conditionFlag[2]:
        print('orange')
    elif n == 4 and conditionFlag[3]:
        print('aruco1')
        condGreen[0] = True
    else:
        print('nothing')
    if (n != 4):
        conditionFlag[n-1] = False

#camera objects
camera = cv2.VideoCapture(0)
open_fullscreen_image("C:/Users/DELL/Desktop/мордочки/face_empty.png")

flagSur = True
flagEnd = True

while True:          
    _, img = camera.read()
    
    #colorGena
    hsv_img = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
    maskGena = cv2.inRange(hsv_img,(0, 149, 180), (12, 242, 255))
    mom = cv2.moments(maskGena, 255)
    try:
        area = mom['m00']
        sum_y_cord = mom['m01']
        sum_x_cord = mom['m10']
        x = int(sum_x_cord/area)
        y = int(sum_y_cord/area)
        cv2.circle(img, (x,y), 20, (50,200,78),-1)
        print(x)
    except: pass    
    if mom['m00'] > 20000:
        sendToArduino(1)
    
    #colorOrange
    hsv_img = cv2.cvtColor(img,cv2.COLOR_BGR2HSV)
    maskOrange = cv2.inRange(hsv_img, (10, 108, 170), (30, 255, 255))
    momOrange= cv2.moments(maskOrange, 255)
    try:
        areaOrange = momOrange['m00']
        sum_x_cordOrange = momOrange['m01']
        sum_y_cordOrange = momOrange['m10']
        xOrange = int(sum_x_cordOrange/areaOrange)
        yOrange = int(sum_y_cordOrange/areaOrange)
        cv2.circle(img, (yOrange,xOrange), 10, (50,200,78),-1)
    except: pass    
    if momOrange['m00'] > 70000:
        main()
        sendToArduino(3)
    
    #125
    maskRat= cv2.inRange(hsv_img,(88, 176, 79), (113, 239, 255))#(101, 123, 37), (124, 211, 255))
    momRat = cv2.moments(maskRat, 255)
    try:
        areaRat = momRat['m00']
        sum_y_cordRat = momRat['m01']
        sum_x_cordRat = momRat['m10']
        xRat = int(sum_x_cordRat/areaRat)
        yRat = int(sum_y_cordRat/areaRat)
        cv2.circle(img, (xRat,yRat), 20, (250,20,78),-1)
        print(xRat)
    except: pass    
    if momRat['m00'] > 12000:
        main()
        sendToArduino(2)
    
    #colorGreen
    maskG = cv2.inRange(hsv_img,(40, 32, 149), (54, 164, 255))
    momG = cv2.moments(maskG, 255)
    try:
        areaG = momG['m00']
        sum_y_cordG = momG['m01']
        sum_x_cordG = momG['m10']
        xG = int(sum_x_cordG/areaG)
        yG = int(sum_y_cordG/areaG)
        cv2.circle(img, (xG,yG), 20, (250,20,78),-1)
        print("green", momG['m00'])
    except: pass    
    if momG['m00'] > 12000:
        main()
        sendToArduino(4)        

    if cv2.waitKey(1) == ord('q'):
        break
        
exit()