#include "initPins.h"
#include "OledLib.h"
#include "RFIDLib.h"
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
}

void loop() 
{
  String tmp = "";  // 暫存用的變數，用來保存轉換後的 UID 數值
 
  if (checkNewCard())//偵測新卡片讀入
  {
      return ;
  }
   if (checkReadRFIDSuccess())  //嘗試讀取卡片序列號（UID）是否成功
  {
      return ;
  }

  // 使用自訂函式 UUIDString()，輸出RFID Tag 常見的10位數字以內的卡值
  uidStr = readRFIDUUID() ; //建立讀取RFID UUID卡號函式
  
  tmp=readRFIDUUIDString() ;//建立讀取RFID UUID卡號函式(Byte內容字串回傳)
 

  // 輸出 UID 字串（十六進位格式）
  Serial.println(tmp);

  // 輸出 十個數字長度的UID 整數（十進位格式）
  Serial.println(uidStr);
  
  PrintCardonOLED(uidStr) ;  //顯示卡號在Oled上
  _updateBuffer();    //更新oled畫面
  SendtoClouding() ;    //傳送感測資料到雲端
  delay(300); // 延遲 300 毫秒，避免過於頻繁的讀取
}

void initDevice() //初始化所有感測器
{
  initOLED() ;  //啟動OLED顯示器，並檢查是否成功啟動。
   _setFont(u8g2_font_logisoso16_tf);  //設定字形 
   _clearBuffer();   //清除之前畫圖等記憶體
   _updateBuffer();    //更新oled畫面

  initRFID() ;//初始化RFID感測器函式

  
}
