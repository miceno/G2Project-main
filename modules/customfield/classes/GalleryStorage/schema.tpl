## mysql
# A_CustomFieldMap_1.0
ALTER TABLE DB_TABLE_PREFIXCustomFieldMap
  DROP INDEX DB_COLUMN_PREFIXitemId,
  ADD INDEX DB_TABLE_PREFIXCustomFieldMap_75985(DB_COLUMN_PREFIXitemId);

UPDATE DB_TABLE_PREFIXSchema
  SET DB_COLUMN_PREFIXmajor=1, DB_COLUMN_PREFIXminor=1
  WHERE DB_COLUMN_PREFIXname='CustomFieldMap' AND DB_COLUMN_PREFIXmajor=1 AND DB_COLUMN_PREFIXminor=0;

# CustomFieldMap
CREATE TABLE DB_TABLE_PREFIXCustomFieldMap(
 DB_COLUMN_PREFIXitemId int(11) NOT NULL,
 DB_COLUMN_PREFIXfield varchar(128) NOT NULL,
 DB_COLUMN_PREFIXvalue varchar(255),
 DB_COLUMN_PREFIXsetId int(11),
 DB_COLUMN_PREFIXsetType int(11),
 INDEX DB_TABLE_PREFIXCustomFieldMap_75985(DB_COLUMN_PREFIXitemId)
) DB_TABLE_TYPE
/*!40100 DEFAULT CHARACTER SET utf8 */;

INSERT INTO DB_TABLE_PREFIXSchema (
 DB_COLUMN_PREFIXname,
 DB_COLUMN_PREFIXmajor,
 DB_COLUMN_PREFIXminor
) VALUES('CustomFieldMap', 1, 1);

## postgres
# A_CustomFieldMap_1.0
DROP INDEX DB_TABLE_PREFIXCustomFieldMap_75985;

CREATE INDEX DB_TABLE_PREFIXCustomFieldMap_75985 ON DB_TABLE_PREFIXCustomFieldMap(DB_COLUMN_PREFIXitemId);

UPDATE DB_TABLE_PREFIXSchema
  SET DB_COLUMN_PREFIXmajor=1, DB_COLUMN_PREFIXminor=1
  WHERE DB_COLUMN_PREFIXname='CustomFieldMap' AND DB_COLUMN_PREFIXmajor=1 AND DB_COLUMN_PREFIXminor=0;

# CustomFieldMap
CREATE TABLE DB_TABLE_PREFIXCustomFieldMap(
 DB_COLUMN_PREFIXitemId INTEGER NOT NULL,
 DB_COLUMN_PREFIXfield VARCHAR(128) NOT NULL,
 DB_COLUMN_PREFIXvalue VARCHAR(255),
 DB_COLUMN_PREFIXsetId INTEGER,
 DB_COLUMN_PREFIXsetType INTEGER
);

CREATE INDEX DB_TABLE_PREFIXCustomFieldMap_75985 ON DB_TABLE_PREFIXCustomFieldMap(DB_COLUMN_PREFIXitemId);

INSERT INTO DB_TABLE_PREFIXSchema (
 DB_COLUMN_PREFIXname,
 DB_COLUMN_PREFIXmajor,
 DB_COLUMN_PREFIXminor
) VALUES('CustomFieldMap', 1, 1);

## oracle
# A_CustomFieldMap_1.0
  DROP INDEX DB_TABLE_PREFIXCustomFieldMap_75985;

CREATE INDEX DB_TABLE_PREFIXCustomFieldMap_75985 ON DB_TABLE_PREFIXCustomFieldMap(DB_COLUMN_PREFIXitemId);

UPDATE DB_TABLE_PREFIXSchema
  SET DB_COLUMN_PREFIXmajor=1, DB_COLUMN_PREFIXminor=1
  WHERE DB_COLUMN_PREFIXname='CustomFieldMap' AND DB_COLUMN_PREFIXmajor=1 AND DB_COLUMN_PREFIXminor=0;

# CustomFieldMap
CREATE TABLE DB_TABLE_PREFIXCustomFieldMap(
 DB_COLUMN_PREFIXitemId INTEGER NOT NULL,
 DB_COLUMN_PREFIXfield VARCHAR2(128) NOT NULL,
 DB_COLUMN_PREFIXvalue VARCHAR2(255),
 DB_COLUMN_PREFIXsetId INTEGER,
 DB_COLUMN_PREFIXsetType INTEGER
);

CREATE INDEX DB_TABLE_PREFIXCustomFieldMap_75985
   ON DB_TABLE_PREFIXCustomFieldMap(DB_COLUMN_PREFIXitemId);

INSERT INTO DB_TABLE_PREFIXSchema (
 DB_COLUMN_PREFIXname,
 DB_COLUMN_PREFIXmajor,
 DB_COLUMN_PREFIXminor
) VALUES('CustomFieldMap', 1, 1);

