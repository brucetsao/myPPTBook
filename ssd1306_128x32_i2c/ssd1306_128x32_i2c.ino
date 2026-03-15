/**************************************************************************
 此範例適用於基於 SSD1306 驅動器的單色 OLED 顯示器

 您今天可以在 adafruit 商店選購一個！
 ------> http://www.adafruit.com/category/63_98

 此範例適用於 128x32 像素的顯示器，使用 I2C 進行通訊
 需要 3 個引腳來連接（兩個 I2C 和一個重置引腳）。

 Adafruit 投入時間和資源提供這個開源程式碼，
 請支援 Adafruit 和開源硬體，購買 Adafruit 的產品！

 由 Limor Fried/Ladyada 為 Adafruit Industries 編寫，
 並感謝開源社群的貢獻。
 採用 BSD 許可證，請查閱 license.txt 獲取更多資訊
 任何再散佈都必須包含上述文字以及下方的啟動畫面。
 **************************************************************************/

// 引入必要的程式庫
#include <SPI.h>          // SPI 通訊用（雖然此範例用 I2C，但 Adafruit_GFX 可能需要）
#include <Wire.h>         // I2C 通訊用
#include <Adafruit_GFX.h> // Adafruit 的圖形核心程式庫，提供繪圖函數
#include <Adafruit_SSD1306.h> // SSD1306 驅動晶片的具體控制程式庫

// 定義螢幕的寬度與高度（像素）
#define SCREEN_WIDTH 128 // OLED 顯示寬度，單位：像素
#define SCREEN_HEIGHT 32 // OLED 顯示高度，單位：像素

// 宣告一個使用 I2C (SDA, SCL 引腳) 連接的 SSD1306 顯示器物件
// I2C 使用的引腳由 Wire 程式庫定義。
// 在 Arduino UNO 上:       A4(SDA), A5(SCL)
// 在 Arduino MEGA 2560 上: 20(SDA), 21(SCL)
// 在 Arduino LEONARDO 上:   2(SDA),  3(SCL), ...
#define OLED_RESET     -1 // 重置引腳編號（如果與 Arduino 重置引腳共用，則設為 -1）
#define SCREEN_ADDRESS 0x3C ///< 請查閱資料手冊取得位址；128x64 通常為 0x3D，128x32 通常為 0x3C
// 建立顯示器物件，將寬度、高度、Wire 物件和重置引腳傳入
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define NUMFLAKES     10 // 動畫範例中雪花的數量

// 定義一個小圖示（Logo）的寬度和高度
#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16
// 一個 16x16 像素的點陣圖資料，存儲在程式記憶體 (PROGMEM) 中以節省 RAM
static const unsigned char PROGMEM logo_bmp[] =
{ 0b00000000, 0b11000000,
  0b00000001, 0b11000000,
  0b00000001, 0b11000000,
  0b00000011, 0b11100000,
  0b11110011, 0b11100000,
  0b11111110, 0b11111000,
  0b01111110, 0b11111111,
  0b00110011, 0b10011111,
  0b00011111, 0b11111100,
  0b00001101, 0b01110000,
  0b00011011, 0b10100000,
  0b00111111, 0b11100000,
  0b00111111, 0b11110000,
  0b01111100, 0b11110000,
  0b01110000, 0b01110000,
  0b00000000, 0b00110000 };

void setup() {
  // 初始化序列通訊，設定速率為 9600，用於輸出除錯訊息
  Serial.begin(9600);

  // 等待顯示器準備就緒
  delay(500);

  // SSD1306_SWITCHCAPVCC = 由 3.3V 內部產生顯示電壓
  // 嘗試初始化顯示器，並傳入 I2C 位址
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    // 如果初始化失敗，列印錯誤訊息並進入無限迴圈，程式停止在此
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // 不要繼續執行，在此無限迴圈
  }

  // 顯示初始的顯示緩衝區內容到螢幕上——
  // 程式庫初始化時會在緩衝區放入 Adafruit 的啟動畫面。
  display.display(); // 將緩衝區的內容實際發送到螢幕顯示
  delay(2000); // 暫停 2 秒，讓您看到啟動畫面

  // 清除顯示緩衝區（將所有像素設為 off）
  display.clearDisplay();

  // 在座標 (10, 10) 畫一個白色像素
  display.drawPixel(10, 10, SSD1306_WHITE);

  // 將緩衝區的內容顯示到螢幕上。在執行繪圖指令後，
  // 必須呼叫 display() 才能使繪圖結果顯示在螢幕上！
  display.display();
  delay(2000);
  // display.display() 不一定要在每個繪圖指令後都呼叫，
  // 您可以批次執行多個繪圖操作，然後一次性呼叫 display() 更新螢幕。
  // 以下範例將展示這兩種方法...

  // 呼叫各個測試函數，展示不同的繪圖功能
  testdrawline();      // 畫多條線
  testdrawrect();      // 畫矩形（空心）
  testfillrect();      // 畫矩形（實心）
  testdrawcircle();    // 畫圓形（空心）
  testfillcircle();    // 畫圓形（實心）
  testdrawroundrect(); // 畫圓角矩形（空心）
  testfillroundrect(); // 畫圓角矩形（實心）
  testdrawtriangle();  // 畫三角形（空心）
  testfilltriangle();  // 畫三角形（實心）
  testdrawchar();      // 用預設字體顯示字元
  testdrawstyles();    // 顯示不同樣式的文字
  testscrolltext();    // 顯示捲動文字
  testdrawbitmap();    // 顯示一個小的點陣圖影像

  // 反相顯示，然後恢復，中間暫停
  display.invertDisplay(true);  // 開啟反相顯示（黑變白，白變黑）
  delay(1000);
  display.invertDisplay(false); // 關閉反相顯示
  delay(1000);

  testanimate(logo_bmp, LOGO_WIDTH, LOGO_HEIGHT); // 執行動畫（雪花飄落效果）
}

