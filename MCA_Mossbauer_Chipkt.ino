/*Made by Edison Giraldo. Remember to connect the signals as follows:
  Start --> D2
  (Start Xor CHA) --> D7
  Pulses --> D8
  */
volatile uint32_t Pulsos[1024]={0}; //Account arrangement on each channel. maximum 1024 channels
volatile uint16_t Canal_p=0; //Current channel
volatile uint16_t n_canales=1;
volatile uint16_t n_canales_1=n_canales-1;
uint32_t ms1=0,ms2=0,mdel=0; //for time counting between sending data
char caracter='0';

void setup() {
  Serial.begin(115200);//serial communication at 115200 bauds
  pinMode(9,OUTPUT); //Supply pin for xor gate
  digitalWrite(9,HIGH); //Supply On
  while(Serial.available()==0); //wait for the arrival of some character
  caracter=Serial.read();   //read character
  while(caracter!='C'){ //character must be C(C = Channels)
    if(Serial.available()!=0){
      caracter=Serial.read(); 
      }
    }
  while(Serial.available()==0); 
  n_canales=Serial.parseInt(); //read what came. should be 128, 256, 512 or 1024 (Channels)
  n_canales_1=n_canales-1; //127, 255, 511 o 1023
  attachInterrupt(1, moss_Start, RISING); //Interruption associated with Start pulses on pin 2 of Chipkit uc32.
  attachInterrupt(2, moss_CHA, RISING);   //Interruption associated with CHA pulses on pin 7 of Chipkit uc32.
  delay(200);
  digitalWrite(9,LOW); //Supply off
  delay(200);
  digitalWrite(9,HIGH); //Supply on
  while(Canal_p!=0){};
  attachInterrupt(3, moss_Pulse, RISING); //Interruption associated with pulses on pin 8 of Chipkit uc32. Start Counting.
  ms1=millis();
  delay(5000);
}

void loop() {
  ms2=millis();
  mdel=ms2-ms1;
  while(mdel<10000){ //Has it been 10 seconds?
    if(Serial.available()!=0){
      caracter=Serial.read();
      if(caracter=='F'){ //Finalize?
        detachInterrupt(3); 
        detachInterrupt(2); 
        detachInterrupt(1); 
       
        for(int i=0; i<n_canales_1; i++){ //Send the data
          Serial.print(Pulsos[i]);
          Serial.print(',');
          }
        Serial.print(Pulsos[n_canales_1]);
        Serial.println(',');
          while(1);
        }
        
      }
      ms2=millis();
      mdel=ms2-ms1;
    }
    ms1=millis();
    for(int i=0; i<n_canales_1; i++){//Send the data
          Serial.print(Pulsos[i]);
          Serial.print(',');
          }
    Serial.print(Pulsos[n_canales_1]);
    Serial.println(',');
}

void moss_Start(){ //change the channel to zero
   Canal_p=0; 
  }

void moss_CHA(){//change the channel
   Canal_p +=1;
  }

void moss_Pulse(){ //count a pulse on the current channel
  Pulsos[Canal_p] +=1;
  }
