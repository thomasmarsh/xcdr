-- ----------------------------------------------------------------------
--
-- CUSTOMER TABLES
--
-- ----------------------------------------------------------------------

DROP TABLE IF EXISTS Customers;
CREATE TABLE IF NOT EXISTS Customers
(
	i_customer	INT UNSIGNED NOT NULL,
	name		VARCHAR(41) NOT NULL DEFAULT ''
) TYPE=InnoDB;

INSERT INTO Customers VALUES
	(0, 'Default Customer');
