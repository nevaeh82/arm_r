@echo off

mysql.exe -u%1 -p%2 --default-character-set=utf8 < %~dp0\arm_r_tables.sql
mysql.exe -u%1 -p%2 --default-character-set=utf8 --database=stations < %~dp0\stations.sql