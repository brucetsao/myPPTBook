#include "initPins.h"
#include "OledLib.h"
#include "clouding.h"


void setup() 
{
  Serial.begin(9600);     // 啟動序列埠，設置傳輸速率為 9600 bps
  while (!Serial);        // 如果序列埠尚未開啟（如 ATMEGA32U4 架構），則持續等待
  initAll() ;   //初始化系統
  initWiFi();   //網路連線，連上熱點
  delay(2000) ;   //延遲2秒鐘
  MacData = GetMacAddress();  // 取得網路卡MAC地址
  ShowInternet() ;  //秀出網路連線資訊
  initDevice(); //初始化所有感測器
  PrintMAConOLED(MacData); //印出網路卡MAC地址在OLED上
  _updateBuffer();    //更新oled畫面
  SendtoClouding() ;    //傳送感測資料到雲端
  
}

void loop() 
{
//  PrintCardonOLED(uidStr) ;  //顯示卡號在Oled上
//
//  delay(300); // 延遲 300 毫秒，避免過於頻繁的讀取
}

void initDevice() //初始化所有感測器
{
  initOLED() ;  //啟動OLED顯示器，並檢查是否成功啟動。
   _setFont(u8g2_font_logisoso16_tf);  //設定字形 
   _clearBuffer();   //清除之前畫圖等記憶體
   _updateBuffer();    //更新oled畫面

}
