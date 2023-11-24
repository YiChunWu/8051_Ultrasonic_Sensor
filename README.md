# 8051_Ultrasonic_Sensor
在大學時課程中有使用超音波測距模組在氣動車上，因為測距模式有兩種，想要看能不能同時讓兩種模式並存。這邊使用8051系列的MCU，來完成UART模式以及脈衝模式的命令發送與感測器回傳值接收。由於此晶片在9600鮑率無法計算精確，這邊會使用自訂Timer計時中斷來做代替。
## Project Description
下方為此專案的硬體配置，使用8051對下方麵包版Buck converter電路進行控制，以及透過UART接收另外一台微處理器量測到的電壓資訊，並將電壓資訊、Duty cycle顯示在LCD顯示屏上。

![image](https://github.com/ZongWeiLin/8051_buck_converter/blob/main/Setup.png)

## Files
* ultrasonic.c
  * 中斷、計時器、UART初始化設定。
  * PWM操作、UART通訊。
* LCD.h
  * LCD顯示器函式庫。
* REGX52.h
  * 8051標準函式庫。

## Note

[Notion](https://complete-filament-cc6.notion.site/Buck-converter-based-on-8051-a02db5f8ce7c4fd9a6dae38f78a522a6)

[Demo影片](https://www.youtube.com/watch?v=v3Ll4IgLot8)
