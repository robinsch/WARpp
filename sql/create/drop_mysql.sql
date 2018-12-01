REVOKE ALL PRIVILEGES ON * . * FROM 'war'@'localhost';

REVOKE ALL PRIVILEGES ON `auth` . * FROM 'war'@'localhost';

REVOKE GRANT OPTION ON `auth` . * FROM 'war'@'localhost';

DROP USER 'war'@'localhost';

DROP DATABASE IF EXISTS `auth`;
