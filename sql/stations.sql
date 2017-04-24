-- --------------------------------------------------------
-- Хост:                         127.0.0.1
-- Версия сервера:               5.6.28-log - MySQL Community Server (GPL)
-- ОС Сервера:                   Win64
-- HeidiSQL Версия:              9.3.0.4984
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

-- Дамп структуры базы данных stations
CREATE DATABASE IF NOT EXISTS `stations` /*!40100 DEFAULT CHARACTER SET utf8 */;
USE `stations`;


-- Дамп структуры для таблица stations.category
CREATE TABLE IF NOT EXISTS `category` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(255) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `name_UNIQUE` (`name`),
  KEY `catID1` (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8;

-- Дамп данных таблицы stations.category: ~2 rows (приблизительно)
/*!40000 ALTER TABLE `category` DISABLE KEYS */;
INSERT INTO `category` (`id`, `name`) VALUES
	(2, 'Black'),
	(1, 'White');
/*!40000 ALTER TABLE `category` ENABLE KEYS */;


-- Дамп структуры для таблица stations.signaltype
CREATE TABLE IF NOT EXISTS `signaltype` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(255) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `name_UNIQUE` (`name`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;

-- Дамп данных таблицы stations.signaltype: ~0 rows (приблизительно)
/*!40000 ALTER TABLE `signaltype` DISABLE KEYS */;
INSERT INTO `signaltype` (`id`, `name`) VALUES
	(1, 'Unknown');
/*!40000 ALTER TABLE `signaltype` ENABLE KEYS */;


-- Дамп структуры для таблица stations.station
CREATE TABLE IF NOT EXISTS `station` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(255) DEFAULT NULL,
  `ip` text,
  PRIMARY KEY (`id`),
  UNIQUE KEY `name_UNIQUE` (`name`),
  KEY `stationID` (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=641 DEFAULT CHARSET=utf8;

-- Дамп данных таблицы stations.station: ~0 rows (приблизительно)
/*!40000 ALTER TABLE `station` DISABLE KEYS */;
/*!40000 ALTER TABLE `station` ENABLE KEYS */;


-- Дамп структуры для таблица stations.stationdata
CREATE TABLE IF NOT EXISTS `stationdata` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `deviceID` int(11) NOT NULL,
  `categoryID` int(11) DEFAULT NULL,
  `frequency` double NOT NULL,
  `bandwidth` double NOT NULL,
  `signalTypeID` int(11) DEFAULT NULL,
  `datetime` datetime NOT NULL,
  `comment` varchar(50) NOT NULL,
  `checked` tinyint(1) unsigned DEFAULT '1',
  PRIMARY KEY (`id`),
  KEY `devIDsd` (`deviceID`),
  KEY `catsd` (`categoryID`),
  KEY `signalTypeID_fk_idx` (`signalTypeID`),
  CONSTRAINT `category_fk` FOREIGN KEY (`categoryID`) REFERENCES `category` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `device_fk` FOREIGN KEY (`deviceID`) REFERENCES `stationdevices` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `signalTypeID_fk` FOREIGN KEY (`signalTypeID`) REFERENCES `signaltype` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Дамп данных таблицы stations.stationdata: ~0 rows (приблизительно)
/*!40000 ALTER TABLE `stationdata` DISABLE KEYS */;
/*!40000 ALTER TABLE `stationdata` ENABLE KEYS */;


-- Дамп структуры для таблица stations.stationdevices
CREATE TABLE IF NOT EXISTS `stationdevices` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `port` smallint(6) NOT NULL DEFAULT '1024',
  `stationID` int(11) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `stID_fk` (`stationID`),
  CONSTRAINT `stID_fk` FOREIGN KEY (`stationID`) REFERENCES `station` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Дамп данных таблицы stations.stationdevices: ~0 rows (приблизительно)
/*!40000 ALTER TABLE `stationdevices` DISABLE KEYS */;
/*!40000 ALTER TABLE `stationdevices` ENABLE KEYS */;


-- Дамп структуры для таблица stations.threshold
CREATE TABLE IF NOT EXISTS `threshold` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `frequency` int(11) DEFAULT NULL,
  `threshold` int(11) DEFAULT NULL,
  `station` varchar(255) DEFAULT NULL,
  UNIQUE KEY `station` (`station`,`frequency`),
  KEY `id` (`id`),
  CONSTRAINT `FK__station` FOREIGN KEY (`station`) REFERENCES `station` (`name`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8;

-- Дамп данных таблицы stations.threshold: ~0 rows (приблизительно)
/*!40000 ALTER TABLE `threshold` DISABLE KEYS */;
/*!40000 ALTER TABLE `threshold` ENABLE KEYS */;
/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
