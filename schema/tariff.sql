DROP TABLE IF EXISTS Tariffs;
CREATE TABLE IF NOT EXISTS Tariffs
(
	i_tariff	INT UNSIGNED NOT NULL,
	name		VARCHAR(32) NOT NULL DEFAULT ''
	-- add more here
);

INSERT INTO Tariffs VALUES
	(0, 'Default Tariff');
