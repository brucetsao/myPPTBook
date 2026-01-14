#include <ArduinoJson.h>  // 引入 ArduinoJson 函式庫，用於處理 JSON 格式資料

void setup() 
{
  // 初始化序列埠通訊
  Serial.begin(9600);   // 設定與電腦通訊的速度為 9600 bps（位元每秒）

  // 確保序列埠準備好再繼續執行程式
  while (!Serial) continue; // 如果監控視窗尚未開啟，程式會停在這裡等待

  // 宣告一個靜態的 JSON 文件物件
  //
  // StaticJsonDocument<容量> 會在堆疊（stack）上配置記憶體。
  // 這裡容量設為 200 bytes，實際應根據 JSON 資料大小調整。
  // 可以使用 ArduinoJson 官方網站的容量計算器輔助計算（arduinojson.org/v6/assistant）。
  StaticJsonDocument<200> doc;

  // 另一種方式是使用 DynamicJsonDocument（動態配置在堆積 heap 上）
  // DynamicJsonDocument doc(200);

  // 定義一個 JSON 格式的字串，作為輸入資料
  //
  // 使用 char[] 字元陣列，可啟用「零拷貝模式（zero-copy mode）」，
  // 減少記憶體用量（JSON 物件直接指向原始資料，而不是複製）
  char json[] =
      "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";
  // 這是一個描述 GPS 感測器資訊的 JSON 字串，包括感測器名稱、時間戳記及位置數據

  // 將 JSON 字串解析（反序列化）到 JSON 文件物件 doc 中
  DeserializationError error = deserializeJson(doc, json);
  // 使用 deserializeJson() 函式解析 json[] 字串，
  // 解析成功與否的結果會儲存在 error 變數中

  // 判斷解析是否成功
  if (error)  // 如果 error 為 true，表示解析失敗
  {
    Serial.print(F("deserializeJson() failed: "));  // 顯示解析失敗訊息
    Serial.println(error.f_str());                  // 印出具體的錯誤訊息
    return; // 解析失敗則不繼續執行後續程式
  }

  // 解析成功後，讀取 JSON 內的各個值
  //
  // 通常可以直接使用隱式轉型，但如果需要，也可以使用 as<型別>() 明確指定型別
  const char* sensor = doc["sensor"];
  // 宣告並讀取字串型態的 "sensor" 欄位，內容是感測器名稱

  long time = doc["time"];
  // 宣告並讀取整數型態的 "time" 欄位，內容是時間戳記（UNIX time）

  double latitude = doc["data"][0];
  // 宣告並讀取陣列型態的 "data" 中第 0 個元素，為緯度 (latitude)

  double longitude = doc["data"][1];
  // 宣告並讀取陣列型態的 "data" 中第 1 個元素，為經度 (longitude)

  // 印出讀取到的各個值
  Serial.println(sensor);     // 印出感測器名稱
  Serial.println(time);       // 印出時間戳記
  Serial.println(latitude, 6); // 印出緯度，顯示 6 位小數
  Serial.println(longitude, 6);// 印出經度，顯示 6 位小數
}

void loop() {
  // 本範例不使用 loop()，因此留空
}
