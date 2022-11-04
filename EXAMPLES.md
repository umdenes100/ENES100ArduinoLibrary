# Initializing the Wifi Module
```arduino
#include "Enes100.h"
void setup() {
    Enes100.begin("It's lit", FIRE, 3, 8, 9);
    Enes100.println("Connected!");
}
```
