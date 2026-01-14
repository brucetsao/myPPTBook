#include <WiFi.h>   // 使用網路函式庫
IPAddress ip;    // 儲存網路卡取得的 IP 位址的變數
String Ipdata;   // 儲存網路卡取得的 IP 位址的字串變數
String Apname;   // 儲存網路熱點名稱的變數
String MacData;   // 儲存網路卡編號 (MAC 地址) 的變數
long rssi;   // 儲存網路連線的訊號強度變數
int status = WL_IDLE_STATUS;  // 儲存網路狀態的變數
