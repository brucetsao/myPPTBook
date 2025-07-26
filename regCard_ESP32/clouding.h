//http://iot.arduino.org.tw:8888/bigdata/rfid/act.php?KEY=0219390674&MAC=B8D61A68DAFC

#include <HTTPClient.h>       // 引入 HTTP 用的函式庫
#include "JSONLib.h"          // 自訂或外部的 JSON 函式庫（可能內含 ArduinoJson）

// 雲端伺服器資訊設定
char iotserver[] = "iot.arduino.org.tw";   // 建國老師的伺服器網址
int iotport = 8888;                        // 使用的 TCP 通訊埠

// 定義完整的 HTTP GET 相關參數
String httphead = "http://iot.arduino.org.tw:8888/bigdata/rfid/act.php";
String strGet = "/bigdata/rfid/act.php"; // 請求行的開頭（未直接使用）
String strHttp = " HTTP/1.1";                       // HTTP 協定版本
String strHost = "Host: iot.arduino.org.tw";        // Host 標頭
String connectstr; // 最後完整的請求 URL 字串將組在這裡
String msg ; //json 回應result的顯示文字

/*
 //http://iot.arduino.org.tw:8888/bigdata/rfid/act.php?KEY=0219390674&MAC=B8D61A68DAFC
   完整的 HTTP 請求範例，分解如下：
  主機位址：iot.arduino.org.tw:8888（域名與通訊埠）。
  應用程式路徑：/bigdata/rfid/act.php?KEY=0219390674&MAC=B8D61A68DAFC（伺服器上的 PHP 程式，用來接收並處理資料）。
  參數：?MAC=B8D61A68DAFC，表示傳送的資料MAC Address
  參數：?KEY=0219390674，表示傳送的資料 RFID Card UUID

 */
// 傳入 MAC 位址與卡號，組成完整參數 "?MAC=xxx&KEY=yyy"
String getRFIDString(String mm, String cc)
{
  return "?MAC=" + mm + "&KEY=" + cc;
}

String sendHttpGetRequest(String url) {
  if (url == "") return "";  // 如果 URL 為空，直接回傳空字串

  HTTPClient http;
  http.begin(url);           // 設定連線的網址
  int httpCode = http.GET(); // 發送 GET 請求
  String payload = "";

  if (httpCode == HTTP_CODE_OK) {
    payload = http.getString(); // 如果成功回應，讀取內容
  }

  http.end(); // 關閉 HTTP 連線
  return payload;
}


bool processJsonResponse(String jsonResponse, String macAddress, String cardNumber) {
  if (jsonResponse == "") return false; // 空資料，表示失敗


  DeserializationError error = deserializeJson(doc, jsonResponse); // 解析 JSON

  if (error) {
    Serial.print("JSON parsing failed: ");
    Serial.println(error.c_str());
    return false;
  }

/*
 {
 "Device":"B8D61A68DAFC",
 "Card":"0219390674",
 "Result":"Data Added and Ativated"
 }
 {
 "Device":"B8D61A68DAFC",
 "Card":"0219390674",
 "Result":"Ativated"
 }
  {
 "Device":"B8D61A68DAFC",
 "Card":"0219390674",
 "Result":"had Ativated"
 } 
 */

  // 從 JSON 中提取欄位
  const char* device = doc["Device"];
  const char* card = doc["Card"];
  const char* result = doc["Result"];

  // 判斷是否匹配並允許通過
  if (String(card) == cardNumber && String(result) == "Data Added and Ativated") 
  {
    msg="Data Added" ;    //無此卡號，加入卡號並啟動
    return true;
  }
  if (String(card) == cardNumber && String(result) == "Ativated") 
  {
    msg="Ativated" ;   //卡號存在但未啟動，讓此卡號啟動
    return true;
  }
  if (String(card) == cardNumber && String(result) == "had Ativated") 
  {
    msg="had Ativated" ;  //卡號存在也已啟動，不動作
    return true;
  }
  return false ;
}

void SendtoClouding() {
  // 組合 HTTP GET 請求字串：加上參數
  connectstr = httphead + getRFIDString(MacData, uidStr);
  Serial.println(connectstr); // 印出完整的 URL，用於除錯

  // 發送 HTTP 請求，並取得回應
  String response = sendHttpGetRequest(connectstr);

  if (response != "") 
  {
    Serial.println("Response: " + response);
    // 解析回應與控制繼電器
    bool success = processJsonResponse(response, MacData, uidStr);

    if (success) 
    {
      Serial.println("Card add is ok");
      PrintCardonOLED(uidStr) ;  //顯示卡號在Oled上
      PrintResultonOLED(msg) ;  //顯示卡號在Oled上
      _updateBuffer();    //更新oled畫面
    } 
    else 
    {
      Serial.println("System Error");
      PrintCardonOLED(uidStr) ;  //顯示卡號在Oled上
      PrintResultonOLED("Error") ;  //顯示卡號在Oled上
      _updateBuffer();    //更新oled畫面
    }
  } 
  else 
  {
    Serial.println("HTTP request failed");
  }

  // 若有 TCP 客戶端連線，主動斷開
  if (client.connected()) {
    client.stop(); // 釋放資源
  }
}

 