void loop() {
  // 主迴圈為空，因為所有展示功能都在 setup() 中依序執行，
  // 最後的 testanimate() 函數本身是一個無限迴圈，會一直執行動畫。
}

// 測試函數：畫多條線，展示 drawLine() 功能
void testdrawline() {
  int16_t i;

  display.clearDisplay(); // 清除顯示緩衝區

  // 從左上角 (0,0) 向右下角畫線，線的終點 x 座標遞增
  for(i=0; i<display.width(); i+=4) {
    // 畫一條從 (0,0) 到 (i, 螢幕高度-1) 的線
    display.drawLine(0, 0, i, display.height()-1, SSD1306_WHITE);
    display.display(); // 每畫一條線就更新一次螢幕，可以看到畫線過程
    delay(1);
  }
  // 從左上角 (0,0) 向右下角畫線，線的終點 y 座標遞增
  for(i=0; i<display.height(); i+=4) {
    display.drawLine(0, 0, display.width()-1, i, SSD1306_WHITE);
    display.display();
    delay(1);
  }
  delay(250); // 短暫暫停

  display.clearDisplay();

  // 從左下角 (0, 高度-1) 向右上角畫線
  for(i=0; i<display.width(); i+=4) {
    display.drawLine(0, display.height()-1, i, 0, SSD1306_WHITE);
    display.display();
    delay(1);
  }
  for(i=display.height()-1; i>=0; i-=4) {
    display.drawLine(0, display.height()-1, display.width()-1, i, SSD1306_WHITE);
    display.display();
    delay(1);
  }
  delay(250);

  display.clearDisplay();

  // 從右下角 (寬-1, 高-1) 向左上角畫線
  for(i=display.width()-1; i>=0; i-=4) {
    display.drawLine(display.width()-1, display.height()-1, i, 0, SSD1306_WHITE);
    display.display();
    delay(1);
  }
  for(i=display.height()-1; i>=0; i-=4) {
    display.drawLine(display.width()-1, display.height()-1, 0, i, SSD1306_WHITE);
    display.display();
    delay(1);
  }
  delay(250);

  display.clearDisplay();

  // 從右上角 (寬-1, 0) 向左下角畫線
  for(i=0; i<display.height(); i+=4) {
    display.drawLine(display.width()-1, 0, 0, i, SSD1306_WHITE);
    display.display();
    delay(1);
  }
  for(i=0; i<display.width(); i+=4) {
    display.drawLine(display.width()-1, 0, i, display.height()-1, SSD1306_WHITE);
    display.display();
    delay(1);
  }

  delay(2000); // 暫停 2 秒
}

// 測試函數：畫空心矩形
void testdrawrect(void) {
  display.clearDisplay();

  // 從中心向外畫一系列逐漸變大的空心矩形
  for(int16_t i=0; i<display.height()/2; i+=2) {
    // drawRect(左上角x, 左上角y, 寬度, 高度, 顏色)
    display.drawRect(i, i, display.width()-2*i, display.height()-2*i, SSD1306_WHITE);
    display.display(); // 每畫一個矩形就更新螢幕
    delay(1);
  }

  delay(2000);
}

// 測試函數：畫實心矩形
void testfillrect(void) {
  display.clearDisplay();

  // 從中心向外畫一系列逐漸變大的實心矩形
  for(int16_t i=0; i<display.height()/2; i+=3) {
    // 使用 INVERSE 顏色，這樣矩形會交替顯示白色/黑色（反相）
    display.fillRect(i, i, display.width()-i*2, display.height()-i*2, SSD1306_INVERSE);
    display.display(); // 更新螢幕
    delay(1);
  }

  delay(2000);
}

