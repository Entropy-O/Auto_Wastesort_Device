from time import sleep
import numpy as np
import RPi.GPIO as GPIO
#GPIO.cleanup()
horizontal_PIN = 12  # 水平舵机端口号
vertical_PIN = 18  # 垂直舵机端口号
GPIO.setmode(GPIO.BCM)
GPIO.setup(horizontal_PIN, GPIO.OUT)
GPIO.setup(vertical_PIN, GPIO.OUT)
horizontal = GPIO.PWM(horizontal_PIN, 50)  # 水平,垂直舵机PWM初始化   频率50Hz
vertical = GPIO.PWM(vertical_PIN, 50)
horizontal.start(2)  # 水平舵机占空比为0
horizontal.ChangeDutyCycle(0)
vertical.start(10.3)  # 垂直舵机占空比为0
vertical.ChangeDutyCycle(0)
sleep(2)


# GPIO.output(12, 0)
# GPIO.output(18, 0)


def Action_Pour_With_Shake():
    sleep(0.5)
    vertical.ChangeDutyCycle(10.3)
    sleep(0.5)
    vertical.ChangeDutyCycle(6.8)
    sleep(1.5)
    vertical.ChangeDutyCycle(10.3)
    sleep(0.5)
    horizontal.ChangeDutyCycle(2)
    sleep(1)
    horizontal.ChangeDutyCycle(0)
    vertical.ChangeDutyCycle(0)



def Pour_0():
    horizontal.ChangeDutyCycle(2)
    sleep(1)
    horizontal.ChangeDutyCycle(0)
    Action_Pour_With_Shake()
    horizontal.ChangeDutyCycle(2)
    sleep(1)
    horizontal.ChangeDutyCycle(0)
    vertical.ChangeDutyCycle(10.3)
    sleep(1)
    vertical.ChangeDutyCycle(0)
    

def Pour_90():
    horizontal.ChangeDutyCycle(6.3)
    sleep(1.5)
    horizontal.ChangeDutyCycle(0)
    Action_Pour_With_Shake()
    horizontal.ChangeDutyCycle(2)
    sleep(1)
    horizontal.ChangeDutyCycle(0)
    vertical.ChangeDutyCycle(10.3)
    sleep(1)
    vertical.ChangeDutyCycle(0)


def Pour_180():
    horizontal.ChangeDutyCycle(8.6)
    sleep(2)
    horizontal.ChangeDutyCycle(0)
    Action_Pour_With_Shake()
    horizontal.ChangeDutyCycle(2)
    sleep(1)
    horizontal.ChangeDutyCycle(0)
    vertical.ChangeDutyCycle(10.3)
    sleep(1)
    vertical.ChangeDutyCycle(0)


def Pour_270():
    horizontal.ChangeDutyCycle(12.2)
    sleep(2.3)
    horizontal.ChangeDutyCycle(0)
    Action_Pour_With_Shake()
    horizontal.ChangeDutyCycle(2)
    sleep(1)
    horizontal.ChangeDutyCycle(0)
    vertical.ChangeDutyCycle(10.3)
    sleep(1)
    vertical.ChangeDutyCycle(0)


def test():
    try:
        Pour_0()
        Pour_90()
        Pour_180()
        Pour_270()
    except KeyboardInterrupt:
        GPIO.cleanup()
def test_Vertical():
    i=0
    for i in np.arange(0,13,0.1):
        vertical.ChangeDutyCycle(i)
        print(i)
        sleep(0.3)
def Pour_To(pos):
    if pos == 0:
        Pour_0()
    elif pos == 1:
        Pour_90()
    elif pos == 2:
        Pour_180()
    elif pos == 3:
        Pour_270()
#test()
