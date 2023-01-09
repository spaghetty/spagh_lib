#the challenge

Initial code to decode:
```
/Td6WFoAAATm1rRGAgAhARYAAAB0L+Wj4Af/AMtdADaZSt/rpVgf8rRS9PomyHJinbmsGDtb
N+3q56JJ+z03x/rzeZh6QSidwyOFQUolCd+0VuXVP6f2HZd2Ay2M7JWLOcGn3uDfHNdMtE4/
Bkyld1+ot69ogMbv1eY7i9Y2PqallTXt4vbc6x1+bzN562uFkeulw0VIzFkt6PZ5f1IIBvVo
H1vYz2ciSySyX8QAWxOg3ejJYH543Wqbqo+AD9tZ8DLlhSGPIIpZRGOzTfOUVNf629q7nV7Y
FMNIJ/UqYeMLRIBJonKJd+0AAACKiaxHZps2BAAB5wGAEAAAHMMLBLHEZ/sCAAAAAARZWg==
```
here the solution:
"  base64 -Dd -i <code> | gunzip "

answare is :
```
message.txt000644 001752 001752 00000000171 14330676613 015777 0ustar00jurgen.hartjurgen.hart000000 000000 Hello,

It's been a while so here we are again.
But this time it all starts here -> https://discord.gg/rzGBTxsVQT

Enjoy
```

the mached list : "https://puzzler.mups.eu/"

the list is: " curl https://puzzler.mups.eu/list"

first challenge hello:

```
You did not tell me your name, so how can I greet you ?
```
`curl -H "MYNAME: spaghetty"  "https://puzzler.mups.eu/hello"`

`Hello there spaghetty , can you keep me posted with CODE 16cf08c5fdd58133f7a3f52cb3d67056 `

`curl -H "MYNAME: spaghetty"  -H "CODE:16cf08c5fdd58133f7a3f52cb3d67056"  -d "CODE=16cf08c5fdd58133f7a3f52cb3d67056" -v  "https://puzzler.mups.eu/hello"`


secondo party:

`curl -H "CODE:16cf08c5fdd58133f7a3f52cb3d67056" -H "NAME:spaghetty"  -d "CODE=16cf08c5fdd58133f7a3f52cb3d67056" -v  "https://puzzler.mups.eu/party/access"`

third chain:



bdc6gwx6fnj!zua7GEK