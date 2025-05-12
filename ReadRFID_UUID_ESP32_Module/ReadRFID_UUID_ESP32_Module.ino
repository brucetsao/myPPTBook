#include "Commlib.h"
#include "RFIDLib.h"

void setup() {
  Serial.begin(9600);     // 啟動序列埠，設置傳輸速率為 9600 bps
  while (!Serial);        // 如果序列埠尚未開啟（如 ATMEGA32U4 架構），則持續等待
  initRFID() ;//初始化RFID感測器函式
}

void loop() 
{
  long tmp = 0;  // 暫存用的變數，用來保存轉換後的 UID 數值
  String uidStr ;// 暫存用的變數，用來保存轉換後的 UID 數值
  if (checkNewCard())//偵測新卡片讀入
  {
      return ;
  }
   if (checkReadRFIDSuccess())  //嘗試讀取卡片序列號（UID）是否成功
  {
      return ;
  }

  // 使用自訂函式 UUIDString()，輸出RFID Tag 常見的10位數字以內的卡值
  tmp = readRFIDUUID() ; //建立讀取RFID UUID卡號函式
  
  uidStr=readRFIDUUIDString() ;//建立讀取RFID UUID卡號函式(Byte內容字串回傳)

  // 輸出 UID 字串（十六進位格式）
  Serial.println(uidStr);

  // 輸出 UID 整數（十進位格式）
  Serial.println(tmp);


  delay(300); // 延遲 300 毫秒，避免過於頻繁的讀取
}