// 測試函數：畫空心圓形
void testdrawcircle(void) {
  display.clearDisplay();

  // 畫一系列半徑逐漸增大的空心圓，圓心在螢幕中心
  for(int16_t i=0; i<max(display.width(),display.height())/2; i+=2) {
    // drawCircle(圓心x, 圓心y, 半徑, 顏色)
    display.drawCircle(display.width()/2, display.height()/2, i, SSD1306_WHITE);
    display.display();
    delay(1);
  }

  delay(2000);
}

// 測試函數：畫實心圓形
void testfillcircle(void) {
  display.clearDisplay();

  // 畫一系列半徑逐漸減小的實心圓，圓心在螢幕中心
  for(int16_t i=max(display.width(),display.height())/2; i>0; i-=3) {
    // 使用 INVERSE 顏色，使圓形交替顯示
    display.fillCircle(display.width() / 2, display.height() / 2, i, SSD1306_INVERSE);
    display.display(); // 更新螢幕
    delay(1);
  }

  delay(2000);
}

// 測試函數：畫空心圓角矩形
void testdrawroundrect(void) {
  display.clearDisplay();

  // 畫一系列逐漸變大的空心圓角矩形
  for(int16_t i=0; i<display.height()/2-2; i+=2) {
    // drawRoundRect(左上角x, 左上角y, 寬度, 高度, 圓角半徑, 顏色)
    display.drawRoundRect(i, i, display.width()-2*i, display.height()-2*i,
      display.height()/4, SSD1306_WHITE);
    display.display();
    delay(1);
  }

  delay(2000);
}

// 測試函數：畫實心圓角矩形
void testfillroundrect(void) {
  display.clearDisplay();

  // 畫一系列逐漸變大的實心圓角矩形
  for(int16_t i=0; i<display.height()/2-2; i+=2) {
    // 使用 INVERSE 顏色，使圓角矩形交替顯示
    display.fillRoundRect(i, i, display.width()-2*i, display.height()-2*i,
      display.height()/4, SSD1306_INVERSE);
    display.display();
    delay(1);
  }

  delay(2000);
}

// 測試函數：畫空心三角形
void testdrawtriangle(void) {
  display.clearDisplay();

  // 畫一系列大小變化的空心三角形
  for(int16_t i=0; i<max(display.width(),display.height())/2; i+=5) {
    // drawTriangle(頂點1x, 頂點1y, 頂點2x, 頂點2y, 頂點3x, 頂點3y, 顏色)
    display.drawTriangle(
      display.width()/2  , display.height()/2-i, // 上頂點
      display.width()/2-i, display.height()/2+i, // 左下頂點
      display.width()/2+i, display.height()/2+i, // 右下頂點
      SSD1306_WHITE);
    display.display();
    delay(1);
  }

  delay(2000);
}

// 測試函數：畫實心三角形
void testfilltriangle(void) {
  display.clearDisplay();

  // 畫一系列大小變化的實心三角形
  for(int16_t i=max(display.width(),display.height())/2; i>0; i-=5) {
    // 使用 INVERSE 顏色，使三角形交替顯示
    display.fillTriangle(
      display.width()/2  , display.height()/2-i,
      display.width()/2-i, display.height()/2+i,
      display.width()/2+i, display.height()/2+i,
      SSD1306_INVERSE);
    display.display();
    delay(1);
  }

  delay(2000);
}

// 測試函數：顯示預設字體的所有字元
void testdrawchar(void) {
  display.clearDisplay();

  display.setTextSize(1);      // 設定文字大小為 1:1 像素比例（正常大小）
  display.setTextColor(SSD1306_WHITE); // 設定文字顏色為白色
  display.setCursor(0, 0);     // 將游標設定在左上角
  display.cp437(true);         // 使用完整的 256 字元 'Code Page 437' 字體（包含特殊符號）

  // 並非所有字元都能同時顯示在螢幕上，這是正常的。
  // 程式庫會盡可能繪製，超出螢幕的部分將被裁剪。
  for(int16_t i=0; i<256; i++) {
    if(i == '\n') display.write(' '); // 如果遇到換行字元，我們改畫一個空格（避免版面混亂）
    else          display.write(i);    // 否則，畫出該編碼對應的字元
  }

  display.display();
  delay(2000);
}

