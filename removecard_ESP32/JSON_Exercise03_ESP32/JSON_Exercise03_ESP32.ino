#include "initPins.h"     // 引入腳位設定與系統模組標頭檔
#include "JSONLIB.h"      // 引入自訂的 JSON 資料處理模組（例如：MQTT Broker 上傳用）
/*
arrayamount：是一個常數，定義了收集多少筆資料後才一起送出（你前面定義是 10 筆）。
json_row：是一個 JSON 陣列，用來累積多筆溫度與濕度資料。
json_data：是儲存最後完整序列化後 JSON 字串的字元陣列。
MacData：儲存裝置的 MAC 位址。
loopdelay：每次循環的延遲時間，單位為毫秒（前面你有定義成 5000，也就是每5秒送一筆）。
*/
// -----------------------------------------------
// 程式初始化區
// -----------------------------------------------
void setup() 
{
    initALL(); // 呼叫初始化函式，進行系統硬體與軟體的基本設定

    arraycount = 0; // 初始化資料計數器，準備累積感測資料
}

// -----------------------------------------------
// 主迴圈區（重複執行）
// -----------------------------------------------
void loop() 
{
    // 如果累積的資料筆數達到預設的上限
    if (arraycount >= arrayamount)
    {
        setjsondata(MacData); // 將目前的 MAC 位址填入 JSON 的 Device 欄位
        serializeJson(json_row, Serial);   // 將 json_row 陣列（多筆感測資料）以 JSON 格式直接輸出到監控視窗
        Serial.println(""); // 換行
        Serial.println(json_data); // 印出序列化後的完整 JSON 字串
        
        arraycount = 0; // 重置資料計數器
        json_row.clear(); // 清空累積的資料列，準備下一輪收集
    } 

    // 產生模擬溫度值（25度為中心，加入-6~+6度間的隨機誤差）
    double Temp_Value = 25 + (double)(random(-300, 300) / 50);
    
    // 產生模擬濕度值（70%為中心，加入-30%~+30%間的隨機誤差）
    double Humid_Value = 70 + (double)(random(-300, 300) / 10);

    // 將模擬的溫濕度資料組合成字串
    String msg = "目前溫度:" + String(Temp_Value) + "°C, 目前濕度：" + String(Humid_Value);

    // 將溫度、濕度資料印到序列埠監控視窗
    Serial.print("Temp: "); Serial.print(Temp_Value); Serial.print(" C");
    Serial.print("\t\t");
    Serial.print("Humidity: "); Serial.print(Humid_Value); Serial.println(" %");

    // 如果目前累積的筆數尚未超過上限
    if (arraycount < arrayamount)
    {
        appendjsondata(Temp_Value, Humid_Value); // 呼叫函式將一筆溫濕度資料附加到 JSON 陣列中
    }

    arraycount++; // 資料筆數加一
    delay(loopdelay); // 延遲一段時間（單位：毫秒），避免資料更新太快
}

// -----------------------------------------------
// 系統初始化區（硬體、軟體、網路連線）
// -----------------------------------------------
void initALL()  
{
    Serial.begin(9600); // 開啟序列埠，設定通訊速率為 9600 bps
    Serial.println("System Start"); // 顯示系統啟動訊息

    MacData = GetMacAddress(); // 取得本裝置的網路卡 MAC 地址
    initWiFi(); // 嘗試連線到事先設定好的 WiFi 熱點
    ShowInternet(); // 顯示目前連線的 WiFi SSID、IP位址、MAC位址
}
