/*
 * --------------------------------------------------------------------------------------------------------------------
 * 讀取 RFID 卡片（PICC）資料並顯示到序列埠的範例
 * --------------------------------------------------------------------------------------------------------------------
 * 這是 MFRC522 函式庫的範例之一；更多詳細說明與其他範例請參見：
 * https://github.com/OSSLibraries/Arduino_MFRC522v2
 * 
 * 這個範例示範如何使用 MFRC522 RFID 讀卡機（透過 SPI 介面）來讀取 RFID 卡片（PICC）的資料，
 * 並將讀取到的 UID、卡片型態及資料區塊內容印到序列監視器上。
 * 
 * 注意：如果讀卡過程中太快移開卡片，可能會出現 "Timeout in communication" 訊息。
 * 
 * 如果讀卡器支援，同時偵測到多張卡片時，程式會依序將每一張卡片的資料輸出。
 * 
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
#include <MFRC522DriverSPI.h>     // 引入 SPI 介面驅動函式庫
#include <MFRC522DriverPinSimple.h> // 引入簡易腳位設定函式庫
#include <MFRC522Debug.h>         // 引入除錯用函式庫

MFRC522DriverPinSimple ss_pin(5); // 建立 SPI SS 腳位控制器，這裡使用腳位 5（根據實際接線可更改）

SPIClass &spiClass = SPI; // 建立 SPI 通訊物件，可以替換成其他 SPI 介面（例如：SPI2 或軟體 SPI）

const SPISettings spiSettings = SPISettings(SPI_CLOCK_DIV4, MSBFIRST, SPI_MODE0); 
// 設定 SPI 通訊參數：分頻時脈4、資料從高位元開始傳輸、SPI模式0
// 如果硬體不是完全相容 Arduino 規格，可能需要特別設定這些參數

MFRC522DriverSPI driver{ss_pin, spiClass, spiSettings}; // 建立 SPI 驅動器，傳入 SS 腳位與 SPI 設定

MFRC522 mfrc522{driver}; // 建立 MFRC522 模組實例

void setup() {
  Serial.begin(9600); // 初始化序列通訊（傳輸速率 9600），用於與電腦端除錯
  while (!Serial);    // 如果序列埠未開啟（例如 Leonardo/Micro），則停在這裡等待
  mfrc522.PCD_Init(); // 初始化 MFRC522 讀卡機模組

  MFRC522Debug::PCD_DumpVersionToSerial(mfrc522, Serial);	
  // 顯示 MFRC522 模組的硬體版本資訊（例如韌體版本、IC 型號等）

  Serial.println(F("請掃描 RFID 卡片，將顯示 UID、SAK、卡片類型與資料區塊內容..."));
}

void loop() {
  // 持續偵測是否有新的 RFID 卡片靠近
	if ( !mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
		// 如果沒有新卡片或讀取失敗，直接跳出 loop，不做任何事
		return;
	}

  // 成功讀取卡片，將卡片的所有資訊（UID、卡型、資料塊）輸出到序列監視器
  MFRC522Debug::PICC_DumpToSerial(mfrc522, Serial, &(mfrc522.uid));
}
