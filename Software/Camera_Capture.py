# coding=utf-8
import cv2
import os
import time 

cap = cv2.VideoCapture(0)
print('开始采集：')
i=0
while True:
    ret, frame = cap.read()
    # 裁剪坐标为[y0:y1, x0:x1]
    frame = frame[15:455, 60:585]
    k=cv2.waitKey(1)
    cv2.imshow("capture", frame)
    if k == 27:
        break
    if k==ord('s'):
        cv2.imwrite("img.jpg", frame)
        a = str(i + 55)
        os.rename("img.jpg", 'juzi'+ a + ".jpg")
        time.sleep(1)
        i += 1
cap.release()
cv2.destroyAllWindows()

