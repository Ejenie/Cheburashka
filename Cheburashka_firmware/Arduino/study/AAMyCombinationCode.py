import cv2
import time
import serial
from pydub import AudioSegment
from pydub.playback import play

prak1 = AudioSegment.from_file('prak3.1.wav', format = 'wav')
prak1 = AudioSegment.from_wav('prak3.1.wav')


prak2 = AudioSegment.from_file('prak3.2.wav', format = 'wav')
prak2 = AudioSegment.from_wav('prak3.2.wav')

orange = AudioSegment.from_file('orange.wav', format = 'wav')
orange = AudioSegment.from_wav('orange.wav')

one = AudioSegment.from_file('1.wav', format = 'wav')
one = AudioSegment.from_wav('1.wav')

two = AudioSegment.from_file('2.wav', format = 'wav')
two = AudioSegment.from_wav('2.wav')

window_name = "Fullscreen Image"
def open_fullscreen_image(image_path):
    image = cv2.imread(image_path)
    if image is None:
        print("Ошибка: изображение не найдено или не может быть загружено.")
        return
    cv2.namedWindow(window_name, cv2.WND_PROP_FULLSCREEN)
    cv2.setWindowProperty(window_name, cv2.WND_PROP_FULLSCREEN, cv2.WINDOW_FULLSCREEN)
    cv2.imshow(window_name, image)

def sendToArduino(n):
    #if (n == 1): nOld+=1
    if n == 1:        
        print('orange')
        s.write(b'orange')
        #if (nOld >= 1):
        play(orange)
    elif n == 2:
        print('face')
        s.write(b'face')
    elif n == 3:
        print('aruco')
        s.write(b'aruco')
        open_fullscreen_image('/home/ejenie/opencv/sources/samples/python/мордочкиБольшие/страх_вбое.png')
        #print("one")

    else:
        print('nothing')
        s.write(b'nothing')
    line = s.readline().decode('utf-8').rstrip()
    if line == "begin programm":
        play(one)
        play(two)
        play(prak1)
        play(prak2)
    print('arduino sent ', line)

#serials object
s = serial.Serial('/dev/ttyACM0', 9600, timeout = 0.1)
s.flush() 

#aroco objects
arucoParam = cv2.aruco.DetectorParameters()
arucoDetect = cv2.aruco.getPredefinedDictionary(cv2.aruco.DICT_4X4_50)

#camera objects
camera = cv2.VideoCapture(0)

nOld = 0
flagBok = True
flagOtherBok = True
xold = 0

while True:  
    _, img = camera.read()
    
    #color
    hsv_img = cv2.cvtColor(img,cv2.COLOR_BGR2HSV)
    mask2 = cv2.inRange(hsv_img, (6, 209, 0), (35, 255, 255))
    #cv2.imshow("Mask2", mask2)
    mom = cv2.moments(mask2, 255)
    try:
        area = mom['m00']
        sum_x_cord = mom['m01']
        sum_y_cord = mom['m10']
        x = int(sum_x_cord/area)
        y = int(sum_y_cord/area)
        cv2.circle(img, (y,x), 10, (50,200,78),-1)
    except: pass
    
    if mom['m00'] > 35000:
        
        sendToArduino(1)
    
    
    #aruco
    cor, idMark, points = cv2.aruco.detectMarkers(cv2.cvtColor(img,cv2.COLOR_BGR2GRAY), arucoDetect)
    if idMark is not None:
        sendToArduino(3)
    print(idMark)
        
    #face
    face_cascade = cv2.CascadeClassifier(cv2.samples.findFile("haarcascades/haarcascade_frontalface_alt.xml"))
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    faces = face_cascade.detectMultiScale(gray, 1.1, 4)
    
    for (x, y, w, h) in faces:
        cv2.rectangle(img, (x, y), (x+w, y+h), (255, 0, 0), 2)
        if x > img.shape[0] / 2: #and (xold < img.shape[0] / 2):
            open_fullscreen_image('/home/ejenie/opencv/sources/samples/python/мордочкиБольшие/улыбка_другой_бок.png')
            xold = x
        elif x < img.shape[0] / 2: #and (xold > img.shape[0] / 2):
            open_fullscreen_image('/home/ejenie/opencv/sources/samples/python/мордочкиБольшие/улыбка_вбок.png')
            xold = x
        sendToArduino(2)
    if faces == (): open_fullscreen_image('/home/ejenie/opencv/sources/samples/python/мордочкиБольшие/улыбка_прямо.png=')
    #print(faces)
                                                                                                            
    #cv2.imshow("IMG", img)
    if cv2.waitKey(1) == ord('q'):
        break
        
exit()
