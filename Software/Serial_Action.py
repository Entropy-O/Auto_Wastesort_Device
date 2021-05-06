import serial
import time

class Servo:
     def __init__(self):
         self.SerPort = serial.Serial('/dev/ttyAMA0', 115200)
         self.SerPort.close()
         
     def ServoTest(self):
        if self.SerPort.isOpen != True:
            self.SerPort.open()
        for i in range (1,5):
            print("Turning to Position  "+ str(i))                # 打开串口
            ch = chr(i)
            chb = bytes(ch, encoding = "utf8")
            self.SerPort.write(chb)
            if i < 3:
                time.sleep(3.5)
            else:
                time.sleep(5)
        self.SerPort.close()
        print("Test End, Serial Port Closed!")
        
     def Pour_0(self):
        if self.SerPort.isOpen != True:
            self.SerPort.open()
        ch = chr(1)
        chb = bytes(ch, encoding = "utf8")
        self.SerPort.write(chb)
        print("Turning to Position  1")
        self.SerPort.close()
        time.sleep(1.5)
        
     def Pour_90(self):
        if self.SerPort.isOpen != True:
            self.SerPort.open()
        ch = chr(2)
        chb = bytes(ch, encoding = "utf8")
        self.SerPort.write(chb)
        print("Turning to Position  2")
        self.SerPort.close()
        time.sleep(3)
        
     def Pour_180(self):
        if self.SerPort.isOpen != True:
           self.SerPort.open()
        ch = chr(3)
        chb = bytes(ch, encoding = "utf8")
        self.SerPort.write(chb)
        print("Turning to Position  3")
        self.SerPort.close()
        time.sleep(4.0)
        
     def Pour_270(self):
        if self.SerPort.isOpen != True:
            self.SerPort.open()
        ch = chr(4)
        chb = bytes(ch, encoding = "utf8")
        self.SerPort.write(chb)
        print("Turning to Position  4")
        self.SerPort.close()
        time.sleep(4.9)
     def Pour_To(self,pos):
        if pos == 0:
            Servo.Pour_0(self)
        elif pos == 1:
            Servo.Pour_90(self)
        elif pos == 2:
            Servo.Pour_180(self)
        elif pos == 3:
            Servo.Pour_270(self)        
        
'''if "__main__" == __name__:
    Servo = Servo()
    #Servo.ServoTest()
    while True:
        Servo.Pour_0()
        Servo.Pour_90()
        Servo.Pour_180()
        Servo.Pour_270()'''


