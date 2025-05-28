<?php
    // 範例網址（用於測試停用 RFID 卡片的功能）：
    // http://iot.arduino.org.tw:8888/bigdata/rfid/disact.php?KEY=0243348066&MAC=B8D61A68DAFC
    // http://localhost:8888/bigdata/rfid/disact.php?KEY=0243348066&MAC=B8D61A68DAFC

    include("../Connections/iotcnn.php"); // 引入資料庫連線設定檔，通常包含 Connection() 函式

    $link = Connection(); // 呼叫 Connection() 函式以建立與資料庫的連線

    // 定義一個用來包裝回應資料的類別（JSON 格式）
    class maindata
    {
        public $Device ; // 儲存裝置 MAC 位址（例如 ESP32 的 MAC）
        public $Card ;   // 儲存 RFID 卡片的號碼
        public $Result ; // 儲存操作結果，例如 "Find"、"notFind"、"Deactivated"
    }

    // 建立 maindata 類別的實體，用來回傳給前端或裝置
    $maindata = new maindata();

    // 取得使用者 IP 位址
    $ip = getenv("REMOTE_ADDR");

    // 取得目前系統時間（格式通常為 yyyymmddhhmmss）
    $sysdt = getdatetime();

    // 檢查網址中是否有提供 MAC（裝置編號）
    if (!isset($_GET["MAC"])) {
        echo "Device MAC lost <br>"; // 未提供 MAC，顯示錯誤訊息
        die(); // 終止程式執行
    }

    // 檢查網址中是否有提供 KEY（RFID 卡號）
    if (!isset($_GET["KEY"])) {
        echo "RFID KEY lost <br>"; // 未提供 KEY，顯示錯誤訊息
        die(); // 終止程式執行
    }

    // 取得參數中的 MAC 與 KEY 並存入變數
    $temp0 = $_GET["MAC"]; // 裝置 MAC
    $temp1 = $_GET["KEY"]; // RFID 卡號

    // 檢查 MAC 是否為空字串
    if (trim($temp0) == "") {
        echo "MAC is empty string <br>"; // 顯示錯誤訊息
        die(); // 終止執行
    }

    // 檢查 RFID 卡號是否為空字串
    if (trim($temp1) == "") {
        echo "Card number is empty string <br>"; // 顯示錯誤訊息
        die(); // 終止執行
    }

    // 建立 SQL 查詢語句，查找資料表中是否有啟用（activate = 1）且符合該 RFID 卡號的紀錄
    $qry = "select * from rfidlist where rfidkey = '%s' and activate = 1 ";
    $qrystr = sprintf($qry, $temp1); // 代入 RFID 卡號至查詢語句中

// 執行查詢語句
$result = mysqli_query($link, $qrystr);
$num = mysqli_num_rows($result); // 取得查詢結果的筆數

    // === 根據查詢結果的筆數執行不同邏輯 ===

    if ($num == 0) {
        // 如果查無此卡號（或尚未啟用），視為卡片不存在
        $passok = "Card Not Exoisted"; // 設定回傳結果為卡片不存在
    }

    if ($num >= 1) 
    {
        // 如果找到了卡號，且卡片處於啟用狀態，則執行停用動作
        $row = mysqli_fetch_array($result); // 取得該筆資料
        $id = $row["id"]; // 取得該筆資料的主鍵 ID

        if ((int)$row["activate"] == 1) 
        {
            // 若卡片目前為啟用狀態（activate=1），則更新為停用（activate=0）
            $qry = "update rfidlist set activate = 0, systime = '%s' , IP = '%s' where id = %s";
            $qrystr = sprintf($qry, $sysdt, $ip, $id); // 代入更新資料用的 SQL 語句
            $result = mysqli_query($link, $qrystr); // 執行更新語句
            $passok = "Deactivated"; // 設定回傳結果為已停用
        }
    }

    // 將操作資訊放入 maindata 實體中
    $maindata->Device = $temp0; // 設定裝置 MAC
    $maindata->Card = $temp1;   // 設定 RFID 卡號
    $maindata->Result = $passok; // 設定操作結果

    // 若 $result 是查詢型態的結果集，釋放資源
    if ($result && $result instanceof mysqli_result) 
    {
        mysqli_free_result($result); // 釋放查詢結果資源
    }

    mysqli_close($link); // 關閉資料庫連線

    // 將結果轉為 JSON 格式輸出（並保留 Unicode 字元）
    echo json_encode($maindata, JSON_UNESCAPED_UNICODE);
?>


<?php
    // 將日期時間轉換為指定格式（yyyymmddhhmmss）
    function getdataorder($dt) {
        $splitTimeStamp = explode(" ", $dt);
        $vdate = explode('-', $splitTimeStamp[0]);
        $vtime = explode(':', $splitTimeStamp[1]);
        $yyyy = str_pad($vdate[0], 4, "0", STR_PAD_LEFT);
        $mm = str_pad($vdate[1], 2, "0", STR_PAD_LEFT);
        $dd = str_pad($vdate[2], 2, "0", STR_PAD_LEFT);
        $hh = str_pad($vtime[0], 2, "0", STR_PAD_LEFT);
        $min = str_pad($vtime[1], 2, "0", STR_PAD_LEFT);
        $sec = str_pad($vtime[2], 2, "0", STR_PAD_LEFT);
        return ($yyyy.$mm.$dd.$hh.$min.$sec);
    }

    // 同上，但不含秒數
    function getdataorder2($dt) {
        $splitTimeStamp = explode(" ", $dt);
        $vdate = explode('-', $splitTimeStamp[0]);
        $vtime = explode(':', $splitTimeStamp[1]);
        $yyyy = str_pad($vdate[0], 4, "0", STR_PAD_LEFT);
        $mm = str_pad($vdate[1], 2, "0", STR_PAD_LEFT);
        $dd = str_pad($vdate[2], 2, "0", STR_PAD_LEFT);
        $hh = str_pad($vtime[0], 2, "0", STR_PAD_LEFT);
        $min = str_pad($vtime[1], 2, "0", STR_PAD_LEFT);
        return ($yyyy.$mm.$dd.$hh.$min);
    }

    // 取得目前系統時間（yyyymmddhhmmss）
    function getdatetime() {
        $dt = getdate();
        $yyyy = str_pad($dt['year'], 4, "0", STR_PAD_LEFT);
        $mm = str_pad($dt['mon'], 2, "0", STR_PAD_LEFT);
        $dd = str_pad($dt['mday'], 2, "0", STR_PAD_LEFT);
        $hh = str_pad($dt['hours'], 2, "0", STR_PAD_LEFT);
        $min = str_pad($dt['minutes'], 2, "0", STR_PAD_LEFT);
        $sec = str_pad($dt['seconds'], 2, "0", STR_PAD_LEFT);
        return ($yyyy.$mm.$dd.$hh.$min.$sec);
    }

    // 將 yyyymmddhhmmss 格式轉回為可讀取的時間格式 yyyy/mm/dd hh:mm:ss
    function trandatetime1($dt) {
        $yyyy = substr($dt, 0, 4);
        $mm = substr($dt, 4, 2);
        $dd = substr($dt, 6, 2);
        $hh = substr($dt, 8, 2);
        $min = substr($dt, 10, 2);
        $sec = substr($dt, 12, 2);
        return ($yyyy . "/" . $mm . "/" . $dd . " " . $hh . ":" . $min . ":" . $sec);
    }
?>
