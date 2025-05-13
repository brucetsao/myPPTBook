// 設定巨集，用來控制是否輸出偵錯訊息
#define _Debug 1    // 啟用偵錯輸出（大寫版本）
#define _debug 1    // 啟用偵錯輸出（小寫版本）

// 設定初始化及迴圈延遲時間（單位：毫秒）
#define initDelay 6000    // 初始化延遲 6000 毫秒 (6秒)
#define loopdelay 5000    // 主程式循環延遲 5000 毫秒 (5秒)

// 引入 WiFi 相關函式庫
#include <WiFi.h>          // 引入基本 WiFi 功能
#include <WiFiClient.h>    // 引入 WiFi 客戶端通訊功能
#include <WiFiMulti.h>     // 引入多熱點（多個 WiFi）管理功能

// 建立 WiFiMulti 物件，用來管理多個 WiFi 熱點
WiFiMulti wifiMulti;

// 函式宣告：將 IPAddress 類型轉成字串型態
String IpAddress2String(const IPAddress& ipAddress);

// 宣告全域變數
IPAddress ip;       // 用來存放 WiFi 網路卡取得的 IP 位址（原生格式）
String IPData;      // 用來存放轉換成字串格式的 IP 位址
String APname;      // 用來存放連線中的 WiFi 熱點名稱
String MacData;     // 用來存放 WiFi 模組的 MAC 地址
long rssi;          // 用來存放 WiFi 訊號強度（RSSI值）
int status = WL_IDLE_STATUS;  // 網路狀態碼，初始為閒置（未連線）

// 網路初始化函式：連接 WiFi 熱點
void initWiFi()
{
  // 新增要連線的 WiFi 熱點（SSID、密碼）
  wifiMulti.addAP("NCNUIOT", "12345678");   // 新增第一組熱點
  wifiMulti.addAP("NCNUIOT2", "12345678");  // 新增第二組熱點
  wifiMulti.addAP("ABC", "12345678");       // 新增第三組熱點

  // 螢幕上輸出連線過程資訊
  Serial.println();
  Serial.println();
  Serial.print("Connecting to "); // 印出提示訊息

  wifiMulti.run(); // 啟動多熱點連線機制，嘗試連線

  // 進入迴圈，直到成功連上 WiFi 熱點
  while (WiFi.status() != WL_CONNECTED)
  {
    // 如果還沒連線成功，每 500 毫秒嘗試一次
    Serial.print("."); // 每次重試印出一個點，表示正在嘗試連線
    delay(500);        // 延遲 500 毫秒
    wifiMulti.run();   // 繼續嘗試下一個已儲存的熱點
  }

  // 成功連線後，印出連線資訊
  Serial.println("WiFi connected"); // 顯示連線成功
  Serial.print("AP Name: ");         // 顯示熱點名稱標題
  APname = WiFi.SSID();              // 讀取目前連線熱點的 SSID
  Serial.println(APname);            // 顯示連線的熱點名稱

  Serial.print("IP address: ");      // 顯示 IP 位址標題
  ip = WiFi.localIP();               // 取得目前的 IP 位址
  IPData = IpAddress2String(ip);     // 呼叫函式轉成字串格式
  Serial.println(IPData);            // 顯示取得的 IP 位址
}

// 顯示目前網路連線資訊
void ShowInternet()
{
  Serial.print("MAC:");     // 顯示 MAC 地址標題
  Serial.print(MacData);    // 顯示 MAC 地址資料
  Serial.print("\n");       // 換行
  Serial.print("SSID:");    // 顯示連線熱點標題
  Serial.print(APname);     // 顯示熱點名稱
  Serial.print("\n");       // 換行
  Serial.print("IP:");      // 顯示 IP 位址標題
  Serial.print(IPData);     // 顯示 IP 位址
  Serial.print("\n");       // 換行
}

//----------Common Lib
// 計算 num 的 expo 次方
long POW(long num, int expo)
{
  long tmp = 1;  //暫存變數

  if (expo > 0) //次方大於零
  { 
    for (int i = 0; i < expo; i++)  //利用迴圈累乘
  {
      tmp = tmp * num;  // 不斷乘以num
    }
    return tmp;   //回傳產生變數
  } 
  else 
  {
    return tmp;  // 若expo小於或等於0，返回1
  }
}

// 生成指定長度的空格字串
String SPACE(int sp)  //sp為傳入產生空白字串長度
{
  String tmp = "";  //產生空字串
  for (int i = 0; i < sp; i++)  //利用迴圈累加空白字元
  {
    tmp.concat(' ');  // 加入空格
  }
  return tmp; //回傳產生空白字串
}

