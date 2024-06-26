import unicodedata
from unidecode import unidecode

table = []

for i in range(192,576):
    s=unichr(i)
    #n=unicodedata.normalize('NFKD', s).encode('ascii', 'ignore').decode("utf-8")
    n = unidecode(s)
    if len(n)==2:
        n = n[0]
    if len(n)==0:
        n=' ';
    #print i,s,n
    table.append(ord(n));

with open('utf2atascii.bin', 'wb') as file:
    file.write(bytearray(table))
