/*
程式原理與流程圖解
這支程式的核心功能是：將 ESP32 裝置的 MAC Address 傳送到雲端伺服器，並取得伺服器回應的授權結果（JSON 格式），根據伺服器是否回傳 "Added"，來確認裝置是否被允許上鎖或解鎖。

程式流程：
  組合網址（connectstr）：
  把基本 URL 加上 ?MAC=裝置MAC位址 組成 GET 請求網址。
  發送 HTTP GET 請求
  使用 HTTPClient 向伺服器發送請求。
  取得伺服器回應（JSON）

範例回應內容：json
{
  "Device":"B8D61A68DAFC",
  "Systime":"2024-06-01 12:00:00",
  "Result":"Added"
}

解析 JSON 回應
  使用 ArduinoJson 提取欄位，檢查：
  是否為本裝置的 MAC Address
  是否為 "Result":"Added"
  印出結果
  顯示是否授權成功或失敗。
 */
