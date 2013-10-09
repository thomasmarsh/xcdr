-- ----------------------------------------------------------------------
--
-- RATE TABLES
--
-- ----------------------------------------------------------------------

DROP TABLE IF EXISTS Rate;
CREATE TABLE IF NOT EXISTS Rate
(
	i_rate		INT UNSIGNED NOT NULL,
	i_tariff	INT UNSIGNED NOT NULL
);

DROP TABLE IF EXISTS RateProfile;
CREATE TABLE IF NOT EXISTS RateProfile
(
	i_rate_profile	INT UNSIGNED NOT NULL
);
