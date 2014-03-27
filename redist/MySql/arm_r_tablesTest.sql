SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='TRADITIONAL,ALLOW_INVALID_DATES';

CREATE SCHEMA IF NOT EXISTS `testStations` DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci ;
USE `testStations` ;

-- -----------------------------------------------------
-- Table `testStations`.`category`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `testStations`.`category` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `name` VARCHAR(255) NOT NULL,
  PRIMARY KEY (`id`),
  INDEX `catID1` (`id` ASC),
  UNIQUE INDEX `name_UNIQUE` (`name` ASC))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `testStations`.`signalType`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `testStations`.`signalType` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `name` VARCHAR(255) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE INDEX `name_UNIQUE` (`name` ASC))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `testStations`.`station`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `testStations`.`station` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `name` VARCHAR(255) NULL,
  `ip` TEXT NULL,
  PRIMARY KEY (`id`),
  INDEX `stationID` (`id` ASC),
  UNIQUE INDEX `name_UNIQUE` (`name` ASC))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `testStations`.`stationDevices`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `testStations`.`stationDevices` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `port` SMALLINT NOT NULL DEFAULT 1024,
  `stationID` INT NOT NULL,
  PRIMARY KEY (`id`),
  CONSTRAINT `stID_fk`
    FOREIGN KEY (`stationID`)
    REFERENCES `testStations`.`station` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `testStations`.`stationData`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `testStations`.`stationData` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `deviceID` INT NOT NULL,
  `categoryID` INT NULL,
  `frequency` DOUBLE NOT NULL,
  `bandwidth` DOUBLE NOT NULL,
  `signalTypeID` INT NULL,
  `datetime` DATETIME NOT NULL,
  PRIMARY KEY (`id`),
  INDEX `devIDsd` (`deviceID` ASC),
  INDEX `catsd` (`categoryID` ASC),
  INDEX `signalTypeID_fk_idx` (`signalTypeID` ASC),
  CONSTRAINT `category_fk`
    FOREIGN KEY (`categoryID`)
    REFERENCES `testStations`.`category` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `signalTypeID_fk`
    FOREIGN KEY (`signalTypeID`)
    REFERENCES `testStations`.`signalType` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `device_fk`
    FOREIGN KEY (`deviceID`)
    REFERENCES `testStations`.`stationDevices` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;
