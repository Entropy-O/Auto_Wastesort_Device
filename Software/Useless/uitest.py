from tkinter import *
import os
import subprocess

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






