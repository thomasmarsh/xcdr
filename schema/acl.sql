-- --------------------------------------------------------------------------
--
-- ACL TABLES
--
-- --------------------------------------------------------------------------

-- --------------------------------------------------------------------------
--
-- Users
--
-- --------------------------------------------------------------------------

DROP TABLE IF EXISTS User;
CREATE TABLE IF NOT EXISTS User
(
	i_user		INT UNSIGNED NOT NULL,
	i_acl_proto	INT UNSIGNED NOT NULL
);

INSERT INTO User VALUES
	(0, 0);

DROP TABLE IF EXISTS Login;
CREATE TABLE IF NOT EXISTS Login
(
	i_user		INT UNSIGNED NOT NULL,
	login		VARCHAR(32) NOT NULL,
	password	VARCHAR(32) NOT NULL,
	email		VARCHAR(128) NOT NULL,
	name		VARCHAR(128) NOT NULL,
	description	VARCHAR(32) NOT NULL
);
INSERT INTO Login VALUES
	(0, 'root', encrypt('letmein'), 'thomas.marsh@seznam.cz', 'Thomas D. Marsh', 'ROOT Account');


-- --------------------------------------------------------------------------
--
-- Groups
--
-- --------------------------------------------------------------------------

DROP TABLE IF EXISTS UserGroup;
CREATE TABLE IF NOT EXISTS UserGroup
(
	i_group		INT UNSIGNED NOT NULL,
	i_acl_proto	INT UNSIGNED NOT NULL,
	name		VARCHAR(32) NOT NULL
);
INSERT INTO UserGroup VALUES
	(0, 0, 'root');

-- --------------------------------------------------------------------------
--
-- Privileges and ACLs
--
-- --------------------------------------------------------------------------

DROP TABLE IF EXISTS Privilege;
CREATE TABLE IF NOT EXISTS Privilege
(
	i_privilege	INT UNSIGNED NOT NULL,
	description	VARCHAR(64)
);
INSERT INTO Privilege VALUES
	(0, 'read'),
	(1, 'lookup'),
	(2, 'insert'),
	(3, 'delete'),
	(4, 'write'),
	(5, 'lock'),
	(6, 'acl');

DROP TABLE IF EXISTS ACL;
CREATE TABLE IF NOT EXISTS ACL
(
	i_acl		INT UNSIGNED NOT NULL,
	i_user		INT UNSIGNED,
	i_group		INT UNSIGNED,
	i_privilege	INT UNSIGNED NOT NULL
);
INSERT INTO ACL VALUES
	(0, 0, 0, NULL),
	(0, 1, 0, NULL),
	(0, 2, 0, NULL),
	(0, 3, 0, NULL),
	(0, 4, 0, NULL),
	(0, 5, 0, NULL),
	(0, 0, NULL, 0),
	(0, 1, NULL, 0),
	(0, 2, NULL, 0),
	(0, 3, NULL, 0),
	(0, 4, NULL, 0),
	(0, 5, NULL, 0);
