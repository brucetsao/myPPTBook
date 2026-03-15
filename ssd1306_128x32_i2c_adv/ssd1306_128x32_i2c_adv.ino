/**************************************************************************
 這是一個基於 SSD1306 驅動的單色 OLED 顯示器範例程式

 您現在就可以在 adafruit 的商店中購買！
 ------> http://www.adafruit.com/category/63_98

 這個範例是為 128x32 像素的顯示器設計，使用 I2C 通訊
 只需要 3 個接腳（兩個 I2C 和一個重置腳）

 Adafruit 投入時間和資源提供這個開源程式碼，
 請透過購買 Adafruit 的產品來支持 Adafruit 和開源硬體！

 由 Limor Fried/Ladyada 為 Adafruit Industries 編寫，
 並感謝開源社群的貢獻。
 採用 BSD 授權，請查閱 license.txt 以獲取更多資訊
 以上所有文字以及下方的啟動畫面都必須包含在再發佈中。
 **************************************************************************/

// 引入必要的程式庫
#include <SPI.h>        // SPI 通訊程式庫（雖然本範例使用 I2C，但仍保留以備不時之需）
#include <Wire.h>       // I2C 通訊程式庫
#include <Adafruit_GFX.h>    // Adafruit 的圖形核心程式庫，提供繪圖函式
#include <Adafruit_SSD1306.h> // SSD1306 顯示器驅動程式庫

// 定義顯示器的寬度與高度（單位：像素）
#define SCREEN_WIDTH 128 // OLED 顯示寬度
#define SCREEN_HEIGHT 32 // OLED 顯示高度

// 使用 I2C 連接 SSD1306 顯示器的宣告
// I2C 的腳位由 Wire 程式庫定義。
// 在 Arduino UNO 上：      A4(SDA), A5(SCL)
// 在 Arduino MEGA 2560 上：20(SDA), 21(SCL)
// 在 Arduino LEONARDO 上：  2(SDA),  3(SCL), ...
#define OLED_RESET     -1 // 重置腳位（如果與 Arduino 重置腳位共用，設為 -1）
#define SCREEN_ADDRESS 0x3C ///< 根據 datasheet，128x64 使用 0x3D，128x32 使用 0x3C
// 建立顯示器物件，傳入寬度、高度、I2C 連線（&Wire）以及重置腳位
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define NUMFLAKES     10 // 動畫範例中的雪花數量

#define LOGO_HEIGHT   16 // 標誌圖案的高度
#define LOGO_WIDTH    16 // 標誌圖案的寬度
// 一個簡單的 16x16 像素標誌圖案，儲存在快閃記憶體中（PROGMEM）
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

// 初始化設定，只執行一次
void setup() {
  Serial.begin(9600); // 啟動序列埠通訊，速率 9600 bps

  // 等待顯示器穩定
  delay(500);

  // SSD1306_SWITCHCAPVCC = 從 3.3V 內部產生顯示器所需電壓
  // 嘗試初始化顯示器，如果失敗則印出錯誤訊息並進入無窮迴圈
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 初始化失敗"));
    for(;;); // 停止執行，進入無限迴圈
  }

  // 顯示初始緩衝區內容到螢幕上 --
  // 程式庫預設會顯示 Adafruit 的啟動畫面。
  display.display();
  delay(2000); // 暫停 2 秒

  // 清除緩衝區
  display.clearDisplay();

  // 在座標 (10, 10) 畫一個白色像素
  display.drawPixel(10, 10, SSD1306_WHITE);

  // 將緩衝區的內容顯示到螢幕上。在繪圖指令後「必須」呼叫 display() 才會顯示！
  display.display();
  delay(2000);
  // 不需要在每個繪圖指令後都呼叫 display.display()，
  // 可以批次執行多個繪圖操作後再統一呼叫 display.display() 更新螢幕。
  // 以下範例會展示這兩種方式。

  // 呼叫各個測試函式，展示不同的繪圖功能
  testdrawline();      // 畫線條
  testdrawrect();      // 畫矩形（空心）
  testfillrect();      // 畫矩形（填滿）
  testdrawcircle();    // 畫圓形（空心）
  testfillcircle();    // 畫圓形（填滿）
  testdrawroundrect(); // 畫圓角矩形（空心）
  testfillroundrect(); // 畫圓角矩形（填滿）
  testdrawtriangle();  // 畫三角形（空心）
  testfilltriangle();  // 畫三角形（填滿）
  testdrawchar();      // 顯示預設字體的字元
  testdrawstyles();    // 顯示不同樣式的文字
  testscrolltext();    // 顯示捲動文字
  testdrawbitmap();    // 顯示一個小型點陣圖

  // 反轉顯示然後恢復，中間暫停
  display.invertDisplay(true);  // 反轉顯示（黑變白，白變黑）
  delay(1000);
  display.invertDisplay(false); // 恢復正常顯示
  delay(1000);

  testanimate(logo_bmp, LOGO_WIDTH, LOGO_HEIGHT); // 執行動畫（雪花飄落）
}

