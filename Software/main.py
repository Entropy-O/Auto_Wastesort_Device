# coding=utf-8
import RPi.GPIO as GPIO
import requests
import json
import cv2
import time
import vlc
from tkinter import *
from Serial_Action import *
from sorting_GX import *
import threading
from mttkinter import mtTkinter as tk
from vplayer import *
import _thread


def SDK_Identify():
    global score
    cap = cv2.VideoCapture(0)
    cap.set(3, 640)  # 设置分辨率
    cap.set(4, 480)
    # print("Opened")
    ret, frame = cap.read()
    frame = frame[15:455, 55:570]
    cv2.imwrite("Camera_shot.jpg", frame)
    # print(frame)
    cap.release()
    # dst = img[50:430, 50:590]   # 裁剪坐标为[y0:y1, x0:x1]
    # cv2.imshow("capture", frame)

    cv2.waitKey(8)  # 程序暂停
    with open('Camera_shot.jpg', 'rb') as f:
        img = f.read()
        # print(img)

    # params 为GET参数 data 为POST Body
    content = requests.post('http://0.0.0.0:24401/', params={'threshold': 0.1},
                            data=img).json()
    # str1 = str(content, 'utf-8')
    # json = json.loads(content)
    json = content
    print(content)
    score = json["results"][0]["confidence"]
    print(score)
    if score >= 0.9:
        if json["results"][0]['label'] == '[default]':
            print("识别错误")
            res = ''
        else:
            # print(json["results"][0]['label'])
            res = json["results"][0]['label']

    else:
        print("置信度过低")
        res = ''
    time_used = json["cost_ms"] / 1000
    return res, time_used


def P1chufa(ser, fl, lj, time_used):
    global flag9
    if fl == 3:
        fl = '可回收垃圾'
    elif fl == 1:
        fl = '有害垃圾'
    elif fl == 0:
        fl = '厨余垃圾'
    elif fl == 2:
        fl = '其他垃圾'
    global window
    if ser%10==0:
        yd=10
    else:
        yd=ser%10
    if fl!='可回收垃圾':
        P1 = Label(window, text="\t%d\t%s\t\t%s\t\t%d\t是        " % (ser, fl, lj, time_used), font=("Arial Bold", 20))
        P1.place(x=3, y=180 + 40 * (yd - 1))
    else:
        P1 = Label(window, text="\t%d\t%s\t%s\t\t%d\t是        " % (ser, fl, lj, time_used), font=("Arial Bold", 20))
        P1.place(x=3, y=180 + 40 * (yd - 1))
    
def GPIO_init():
    #SIG_PB0 = 21
    SIG_PB3 = 20
    SIG_PB6 = 16
    SIG_PB7 = 12
    GPIO.setmode(GPIO.BCM)

    #GPIO.setup(SIG_PB0,GPIO.IN)
    GPIO.setup(SIG_PB3,GPIO.IN)
    GPIO.setup(SIG_PB6,GPIO.IN)
    GPIO.setup(SIG_PB7,GPIO.IN) 
    #GPIO.add_event_detect(SIG_PB0, GPIO.RISING, callback=PB0_Rec, bouncetime=10000)
    GPIO.add_event_detect(SIG_PB3, GPIO.RISING, callback=PB3_Rec, bouncetime=10000)
    GPIO.add_event_detect(SIG_PB6, GPIO.RISING, callback=PB6_Rec, bouncetime=10000)
    GPIO.add_event_detect(SIG_PB7, GPIO.RISING, callback=PB7_Rec, bouncetime=10000)
def PB0_Rec(self):      #youhailaji
    print("PB0!")
    global flag9
    flag9=1
def PB3_Rec(self):      #chuyu
    print("PB3!")
    global flag9
    flag9=2
def PB6_Rec(self):      #kehuishou
    global window
    print("PB6!")
    global flag9
    flag9=3
def PB7_Rec(self):      #qitalaji
    global window
    print("PB7!")
    global flag9
    flag9=4

def tk_display():
    global window
    window = Tk()

    window.title("垃圾分类信息显示")
    window.geometry("1280x720")
    BT = Label(window, text="垃圾分类信息显示", font=("Arial Bold", 50))
    BT.place(x=0, y=00)

    HX = Label(window,
               text="________________________________________________________________________________________________________",
               font=("Arial Bold", 20))
    HX.place(x=0, y=90)

    CAIDAN = Label(window, text="\t序号\t垃圾种类\t\t详细信息\t\t数量\t是否成功        ", font=("Arial Bold", 20))
    CAIDAN.place(x=0, y=140)


def my_call_back(event):
    print("call:", player.get_time())
    if player.get_time() > 300000:
        player.set_position(0.01)


def Video_Play():
    # player = Player()
    player.add_callback(vlc.EventType.MediaPlayerTimeChanged, my_call_back)
    # 播放本地mp3
    player.play("/home/pi/Downloads/1.mp4")
    while True:
        res, time_used = SDK_Identify()
        if res != 'kong'and score > 0.9 and res != '':
            player.stop()
            player.release()
            break
    #
    #    pass


if __name__ == '__main__':

    player = Player()
    Video_Play()
    ################################ Video Play Init  #########################
    Servo = Servo()
    #############Servo Init##################
    tk_display()
    # Servo.ServoTest()

    last_res = ''
    num = 0
    shulang=[0,0,0,0];
    GPIO_init()
    flag9=0
    while True:
        global window
        res, time_used = SDK_Identify()
        if flag9==1:
            P1 = Label(window, text="有害垃圾已满" , font=("Arial Bold", 30))
            P1.place(x=800, y=20)
            window.update()
        elif flag9==2:
            P1 = Label(window, text="厨余垃圾已满" , font=("Arial Bold", 30))
            P1.place(x=800, y=20)
            window.update()
        elif flag9==3:
            P1 = Label(window, text="可回收垃圾已满" , font=("Arial Bold", 30))
            P1.place(x=800, y=20)
            window.update()
        elif flag9==4:
            P1 = Label(window, text="其他垃圾已满" , font=("Arial Bold", 30))
            P1.place(x=800, y=20)
            window.update()
        print(flag9)
        if res != '' and res != last_res and res != 'kong':
            # if 1:
            num += 1
            window.state('normal')
            shulang[Evaluator(res)]=shulang[Evaluator(res)]+1
            P1chufa(num, Evaluator(res), ChineseConvert(res), shulang[Evaluator(res)])
            print(Evaluator(res))
            Servo.Pour_To(Evaluator(res))
            window.update()
            #time.sleep(6)
            if num == 10:
                pass
            #window.state('icon')
            #
            # window.destroy()
        last_res = res