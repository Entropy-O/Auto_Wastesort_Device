# coding=utf-8
import requests
import json
import cv2
import time
from tkinter import *
from Action import *
from sorting_GX import *
import threading

def SDK_Identify():
    cap = cv2.VideoCapture(0)
    cap.set(3, 640)  # 设置分辨率
    cap.set(4, 480)
    #print("Opened")
    ret, frame = cap.read()
    cv2.imwrite("Camera_shot.jpg", frame)
    # print(frame)
    cap.release()
    with open('51.jpg', 'rb') as f:
        img = f.read()
        #print(img)

    # params 为GET参数 data 为POST Body
    content = requests.post('http://0.0.0.0:24401/', params={'threshold': 0.1},
                            data=img).json()
    # str1 = str(content, 'utf-8')
    # json = json.loads(content)
    json = content
    #print(content)
    score = json["results"][0]["confidence"]
    print(score)
    if score >= 0.9:
        if json["results"][0]['label'] == '[default]':
            print("识别错误")
            res = ''
        else:
            #print(json["results"][0]['label'])
            res = json["results"][0]['label']
            
    else:
        print("置信度过低")
        res = ''
    time_used = json["cost_ms"]/1000
    return res,time_used

def SHIPIN(z):
    path='/home/pi/Downloads/1.mp4'
    subprocess.call(["xdg-open",path])


def P1chufa(ser,fl,lj,time_used):
    if fl == 0:
        fl = '可回收垃圾'
    elif fl ==1:
        fl ='有害垃圾'
    elif fl == 2:
        fl = '厨余垃圾'
    elif fl == 3:
        fl = '其他垃圾'
    global window
    P1= Label(window, text="    %d                   %s           %s                          %.3f                             是        " % (ser,fl,lj,time_used), font=("Arial Bold", 20))
    P1.place(x=3, y=180+40*(ser-1))

def tk_display():
    global window
    window = Tk()

    window.title("垃圾分类信息显示")
    window.geometry("1280x720")
    BT = Label(window, text="垃圾分类信息显示", font=("Arial Bold", 50))
    BT.place(x=0, y=00)

    HX = Label(window, text="________________________________________________________________________________________________________", font=("Arial Bold", 20))
    HX.place(x=0, y=90)

    CAIDAN = Label(window, text="   序号              垃圾种类           详细信息               用时                 是否成功        ", font=("Arial Bold", 20))
    CAIDAN.place(x=0, y=140)

def thread_play():
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
def thread_dectect():
    last_res = ''
    num=0
    tk_display()
    while True:
        global window
        #t.stop(3)
        #t.pause()
        time.sleep(3)
        res,time_used=SDK_Identify()
        
    #     if last_rec == res:
    #         i+=1
    #     else:
    #         i=0
    #     if i==2:
    #         print(res)
    #         i=0
    #     last_rec = res
        if res!='' and last_res!=res:
            num+=1
            window.state('normal')
            P1chufa(num,Evaluator(res),ChineseConvert(res),time_used)
            window.update()
            time.sleep(6)
            window.state('icon')
            #Pour_to(Evaluator(res))
            last_res=res
def run():
    t1 = threading.Thread(target = thread_play)
    t2 = threading.Thread(target = thread_dectect)
    t1.start()
    t2.start()
    
if __name__ == '__main__':
    run()
    