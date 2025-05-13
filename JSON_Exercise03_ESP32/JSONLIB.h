#include <ArduinoJson.h>  // 引入 ArduinoJson 函式庫，用來處理 JSON 格式資料

// 宣告一個靜態的 JSON 文件，大小為 3000 bytes，主要用來存放完整的 JSON 結構
StaticJsonDocument<3000> json_doc;

// 宣告一個靜態的 JSON 文件，大小為 100 bytes，暫時用來存放單一列資料
StaticJsonDocument<100> json_rowdata;

// 計算 JSON 陣列容量
// capacity = 陣列大小（10個元素）+ 每個元素（Object）大小（每個有2個欄位）
const int capacity = JSON_ARRAY_SIZE(10) + 10 * JSON_OBJECT_SIZE(2);
StaticJsonDocument<capacity> json_row; // 宣告一個陣列，裡面會放每筆溫濕度資料

int arraycount = 0;        // 陣列目前已加入的資料筆數
#define arrayamount 10     // 定義陣列最多可容納 10 筆資料

char json_data[5000];      // 宣告一個字元陣列，用來暫存序列化後的 JSON 字串資料

DeserializationError json_error; // 宣告一個反序列化錯誤變數，用來接收解析 JSON 錯誤

/*
範例 JSON 格式如下（最終會產生類似這樣的資料）：
{
  "Device": "E89F6DE8F3BC",
  "Temperature": 24,
  "Humidity": 77
}
*/

// === 函式區 ===

// 初始化 JSON 資料的函式（目前尚未實作任何動作）
void initjson()
{
  // 清空主 JSON 文件
  json_doc.clear();
  
  // 清空存放單筆溫濕度資料的 JSON 文件
  json_rowdata.clear();
  
  // 清空存放所有列資料的陣列
  json_row.clear();
  
  // 將字元陣列清空
  memset(json_data, 0, sizeof(json_data));
  
  // 重置計數器（如果有用到）
  arraycount = 0;
}
// 設定主 JSON 結構的裝置編號，同時將資料陣列加入主結構
void setjsondata(String mm)
{
  json_doc["Device"] = mm;      // 設定 "Device" 欄位，內容為傳入的裝置編號 mm
  json_doc["Data"] = json_row;  // 將陣列 json_row 放入主 JSON 裡的 "Data" 欄位
  
  // 將整個 json_doc 結構序列化（轉成 JSON 字串），存到 json_data 字元陣列
  serializeJson(json_doc, json_data);
}

// 新增一筆溫濕度資料到資料陣列中
void appendjsondata(double d1, double d2)
{
  // 設定單筆資料（rowdata）裡的 "Temperature" 欄位，數值為 d1
  json_rowdata["Temperature"] = d1;
  
  // 設定單筆資料（rowdata）裡的 "Humidity" 欄位，數值為 d2
  json_rowdata["Humidity"] = d2;
  
  // 把這筆 rowdata 加入到 json_row 陣列裡
  json_row.add(json_rowdata);
  
  // ※ 注意：這裡沒有清除 json_rowdata，會造成加入的是同一物件的修改版（若要避免，可用臨時 JsonObject）
}
