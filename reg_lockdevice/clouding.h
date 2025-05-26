#include <ArduinoJson.h>  // 引入 ArduinoJson 函式庫，用於解析 JSON 字串資料
#include <HTTPClient.h>   // 引入 HTTPClient，用來進行 HTTP 通訊（GET、POST 等）

// 定義雲端伺服器的主機名稱與通訊埠
char iotserver[] = "iot.arduino.org.tw";    // 雲端伺服器位址（建國老師的伺服器）
int iotport = 8888 ;                        // HTTP 服務的通訊埠號

// 組成 HTTP 請求用的相關字串
String httphead = "http://iot.arduino.org.tw:8888/bigdata/rfid/reglockdevice.php" ;
String strGet = "GET /bigdata/rfid/reglockdevice.php";   // GET 方法與程式路徑
String strHttp = " HTTP/1.1";                             // HTTP 協定版本
String strHost = "Host: iot.arduino.org.tw";              // 主機標頭
String connectstr ;                                       // 儲存組合完成的 HTTP URL 字串

//============================
// 函式：gethttpString()
// 功能：產生 GET 請求參數字串
// 輸入：mm -> MAC Address
//============================
String gethttpString(String mm)
{
  return "?MAC=" + mm;  // 回傳組好的 GET 參數（範例：?MAC=112233445566）
}

//============================
// 函式：sendHttpGetRequest()
// 功能：向指定 URL 發送 HTTP GET 請求，並回傳伺服器的回應字串
//============================
String sendHttpGetRequest(String url) {
  if (url == "") {
    return "";
  }
  HTTPClient http;           // 建立 HTTP 物件
  http.begin(url);           // 指定要連接的網址
  int httpCode = http.GET(); // 執行 GET 請求
  String payload = "";       
  if (httpCode == HTTP_CODE_OK) {  // 如果伺服器回應 200 OK
    payload = http.getString();    // 取得伺服器回傳的資料
  }
  http.end();               // 關閉 HTTP 連線
  return payload;           // 回傳回應資料
}

//============================
// 函式：processJsonResponse()
// 功能：解析從伺服器取得的 JSON 資料，並確認 MAC 位址與結果是否為 "Added"
//============================
bool processJsonResponse(String jsonResponse, String macAddress) 
{
  if (jsonResponse == "") 
  {
    return false;  // 若回傳空字串，直接視為失敗
  }

  StaticJsonDocument<200> doc; // 宣告一個 JSON 文件物件（最多 200 bytes）

  // 將 JSON 字串解析成 ArduinoJson 的資料物件
  DeserializationError error = deserializeJson(doc, jsonResponse);

  // 若解析錯誤，印出錯誤訊息並結束
  if (error) 
  {
    Serial.print("JSON parsing failed: ");
    Serial.println(error.c_str());
    return false;
  }

  // 顯示整個 payload 字串（方便除錯）
  Serial.print("Payload:(");
  Serial.print(jsonResponse);
  Serial.print(")\n");

  // 解析 JSON 欄位
  const char* device = doc["Device"];   // 取得回傳資料中的 MAC 位址
  const char* systime = doc["Systime"]; // 系統時間（目前未使用）
  const char* result = doc["Result"];   // 結果欄位（例如 "Added"）

  // 判斷是否 MAC 位址正確，且結果為 "Added"
  if (String(device) == macAddress && String(result) == "Added") 
  {
    Serial.println("LOCK DEVICE Added"); // 顯示授權成功
    return true;
  } 
  else 
  {
    Serial.println("LOCK DEVICE Add Fail"); // 顯示授權失敗
    return false;
  }
}

//============================
// 函式：SendtoClouding()
// 功能：將裝置的 MAC 位址透過 HTTP GET 傳送到雲端伺服器
//============================
void SendtoClouding()     
{
  // 組合完整的 HTTP GET 請求網址（含參數）
  connectstr = httphead + gethttpString(MacData); // MacData 是裝置的 MAC（需先定義）
  
  Serial.println(connectstr);  // 印出組合好的 URL

  // 發送 HTTP 請求並取得回應
  String response = sendHttpGetRequest(connectstr);

  // 若有成功取得回應
  if (response != "") 
  {
    Serial.println("Response: " + response);  // 顯示回應內容

    // 分析 JSON 並檢查是否授權成功
    bool success = processJsonResponse(response, MacData);
  } 
  else 
  {
    Serial.println("HTTP request failed"); // 顯示請求失敗
  }
}