// 主迴圈，這裡為空，因為所有示範都在 setup 中執行完畢，最後 testanimate 是一個無窮迴圈
void loop() {
}

// 畫線條的測試函式
void testdrawline() {
  int16_t i;

  display.clearDisplay(); // 清除顯示緩衝區

  // 從左上角 (0,0) 畫線到左邊緣的不同點
  for(i=0; i<display.width(); i+=4) {
    display.drawLine(0, 0, i, display.height()-1, SSD1306_WHITE);
    display.display(); // 每畫一條線就更新螢幕
    delay(1);
  }
  // 從左上角 (0,0) 畫線到上邊緣的不同點
  for(i=0; i<display.height(); i+=4) {
    display.drawLine(0, 0, display.width()-1, i, SSD1306_WHITE);
    display.display();
    delay(1);
  }
  delay(250);

  display.clearDisplay();

  // 從左下角 (0, height-1) 畫線
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

  // 從右下角 (width-1, height-1) 畫線
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

  // 從右上角 (width-1, 0) 畫線
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

// 畫空心矩形的測試函式
void testdrawrect(void) {
  display.clearDisplay();

  // 從外到內畫一系列矩形
  for(int16_t i=0; i<display.height()/2; i+=2) {
    display.drawRect(i, i, display.width()-2*i, display.height()-2*i, SSD1306_WHITE);
    display.display(); // 每畫一個矩形就更新螢幕
    delay(1);
  }

  delay(2000);
}

// 畫填滿矩形的測試函式
void testfillrect(void) {
  display.clearDisplay();

  // 從外到內畫一系列填滿矩形
  for(int16_t i=0; i<display.height()/2; i+=3) {
    // 使用 INVERSE 顏色，使矩形交替為白/黑
    display.fillRect(i, i, display.width()-i*2, display.height()-i*2, SSD1306_INVERSE);
    display.display(); // 每畫一個矩形就更新螢幕
    delay(1);
  }

  delay(2000);
}

// 畫空心圓的測試函式
void testdrawcircle(void) {
  display.clearDisplay();

  // 從小到大畫一系列同心圓
  for(int16_t i=0; i<max(display.width(),display.height())/2; i+=2) {
    display.drawCircle(display.width()/2, display.height()/2, i, SSD1306_WHITE);
    display.display();
    delay(1);
  }

  delay(2000);
}

// 畫填滿圓的測試函式
void testfillcircle(void) {
  display.clearDisplay();

  // 從大到小畫一系列填滿圓
  for(int16_t i=max(display.width(),display.height())/2; i>0; i-=3) {
    // 使用 INVERSE 顏色，使圓交替為白/黑
    display.fillCircle(display.width() / 2, display.height() / 2, i, SSD1306_INVERSE);
    display.display(); // 每畫一個圓就更新螢幕
    delay(1);
  }

  delay(2000);
}

// 畫空心圓角矩形的測試函式
void testdrawroundrect(void) {
  display.clearDisplay();

  // 從外到內畫一系列圓角矩形
  for(int16_t i=0; i<display.height()/2-2; i+=2) {
    display.drawRoundRect(i, i, display.width()-2*i, display.height()-2*i,
      display.height()/4, SSD1306_WHITE);
    display.display();
    delay(1);
  }

  delay(2000);
}

// 畫填滿圓角矩形的測試函式
void testfillroundrect(void) {
  display.clearDisplay();

  // 從外到內畫一系列填滿圓角矩形
  for(int16_t i=0; i<display.height()/2-2; i+=2) {
    // 使用 INVERSE 顏色，使圓角矩形交替為白/黑
    display.fillRoundRect(i, i, display.width()-2*i, display.height()-2*i,
      display.height()/4, SSD1306_INVERSE);
    display.display();
    delay(1);
  }

  delay(2000);
}

// 畫空心三角形的測試函式
void testdrawtriangle(void) {
  display.clearDisplay();

  // 畫一系列大小變化的三角形
  for(int16_t i=0; i<max(display.width(),display.height())/2; i+=5) {
    display.drawTriangle(
      display.width()/2  , display.height()/2-i, // 頂點
      display.width()/2-i, display.height()/2+i, // 左下
      display.width()/2+i, display.height()/2+i, // 右下
      SSD1306_WHITE);
    display.display();
    delay(1);
  }

  delay(2000);
}

// 畫填滿三角形的測試函式
void testfilltriangle(void) {
  display.clearDisplay();

  // 畫一系列大小變化的填滿三角形
  for(int16_t i=max(display.width(),display.height())/2; i>0; i-=5) {
    // 使用 INVERSE 顏色，使三角形交替為白/黑
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

// 顯示字元的測試函式（預設字體）
void testdrawchar(void) {
  display.clearDisplay();

  display.setTextSize(1);      // 正常 1:1 像素比例
  display.setTextColor(SSD1306_WHITE); // 設定文字顏色為白色
  display.setCursor(0, 0);     // 從左上角開始
  display.cp437(true);         // 使用完整的 256 字元 'Code Page 437' 字體

  // 並非所有字元都能顯示在螢幕上，這是正常的。
  // 程式庫會盡量繪製，超出範圍的部分會被裁剪。
  for(int16_t i=0; i<256; i++) {
    if(i == '\n') display.write(' '); // 遇到換行字元就改畫空格
    else          display.write(i);    // 否則畫出該字元
  }

  display.display();
  delay(2000);
}

// 顯示不同樣式文字的測試函式
void testdrawstyles(void) {
  display.clearDisplay();

  display.setTextSize(1);             // 正常 1:1 像素比例
  display.setTextColor(SSD1306_WHITE);        // 白色文字
  display.setCursor(0,0);             // 從左上角開始
  display.println(F("Hello, world!")); // 印出文字（並換行）

  display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); // 反白文字（黑字白底）
  display.println(3.141592);           // 印出圓周率

  display.setTextSize(2);             // 2 倍放大文字
  display.setTextColor(SSD1306_WHITE); // 白色文字
  display.print(F("0x")); display.println(0xDEADBEEF, HEX); // 以十六進位印出數值

  display.display();
  delay(2000);
}

// 捲動文字的測試函式
void testscrolltext(void) {
  display.clearDisplay();

  display.setTextSize(2); // 2 倍放大文字
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 0);
  display.println(F("scroll")); // 顯示 "scroll"
  display.display();      // 顯示初始文字
  delay(100);

  // 向不同方向捲動，中間暫停：
  display.startscrollright(0x00, 0x0F); // 向右捲動（從第0行到第15行）
  delay(2000);
  display.stopscroll();                 // 停止捲動
  delay(1000);
  display.startscrollleft(0x00, 0x0F);  // 向左捲動
  delay(2000);
  display.stopscroll();
  delay(1000);
  display.startscrolldiagright(0x00, 0x07); // 向右對角捲動
  delay(2000);
  display.startscrolldiagleft(0x00, 0x07);  // 向左對角捲動
  delay(2000);
  display.stopscroll();
  delay(1000);
}

// 顯示點陣圖的測試函式
void testdrawbitmap(void) {
  display.clearDisplay();

  // 將標誌圖案畫在螢幕中央
  display.drawBitmap(
    (display.width()  - LOGO_WIDTH ) / 2,
    (display.height() - LOGO_HEIGHT) / 2,
    logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display();
  delay(1000);
}

// 定義存取 'icons' 陣列的索引常數
#define XPOS   0 // X 座標
#define YPOS   1 // Y 座標
#define DELTAY 2 // Y 軸移動速度

// 點陣圖動畫測試函式（模擬雪花飄落）
void testanimate(const uint8_t *bitmap, uint8_t w, uint8_t h) {
  int8_t f, icons[NUMFLAKES][3]; // 用來儲存每個雪花的位置與速度

  // 初始化每個雪花的位置
  for(f=0; f< NUMFLAKES; f++) {
    icons[f][XPOS]   = random(1 - LOGO_WIDTH, display.width()); // X 座標隨機，可在畫面左邊外開始
    icons[f][YPOS]   = -LOGO_HEIGHT;                             // Y 座標從畫面上方外開始
    icons[f][DELTAY] = random(1, 6);                             // 下落速度隨機（1~5 像素/影格）
    // 透過序列埠印出每個雪花的初始狀態
    Serial.print(F("x: "));
    Serial.print(icons[f][XPOS], DEC);
    Serial.print(F(" y: "));
    Serial.print(icons[f][YPOS], DEC);
    Serial.print(F(" dy: "));
    Serial.println(icons[f][DELTAY], DEC);
  }

  for(;;) { // 無限迴圈，持續執行動畫
    display.clearDisplay(); // 清除顯示緩衝區

    // 繪製每個雪花
    for(f=0; f< NUMFLAKES; f++) {
      display.drawBitmap(icons[f][XPOS], icons[f][YPOS], bitmap, w, h, SSD1306_WHITE);
    }

    display.display(); // 更新螢幕顯示
    delay(200);        // 暫停 0.2 秒，控制動畫速度

    // 更新每個雪花的座標
    for(f=0; f< NUMFLAKES; f++) {
      icons[f][YPOS] += icons[f][DELTAY]; // 根據速度向下移動
      // 如果雪花已經超出畫面底部
      if (icons[f][YPOS] >= display.height()) {
        // 重新初始化該雪花，讓它從畫面上方外重新出現
        icons[f][XPOS]   = random(1 - LOGO_WIDTH, display.width());
        icons[f][YPOS]   = -LOGO_HEIGHT;
        icons[f][DELTAY] = random(1, 6);
      }
    }
  }
}
