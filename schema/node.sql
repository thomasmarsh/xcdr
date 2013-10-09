-- ----------------------------------------------------------------------
--
-- NODES
--
-- ----------------------------------------------------------------------

DROP TABLE IF EXISTS NodeTypes;
CREATE TABLE IF NOT EXISTS NodeTypes
(
	i_type		INT UNSIGNED NOT NULL,
	i_name		VARCHAR(64)
) TYPE=InnoDB;

INSERT INTO NodeTypes VALUES
	(0, "UNKNOWN"),
	(1, "Sonus GSX"),
	(2, "Lucent MVAM TNT"),
	(3, "Siemens DCO"),
	(4, "Cisco"),
	(5, "Quintum");

DROP TABLE IF EXISTS Nodes;
CREATE TABLE IF NOT EXISTS Nodes
(
	i_node		INT UNSIGNED NOT NULL,
	i_type		INT UNSIGNED NOT NULL,
	identifier	VARCHAR(64),
	PRIMARY KEY NodeKey(i_node)
) TYPE=InnoDB;

INSERT INTO Nodes VALUES
	(0, 0, "UNKNOWN"),
	(1, 1, "PHAETON"),
	(2, 1, "rambler");
