#!/bin/sh
MySQL_username='root'
MySQL_password=''
kickass_userhash=''

#Downloading update database using the kickass api. Pleas register and send a request to get api access.
cd /root
rm -f hourlydump.txt.gz
wget -O 'hourlydump.txt.gz' --post-data "userhash=$kickass_userhash" 'https://kat.cr/api/get_dump/hourly/'
gzip -fd hourlydump.txt.gz
rm -f hourlydump.txt.gz

#Import the txt update file into a temporary database and convert this into the openbay format.
mysql --local-infile=1 --user=$MySQL_username --password=$MySQL_password << "EOF"

SET GLOBAL local_infile = 'ON';
CREATE DATABASE IF NOT EXISTS kickass
DEFAULT CHARACTER SET utf8
DEFAULT COLLATE utf8_general_ci;
USE kickass;

DROP TABLE IF EXISTS `update`;
CREATE TABLE IF NOT EXISTS `update` (
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

ALTER TABLE `update`
  ADD PRIMARY KEY (`id`);
ALTER TABLE `update`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

LOAD DATA LOCAL INFILE '/root/hourlydump.txt'
INTO TABLE `update`
FIELDS TERMINATED BY '|'
LINES TERMINATED BY '\n'
(hash,name,tags,@dummy,@dummy,size,@dummy,files_count,seeders,leechers,@upload_date,torrent_status)
set created_at = FROM_UNIXTIME(@upload_date);

DELETE FROM `update` WHERE LENGTH(`hash`)<>40;
DELETE FROM `update` WHERE `name`='';
UPDATE `update` SET scrape_date=created_at;
UPDATE `update` SET description='' WHERE description IS NULL;
UPDATE `update` SET category_id=1 WHERE tags='Anime';
UPDATE `update` SET category_id=2 WHERE tags='Applications';
UPDATE `update` SET category_id=3 WHERE tags='Games';
UPDATE `update` SET category_id=4 WHERE tags='XXX';
UPDATE `update` SET category_id=5 WHERE tags='Movies';
UPDATE `update` SET category_id=6 WHERE tags='Music';
UPDATE `update` SET category_id=8 WHERE tags='TV';
UPDATE `update` SET category_id=9 WHERE tags='Books';

ALTER IGNORE TABLE `update` ADD UNIQUE (`hash`);

#Copy the temporary into the main database and ignore dublicates
INSERT IGNORE INTO `torrents` (`name`, `description`, `category_id`, `size`, `hash`, `files_count`, `created_at`, `torrent_status`, `visible_status`, `downloads_count`, `scrape_date`, `seeders`, `leechers`, `tags`, `updated_at`)
SELECT `name`, `description`, `category_id`, `size`, `hash`, `files_count`, `created_at`, `torrent_status`, `visible_status`, `downloads_count`, `scrape_date`, `seeders`, `leechers`, `tags`, `updated_at`
   FROM `update`;
DROP TABLE IF EXISTS `update`;
quit
EOF

rm -f dailydump.txt
echo done
