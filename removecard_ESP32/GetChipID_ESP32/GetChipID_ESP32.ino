/* 
 * 真實的 ESP32 晶片 ID 實際上是其 MAC 位址。
 * 這個程式提供了一個替代的晶片 ID，與 ESP8266 上的 ESP.getChipId() 函數輸出相符
 * （即一個 32 位整數，對應 MAC 位址的最後 3 個位元組）。
 * 這個 ID 比完整的 MAC 位址唯一性低，但當你需要一個不超過 32 位整數的識別碼時（例如用於 switch...case 語句），它非常有用。
 *
 * 創建日期：2020-06-07
 * 作者：cweinhofer
 * 感謝 Cicicok 的幫助
 */

uint32_t chipId = 0;  // 定義一個 32 位無符號整數變數 chipId，用來儲存晶片 ID，初始值為 0

void setup() {
  Serial.begin(9600);  // 初始化序列監控器，設定波特率為 9600，用於後續輸出資料到電腦
}

void loop() {
  for (int i = 0; i < 17; i = i + 8) {  // 迴圈從 0 開始，每次增加 8，直到 i < 17（實際運行 0、8）
    chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;  // 計算並組合晶片 ID 的各個位元組
    // 解釋：
    // 1. ESP.getEfuseMac() 獲取 ESP32 的 MAC 位址（48 位，6 個位元組）
    // 2. >> (40 - i) 執行右移運算，根據 i 的值選擇 MAC 位址的不同部分
    // 3. & 0xff 取低 8 位（一個位元組），去除高位干擾
    // 4. << i 將結果左移 i 位，放入適當的位置
    // 5. |= 將結果與 chipId 進行位或運算，逐步組合成最終的 32 位 ID
  }

  Serial.printf("ESP32 Chip model = %s Rev %d\n", ESP.getChipModel(), ESP.getChipRevision());
  // 輸出 ESP32 的晶片型號（ESP.getChipModel()）和修訂版本（ESP.getChipRevision()），如 "ESP32-D0WDQ6 Rev 1"
  
  Serial.printf("This chip has %d cores\n", ESP.getChipCores());
  // 輸出晶片的核心數量（ESP.getChipCores()），ESP32 通常有 2 個核心

  Serial.print("Chip ID: ");
  Serial.println(chipId);  // 輸出最終計算出的 32 位晶片 ID

  delay(3000);  // 延遲 3000 毫秒（3 秒），控制輸出頻率，避免資料過於密集
}
