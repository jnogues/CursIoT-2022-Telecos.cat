## Blynk HTTP RestFul API

### Llegir l'estat d'un pin Digital

``` 
http://5.196.88.155:8080/xxxxTOKENxxxx/get/D12
```

### Escriure un pin digital

``` 
http://5.196.88.155:8080/xxxxTOKENxxxx/update/D12?value=1
```

### Llegir l'estat d'un pin Virtual

Per exemple per llegir A0 si cada cert temps fem un enviament del valor al pin virtual V1

``` 
http://5.196.88.155:8080/xxxxTOKENxxxx/get/V1
```

### Saber si el hardware o l'app estan connectades

Pel hardware:
```
http://5.196.88.155:8080/xxxxTOKENxxxx/isHardwareConnected
```

Per l'app:
```
http://5.196.88.155:8080/xxxxTOKENxxxx/isAppConnected
```
