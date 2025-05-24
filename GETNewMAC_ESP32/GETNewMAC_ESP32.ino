#include "initPins.h"  // 引入自訂的系統腳位函式庫
void initAll();    //初始化系統 in initPins.h
void initWiFi();   //網路連線，連上熱點
String GetMacAddress() ;   //取得網路卡編號
void ShowInternet() ;  //秀出網路連線資訊
void ShowMAC() ;// 在串列埠顯示MAC地址


void setup() 
{
  initAll();  // 系統初始化
    initWiFi();      // 呼叫函數：初始化網路連線，連接到指定的 WiFi 熱點（如路由器）
    delay(2000) ;
    MacData = GetMacAddress() ; // 取得網路卡的MAC地址
    ShowMAC() ;// 在串列埠顯示MAC地址
    ShowInternet();  // 呼叫函數：顯示當前的網路連線資訊，例如 IP 地址，方便除錯

}

void loop() 
{
  
  ShowMAC();  // 在串列埠中印出網路卡的MAC地址
  delay(2000) ;
}
