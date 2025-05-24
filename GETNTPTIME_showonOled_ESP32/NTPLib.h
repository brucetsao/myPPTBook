#include <time.h>
// NTP 伺服器與時區設定
const char* ntpServer = "pool.ntp.org";  // NTP 伺服器
const char* time_zone = "CST-8";  // 台灣時區，UTC+8，無夏令時間

time_t now; //宣告now時間變數，型態為time_t
struct tm timeinfo;
void ShowDateTime(String ss); //顯示ｓｙｓｔｉｍｅ
void ShowNTPTime();  //秀網路日期時間  
String getNTPTimeString(); //取得網路日期時間，以YYYYMMDDhhmmss 字串回傳
tm getNTPTime(); //取得網路日期時間，以tm型態傳回
void initNTP();  //初始化網路時間

void initNTP()  //初始化網路時間
{
  // 初始化並同步時間
  configTzTime(time_zone, ntpServer);  // 設定時區並從 NTP 伺服器獲取時間

  // 等待時間同步
  Serial.println("正在同步時間...");
  delay(2000);  // 等待 2 秒確保時間同步完成
  
}

tm getNTPTime() //取得網路日期時間，以tm型態傳回
{
    time(&now);  // 獲取當前時間(from 時間函式庫)==>now變數得到本地標準時間
  localtime_r(&now, &timeinfo);  // 將本地標準時間轉換為timeinfo的時間結構變數，
  Serial.println("------");
  Serial.println(timeinfo.tm_year);
  Serial.println(timeinfo.tm_mon);
  Serial.println(timeinfo.tm_mday);
  Serial.println(timeinfo.tm_hour);
  Serial.println(timeinfo.tm_min);
  Serial.println(timeinfo.tm_sec);
  
  return timeinfo ; 
}
String getNTPTimeString() //取得網路日期時間，以YYYYMMDDhhmmss 字串回傳
{
  String tmps = ""  ;
    tm tmp;
    tmp = getNTPTime();
    tmps = strzero((long)tmp.tm_year+1900,4,10)+strzero((long)tmp.tm_mon+1,2,10)+strzero((long)tmp.tm_mday,2,10)+strzero((long)tmp.tm_hour,2,10)+strzero((long)tmp.tm_min,2,10)+strzero((long)tmp.tm_sec,2,10) ;
  return tmps;
}
void ShowNTPTime()  //秀網路日期時間
{
    time(&now);  // 獲取當前時間(from 時間函式庫)==>now變數得到本地標準時間
  localtime_r(&now, &timeinfo);  // 將本地標準時間轉換為timeinfo的時間結構變數，

  // 顯示時間
  Serial.printf("台灣時間：%04d-%02d-%02d %02d:%02d:%02d\n",
                timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday,
                timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);

}
void ShowDateTime(String ss) //顯示ｓｙｓｔｉｍｅ
{
  Serial.print("Now Date Time is :") ;
  Serial.print(ss) ;
  Serial.print("\n") ;
  
}
