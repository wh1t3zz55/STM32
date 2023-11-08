# STM32-Desk-Clock
基于 STM32 的桌面电磁力时钟摆设计
2023@Wh1t3zZ
硬件：
IIC4针OLED DHT11 USBTTL串口
接线方法：
OLED:   VCC->3.3V    GND->GND    SCL->PB6    SDA->PB7
串口：  VCC->5V      GND->GND    TX->PA10    RX->PA9
DHT11:  VCC->3.3V    GND->GND    DATA->PB15
DAC:    CLK->PB11    DIN->PB10   CS->PB1
