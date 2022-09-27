
<h1 style="text-align:center"><a style="color:red">R</a> <a style="color:green">G</a> <a style="color:blue">B </a> <a style="color:white">- LED Strip Control </a></h1>


## __With__
- [Arduino Nano](https://store.arduino.cc/products/arduino-nano)

<img src="https://cdn.shopify.com/s/files/1/0438/4735/2471/products/A000005_01.iso_643x483.jpg?v=1628695103" alt="NANO" width="200"/>

__and__

- [TOSHIBA ULN2803APG](https://www.tme.eu/Document/5d0c0d4b35a4dbdbe085a04733be1a83/ULN2803APG.pdf)

__and__
- one push button
- one potentiometer

__and__
- 12V 1A power supply

---

### Menu States

0. Off *(after turn on)*

1. ```analogRead(potPin)``` and set LEDs brightness simultaneously

2. ```blinkDelay``` is 100mS (blink without delays)

3. ```analogRead(potPin)``` increase or decrease delay (blink without delays)

4. The LEDs flash one after the other ```delay(250)```

5. Police Flash light (RED RED BLUE BLUE) ```delay(100)```

6. The LEDs flash one after the other, ```analogRead(potPin)``` and set the flash ```delay(brightness * 2)```


#### Version

- v0.31

---

2022-09-27
