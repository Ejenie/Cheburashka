import cv2

camera = cv2.VideoCapture(1)

while True:
    _, img = camera.read()
    hsv_img = cv2.cvtColor(img,cv2.COLOR_BGR2HSV)
    mask2 = cv2.inRange(hsv_img, (0, 146, 108), (50, 255, 255))
    mom = cv2.moments(mask2, 255)
    if mom['m00'] > 42000:
        print('gena')
    else:
        print('none')

    cv2.imshow("Mask2", mask2)
    cv2.imshow("IMG",img)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break
cv2.destroyAllWindows()
exit()