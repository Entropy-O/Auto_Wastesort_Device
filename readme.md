YuTian's Project in TianGong University.

Some hardware and software design of Competition in National Undergraduate IoT design and engineering training competition.

<u>The Hardware and PCB Layout descriptions are as follows:</u>

------

![image](https://github.com/Entropy-O/picgo/blob/master/img/2.jpg)

![image](https://github.com/Entropy-O/picgo/blob/master/img/PCB%20Layout.jpg)

![](https://github.com/Entropy-O/picgo/img/Screenshot_2021-04-27-22-07-57-38.jpg)



Features：

1. Using XT60 connector, which can be easily connected to the current Li-Po 3S Battery.
2. Double IP6518C Power supply Soc, 5v to 20+V output. This part used for power supply of Raspberry Pi and HDMI screen via USB Type A.
3. LM2596S 5V and LM2596 ADJ to generate 5V power supply and servo voltage supply.
4. HX8358 Audio Power Amplifier.
5. STM32F103C8T6 micro controller. using USART to connect to the Raspberry Pi, and upload sensor signal and generate PWM to control servo.

------

Software Descriptions:

(Not Updated yet)