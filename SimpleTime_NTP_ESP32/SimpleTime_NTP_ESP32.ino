#include <WiFi.h>
#include <time.h>

// Wi-Fi 設定
const char* ssid = "NCNUIOT";//你的WiFi名稱
const char* password = "0123456789";//你的WiFi密碼

// NTP 伺服器與時區設定
const char* ntpServer = "pool.ntp.org";  // NTP 伺服器
const char* time_zone = "CST-8";  // 台灣時區，UTC+8，無夏令時間

time_t now; //宣告now時間變數，型態為time_t
struct tm timeinfo;

void setup() {
  Serial.begin(9600);

  // 連接到 Wi-Fi
  Serial.printf("正在連接到 %s ...\n", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi 已連線！");

  // 初始化並同步時間
  configTzTime(time_zone, ntpServer);  // 設定時區並從 NTP 伺服器獲取時間

  // 等待時間同步
  Serial.println("正在同步時間...");
  delay(2000);  // 等待 2 秒確保時間同步完成
}

void loop() 
{
  time_t now; //宣告now時間變數，型態為time_t
  struct tm timeinfo;
  time(&now);  // 獲取當前時間(from 時間函式庫)==>now變數得到本地標準時間
  localtime_r(&now, &timeinfo);  // 將本地標準時間轉換為timeinfo的時間結構變數，

  // 顯示時間
  Serial.printf("台灣時間：%04d-%02d-%02d %02d:%02d:%02d\n",
                timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday,
                timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);

  delay(1000);  // 每秒更新一次
}
