DROP TABLE IF EXISTS test_flags;
CREATE TABLE IF NOT EXISTS test_flags
(
	destination	VARCHAR(15) NOT NULL,
	i_flag		INT UNSIGNED NOT NULL,
	value		VARCHAR(10) NOT NULL,

	effective_start	DATETIME NOT NULL,
	effective_end	DATETIME NOT NULL
) TYPE=InnoDB;

SELECT i_flag, value FROM test_flags WHERE 
