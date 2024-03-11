#define PIN_LED_BLUE 5                                            //Definera som konstant global variabler med ett namn och GPIO nummer.
#define PIN_ANALOG_BLUE_INPUT 36                                  //Potentiometer är kopplat till en ext. 3,3V

void setup() {
  
  Serial.begin(921600);                                           //Etablera en serial communication med samma baudhastighet tillsammans med arduino IDE.
  pinMode(PIN_LED_BLUE, OUTPUT);                                  //Konfigurera GPIO pinnen som utgång för att skicka data och tända eller släcka lampa som resultat.
  pinMode(PIN_ANALOG_BLUE_INPUT, INPUT);                          //Konfigurera GPIO pinnen som ingång för inläsning för potentiometern.

}

void loop() {  
  //BLUE
  int potentiometerValue = analogRead(PIN_ANALOG_BLUE_INPUT);     //Funktionen analogRead läser det analoga värdet frå potentiometer och sparar det i variaberln potentiometerValue.
  int brightness = map(potentiometerValue, 0, 4095, 0, 255);      //Funktionen map tar potentiometerValue och mappar värdet från ett ursprunglit intervall till ett nytt intervall. Ursprungliga intervall [0-4095] till nya intervall [0-255].
  double voltage = potentiometerValue / 4095.0 * 3.3;             //Konverterings formula för Analoga spänningen. För att skriva ut till serial monitor.
  analogWrite(PIN_LED_BLUE, brightness);                          //Funktionen analogWrite styr och justerar ljusstyrkanvärdet
  Serial.printf("Potentiometer Value (ADC Value): %d, \t Brighteness Value (DAC Value) Val: %d, \t Voltage Value: %.2fV\n", potentiometerValue, brightness, voltage);
  delay(300);                                                     //300 msecs intervall är kravet på inlämnningen.
  
}

