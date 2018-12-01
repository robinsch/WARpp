GRANT USAGE ON * . * TO 'war'@'war' IDENTIFIED BY 'war' WITH MAX_QUERIES_PER_HOUR 0 MAX_CONNECTIONS_PER_HOUR 0 MAX_UPDATES_PER_HOUR 0 ;

CREATE DATABASE `auth` DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;

GRANT ALL PRIVILEGES ON `auth` . * TO 'trinity'@'localhost' WITH GRANT OPTION;