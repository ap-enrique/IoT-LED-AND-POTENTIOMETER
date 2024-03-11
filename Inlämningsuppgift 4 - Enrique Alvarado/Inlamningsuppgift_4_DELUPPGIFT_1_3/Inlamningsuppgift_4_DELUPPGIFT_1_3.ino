#define PIN_LED_RED 2
#define PIN_BUTTON_RED 25
#define PIN_LED_GREEN 4
#define PIN_BUTTON_GREEN 14
#define PIN_LED_BLUE 5
#define PIN_ANALOG_BLUE_INPUT 36

//Global variabel
byte prevButtonStateRed;                                              //Hålla koll på knappar om det var för att tända eller släcka förra loop.
byte ledStateRed = LOW;                                               //Hålla koll på led lampan är tänd eller släckt. Vi startar med att det är släckt.

byte prevButtonStateGreen;
byte ledStateGreen = LOW;

void setup() {
  Serial.begin(921600);                                               //Etablera en serial communication med samma baudhastighet tillsammans med arduino IDE.

  //PINMODE LED RED
  pinMode(PIN_LED_RED, OUTPUT);                                       //OUTPUT för att vi vill kontrollera utgången dvs. lyset på lampan.
  pinMode(PIN_BUTTON_RED, INPUT);                                     //INPUT för att vi vill läsa datan dvs. när knappen trycks ner.
  prevButtonStateRed = digitalRead(PIN_BUTTON_RED);                   //Här spara först tillstånden på knappen för att jämföra senare vid första körning av programmet.

  //PINMODE LED GREEN
  pinMode(PIN_LED_GREEN, OUTPUT);
  pinMode(PIN_BUTTON_GREEN, INPUT);
  prevButtonStateGreen = digitalRead(PIN_BUTTON_GREEN);
  
  //PINMODE LED BLUE
  pinMode(PIN_LED_BLUE, OUTPUT);
  pinMode(PIN_ANALOG_BLUE_INPUT, INPUT);

  //Starta med alla LED lampor släckta
  digitalWrite(PIN_LED_RED, LOW);                                     //Säkerhetsåtgärd för att säkerhetsställa att lampor är släckta när programmet startar.
  digitalWrite(PIN_LED_GREEN, LOW);

}

void loop() {

  //RED_LED
  byte buttonStateRed = digitalRead(PIN_BUTTON_RED);                  //Läser om användare har tryck på knappen (HIGH) eller har släppt knappen (LOW)
  //Jämföra om det har blivit en ändring.
  //Först fall: Vi har tryck på knappen. Först = LOW och nu HIGH.
  //Andra fall: Vi har släppt knappen. Först = HIGH och nu LOW.
  if (buttonStateRed != prevButtonStateRed) {                         //Här jämförs nuvarande status på knappen med föregående status.
    prevButtonStateRed = buttonStateRed;                              //Ändring upptäcks och sparas för näst kommande loop.
    if (buttonStateRed == LOW) {                                      //Användare har tryck färdigt och släppt på knappen nu
      //Toogle mekanism
      if (ledStateRed == HIGH) {                                      //Om lampan är först tänd
        Serial.println("LED RED OFF");
        ledStateRed = LOW;                                            //Lampan ska släckas nu
      } else {
        Serial.println("LED RED ON");
        ledStateRed = HIGH;                                           //Annars betyder att lampan är släckt först med tänds nu.
      }
      digitalWrite(PIN_LED_RED, ledStateRed);                         //LED RED får instruktion om att lysa eller släcka beror på If-loopen.
    }
  }

  //GREEN_LED
  byte buttonStateGreen = digitalRead(PIN_BUTTON_GREEN);
  if (buttonStateGreen != prevButtonStateGreen) {
    prevButtonStateGreen = buttonStateGreen;
    if (buttonStateGreen == LOW) {
      if (ledStateGreen == HIGH) {
        Serial.println("LED GREEN OFF");
        ledStateGreen = LOW;
      } else {
        Serial.println("LED GREEN ON");
        ledStateGreen = HIGH;
      }
      digitalWrite(PIN_LED_GREEN, ledStateGreen);
    }
  }  

  
  //BLUE
  int potentiometerValue = analogRead(PIN_ANALOG_BLUE_INPUT);         //Funktionen analogRead läser det analoga värdet frå potentiometer och sparar det i variaberln potentiometerValue.
  int brightness = map(potentiometerValue, 0, 4095, 0, 255);          //Funktionen map tar potentiometerValue och mappar värdet från ett ursprunglit intervall till ett nytt intervall. Ursprungliga intervall [0-4095] till nya intervall [0-255].
  double voltage = potentiometerValue / 4095.0 * 3.3;                 //Konverterings formula för Analoga spänningen.
  analogWrite(PIN_LED_BLUE, brightness);                              //Funktionen analogWrite styr och justerar ljusstyrkanvärdet
  Serial.printf("Potentiometer Value (ADC Value): %d, \t Brighteness Value (DAC Value) Val: %d, \t Voltage Value: %.2fV\n", potentiometerValue, brightness, voltage);
  delay(300);                                                         //300 msecs intervall är kravet på inlämnningen.
  
}
