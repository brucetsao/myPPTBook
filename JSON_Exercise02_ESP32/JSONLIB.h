#include <ArduinoJson.h>  // 引入 ArduinoJson 函式庫，用來處理 JSON 資料格式

// 建立一個靜態 JSON 文件（物件），大小為 500 Bytes
StaticJsonDocument<500> json_doc;

// 定義一個字元陣列，用來存放序列化（轉成字串格式後的）JSON 資料
char json_data[1000];

// 定義一個錯誤檢查變數，用來存放 JSON 解析時的錯誤資訊
DeserializationError json_error;


/*
範例 JSON 資料格式說明：
{
  "Device": "E89F6DE8F3BC",   // 設備識別碼（字串）
  "Temperature": 24,          // 溫度（整數或浮點數）
  "Humidity": 77              // 濕度（整數或浮點數）
}
這是將來要組合的 JSON 結構範例
*/

// 初始化 JSON 的函式（目前未實作任何內容）
void initjson()
{
  // 尚未定義動作，可以用來清空 json_doc 或預設初始化設定
  // 例如可以呼叫 json_doc.clear();
}

// 設定 JSON 資料的函式
// 傳入：
// - mm：設備代碼（字串型態）
// - d1：溫度數值（浮點數）
// - d2：濕度數值（浮點數）
void setjsondate(String mm, double d1, double d2)
{
  // 將傳入的設備代碼設定到 JSON 文件的 "Device" 欄位
  json_doc["Device"] = mm;
  
  // 將傳入的溫度數值設定到 JSON 文件的 "Temperature" 欄位
  json_doc["Temperature"] = d1;
  
  // 將傳入的濕度數值設定到 JSON 文件的 "Humidity" 欄位
  json_doc["Humidity"] = d2;
  
  // 將 json_doc 內部的 JSON 資料序列化（轉成 JSON 字串格式），
  // 並存放到字元陣列 json_data 中
  serializeJson(json_doc, json_data);
}
