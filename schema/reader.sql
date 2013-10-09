-- ----------------------------------------------------------------------
--
-- READER TABLES
--
-- ----------------------------------------------------------------------

DROP TABLE IF EXISTS ReaderProfile;
CREATE TABLE IF NOT EXISTS ReaderProfile
(
	i_reader	 INT UNSIGNED NOT NULL PRIMARY KEY,
	i_dir		 INT UNSIGNED NOT NULL,
	maxLinesPerPoll  INT UNSIGNED NOT NULL DEFAULT 1000,
	readMaxTime	 INT UNSIGNED NOT NULL DEFAULT 2,
	refreshFrequency INT UNSIGNED NOT NULL DEFAULT 10,
	commitFrequency  INT UNSIGNED NOT NULL DEFAULT 30,
	pollFrequency	 INT UNSIGNED NOT NULL DEFAULT 10,
	depositFrequency INT UNSIGNED NOT NULL DEFAULT 10
) TYPE=InnoDB;

TRUNCATE TABLE ReaderProfile;
INSERT INTO ReaderProfile VALUES
	(0, 0, 1000, 2, 10, 3, 3, 3);

DROP TABLE IF EXISTS ReaderDirectories;
CREATE TABLE IF NOT EXISTS ReaderDirectories
(
	i_dir		INT UNSIGNED NOT NULL PRIMARY KEY,
	path		VARCHAR(255) NOT NULL
) TYPE=InnoDB;

TRUNCATE TABLE ReaderDirectories;
INSERT INTO ReaderDirectories VALUES
	(0, "/tmp/cdr");

CREATE TABLE IF NOT EXISTS ReaderLog
(
	i_reader	INT UNSIGNED NOT NULL,
	i_dir		INT UNSIGNED NOT NULL,
	filename	VARCHAR(255) NOT NULL,
	last_offset	BIGINT UNSIGNED NOT NULL,
	completed	TINYINT(1) NOT NULL DEFAULT 0
) TYPE=InnoDB;
