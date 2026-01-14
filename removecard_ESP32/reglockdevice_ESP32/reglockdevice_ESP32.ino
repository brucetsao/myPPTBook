#include "initPins.h"
#include "OledLib.h"
#include "clouding.h"

String readcmd ; //從Serial讀取文字字串

void setup() 
{
  Serial.begin(9600);     // 啟動序列埠，設置傳輸速率為 9600 bps
  while (!Serial);        // 如果序列埠尚未開啟（如 ATMEGA32U4 架構），則持續等待
  initAll() ;   //初始化系統
  initWiFi();   //網路連線，連上熱點取得網路卡MAC地址
  ShowInternet() ;  //秀出網路連線資訊
  initDevice(); //初始化所有感測器
  PrintMAConOLED(MacData); //印出網路卡MAC地址在OLED上
  _updateBuffer();    //更新oled畫面
}

void loop() 
{
 if (Serial.available()>0)
 {
      readcmd = Serial.readStringUntil('\n');  // 讀到換行符號為止
      readcmd.trim();  // 去除前後空白與換行符號
      readcmd.toUpperCase() ; //轉大寫
      if (readcmd == "REG")
        {
        SendtoClouding();   //透過http GET傳送資料到指定URL
          Serial.println(readcmd) ;
          _clearBuffer();   //清除之前畫圖等記憶體
          _updateBuffer();    //更新oled畫面
            _setFont(u8g2_font_logisoso16_tf);  //設定字形   
          PrintMAConOLED(MacData); //印出網路卡MAC地址在OLED上
          PrintMSGonOLED(jsonresult); // 顯示訊息在OLED上
          _updateBuffer();    //更新oled畫面

        }
        else
        {
          Serial.println("you input not reg") ;
          Serial.println(readcmd) ;
        }
        
 }
}

void initDevice() //初始化所有感測器
{
  initOLED() ;  //啟動OLED顯示器，並檢查是否成功啟動。
   _setFont(u8g2_font_logisoso16_tf);  //設定字形 
   _clearBuffer();   //清除之前畫圖等記憶體
   _updateBuffer();    //更新oled畫面
  initjson() ;  // 初始化 JSON 資料
}
