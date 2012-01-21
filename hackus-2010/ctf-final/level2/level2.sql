DROP DATABASE IF EXISTS level2;
CREATE DATABASE IF NOT EXISTS level2;

CONNECT level2;

CREATE TABLE code (
    id INT NOT NULL AUTO_INCREMENT,
    code VARCHAR(16) NOT NULL,
    path VARCHAR(100) NOT NULL,
    PRIMARY KEY (id)
);

GRANT SELECT ON level2.code TO 'alice'@'localhost' IDENTIFIED BY 'W0nd3rLand';

INSERT INTO code (id,code,path) VALUES (42,'IFoundCharlie!!!', '/var/cache/l2/sub/src');
