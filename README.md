# Embedded
A Library for µC's.
Currently only for STM32fx Processors and based on STM-HAL-Library!
The examples are all tested with STM32f4 discovery board

---

## DS_Delay
### defines
```c++
#define F_CPU 		168000000UL
#define F_CPU_MHZ 	(F_CPU/1000000UL)
```

### functions
```c++
void DS_Delay_us(uint32_t time);
void DS_Delay_init(void);
```

### infos
It' neccessary to call init function 1st of all


### example
```c++
/* any includes */
#include "DS_Delay.h"
/* any startup code */
DS_Delay_init();
/* any code */
DS_Delay_us(500); // wait 500µs
/* any code*/
```

---

## One Wire
### defines
```c++
#define ONEWIRE_CMD_READROM			0x33
#define ONEWIRE_CMD_SKIPROM			0xcc

#ifndef ONEWIRE_DATA_Pin
 #define ONEWIRE_DATA_Pin			GPIO_PIN_1
#endif

#ifndef ONEWIRE_DATA_GPIO_Port
 #define ONEWIRE_DATA_GPIO_Port		GPIOA
#endif
```

### functions
```c++
OneWire();
OneWire(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
virtual ~OneWire();
void Init();
uint8_t ReadSlaveInfos(uint64_t *id, uint8_t *familyCode); // not implemented yet
OneWire_ErrorState ReadScratchpad(uint8_t *scratch);
void InitPresencePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void InitErrorPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
```

### properties
```c++
// ports/pins
uint32_t DataPin;   // DQ Pin
GPIO_TypeDef *DataPort; // DQ Port
uint32_t PresencePin;   // is high when slave detected
GPIO_TypeDef *PresencePort;
uint32_t ErrorPin;      // is high when internal error detected (for more Detail take a look at SetErrorState)
GPIO_TypeDef *ErrorPort;
// states
bool IsPresent = false;
OneWire_ErrorState ErrorState = Undefined;
bool IsError = false;
// other
bool ParasitePower = false; // !!! NOT IMPLENENT YET !!!
```

### infos
Parasite power isn't implemented yet!
CRC isn't implemented yer!

### example
```c++
/* any include */
#include "OneWire.h"
/* any startup code */
OneWire ow = OneWire();
ow.InitPresencePin(LED_PORT, LED_GREEN);    // setup presence pin
ow.InitErrorPin(LED_PORT, LED_RED); // setup error pin
uint8_t scratch[9];
ow.ReadScratchpad(scratch);
/* any code */
while(1) {
    /* any code */
}
```

---

## DS18x20
### defines
```c++
#define ONEWIRE_CMD_READSCRATCH		0xbe
#define ONEWIRE_CMD_CONVERTT		0x44
```

### functions
```c++
DS18x20(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin); // only for single slave
DS18x20(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint64_t romID);
virtual ~DS18x20();
OneWire_ErrorState ConvertTemp(void);
OneWire_ErrorState WaitForNotBusy(void);
void ReadTemp();
OneWire_ErrorState ReadTempAndWait();
```

### infos
Currently is this library only tested for DS18B20.  
Please note: The DS18S20 is not implemented yet!

### example
```c++
/* any include */
#include "OneWire.h"
/* any startup code */
DS18x20 ds = DS18x20(GPIOA, GPIO_PIN_2);
ds.InitPresencePin(LED_PORT, LED_GREEN);    // setup presence pin
ds.InitErrorPin(LED_PORT, LED_RED); // setup error pin
while(1) {
    /* any code */
    ds.ReadTempAndWait();   // convert temp=>wait for slave is done => convert temp in string (°C) and write into 'ds.TempAsString'
    if (ds.ErrorState == NoError) // check for internal errors
        CDC_Transmit_FS((uint8_t*)ds.TempAsString, strlen(ds.TempAsString));    // send vias usb to antoher device
    /* any code */
}
```

---

### tags
#ONEWIRE #DS1820 #DS18D20 #DELAY #MILLISECONDS #STM32f4

<!--
## Template
### defines
```c++
```

### functions
```c++
```

### infos


### example
```c++
```
---
-->
