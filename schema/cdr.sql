-- ----------------------------------------------------------------------
--
-- CDR Data Tables
--
-- ----------------------------------------------------------------------

CREATE TABLE IF NOT EXISTS CDR_Types
(
	i_type		INT UNSIGNED NOT NULL,
	name		VARCHAR(255) NOT NULL
) TYPE=InnoDB;

TRUNCATE TABLE CDR_Types;
INSERT INTO CDR_Types VALUES
	(0, "UNKNOWN"),
	(1, "VERSION"),
	(2, "REBOOT"),
	(3, "SWITCHOVER"),
	(4, "START"),
	(5, "STOP"),
	(6, "ATTEMPT"),
	(7, "INTERMEDIATE");

DROP TABLE IF EXISTS CDR_Raw;
CREATE TABLE IF NOT EXISTS CDR_Raw
(
	i_reader	INT UNSIGNED NOT NULL, 
	i_node		INT UNSIGNED NOT NULL,
	i_node_type	INT UNSIGNED NOT NULL,
	i_cdr_type	INT UNSIGNED NOT NULL,	
	t_timestamp	TIMESTAMP,
	commited	TINYINT(1) NOT NULL DEFAULT 0,
	call_id		VARCHAR(64),
	cdr_time	DATETIME,
	cdr_offset	BIGINT UNSIGNED NOT NULL
) TYPE=InnoDB;

DROP TABLE IF EXISTS CDR_Mediated;
CREATE TABLE IF NOT EXISTS CDR_Mediated
(
	i_call		CHAR(16) NOT NULL,
	call_id		VARCHAR(64) NOT NULL,
	i_type_node	INT UNSIGNED NOT NULL,
	i_ingress	INT UNSIGNED NOT NULL,
	i_egress	INT UNSIGNED NOT NULL,
	start_time	DATETIME,
	stop_time	DATETIME,
	start_time_local DATETIME,
	stop_time_local	DATETIME,
	setup_seconds	INT UNSIGNED NOT NULL,
	elapsed_seconds	INT UNSIGNED NOT NULL,
	bytes_sent	INT UNSIGNED NOT NULL,
	bytes_rcvd	INT UNSIGNED NOT NULL,
	cli		VARCHAR(32) NOT NULL,
	cld		VARCHAR(32) NOT NULL,
	i_dest_source	INT UNSIGNED NOT NULL,
	i_dest_target	INT UNSIGNED NOT NULL,
	i_customer	INT UNSIGNED NOT NULL,
	-- maybe move these to a call attributes table?
	payphone	TINYINT(1) NOT NULL DEFAULT 0,
	prison		TINYINT(1) NOT NULL DEFAULT 0,
	billable	TINYINT(1) NOT NULL DEFAULT 0
) TYPE=InnoDB;

DROP TABLE IF EXISTS CDR_Source;
CREATE TABLE IF NOT EXISTS CDR_Source
(
	i_call		CHAR(16) NOT NULL,
	cdr		BLOB NOT NULL
) TYPE=InnoDB;
