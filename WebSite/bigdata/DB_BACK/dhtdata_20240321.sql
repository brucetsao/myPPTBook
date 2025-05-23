-- phpMyAdmin SQL Dump
-- version 5.2.1
-- https://www.phpmyadmin.net/
--
-- 主機： 127.0.0.1
-- 產生時間： 2024-03-23 02:57:39
-- 伺服器版本： 10.4.28-MariaDB
-- PHP 版本： 8.2.4

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- 資料庫： `big`
--

-- --------------------------------------------------------

--
-- 資料表結構 `dhtdata`
--

CREATE TABLE `dhtdata` (
  `id` int(11) NOT NULL COMMENT '主鍵',
  `MAC` char(12) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL COMMENT '裝置MAC值',
  `crtdatetime` timestamp NOT NULL DEFAULT current_timestamp() ON UPDATE current_timestamp() COMMENT '資料輸入時間',
  `temperature` float NOT NULL COMMENT '溫度值',
  `humidity` float NOT NULL COMMENT '濕度值',
  `systime` char(14) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL COMMENT '使用者更新時間'
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='溫溼度感測器資料';

--
-- 傾印資料表的資料 `dhtdata`
--

INSERT INTO `dhtdata` (`id`, `MAC`, `crtdatetime`, `temperature`, `humidity`, `systime`) VALUES
(1, 'AABBCCDDEEFF', '2024-03-18 08:55:20', 25.6, 88.2, '20240318165101'),
(2, 'AABBCCDDEEFF', '2024-03-18 08:57:28', 25.6, 88.2, '20240318165101'),
(3, 'AABBCCDDEEFF', '2024-03-18 08:57:44', 25.6, 88.2, '20240318165101'),
(4, 'AABBCCDDEEFF', '2024-03-18 08:57:44', 25.6, 88.2, '20240318165101'),
(5, 'AABBCCDDEEFF', '2024-03-18 08:57:44', 25.6, 88.2, '20240318165101'),
(6, 'AABBCCDDEEFF', '2024-03-18 08:57:44', 25.6, 88.2, '20240318165101'),
(7, 'AABBCCDDEEFF', '2024-03-18 08:57:44', 25.6, 88.2, '20240318165101'),
(8, 'AABBCCDDEEFF', '2024-03-18 08:57:44', 25.6, 88.2, '20240318165101'),
(9, 'AABBCCDDEEFF', '2024-03-18 08:57:44', 25.6, 88.2, '20240318165101'),
(10, 'AABBCCDDEEFF', '2024-03-18 08:57:44', 25.6, 88.2, '20240318165101'),
(11, 'AABBCCDDEEFF', '2024-03-18 08:57:44', 25.6, 88.2, '20240318165101'),
(12, 'AABBCCDDEEFF', '2024-03-18 08:57:44', 25.6, 88.2, '20240318165101'),
(13, 'AABBCCDDEEFF', '2024-03-18 08:57:44', 25.6, 88.2, '20240318165101'),
(14, 'AABBCCDDEEFF', '2024-03-18 08:57:44', 25.6, 88.2, '20240318165101');

--
-- 已傾印資料表的索引
--

--
-- 資料表索引 `dhtdata`
--
ALTER TABLE `dhtdata`
  ADD PRIMARY KEY (`id`),
  ADD KEY `mac` (`MAC`,`systime`);

--
-- 在傾印的資料表使用自動遞增(AUTO_INCREMENT)
--

--
-- 使用資料表自動遞增(AUTO_INCREMENT) `dhtdata`
--
ALTER TABLE `dhtdata`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '主鍵', AUTO_INCREMENT=15;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
