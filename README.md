# Porte de Garage MQTT
![porte](https://github.com/iooner/Garage-Door/blob/master/fonctionnement.jpg)  
  

Deux [reed switch (ou door switch)](https://www.sparkfun.com/products/13247) sont en début et en fin de course du bras d'entrainement de la porte.  
Les deux switch sont lus en pullup par l'arduino et publie l'état de la porte. Ouvert/Fermé/En mouvement/Erreur sur le broker MQTT.  
Il est évidement possible de réduire le montage avec un ESP8266 par exemple.
