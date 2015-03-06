@echo off

mysql.exe -u%1 -p%2 --default-character-set=utf8 < %~dp0\arm_r_tablesTest.sql
mysql.exe -u%1 -p%2 --default-character-set=utf8 --database=testStations < %~dp0\stations.sql