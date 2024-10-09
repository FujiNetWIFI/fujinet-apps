10 CLS 0
15 FOR I=0 TO 15:? @ 256+15-I, " ";:? @ 256+16+I, " ";:NEXT 
20 PRINT @ 256, "        LOADING LOBBY...        ";
25 REM PRINT @ 234, "loading";:PRINT @ 242, "lobby";
30 LOADM"LOBBY":EXEC