// 轉換數字為指定長度與進位制的字串，並補零
String strzero(long num, int len, int base)
{
  //num 為傳入的數字
  //len為傳入的要回傳字串長度之數字
  // base 幾進位
  String retstring = String("");  //產生空白字串
  int ln = 1; //暫存變數
  int i = 0;  //計數器
  char tmp[10]; //暫存回傳內容變數
  long tmpnum = num;  //目前數字
  int tmpchr = 0; //字元計數器
  char hexcode[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
  //產生字元的對應字串內容陣列
  while (ln <= len) //開始取數字
  {
    tmpchr = (int)(tmpnum % base);  //取得第n個字串的數字內容，如1='1'、15='F'
    tmp[ln - 1] = hexcode[tmpchr];  //根據數字換算對應字串
    ln++;
    tmpnum = (long)(tmpnum / base); // 求剩下數字
  }
  for (i = len - 1; i >= 0; i--)
  {
    retstring.concat(tmp[i]);//連接字串
  }
  return retstring;  //回傳內容
}


// 轉換指定進位制的字串為數值
unsigned long unstrzero(String hexstr, int base) 
{
  String chkstring; //暫存字串
  int len = hexstr.length();  // 取得長度
  unsigned int i = 0;
  unsigned int tmp = 0; //取得文字之字串位置變數
  unsigned int tmp1 = 0;  //取得文字之對應字串位置變數
  unsigned long tmpnum = 0; //目前數字
  String hexcode = String("0123456789ABCDEF");   //產生字元的對應字串內容陣列
  for (i = 0; i < len; i++)
  {
    hexstr.toUpperCase(); //先轉成大寫文字
    tmp = hexstr.charAt(i); //取第i個字元
    tmp1 = hexcode.indexOf(tmp);  //根據字元，判斷十進位數字
    tmpnum = tmpnum + tmp1 * POW(base, (len - i - 1));  //計算數字
  }
  return tmpnum;  //回傳內容
} 
// 轉換數字為 16 進位字串，若小於 16 則補 0
String print2HEX(int number) 
{
  String ttt;   //暫存字串
  if (number >= 0 && number < 16) //判斷是否在區間
  {
    ttt = String("0") + String(number, HEX);  //產生前補零之字串
  }
  else
  {
    ttt = String(number, HEX);//產生字串
  }
  return ttt; //回傳內容
} 

// 將 char 陣列轉為字串
String chrtoString(char *p)
{
    String tmp; //暫存字串
    char c; //暫存字元
    int count = 0;  //計數器
    while (count < 100) //100個字元以內
    {
        c = *(p+count); //取得字串之每一個字元內容
        if (c != 0x00)  //是否未結束
        {
            tmp.concat(String(c));  //字元累積到字串
        }
        else
        {
            return tmp; //回傳內容
        }
        count++;  // 計數器加一
    }
     return tmp; //回傳內容
}

// 複製 String 到 char 陣列
void CopyString2Char(String ss, char *p)
{
  if (ss.length() <= 0) //是否為空字串
  {
    *p = 0x00;  //加上字元陣列結束0x00
    return; //結束
  }
  ss.toCharArray(p, ss.length() + 1); //利用字串轉字元命令
}

// 比較兩個 char 陣列是否相同
boolean CharCompare(char *p, char *q) 
{
  // *p第一字元陣列的指標 :陣列第一字元的字元指標(用&chararray[0]取得)
  boolean flag = false; //是否結束旗標
  int count = 0;  //計數器
  int nomatch = 0;  //不相同比對計數器
  while (count < 100)  ////是否結束
  {
      if (*(p + count) == 0x00 || *(q + count) == 0x00) //是否結束
          break;  //離開
      if (*(p + count) != *(q + count)) //比較不同
      {
          nomatch++;      //不相同比對計數器累加
      }
      count++;    //計數器累加
  }
  return nomatch == 0;  //回傳是否有不同
}

// 將 double 轉為字串，保留指定小數位數
String Double2Str(double dd, int decn)
{
  //double dd==>傳入之浮點數
  //int decn==>傳入之保留指定小數位數
  int a1 = (int)dd; // 先取整數位數字
  int a3;   //小數點站存變數
  if (decn > 0) //保留指定小數位數大於零
  {
      double a2 = dd - a1;  //取小數位數字
      a3 = (int)(a2 * pow(10, decn)); // 將取得之小數位數字放大10的decn倍
  }
  if (decn > 0) //保留指定小數位數大於零
  {
      return String(a1) + "." + String(a3); 
      //將整數位轉乘之文字+小數點+小數點之擴大長度之數字轉換文字==>產生新字串回傳
  }
  else
  {
      return String(a1);//將整數位轉乘之文字==>產生新字串回傳
  }
}

// 函式名稱：GetMacAddress
// 功能：取得 WiFi 網路卡的 MAC 位址（硬體識別碼），並以字串形式回傳
String GetMacAddress() 
{
  String Tmp = "";    // 建立一個空的字串，用來儲存最終的 MAC 位址
  byte mac[6];        // 宣告一個 6 位元組的陣列，用來存放 MAC 地址（6個Byte）

  // 取得 WiFi 模組的 MAC 位址，存入 mac 陣列
  WiFi.macAddress(mac);

  // 將每一個位元組轉成兩位的十六進位字串，依序串接起來
  for (int i = 0; i < 6; i++) 
  {
      Tmp.concat(print2HEX(mac[i])); // 使用自定義函式 print2HEX() 將單個位元組轉成 HEX 字串後，連接到 Tmp
  }
  
  Tmp.toUpperCase();  // 將 MAC 位址字串轉成全大寫形式（符合常見 MAC 地址格式）
  
  return Tmp;         // 回傳 MAC 位址字串
}

// 函式名稱：ShowMAC
// 功能：將 MAC 地址輸出到序列埠監控視窗
void ShowMAC() 
{
  Serial.print("MAC Address:(");  // 印出開頭標題 "MAC Address:("
  Serial.print(MacData);          // 印出變數 MacData 的內容（注意：這個變數應該是全域變數或外部定義）
  Serial.print(")\n");            // 印出結尾的括號與換行符號
}

// 函式名稱：IpAddress2String
// 功能：將 IPAddress 類型的 IP 位址轉換為可讀取的字串格式（如 192.168.0.1）
String IpAddress2String(const IPAddress& ipAddress)
{
  // 將 IP 位址的四個部份以點（"."）串接成一個標準的 IP 位址字串
  return String(ipAddress[0]) + String(".") + \
         String(ipAddress[1]) + String(".") + \
         String(ipAddress[2]) + String(".") + \
         String(ipAddress[3]);
}
