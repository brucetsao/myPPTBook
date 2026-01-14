/*
這段程式碼的註解說明了如何使用 ArduinoJson 庫，
將回傳的資料序列化成 JSON 格式。
 */
#include <ArduinoJson.h>  // Json 使用元件
StaticJsonDocument<200> doc; // 建立 JSON 文件物件

/*
{
{
"Device":"3367216813",
"Result":"Find"
}
{
"Device":"336721622",
"Result":"notFind"
}
   找到==>"Result":"Find"
   找不到==>"Result":"notFind"
}
*/

void initjson()   // 初始化 JSON 資料
{

}
