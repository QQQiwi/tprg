### Примеры запуска генераторов

#### Линейный конгруэнтный генератор

`gen.exe /g:lc /i:1223,7,11,3 /n:1000000`

#### Аддитивный генератор

`gen.exe /g:add /i:255,4,3,2,6,7,12,33,41 /f:add_method.dat`

#### Пятипараметрический генератор

`gen.exe /g:5p /i:4,1,2,13,133`

#### РСЛОС

`gen.exe /g:lfsr /i:00000000000000000000000000000001,00000000000000000000000000000011`

#### Нелинейная комбинация РСЛОС

`/g:nfsr /i:00000000000000000000000000000001,00000000000000000000000000000001,00000000000000000000000000100001,32,3,4,7`

#### Вихрь Мерсенна

`/g:mt /i:2193,1231`

#### RC4

`/g:rc4 /i:1,2,3,4,5,6,7,8,1,2,3,4,5,6,7,8,1,2,3,4,5,6,7,8,1,2,3,4,5,6,7,8,1,2,3,4,5,6,7,8,1,2,3,4,5,6,7,8,1,2,3,4,5,6,7,8,1,2,3,4,5,6,7,8,1,2,3,4,5,6,7,8,1,2,3,4,5,6,7,8,1,2,3,4,5,6,7,8,1,2,3,4,5,6,7,8,1,2,3,4,5,6,7,8,1,2,3,4,5,6,7,8,1,2,3,4,5,6,7,8,1,2,3,4,5,6,7,8,1,2,3,4,5,6,7,8,1,2,3,4,5,6,7,8,1,2,3,4,5,6,7,8,1,2,3,4,5,6,7,8,1,2,3,4,5,6,7,8,1,2,3,4,5,6,7,8,1,2,3,4,5,6,7,8,1,2,3,4,5,6,7,8,1,2,3,4,5,6,7,8,1,2,3,4,5,6,7,8,1,2,3,4,5,6,7,8,1,2,3,4,5,6,7,8,1,2,3,4,5,6,7,8,1,2,3,4,5,6,7,8,1,2,3,4,5,6,7,8,1,2,3,4,5,6,7,8`

#### RSA

`/g:rsa /i:39203,1024,10,12395`

#### Алгоритм Блюма-Блюма-Шуба

`/g:bbs /i:3229`