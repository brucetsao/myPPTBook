#include "initPins.h"  // 引入自訂的系統腳位函式庫
#include "OledLib.h"  // 引入OLED 顯示器的自訂函式庫

void initAll();    //初始化系統 in initPins.h
void initWiFi();   //網路連線，連上熱點
String GetMacAddress() ;   //取得網路卡編號
void ShowInternet() ;  //秀出網路連線資訊
void ShowMAC() ;// 在串列埠顯示MAC地址
void printMAConOLED(String ss);  //在OLED顯示器上顯示MAC
void initDevice() ;//初始化連接到開發板之所有感測裝置


char macvalue[14] ;  //列印MAC轉換字串陣列


void setup() 
{
  initAll();  // 系統初始化
  initDevice();   //初始化連接到開發板之所有感測裝置
    initWiFi();      // 呼叫函數：初始化網路連線，連接到指定的 WiFi 熱點（如路由器）
    delay(2000) ;
    MacData = GetMacAddress() ; // 取得網路卡的MAC地址
    ShowMAC() ;// 在串列埠顯示MAC地址
    ShowInternet();  // 呼叫函數：顯示當前的網路連線資訊，例如 IP 地址，方便除錯

}

void loop() 
{
  
  ShowMAC();  // 在串列埠中印出網路卡的MAC地址
  printMAConOLED(MacData) ;//在OLED顯示器上顯示MAC
  _updateBuffer();    //更新oled畫面
  delay(2000) ;
}

void initDevice()//初始化連接到開發板之所有感測裝置
{
    initOLED() ;  //啟動OLED顯示器，並檢查是否成功啟動。
   _clearBuffer();   //清除之前畫圖等記憶體
   _setFont(u8g2_font_logisoso16_tf);  //設定字形  
   _updateBuffer();    //更新oled畫面
   
}

void printMAConOLED(String ss)  //在OLED顯示器上顯示MAC
{
    sprintf(macvalue,"%s",ss)  ;  //轉換string字串到char array
    _TPrint(1,16,macvalue);  //(50, 15) 顯示卡號。
}
