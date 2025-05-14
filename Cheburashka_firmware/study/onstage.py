import cv2
import time
import serial
from pydub import AudioSegment
from pydub.playback import play

sur = AudioSegment.from_file('сюрприз.wav', format = 'wav')
sur = AudioSegment.from_wav('сюрприз.wav')

oy = AudioSegment.from_file('ойойой.wav', format = 'wav')
oy = AudioSegment.from_wav('ойойой.wav')

gena = AudioSegment.from_file('упал.wav', format = 'wav')
gena = AudioSegment.from_wav('упал.wav')

rat = AudioSegment.from_file('лариска.wav', format = 'wav')
rat = AudioSegment.from_wav('лариска.wav')

tish = AudioSegment.from_file('tish.wav', format = 'wav')
tish = AudioSegment.from_wav('tish.wav')

end = AudioSegment.from_file('end.wav', format = 'wav')
end = AudioSegment.from_wav('end.wav')

window_name = "Fullscreen Image"
def open_fullscreen_image(image_path):
    image = cv2.imread(image_path)
    if image is None:
        print("Ошибка: изображение не найдено или не может быть загружено.")
        return
    cv2.namedWindow(window_name, cv2.WND_PROP_FULLSCREEN)
    cv2.setWindowProperty(window_name, cv2.WND_PROP_FULLSCREEN, cv2.WINDOW_FULLSCREEN)
    cv2.imshow(window_name, image)


conditionFlag = [True, True, True, True]
def sendToArduino(n):
    #if (n == 1): nOld+=1
    if n == 1 and conditionFlag[0]:        
        print('gena')
        for i in range(5):
            s.write(b'gena\n')
        play(oy)
        play(gena)
    elif n == 2 and conditionFlag[1]:
        print('rat')
        for i in range(5):
            s.write(b'rat\n')
        play(rat)
    elif n == 3 and conditionFlag[2]:
        print('orange')
        for i in range(5):
            s.write(b'orange\n')
    elif n == 4 and conditionFlag[3]:
        print('aruco1')
        for i in range(5):
            s.write(b'aruco1\n')
        play(tish)
        play(end)
    else:
        s.write(b'nothing')
    conditionFlag[n-1] = False

#serials object
s = serial.Serial('/dev/ttyACM0', 9600, timeout = 0.1)
s.flush() 

#aruco objects
arucoParam = cv2.aruco.DetectorParameters()
arucoDetect = cv2.aruco.getPredefinedDictionary(cv2.aruco.DICT_4X4_50)

#camera objects
camera = cv2.VideoCapture(0)
open_fullscreen_image('/home/ejenie/opencv/sources/samples/python/мордочкиБольшие/улыбка_прямо.png')

flagSur = True
flagEnd = True
while True:      
    line = s.readline().decode('utf-8').rstrip()
    if line == "begin programm":
        play(tish)
        play(sur) 
        #pass      
    print('arduino sent ', line)
    _, img = camera.read()
    
    #colorGena
    hsv_img = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
    maskGena = cv2.inRange(hsv_img,(73, 136, 86), (180, 255, 201))
    #cv2.imshow("maskRat", maskRat)
    mom = cv2.moments(maskGena, 255)
    try:
        area = mom[' m00']
        sum_y_cord = mom['m01']
        sum_x_cord = mom['m10']
        x = int(sum_x_cord/area)
        y = int(sum_y_cord/area)
        cv2.circle(img, (x,y), 20, (50,200,78),-1)
        print(x)
    except: pass    
    if mom['m00'] > 100000:        
        open_fullscreen_image('/home/ejenie/opencv/sources/samples/python/мордочкиБольшие/страх_вбое.png')     
        sendToArduino(1)
    
    #colorOrange
    hsv_img = cv2.cvtColor(img,cv2.COLOR_BGR2HSV)
    maskOrange = cv2.inRange(hsv_img, (13, 122, 101), (20, 255, 255))
    #cv2.imshow("maskOrange", maskOrange)
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
        open_fullscreen_image('/home/ejenie/opencv/sources/samples/python/мордочкиБольшие/улыбка_прямо.png')          
        sendToArduino(3)
    
    #colorRat
    maskRat= cv2.inRange(hsv_img,(101, 151, 81), (123, 215, 255))#(101, 123, 37), (124, 211, 255))
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
    if momRat['m00'] > 20000:
        sendToArduino(2)
    
    #colorGreen1
    maskG= cv2.inRange(hsv_img,(59, 49, 57), (88, 95, 255))
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
    if momG['m00'] > 20000:
        sendToArduino(4)

    #aruco
    '''
    cor, idMark, points = cv2.aruco.detectMarkers(cv2.cvtColor(img,cv2.COLOR_BGR2GRAY), arucoDetect)
    if idMark is not None:
        if idMark== [[1]] and flagSur:
            sendToArduino(4)
            flagSur = False
            play(tish)                                                                  
            play(sur)
        elif idMark == [[3]] and flagEnd:
            flagEnd = False
            play(tish)
            play(end)
    print(idMark)
    '''
        
    #face
    '''
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
    if faces == (): open_fullscreen_image('/home/ejenie/opencv/sources/samples/python/мордочкиБольшие/улыбка_прямо.png')
    print(faces)
    '''
    
    #cv2.imshow("IMG", img)
    if cv2.waitKey(1) == ord('q'):
        break
        
exit()