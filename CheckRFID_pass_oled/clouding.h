//http://iot.arduino.org.tw:8888/bigdata/rfid/rfidpass.php?KEY=0243348066

char iotserver[] = "iot.arduino.org.tw";    // 建國老師 的雲端服務器
int iotport = 8888 ;  //定義雲端伺服器的通訊埠號，這裡設為 8888，用於後續的網路連線。
// Server URL: iot.arduino.org.tw 
// Server Port: 8888
// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):


String strGet="GET /bigdata/rfid/rfidpass.php";
//  DB Agent 的 HTTP GET 請求的開頭，指向雲端伺服器上的特定程式
String strHttp=" HTTP/1.1";   // HTTP 協議的版本標頭，這裡是 HTTP/1.1，表示使用 HTTP 1.1 版本進行通訊。 
String strHost="Host: iot.arduino.org.tw";  // 指定目標主機的域名，這裡是 iot.arduino.org.tw，這是 HTTP 請求頭的一部分。
 String connectstr ;    //一個空的字串變數，後續用來動態組成完整的 RESTful 請求參數。
//http://iot.arduino.org.tw:8888/bigdata/rfid/rfidpass.php?KEY=0243348066// host is  ==>nuk.arduino.org.tw:8888

//http://iot.arduino.org.tw:8888/bigdata/rfid/rfidpass.php?KEY=0243348066

/*
   完整的 HTTP 請求範例，分解如下：
  主機位址：iot.arduino.org.tw:8888（域名與通訊埠）。
  應用程式路徑：/bigdata/rfid/rfidpass.php?KEY=0243348066（伺服器上的 PHP 程式，用來接收並處理資料）。
  參數：?KEY=0243348066，表示傳送的資料：
  KEY：RFID設備的唯一識別碼（例如 112233445566）。

 
 */
void SendtoClouding()     //傳送感測資料到雲端
{
  // 自訂函數，用來將感測器的資料（例如溫度和濕度）透過 HTTP GET 請求傳送到雲端伺服器。
          connectstr = "?KEY="+uidStr;    //組出 http get 淂程式與參數
          /*
          組成GET Format 的Resetful  的 Parameters 字串
          connectstr：動態組成 RESTful 請求的參數部分：
          MacData：設備的 MAC 位址（假設已在程式其他地方定義）。
          Tvalue：溫度值，轉換成字串格式。
          Hvalue：濕度值，轉換成字串格式。
          */
          
 Serial.println(connectstr) ;//將組合好的參數字串輸出到序列監控視窗，用於除錯
 
 if (client.connect(iotserver, iotport)) //  連線到雲端主機，嘗試連線到指定的雲端伺服器
 {
         Serial.println("Make a HTTP request ... ");
         //### Send to Server
         String strHttpGet = strGet + connectstr + strHttp;
         //組成完整的 HTTP GET 請求 ex.GET /bigdata/rfid/rfidpass.php?KEY=0243348066 HTTP/1.1
         // strHt = strGet="GET /bigdata/rfid/rfidpass.php +
        // connectstr = "?KEY=0243348066
        // String strHttp=" HTTP/1.1";   // HTTP 協議的版本標頭，這裡是 HTTP/1.1，表示使用 HTTP 1.1 版本進行通訊。
         
         Serial.println(strHttpGet);    //　將完整的請求輸出到序列監控視窗，用於除錯。
          //傳送那段REST Ful API字串到雲端---start ----
         client.println(strHttpGet);    //　將 HTTP GET 請求發送到雲端伺服器。
         client.println(strHost);       //　印出結尾:strHost
         client.println();     //　發送一個空行，表示 HTTP 請求結束。
         //傳送那段REST Ful API字串到雲端---end---
       } 
    
  if (client.connected()) //檢查是否仍與伺服器保持連線
  { 
    client.stop();  //如果連線存在，則主動斷開與伺服器的連接，釋放資源。
  }

    
}
 
