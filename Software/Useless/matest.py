from tkinter import *
def mz_warning():
    ttkk = Tk()
    ttkk.geometry("700x200")
    BT = Label(ttkk, text="有害垃圾桶已满载", font=("Arial Bold", 50))
    BT.place(x=80, y=40)
    ttkk.update()
    time.sleep(5)
    ttkk.destroy()
    
mz_warning()