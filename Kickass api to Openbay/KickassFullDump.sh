#!/bin/sh
MySQL_password=

#Downloading database using the kickass api
cd /root
rm -f dailydump.txt.gz
wget https://kat.cr/dailydump.txt.gz
gzip -fd dailydump.txt.gz
rm -f dailydump.txt.gz

#Import the txt file and convert it into the openbay format.
mysql --local-infile=1 --user=root --password=$MySQL_password << "EOF"

SET GLOBAL local_infile = 'ON';
CREATE DATABASE IF NOT EXISTS kickass
DEFAULT CHARACTER SET utf8
DEFAULT COLLATE utf8_general_ci;
USE kickass;

DROP TABLE IF EXISTS `torrents`;
CREATE TABLE `torrents` (
  `id` int(11) unsigned NOT NULL,
  `name` varchar(255) DEFAULT NULL,
  `description` text,
  `category_id` tinyint(4) DEFAULT '7',
  `size` bigint(20) unsigned DEFAULT NULL,
  `hash` varchar(40) NOT NULL,
  `files_count` int(11) DEFAULT '0',
  `created_at` datetime DEFAULT NULL,
  `torrent_status` tinyint(1) DEFAULT '0',
  `visible_status` tinyint(1) DEFAULT '1',
  `downloads_count` mediumint(8) unsigned NOT NULL DEFAULT '0' COMMENT 'umax = 16777215',
  `scrape_date` datetime DEFAULT NULL,
  `seeders` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `leechers` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `tags` varchar(500) DEFAULT NULL,
  `updated_at` datetime DEFAULT NULL
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

ALTER TABLE `torrents`
  ADD PRIMARY KEY (`id`);
ALTER TABLE `torrents`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

LOAD DATA LOCAL INFILE '/root/dailydump.txt'
INTO TABLE torrents
FIELDS TERMINATED BY '|'
LINES TERMINATED BY '\n'
(hash,name,tags,@dummy,@dummy,size,@dummy,files_count,seeders,leechers,@upload_date,torrent_status)
set created_at = FROM_UNIXTIME(@upload_date);

DELETE FROM `torrents` WHERE LENGTH(`hash`)<>40;
DELETE FROM `torrents` WHERE `name`='';
UPDATE `torrents` SET scrape_date=created_at;
UPDATE `torrents` SET description='' WHERE description IS NULL;
UPDATE `torrents` SET category_id=1 WHERE tags='Anime';
UPDATE `torrents` SET category_id=2 WHERE tags='Applications';
UPDATE `torrents` SET category_id=3 WHERE tags='Games';
UPDATE `torrents` SET category_id=4 WHERE tags='XXX';
UPDATE `torrents` SET category_id=5 WHERE tags='Movies';
UPDATE `torrents` SET category_id=6 WHERE tags='Music';
UPDATE `torrents` SET category_id=8 WHERE tags='TV';
UPDATE `torrents` SET category_id=9 WHERE tags='Books';
ALTER IGNORE TABLE `torrents` ADD UNIQUE (`hash`);
quit
EOF

rm -f dailydump.txt
echo done
