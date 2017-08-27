#include<Servo.h>
#include <CurieBLE.h>

//Bluetooth setup
BLEPeripheral blePeripheral;
BLEService uartService = BLEService("6E400001-B5A3-F393-E0A9-E50E24DCCA9E");
BLECharacteristic rxCharacteristic = BLECharacteristic("6E400002-B5A3-F393-E0A9-E50E24DCCA9E", BLEWriteWithoutResponse, 20);  // == TX on central (android app)
BLECharacteristic txCharacteristic = BLECharacteristic("6E400003-B5A3-F393-E0A9-E50E24DCCA9E", BLENotify , 20); // == RX on central (android app)

Servo FLJaguar;
Servo FRJaguar;
Servo BLJaguar;
Servo BRJaguar;
int FLJagPin = 9;
int FRJagPin = 6;
int BLJagPin = 5;
int BRJagPin = 3;
int v = 1500;
int m = 0;


void setup() {
  FLJaguar.attach(FLJagPin);
  FRJaguar.attach(FRJagPin);
  BLJaguar.attach(BLJagPin);
  BRJaguar.attach(BRJagPin);
  Serial.begin(57600);

  bleInitialize();
}

void loop() {
  blePeripheral.poll();
}

void stay(){
  FLJaguar.write(v);
  FRJaguar.write(v);
  BLJaguar.write(v);
  BRJaguar.write(v);
}

void forward(){
  FLJaguar.write(v+500);
  FRJaguar.write(v-500);
  BLJaguar.write(v+500);
  BRJaguar.write(v-500);
}

void reverse(){
  FLJaguar.write(v-500);
  FRJaguar.write(v+500);
  BLJaguar.write(v-500);
  BRJaguar.write(v+500);
}

void rotCW(){
  FLJaguar.write(v+500);
  FRJaguar.write(v+500);
  BLJaguar.write(v+500);
  BRJaguar.write(v+500);
}

void rotCCW(){
  FLJaguar.write(v-500);
  FRJaguar.write(v-500);
  BLJaguar.write(v-500);
  BRJaguar.write(v-500);
}

void strafeR(){
  FLJaguar.write(v+500);
  FRJaguar.write(v+500);
  BLJaguar.write(v-500);
  BRJaguar.write(v-500);
}

void strafeL(){
  FLJaguar.write(v-500);
  FRJaguar.write(v-500);
  BLJaguar.write(v+500);
  BRJaguar.write(v+500);
}


//control
void actions(char command){
  switch(command){
    case 'a':
      strafeL();
      break;
    case 'w':
      forward();
      break;
    case 's':
      reverse();
      break;
    case 'd':
      strafeR();
      break;
    case 'z':
      stay();
      break;
    case 'x':
      rotCW();
      break;
    case 'v':
      rotCCW();
      break;
  }
}


//Bluetooth copied from miniSteamworks
void rxCharacteristicWritten(BLECentral& central, BLECharacteristic& characteristic) {
  Serial.print("Characteristic event, written: ");

  if (characteristic.value()) {
    char state = *characteristic.value();
    actions(state);
  }
}


//Bluetooth polling and connecting
void bleInitialize(){
  blePeripheral.setLocalName("BLESteam");
  blePeripheral.setAdvertisedServiceUuid(uartService.uuid());

  blePeripheral.addAttribute(uartService);
  blePeripheral.addAttribute(rxCharacteristic);
  blePeripheral.addAttribute(txCharacteristic);

  blePeripheral.setEventHandler(BLEConnected, blePeripheralConnectHandler);
  blePeripheral.setEventHandler(BLEDisconnected, blePeripheralDisconnectHandler);
  rxCharacteristic.setEventHandler(BLEWritten, rxCharacteristicWritten);

  blePeripheral.begin();
}


void blePeripheralConnectHandler(BLECentral& central){
  Serial.print("Connected event, central: ");
  Serial.println(central.address());
}


void blePeripheralDisconnectHandler(BLECentral& central){
  Serial.print("Disconnected event, central: ");
  Serial.println(central.address());
  // todo: stop bot here
}