## db2
# A_CustomFieldMap_1.0
DROP INDEX DB_TABLE_PREFIXCusto65_75985;

CREATE INDEX DB_TABLE_PREFIXCusto65_75985 ON DB_TABLE_PREFIXCustomFieldMap(DB_COLUMN_PREFIXitemId);

UPDATE DB_TABLE_PREFIXSchema
  SET DB_COLUMN_PREFIXmajor=1, DB_COLUMN_PREFIXminor=1
  WHERE DB_COLUMN_PREFIXname='CustomFieldMap' AND DB_COLUMN_PREFIXmajor=1 AND DB_COLUMN_PREFIXminor=0;

# CustomFieldMap
CREATE TABLE DB_TABLE_PREFIXCustomFieldMap(
 DB_COLUMN_PREFIXitemId INTEGER NOT NULL,
 DB_COLUMN_PREFIXfield VARCHAR(128) NOT NULL,
 DB_COLUMN_PREFIXvalue VARCHAR(255),
 DB_COLUMN_PREFIXsetId INTEGER,
 DB_COLUMN_PREFIXsetType INTEGER
);

CREATE INDEX DB_TABLE_PREFIXCusto65_75985
   ON DB_TABLE_PREFIXCustomFieldMap(DB_COLUMN_PREFIXitemId);

INSERT INTO DB_TABLE_PREFIXSchema (
 DB_COLUMN_PREFIXname,
 DB_COLUMN_PREFIXmajor,
 DB_COLUMN_PREFIXminor
) VALUES('CustomFieldMap', 1, 1);

## mssql
# A_CustomFieldMap_1.0
DROP INDEX DB_TABLE_PREFIXCustomFieldMap.DB_TABLE_PREFIXCustomFieldMap_75985;

CREATE INDEX DB_TABLE_PREFIXCustomFieldMap_75985 ON DB_TABLE_PREFIXCustomFieldMap(DB_COLUMN_PREFIXitemId);

UPDATE DB_TABLE_PREFIXSchema
  SET DB_COLUMN_PREFIXmajor=1, DB_COLUMN_PREFIXminor=1
  WHERE DB_COLUMN_PREFIXname='CustomFieldMap' AND DB_COLUMN_PREFIXmajor=1 AND DB_COLUMN_PREFIXminor=0;

# CustomFieldMap
CREATE TABLE DB_TABLE_PREFIXCustomFieldMap(
 DB_COLUMN_PREFIXitemId INT NOT NULL,
 DB_COLUMN_PREFIXfield NVARCHAR(128) NOT NULL,
 DB_COLUMN_PREFIXvalue NVARCHAR(255) NULL,
 DB_COLUMN_PREFIXsetId INT NULL,
 DB_COLUMN_PREFIXsetType INT NULL
);

CREATE INDEX DB_TABLE_PREFIXCustomFieldMap_75985 ON DB_TABLE_PREFIXCustomFieldMap(DB_COLUMN_PREFIXitemId);

INSERT INTO DB_TABLE_PREFIXSchema (
 DB_COLUMN_PREFIXname,
 DB_COLUMN_PREFIXmajor,
 DB_COLUMN_PREFIXminor
) VALUES('CustomFieldMap', 1, 1);

## sqlite
# A_CustomFieldMap_1.0
DROP INDEX DB_TABLE_PREFIXCustomFieldMap_75985;

CREATE INDEX DB_TABLE_PREFIXCustomFieldMap_75985 ON DB_TABLE_PREFIXCustomFieldMap(DB_COLUMN_PREFIXitemId);

UPDATE DB_TABLE_PREFIXSchema
  SET DB_COLUMN_PREFIXmajor=1, DB_COLUMN_PREFIXminor=1
  WHERE DB_COLUMN_PREFIXname='CustomFieldMap' AND DB_COLUMN_PREFIXmajor=1 AND DB_COLUMN_PREFIXminor=0;

# CustomFieldMap
CREATE TABLE DB_TABLE_PREFIXCustomFieldMap(
 DB_COLUMN_PREFIXitemId INTEGER NOT NULL,
 DB_COLUMN_PREFIXfield TEXT NOT NULL,
 DB_COLUMN_PREFIXvalue TEXT,
 DB_COLUMN_PREFIXsetId INTEGER,
 DB_COLUMN_PREFIXsetType INTEGER
);

CREATE INDEX DB_TABLE_PREFIXCustomFieldMap_75985 ON DB_TABLE_PREFIXCustomFieldMap(DB_COLUMN_PREFIXitemId);

INSERT INTO DB_TABLE_PREFIXSchema (
 DB_COLUMN_PREFIXname,
 DB_COLUMN_PREFIXmajor,
 DB_COLUMN_PREFIXminor
) VALUES('CustomFieldMap', 1, 1);

