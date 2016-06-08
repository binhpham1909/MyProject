-- =============================================================================
-- Diagram Name: database
-- Created on: 5/19/2016 11:03:01 PM
-- Diagram Version: 
-- =============================================================================


CREATE DATABASE IF NOT EXISTS `pkn` 
CHARACTER SET utf8 
COLLATE utf8_general_ci;

USE `pkn`;

SET FOREIGN_KEY_CHECKS=0;

CREATE TABLE `pkn_manufactor` (
  `manufactorID` int(11) NOT NULL,
  `manufactorName` varchar(100),
  PRIMARY KEY(`manufactorID`)
)
ENGINE=INNODB;

CREATE TABLE `pkn_unit` (
  `unitID` int(11) NOT NULL,
  `unitSymbol` varchar(50),
  `unitName` varchar(50),
  `unitFactor` float(32,12),
  PRIMARY KEY(`unitID`)
)
ENGINE=INNODB;

CREATE TABLE `pkn_conditionStore` (
  `conditionStoreID` int(11) NOT NULL,
  `conditionStoreDescription` varchar(100),
  PRIMARY KEY(`conditionStoreID`)
)
ENGINE=INNODB;

CREATE TABLE `pkn_certificate` (
  `certificateID` int(11) NOT NULL,
  `certificateLink` int(11),
  PRIMARY KEY(`certificateID`)
)
ENGINE=INNODB;

CREATE TABLE `pkn_user` (
  `userID` int(11) NOT NULL,
  `userName` varchar(50),
  `userFullName` varchar(50),
  `userPassword` varchar(50),
  PRIMARY KEY(`userID`)
)
ENGINE=INNODB;

CREATE TABLE `pkn_provider` (
  `providerID` int(11) NOT NULL,
  `providerName` int(11),
  PRIMARY KEY(`providerID`)
)
ENGINE=INNODB;

CREATE TABLE `pkn_chemical` (
  `chemicalID` int(11) NOT NULL,
  PRIMARY KEY(`chemicalID`)
)
ENGINE=INNODB;

CREATE TABLE `pkn_chemicalName` (
  `chemicalNameID` int(11) NOT NULL,
  `chemicalID` int(11) NOT NULL DEFAULT '0',
  `chemicalNameDefault` bit(1),
  PRIMARY KEY(`chemicalNameID`),
  CONSTRAINT `chemical name list` FOREIGN KEY (`chemicalID`)
    REFERENCES `pkn_chemical`(`chemicalID`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION
)
ENGINE=INNODB;

CREATE TABLE `pkn_chemicalInfo` (
  `chemicalInfoID` int(11) NOT NULL,
  `chemicalID` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY(`chemicalInfoID`),
  CONSTRAINT `chemical properties` FOREIGN KEY (`chemicalID`)
    REFERENCES `pkn_chemical`(`chemicalID`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION
)
ENGINE=INNODB;

CREATE TABLE `pkn_materialCode` (
  `materialCodeID` int(11) NOT NULL,
  `materialCodeCode` varchar(50),
  `manufactorID` int(11) NOT NULL DEFAULT '0',
  `materialCodeLink` varchar(100),
  PRIMARY KEY(`materialCodeID`),
  CONSTRAINT `Manufactor` FOREIGN KEY (`manufactorID`)
    REFERENCES `pkn_manufactor`(`manufactorID`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION
)
ENGINE=INNODB;

CREATE TABLE `pkn_chemicalMaterial` (
  `chemicalMaterialID` int(11) NOT NULL,
  `chemicalID` int(11) NOT NULL DEFAULT '0',
  `conditionStoreID` int(11) NOT NULL DEFAULT '0',
  `providerID` int(11) NOT NULL DEFAULT '0',
  `materialCodeID` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY(`chemicalMaterialID`),
  CONSTRAINT `chemical` FOREIGN KEY (`chemicalID`)
    REFERENCES `pkn_chemical`(`chemicalID`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `conditional store` FOREIGN KEY (`conditionStoreID`)
    REFERENCES `pkn_conditionStore`(`conditionStoreID`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `provider id` FOREIGN KEY (`providerID`)
    REFERENCES `pkn_provider`(`providerID`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `code PO` FOREIGN KEY (`materialCodeID`)
    REFERENCES `pkn_materialCode`(`materialCodeID`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION
)
ENGINE=INNODB;

CREATE TABLE `pkn_chemicalImport` (
  `chemicalImportID` int(11) NOT NULL,
  `chemicalMaterialID` int(11) NOT NULL DEFAULT '0',
  `chemicalImportWeight` float(9,3),
  `unitID` int(11) NOT NULL DEFAULT '0',
  `userImportID` int(11) NOT NULL DEFAULT '0',
  `certificateID` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY(`chemicalImportID`),
  CONSTRAINT `chemical material import` FOREIGN KEY (`chemicalMaterialID`)
    REFERENCES `pkn_chemicalMaterial`(`chemicalMaterialID`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `import unit` FOREIGN KEY (`unitID`)
    REFERENCES `pkn_unit`(`unitID`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `user Import` FOREIGN KEY (`userImportID`)
    REFERENCES `pkn_user`(`userID`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `chemical certificate` FOREIGN KEY (`certificateID`)
    REFERENCES `pkn_certificate`(`certificateID`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION
)
ENGINE=INNODB;

CREATE TABLE `pkn_chemicalExport` (
  `chemicalExportID` int(11) NOT NULL,
  `chemicalMaterialID` int(11) NOT NULL DEFAULT '0',
  `chemicalExportWeight` float(9,3),
  `unitID` int(11) NOT NULL DEFAULT '0',
  `userExportID` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY(`chemicalExportID`),
  CONSTRAINT `chemical material export` FOREIGN KEY (`chemicalMaterialID`)
    REFERENCES `pkn_chemicalMaterial`(`chemicalMaterialID`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `Export unit` FOREIGN KEY (`unitID`)
    REFERENCES `pkn_unit`(`unitID`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `user Export` FOREIGN KEY (`userExportID`)
    REFERENCES `pkn_user`(`userID`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION
)
ENGINE=INNODB;

SET FOREIGN_KEY_CHECKS=1;
