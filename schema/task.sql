-- ----------------------------------------------------------------------
--
-- Task List
--
-- ----------------------------------------------------------------------

CREATE TABLE IF NOT EXISTS TaskList
(
	i_task		INT UNSIGNED NOT NULL PRIMARY KEY,
	interface	INT UNSIGNED NOT NULL,
	command		VARCHAR(255) NOT NULL DEFAULT "noop"
) TYPE=InnoDB;

CREATE TABLE IF NOT EXISTS TaskArguments
(
	i_task		INT UNSIGNED NOT NULL,
	attr		VARCHAR(255) NOT NULL DEFAULT "attr",
	value		VARCHAR(255) NOT NULL DEFAULT "value"
) TYPE=InnoDB;
