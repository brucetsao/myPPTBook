/*
 1. 什麼是 time_t？
time_t 定義：
time_t 是一種資料型態，
通常定義為一個 無符號長整數（unsigned long） 或 有符號長整數（long），
具體取決於編譯器和平台
（在 Arduino 的 ESP32/ESP8266 環境中，通常是 long）。

它是用來表示時間的標準型態，
特別是在 C 語言的 <time.h> 庫中。
其值通常是以 從 1970 年 1 月 1 日 00:00:00 UTC（Unix 紀元時間）起經過的秒數 
表示，這種表示方式稱為 Unix 時間戳（Unix Timestamp）。

範圍：
由於 time_t 是長整數（通常 32 位或 64 位），
32 位系統的 time_t 最大值約為 2038 年 1 月 19 日（因「2038 年問題」），
而 64 位系統幾乎可以無限期使用。
在 ESP32/ESP8266 的 Arduino 環境中，
time_t 通常是 32 位，
但透過 NTP 同步後，處理的是 UTC 時間。

2. 程式中的 time_t now;
宣告：
time_t now; 定義了一個名為 now 的變數，
型態為 time_t，用來儲存當前時間。
這一行只是宣告變數，
尚未賦值，
初始值為未定義（通常為 0 或隨機值，依賴記憶體狀態）。
賦值：
在 loop 函數中，
time(&now); 
會調用 <time.h> 庫中的 time 函數，
將當前時間（從系統時鐘或 NTP 伺服器同步的 UTC 時間）以秒為單位儲存到 now 變數中。
例如，
假設當前時間是 2025 年 5 月 19 日下午 2:09 PM CST（UTC+8），
對應的 UTC 時間約為 2025 年 5 月 19 日上午 6:09 AM，
now 將儲存從 1970 年 1 月 1 日到這個時間點的總秒數（大約為 1,740,000,000 秒，具體值取決於精確的秒數）。


3. 與 struct tm 的關係
time_t 和 struct tm：
time_t 儲存的是單一的秒數，
這種格式雖然精確但不直觀（例如無法直接得知是哪一年、哪個月）。
為了將 time_t 轉換成人類可讀的日期和時間，
程式中使用了 localtime_r(&now, &timeinfo); 
將 now 轉換成 struct tm 型態的結構。
struct tm 是一個結構體，定義如下（來自 <time.h>）：

struct tm {
    int tm_sec;    // 秒數 (0-59)
    int tm_min;    // 分鐘 (0-59)
    int tm_hour;   // 小時 (0-23)
    int tm_mday;   // 日期 (1-31)
    int tm_mon;    // 月份 (0-11，0 表示 1 月)
    int tm_year;   // 年份 (從 1900 起算，例如 2025 為 125)
    int tm_wday;   // 星期幾 (0-6，0 表示星期日)
    int tm_yday;   // 一年中的第幾天 (0-365)
    int tm_isdst;  // 夏令時間標誌 (-1/0/1)
};

localtime_r 函數會根據設定的時區（例如 CST-8），
將 now 的 UTC 秒數轉換為本地時間，
並填入 timeinfo 結構中。

程式中的應用：
在範例程式中，localtime_r(&now, &timeinfo); 
將 now 轉換為台灣本地時間（UTC+8），
然後使用 timeinfo 的成員（如 tm_year、tm_mon 等）來格式化輸出，

例如：
Serial.printf("台灣時間：%04d-%02d-%02d %02d:%02d:%02d\n",
              timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday,
              timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);

 假設當前時間為 2025 年 5 月 19 日下午 2:09 PM CST，
 輸出將是 2025-05-19 14:09:XX（秒數依同步時機而定）。
 
4. 運作原理
時間來源：
ESP32/ESP8266 本身沒有內建實時時鐘（RTC），
時間通常透過網路從 NTP（Network Time Protocol）伺服器同步獲得。
configTzTime(time_zone, ntpServer); 
會連接到 pool.ntp.org 等伺服器，
獲取當前 UTC 時間，並根據 CST-8 調整為台灣時間。

從 time_t 到可讀時間：
time(&now) 獲取當前 UTC 秒數。
localtime_r 根據時區將秒數轉換為 struct tm 結構。
最終透過 printf 將結構中的值格式化輸出。
5. 實際例子
假設今天是 2025 年 5 月 19 日下午 2:09 PM CST（UTC+8）：

now 的值約為 1740000000 秒（從 1970 年 1 月 1 日到 2025 年 5 月 19 日 6:09 AM UTC 的總秒數）。
localtime_r 將其轉換為：
tm_year = 125（2025 - 1900）
tm_mon = 4（5 月，0 基數）
tm_mday = 19
tm_hour = 14（下午 2 點）
tm_min = 9
tm_sec = X（依同步時機而定）
輸出為 2025-05-19 14:09:XX。

6. 注意事項
同步時間：now 的值依賴 NTP 同步，初次執行可能需要幾秒鐘，
建議加入延遲（如 delay(2000)）確保時間有效。

時區正確性：
確保 time_zone 設為 CST-8，
否則 localtime_r 會根據錯誤的時區計算。
溢位問題：
若使用 32 位 time_t，
需注意 2038 年問題，
建議未來升級到支援 64 位的系統。

總結
time_t now; 宣告了一個用於儲存從 1970 年 1 月 1 日起秒數的變數，
通過 time(&now) 獲取當前 UTC 時間，
然後透過 localtime_r 根據台灣時區（CST-8）轉換為本地時間，
最終格式化輸出為人類可讀的日期和時間。
這是 Arduino 環境中實現時間管理的基礎機制
             
 
 */
