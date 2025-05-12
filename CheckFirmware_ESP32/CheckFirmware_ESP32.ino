/*
 * --------------------------------------------------------------------------------------------------------------------
 * 測試韌體的範例程式
 * --------------------------------------------------------------------------------------------------------------------
 * 這是 MFRC522 函式庫的範例之一；更多詳細說明與其他範例請參見：
 * https://github.com/OSSLibraries/Arduino_MFRC522v2
 * 
 * 本範例用來測試 MFRC522 讀卡模組的韌體版本。注意，即使測試通過，也不代表模組完全正常！
 * 有些模組可能天線損壞，或是PICC（標籤或卡片）本身有問題。
 * 
 * 作者：Rotzbua
 * 授權：公有領域 (Public Domain)
 * 
 * 常見接線方式如下：
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
 * 訊號         腳位          腳位           腳位       腳位        腳位              腳位
 * -----------------------------------------------------------------------------------------
 * SPI SS      SDA(SS)      10            53        D10        10               10
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
 *
 * 更多資訊請參考：https://github.com/OSSLibraries/Arduino_MFRC522v2#pin-layout
 */

#include <MFRC522v2.h>            // 引入 MFRC522 主函式庫
#include <MFRC522DriverSPI.h>     // 引入 SPI 介面驅動
//#include <MFRC522DriverI2C.h>   // (註解掉) 若要使用 I2C 介面，可以引入此驅動
#include <MFRC522DriverPinSimple.h> // 引入簡單腳位設定的驅動
#include <MFRC522Debug.h>         // 引入除錯用函式庫

MFRC522DriverPinSimple ss_pin(5); // 宣告 SPI 的 SS（Slave Select）腳位，這裡用腳位 5，可以根據實際接線修改

MFRC522DriverSPI driver{ss_pin};  // 建立 SPI 通訊的驅動器
//MFRC522DriverI2C driver{};      // (註解掉) 若要改用 I2C 介面，可以使用這行
MFRC522 mfrc522{driver};          // 建立 MFRC522 實例

/**
 * 在開機時只檢查一次韌體
 */
void setup() {
  Serial.begin(9600);  // 初始化與PC的序列通訊，傳輸速率為9600
  while (!Serial);     // 如果尚未開啟序列埠（針對使用ATMEGA32U4的板子，如Leonardo），則等待
  mfrc522.PCD_Init();  // 初始化MFRC522模組
  
  // 在序列埠輸出提示訊息
  Serial.println(F("*****************************"));
  Serial.println(F("MFRC522 數位自我測試"));
  Serial.println(F("*****************************"));
  
  MFRC522Debug::PCD_DumpVersionToSerial(mfrc522, Serial);  
  // 輸出 MFRC522 模組的版本資訊（包含韌體版本號）

  Serial.println(F("-----------------------------"));
  Serial.println(F("僅支援已知版本"));
  Serial.println(F("-----------------------------"));
  Serial.println(F("執行測試..."));
  
  bool result = mfrc522.PCD_PerformSelfTest(); 
  // 執行自我測試，回傳結果（true: 正常，false: 故障或未知）

  Serial.println(F("-----------------------------"));
  Serial.print(F("結果："));
  
  if (result)
    Serial.println(F("正常 (OK)"));
  else
    Serial.println(F("故障或未知 (DEFECT or UNKNOWN)"));

  Serial.println();
}

void loop() {} // 主迴圈，這個範例沒有任何操作
