@echo off

mysql.exe -u%1 -p%2 --default-character-set=utf8 < %~dp0\stations.sql