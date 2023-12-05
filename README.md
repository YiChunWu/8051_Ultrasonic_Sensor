# 8051_Ultrasonic_Sensor
在大學時課程中有使用超音波測距模組在氣動車上，因為測距模式有兩種，想要看能不能同時讓兩種模式並存。這邊使用8051系列的MCU，來完成UART模式以及脈衝模式的命令發送與感測器回傳值接收。由於此晶片在9600鮑率無法計算精確，這邊會使用自訂Timer計時中斷來做代替。
## Project Description
下方為此專案的硬體配置，使用8051實現超音波測距，透過傳送指令給超音波模組，接收回傳值換算模組與障礙物間的距離，並透過LCD面板顯示距離及當前模式。

![image](https://github.com/YiChunWu/8051_Ultrasonic_Sensor/blob/main/setup.png)

## Files
* ultrasonic.c
  * 中斷、計時器、初始化設定。
  * UART發送接收、距離計算。
* LCD.h
  * LCD顯示器函式庫。
* REGX52.h
  * 8051標準函式庫。

## Note

[Notion](https://dirt-bramble-43c.notion.site/Ultrasonic-Sensor-on-8051-621c02e1827445cea55117b975528a6c?pvs=4)

[Demo影片](https://youtu.be/NNE64kVdexI)
