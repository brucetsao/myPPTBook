//http://iot.arduino.org.tw:8888/bigdata/rfid/disact.php?MAC=112233445566&KEY=0079262864

#include <ArduinoJson.h>  // 引入 ArduinoJson 函式庫，用來處理 JSON 格式資料
#include <HTTPClient.h> 

char iotserver[] = "iot.arduino.org.tw";    // 建國老師 的雲端服務器
int iotport = 8888 ;  //定義雲端伺服器的通訊埠號，這裡設為 8888，用於後續的網路連線。
// Server URL: iot.arduino.org.tw 
// Server Port: 8888
String httphead = "http://iot.arduino.org.tw:8888/bigdata/rfid/disact.php" ;
String strGet="GET /bigdata/rfid/disact.php";
//  DB Agent 的 HTTP GET 請求的開頭，指向雲端伺服器上的特定程式
String strHttp=" HTTP/1.1";   // HTTP 協議的版本標頭，這裡是 HTTP/1.1，表示使用 HTTP 1.1 版本進行通訊。 
String strHost="Host: iot.arduino.org.tw";  // 指定目標主機的域名，這裡是 iot.arduino.org.tw，這是 HTTP 請求頭的一部分。
 String connectstr ;    //一個空的字串變數，後續用來動態組成完整的 RESTful 請求參數。

/*
    http://iot.arduino.org.tw:8888/bigdata/rfid/disact.php?MAC=112233445566&KEY=0079262864
   完整的 HTTP 請求範例，分解如下：
  主機位址：iot.arduino.org.tw:8888（域名與通訊埠）。
  應用程式路徑：/bigdata/rfid/disact.php?MAC=112233445566&KEY=0079262864（伺服器上的 PHP 程式，用來接收並處理資料）。
  參數：?MAC=112233445566，表示傳送的資料MAC Address
  參數：?KEY=0079262864，表示傳送的資料 RFID Card UUID

 */
String getRFIDString(String mm, String cc)
{
  //http://iot.arduino.org.tw:8888/bigdata/rfid/disact.php?MAC=112233445566&KEY=0079262864
  return "?MAC="+mm+"&KEY="+cc;
}
 String sendHttpGetRequest(String url) {
  if (url == "") {
    return "";
  }
  HTTPClient http;
  http.begin(url);
  int httpCode = http.GET();
  String payload = "";
  if (httpCode == HTTP_CODE_OK) {
    payload = http.getString();
  }
  http.end();
  return payload;
}

//bool processJsonResponse(String jsonResponse, String macAddress, String cardNumber) {
//  if (jsonResponse == "") {
//    return false;
//  }
//  // Allocate the JSON document
//  StaticJsonDocument<200> doc;
//  // Parse JSON
//  DeserializationError error = deserializeJson(doc, jsonResponse);
//  if (error) {
//    Serial.print("JSON parsing failed: ");
//    Serial.println(error.c_str());
//    return false;
//  }
//  // Extract Device, Card, and Result
//  const char* device = doc["Device"];
//  const char* card = doc["Card"];
//  const char* result = doc["Result"];
//  // Compare Device with MAC, Card with cardNumber, and check if Result is "Find"
//  if (String(device) == macAddress && String(card) == cardNumber && String(result) == "Find") 
//  {
//    Relayon() ; //控制板上Relay Modules打開
//    return true;
//  } 
//  else 
//  {
//    Relayoff() ;//控制板上Relay Modules關閉 
//    return false;
//  }
//}

void SendtoClouding()     //傳送感測資料到雲端
{
  // 自訂函數，用來將感測器的資料（例如溫度和濕度）透過 HTTP GET 請求傳送到雲端伺服器。
          connectstr = httphead+getRFIDString(MacData,uidStr) ;    //組出 http get 淂程式與參數
          /*
          組成GET Format 的Resetful  的 Parameters 字串
          connectstr：動態組成 RESTful 請求的參數部分：
          MacData：設備的 MAC 位址（假設已在程式其他地方定義）。
          Tvalue：溫度值，轉換成字串格式。
          Hvalue：濕度值，轉換成字串格式。
          */
          
 Serial.println(connectstr) ;//將組合好的參數字串輸出到序列監控視窗，用於除錯
 String response = sendHttpGetRequest(connectstr);
   if (response != "") 
   {
      Serial.println("Response: " + response);
//      // Function 4: Process JSON and control relay
//      bool success = processJsonResponse(response, MacData, uidStr);
//      if (success) 
//      { 
//        //Relayon() ; //控制板上Relay Modules打開
//        Serial.println("Access granted: Relay ON");
//      } 
//      else 
//      {
//        //Relayoff() ;//控制板上Relay Modules關閉  
//        Serial.println("Access denied: Relay OFF");
//        }
  } 
  else 
  {
      Serial.println("HTTP request failed");
  }
      
}
 