// 測試函數：顯示不同樣式的文字
void testdrawstyles(void) {
  display.clearDisplay();

  // 正常文字
  display.setTextSize(1);             // 正常 1:1 像素比例
  display.setTextColor(SSD1306_WHITE); // 白色文字
  display.setCursor(0,0);             // 游標設在左上角
  display.println(F("Hello, world!")); // 印出字串，並換行

  // 反相文字（黑色文字，白色背景）
  display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); // 設定文字顏色為黑色，背景為白色
  display.println(3.141592); // 印出圓周率數字

  // 兩倍大小的文字
  display.setTextSize(2);             // 設定為 2 倍大小
  display.setTextColor(SSD1306_WHITE); // 白色文字
  display.print(F("0x")); display.println(0xDEADBEEF, HEX); // 印出 "0x" 然後印出十六進位數 DEADBEEF

  display.display();
  delay(2000);
}

// 測試函數：顯示捲動文字
void testscrolltext(void) {
  display.clearDisplay();

  display.setTextSize(2); // 設定為 2 倍大小文字
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 0);
  display.println(F("scroll")); // 印出 "scroll"
  display.display();      // 顯示初始文字
  delay(100);

  // 在不同方向上捲動，中間暫停：
  display.startscrollright(0x00, 0x0F); // 開始向右捲動，從第0行到第15行
  delay(2000);
  display.stopscroll(); // 停止捲動
  delay(1000);
  display.startscrollleft(0x00, 0x0F); // 開始向左捲動
  delay(2000);
  display.stopscroll();
  delay(1000);
  display.startscrolldiagright(0x00, 0x07); // 開始向右下對角線捲動
  delay(2000);
  display.startscrolldiagleft(0x00, 0x07); // 開始向左下對角線捲動
  delay(2000);
  display.stopscroll();
  delay(1000);
}

// 測試函數：顯示點陣圖
void testdrawbitmap(void) {
  display.clearDisplay();

  // 將 logo_bmp 點陣圖繪製在螢幕中央
  // drawBitmap(x, y, bitmap資料, 寬度, 高度, 顏色)
  display.drawBitmap(
    (display.width()  - LOGO_WIDTH ) / 2, // 計算 X 座標使其置中
    (display.height() - LOGO_HEIGHT) / 2, // 計算 Y 座標使其置中
    logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1); // 1 表示使用前景色（白色）
  display.display();
  delay(1000);
}

// 用於動畫中追蹤雪花位置的索引常數
#define XPOS   0 // X 座標在 icons 陣列中的索引
#define YPOS   1 // Y 座標在 icons 陣列中的索引
#define DELTAY 2 // Y 軸移動速度在 icons 陣列中的索引

// 測試函數：執行動畫（雪花飄落效果）
void testanimate(const uint8_t *bitmap, uint8_t w, uint8_t h) {
  int8_t f, icons[NUMFLAKES][3]; // 建立一個陣列來儲存每個雪花的位置和速度

  // 初始化每個雪花的起始位置和速度
  for(f=0; f< NUMFLAKES; f++) {
    icons[f][XPOS]   = random(1 - LOGO_WIDTH, display.width()); // 隨機 X 座標，可能從左邊界外開始
    icons[f][YPOS]   = -LOGO_HEIGHT; // 從螢幕上方外部開始（-LOGO_HEIGHT）
    icons[f][DELTAY] = random(1, 6); // 隨機下落速度 (1 到 5 像素/影格)
    // 透過序列埠印出初始位置資訊，用於除錯
    Serial.print(F("x: "));
    Serial.print(icons[f][XPOS], DEC);
    Serial.print(F(" y: "));
    Serial.print(icons[f][YPOS], DEC);
    Serial.print(F(" dy: "));
    Serial.println(icons[f][DELTAY], DEC);
  }

  for(;;) { // 無限迴圈，持續執行動畫
    display.clearDisplay(); // 清除顯示緩衝區

    // 繪製每一個雪花：
    for(f=0; f< NUMFLAKES; f++) {
      // 在當前位置繪製雪花點陣圖
      display.drawBitmap(icons[f][XPOS], icons[f][YPOS], bitmap, w, h, SSD1306_WHITE);
    }

    display.display(); // 將緩衝區內容顯示到螢幕上
    delay(200);        // 暫停 200 毫秒，控制動畫速度

    // 更新每個雪花的座標
    for(f=0; f< NUMFLAKES; f++) {
      icons[f][YPOS] += icons[f][DELTAY]; // Y 座標增加速度值，即向下移動
      // 如果雪花已經完全移出螢幕底部
      if (icons[f][YPOS] >= display.height()) {
        // 重新初始化這個雪花，讓它從頂部再次出現
        icons[f][XPOS]   = random(1 - LOGO_WIDTH, display.width()); // 隨機 X 座標
        icons[f][YPOS]   = -LOGO_HEIGHT; // 重置到頂部外部
        icons[f][DELTAY] = random(1, 6); // 隨機新的下落速度
      }
    }
  }
}