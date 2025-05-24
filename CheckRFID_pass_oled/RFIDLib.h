/*
  Rui Santos & Sara Santos - Random Nerd Tutorials
  完整專案說明：https://RandomNerdTutorials.com/esp32-mfrc522-rfid-reader-arduino/
  此軟體可免費使用、修改與分發，須附上原作者聲明。
*/
#include <String.h>
#include <MFRC522v2.h>           // 引入 MFRC522 RFID 模組主程式庫
#include <MFRC522DriverSPI.h>    // 引入 SPI 通訊用的驅動程式庫
//#include <MFRC522DriverI2C.h>  // （註解掉）如果使用 I2C 通訊，可以引入這個驅動
#include <MFRC522DriverPinSimple.h> // 引入簡易腳位設定的驅動程式
#include <MFRC522Debug.h>        // 引入除錯用的輔助函式庫

char cardvalue[12] ;  //列印卡號轉換字串陣列
// 更多關於 SPI/I2C 接線資訊可參考：https://github.com/OSSLibraries/Arduino_MFRC522v2#pin-layout

MFRC522DriverPinSimple ss_pin(5); // 建立一個使用 GPIO5 (D5 腳位) 作為 SS（Slave Select）的簡易腳位物件

MFRC522DriverSPI driver{ss_pin};  // 建立一個 SPI 通訊驅動物件
//MFRC522DriverI2C driver{};      // （註解掉）建立一個 I2C 通訊驅動物件
MFRC522 mfrc522{driver};           // 建立一個 MFRC522 物件並指定通訊方式

// 預先宣告 UUIDString 函式：將 4 個 byte 資料組合成一個 long 型態的數值
long UUIDString(int d4,int d3, int d2, int d1);
 String uidStr = "" ;// RFID卡號的String變數
 
void initRFID() //初始化RFID感測器函式
{
  mfrc522.PCD_Init();     // 初始化 MFRC522 RFID 模組
  MFRC522Debug::PCD_DumpVersionToSerial(mfrc522, Serial); // 輸出讀卡機的硬體版本與相關資訊
  Serial.println(F("Scan PICC to see UID")); // 提示訊息：請刷卡
}

// 輸出RFID Tag 常見的10位數字以內的卡值(將 4 個 byte 數據轉換成一個 long 整數（32位元）)
long UUIDString(int d4,int d3, int d2, int d1) 
{
  //讀取 RFID 卡片的 UID，然後把 UID 傳入十六進位轉成之整數與十進位數值方式輸出
  long tmp = 0;
  tmp = d4 * (long)16777216 + d3 * (long)65536 + d2 * (long)256 + d1; 
  // 依據每個 byte 的位元組位置，加權組合成一個長整數
  return tmp;
}

boolean checkNewCard()//偵測新卡片讀入
{
 // Serial.println("checkNewCard()") ;
  // 若沒有偵測到新卡片，則直接跳出 loop（避免無謂的重複執行）
  if (!mfrc522.PICC_IsNewCardPresent()) //沒有偵測到新卡片
  {
   return true ;
  }
  else
  {
     return false ;
  }

 
}

boolean checkReadRFIDSuccess()  //嘗試讀取卡片序列號（UID）是否成功
{
 // Serial.println("checkReadRFIDSuccess()") ;
    // 嘗試讀取卡片序列號（UID），若失敗則跳出 loop
  if (!mfrc522.PICC_ReadCardSerial()) 
  {
    return true  ;
  }
  else
  {
     return false ;
  }

}


String readRFIDUUID() //建立讀取RFID UUID卡號函式
{
  String tmp = "" ;  //宣告回傳變數
  // 顯示卡片的 UID
  //Serial.print("Card UID: ");
  //MFRC522Debug::PrintUID(Serial, (mfrc522.uid));
  //Serial.println();

  // 使用自訂函式 UUIDString()，輸出RFID Tag 常見的10位數字以內的卡值
  tmp = UUIDString(
          (int)mfrc522.uid.uidByte[3],
          (int)mfrc522.uid.uidByte[2],
          (int)mfrc522.uid.uidByte[1],
          (int)mfrc522.uid.uidByte[0]
        );
      int len = tmp.length() ;
    if (len <10)
    {
        return genstr('0',10-len)+tmp ;
    }
    else
    {
       return tmp; //回傳卡號到readRFIDUUIDString()函式
    }
  return tmp ;//回傳卡號到readRFIDUUID()函式
}

String readRFIDUUIDString() //建立讀取RFID UUID卡號函式(Byte內容字串回傳)
{
    // 將 UID 轉換成字串格式儲存
  String uidString = "";  //宣告回傳變數
  for (byte i = 0; i < mfrc522.uid.size; i++)   //迴圈讀取RFID四個byte內容
  {
     uidString += print2HEX((int)mfrc522.uid.uidByte[i]);  // 若小於 16，前面補零，使每個 byte 保持兩位數
  }
    uidString.toUpperCase() ; //字串轉成大寫英文
    return uidString ;

}


void PrintCardonOLED(String ss) //顯示卡號在Oled上
{
    sprintf(cardvalue,"%s",ss)  ;  //轉換string字串到char array
      _TPrint(1,16,cardvalue);  //(50, 15) 顯示卡號。
}
