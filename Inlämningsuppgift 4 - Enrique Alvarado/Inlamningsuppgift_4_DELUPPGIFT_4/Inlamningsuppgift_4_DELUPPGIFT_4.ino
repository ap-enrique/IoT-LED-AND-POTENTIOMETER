#define PIN_LED_RED 2
#define PIN_BUTTON_RED 25
#define PIN_LED_GREEN 4
#define PIN_BUTTON_GREEN 14
#define PIN_LED_BLUE 5
#define PIN_ANALOG_BLUE_INPUT 36

//Global variabel
volatile byte ledStateRed = LOW;                                                                //Volatile talar om för kompilatorn att värdet på variabeln kan ändras av något utanför den ordinarie programflödet.
volatile byte ledStateGreen = LOW;                                                              //Volatile säkertställer att förändringar uppdateras korrekt även om det sker utanför void loop().


// Variabler för av-studsning
unsigned long lastDebounceTimeRed = millis();
unsigned long lastDebounceTimeGreen = millis();
unsigned long debounceDelay = 50; //Av-studsningstid i millisekunder

//Potentiometer
bool potentiometerEnabled = true;                                                               //Variabel flagga för att hålla koll på vridpotentiometerns om den är aktiv. Sant eller falsk.
int blueLedPower = 0;                                                                           //Variabel för att lagra den analoga PWM-styrkan hos blå lysdioden

// Interrupt Service Routine (ISR) funktion för RED LED.
void redButtonInterrupt() {                                                                     //Funktion som är en avbrottsutlösare för röda knappen. När knappen trycks ner kommer denna funktion att aktiveras. 
  if ((millis() - lastDebounceTimeRed) >= debounceDelay) {                                      //Vv-studsning (debounce) mekanism. Om tidsskillanden mellan föregåend och nu debounce tid är stor då fortsätt med knapptryckning.
    ledStateRed = !ledStateRed;                                                                 //Här vänds LED-tillståndet från LOW to HIGH eller viceversa.
    digitalWrite(PIN_LED_RED, ledStateRed);                                                     //Det beror på vad ledStateRed är så lyser eller släcks lampa nu.
    lastDebounceTimeRed = millis();                                                             //Uppdaterar variabel till aktuella tidsvärde för att jämföra till nästa loop.
  }
}

// Interrupt Service Routine (ISR) funktion för GREEN LED.
void greenButtonInterrupt() {
  if ((millis() - lastDebounceTimeGreen) >= debounceDelay) {
    ledStateGreen = !ledStateGreen; // Vänd LED-tillståndet
    digitalWrite(PIN_LED_GREEN, ledStateGreen);
    lastDebounceTimeGreen = millis();
  }
}

void setup() {
  Serial.begin(921600);                                                                         //Etablera en serial communication med samma baudhastighet tillsammans med arduino IDE.

  //PINMODE LED RED
  pinMode(PIN_LED_RED, OUTPUT);                                                                 //OUTPUT för att vi vill kontrollera utgången dvs. lyset på lampan.
  pinMode(PIN_BUTTON_RED, INPUT_PULLUP);                                                        //Aktivera den inbyggda pull-up motorn
  attachInterrupt(digitalPinToInterrupt(PIN_BUTTON_RED), redButtonInterrupt, FALLING);          //(InterruptVector, ISR, Mode) - Kopplar avbrottmekanism för röda knappen då input går från HIGH till LOW. Knappen är färdigt nertryck.


  //PINMODE LED GREEN
  pinMode(PIN_LED_GREEN, OUTPUT);
  pinMode(PIN_BUTTON_GREEN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PIN_BUTTON_GREEN), greenButtonInterrupt, FALLING);      //Kopplar avbrottmekanism för gröna knappen då input går från HIGH till LOW. Knappen är färdigt nertryck.

  
  //PINMODE LED BLUE
  pinMode(PIN_LED_BLUE, OUTPUT);
  pinMode(PIN_ANALOG_BLUE_INPUT, INPUT);

  //Starta med alla RED GREEB LED lampor släckta
  digitalWrite(PIN_LED_RED, LOW);
  digitalWrite(PIN_LED_GREEN, LOW);

}

void loop() {

  if (Serial.available()) {                                                                     //Kontrollera om om det finns några inkommande data tillgängliga att läsa från den seriella porten.
    String command = Serial.readString();                                                       //Om det finns data tillgängligt att läsa, läs med readString().
    if(command.startsWith("ledpower ")) {                                                       //Justera PWM-styrkan för BLUE LED med analog värde [0-255].
      if (command == "ledpower -1") {                                                           //kommando för att aktivera vrid förmåga för potentiometer
        potentiometerEnabled = true;                                                            //Aktiviera potentiometer
        Serial.println("POTENTIOMETER ENABLED");
      } else {                                                                                  //Forsätt att justera PWM-styrkan med analog värde
        blueLedPower = command.substring(9).toInt();                                            //från sträng subtrahera bort de för nio tecken och sedan konvertera till integral.
        Serial.print("Blue LED power set to: ");
        Serial.println(blueLedPower);
        potentiometerEnabled = false;                                                           //Potentiometern fortsätter vara oaktiverad.
      }
    }
  }
  
  //BLUE
  // Vridpotentiometern och styr ljusstyrkan på den blå lysdioden
  if (potentiometerEnabled) {                                                                   //Om det är sant att poterntiometer är aktivierad
    int potentiometerValue = analogRead(PIN_ANALOG_BLUE_INPUT);                                 //Funktionen analogRead läser det analoga värdet frå potentiometer och sparar det i variaberln potentiometerValue.
    int brightness = map(potentiometerValue, 0, 4095, 0, 255);                                  //Funktionen map tar potentiometerValue och mappar värdet från ett ursprunglit intervall till ett nytt intervall. Ursprungliga intervall [0-4095] till nya intervall [0-255].
    analogWrite(PIN_LED_BLUE, brightness);                                                      //Funktionen analogWrite styr och justerar ljusstyrkanvärdet
    Serial.printf("Potentiometer value: %d, Brightness: %d\n", potentiometerValue, brightness);
  } else {                                                                                      //Om det är inte sant att potentiometer är aktivierad då PWM-styrkan definieras med kommandtolken
    analogWrite(PIN_LED_BLUE, blueLedPower);                                                    //Med funktionen analogWrite styrs och justeras ljusstyrkanvärdet
    Serial.printf("Blue LED power set to: %d\n", blueLedPower);
  }
}
