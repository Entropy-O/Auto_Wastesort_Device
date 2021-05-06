import cv2
cap1 = cv2.VideoCapture("/home/pi/Downloads/1.mp4")
while True:
    key = cv2.waitKey(25)
    if key == 182:
        cv2.destroyAllWindows()
        time.sleep(7)
    ret1, frame1 = cap1.read()
    cv2.namedWindow("capture", 0)  # 0可调大小，注意：窗口名必须imshow里面的一窗口名一直
    cv2.resizeWindow("capture", 1280, 720)
    cv2.imshow("capture", frame1)
