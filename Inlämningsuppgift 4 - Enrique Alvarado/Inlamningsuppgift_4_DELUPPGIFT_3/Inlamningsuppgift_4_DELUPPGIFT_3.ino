#define PIN_LED_RED 2
#define PIN_BUTTON_RED 25
#define PIN_LED_GREEN 4
#define PIN_BUTTON_GREEN 14
#define PIN_LED_BLUE 5
#define PIN_ANALOG_BLUE_INPUT 36

//Global variabel
byte prevButtonStateRed;                                                                        //Hålla koll på knappar om det var för att tända eller släcka förra loop.
byte ledStateRed = LOW;                                                                         //Hålla koll på led lampan är tänd eller släckt. Vi startar med att det är släckt.
bool redButtonEnabled = false;                                                                  //Flagga för knappen om den är aktiv. Sant eller falsk.

byte prevButtonStateGreen;
byte ledStateGreen = LOW;
bool greenButtonEnabled = false;

// Variabler för av-studsning
unsigned long lastTimeRedButtonStateChanged = millis();                                         //unsigned long för att lagra stora heltal utan negativa värden. Värdern används sedan för att jämföra.
unsigned long lastTimeGreenButtonStateChanged = millis();                                       //Funktionen millis() returnerar hur många millisekunder som har gått sedan ESP32 startades
unsigned long debounceDelay = 50;                                                               //Av-studsningstid i millisekunder

//Pontentiometer
bool potentiometerEnabled = true;                                                               //Variabel flagga för att hålla koll på vridpotentiometerns om den är aktiv. Sant eller falsk.
int blueLedPower = 0;                                                                           //Variabel för att sparas PWM-styrkan hos blå lysdioden

void setup() {

  Serial.begin(921600);                                                                         //Etablera en serial communication med samma baudhastighet tillsammans med arduino IDE.

  //PINMODE LED RED
  pinMode(PIN_LED_RED, OUTPUT);                                                                 //OUTPUT för att vi vill kontrollera utgången dvs. lyset på lampan.
  pinMode(PIN_BUTTON_RED, INPUT_PULLUP);                                                        //INPUT_PULLUP aktiverar den inbyggda pull-up motorn hos ESP32.
  prevButtonStateRed = digitalRead(PIN_BUTTON_RED);

  //PINMODE LED GREEN
  pinMode(PIN_LED_GREEN, OUTPUT);
  pinMode(PIN_BUTTON_GREEN, INPUT_PULLUP);
  prevButtonStateGreen = digitalRead(PIN_BUTTON_GREEN);
  
  //PINMODE LED BLUE
  pinMode(PIN_LED_BLUE, OUTPUT);
  pinMode(PIN_ANALOG_BLUE_INPUT, INPUT);

  //Starta med alla RED GREEB LED lampor släckta
  digitalWrite(PIN_LED_RED, LOW);                                                               //Säkerhetsåtgärd för att säkerhetsställa att lampor är släckta när programmet startar.
  digitalWrite(PIN_LED_GREEN, LOW);

}

void loop() {
  // Läser och sparar knappens tillstånd
  int buttonStateRed = digitalRead(PIN_BUTTON_RED);                  
  int buttonStateGreen = digitalRead(PIN_BUTTON_GREEN);

  Serial.print("Enable button red ");
  Serial.print(buttonStateRed);
  Serial.print("\t");
  Serial.print("Enable button green ");
  Serial.println(buttonStateGreen);

  if (Serial.available()) {                                                                     // Kontrollera om om det finns några inkommande data tillgängliga att läsa från den seriella porten.
    String command = Serial.readString();                                                       // Om det finns data tillgängligt att läsa, läs med readString().
    if(command == "enable red") {                                                               //Enable RED BUTTON
      pinMode(PIN_BUTTON_RED, INPUT_PULLUP);
      redButtonEnabled = true;
      Serial.println("RED BUTTON ENABLED");
    } else if(command == "disable red") {                                                       //Disable RED BUTTON
      pinMode(PIN_BUTTON_RED, INPUT);
      redButtonEnabled = false;
      Serial.println("RED BUTTON DISABLED");
    } else if(command == "enable green") {                                                      //Enable GREEN BUTTON
      pinMode(PIN_BUTTON_GREEN, INPUT_PULLUP);
      greenButtonEnabled = true;
      Serial.println("GREEN BUTTOM ENABLED");
    } else if(command == "disable green") {                                                     //Disable GREEN BUTTON
      pinMode(PIN_BUTTON_GREEN, INPUT);
      greenButtonEnabled = false;
      Serial.println("GREEN BUTTON DISABLED");
    } else if(command == "ledon red") {                                                         //Tända RED LED
      digitalWrite(PIN_LED_RED, HIGH);
      Serial.println("RED LED ON");
    } else if(command == "ledoff red") {                                                        //Släck RED LED
      digitalWrite(PIN_LED_RED, LOW);
      Serial.println("Red LED OFF");
    } else if(command == "ledon green") {                                                       //Tända GREEN LED
      digitalWrite(PIN_LED_GREEN, HIGH);
      Serial.println("GREEN LED ON");
    } else if(command == "ledoff green") {                                                      //Släck GREEN LED
      digitalWrite(PIN_LED_GREEN, LOW);
      Serial.println("GREEN LED OFF");
    } else if(command.startsWith("ledpower ")) {                                                //Justera PWM-styrkan för BLUE LED med analog värde [0-255].
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

  //RED_LED
  if (redButtonEnabled && millis() - lastTimeRedButtonStateChanged >= debounceDelay) {          //Här kontrolleras om det har gått tillräckligt med tid sedan det senaste knapptrycket för att hantera avstudsning.
                                                                                                //Och att redbuttonEnabled är sant. 
                                                                                                //Tiden som har gått är större än eller lika med avstudsningstiden då fortsätter vi till nästa steg.
    byte buttonStateRed = digitalRead(PIN_BUTTON_RED);                                          //Läser om jag har tryck på knappen (HIGH) eller har släppt knappen (LOW)
    //Jämföra om det har blivit en ändring.
    //Först fall: Vi har tryck på knappen. Först = LOW och nu HIGH.
    //Andra fall: Vi har släppt knappen. Först = HIGH och nu LOW.
    if (buttonStateRed != prevButtonStateRed) {                                                 //Om tillståndet skiljer sig från föregående innebär då att har skett e knapptrycksändring.
      lastTimeRedButtonStateChanged = millis();                                                 //Här läses tiden och sparar värdet för nästa loop.
      prevButtonStateRed = buttonStateRed;                                                      //Då ändring upptäcks, uppdateras och sparas för nästa loop.
      if (buttonStateRed == LOW) {                                                              //Vi har tryck färdigt och släppt på knappen nu
        //Toogle mekanism
        if (ledStateRed == HIGH) {                                                              //Om lampan är först tänd
          Serial.println("LED RED OFF");
          ledStateRed = LOW;                                                                    //Lampan ska släckas nu
        } else {
          Serial.println("LED RED ON");
          ledStateRed = HIGH;                                                                   //Annars betyder att lampan är släckt först med tänds nu.
        }
        digitalWrite(PIN_LED_RED, ledStateRed);                                                 //LED RED får instruktion om att lysa eller släcka beror på If-loopen.
      }
    }
  }

  //GREEN_LED
  if (greenButtonEnabled && millis() - lastTimeGreenButtonStateChanged >= debounceDelay) {
    byte buttonStateGreen = digitalRead(PIN_BUTTON_GREEN);
    if (buttonStateGreen != prevButtonStateGreen) {
      lastTimeGreenButtonStateChanged = millis();
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
  }

  //BLUE
  // Vridpotentiometern och styr ljusstyrkan på den blå lysdioden
  if (potentiometerEnabled) {                                                                     //Om det är sant att poterntiometer är aktivierad
    int potentiometerValue = analogRead(PIN_ANALOG_BLUE_INPUT);                                   //Funktionen analogRead läser det analoga värdet frå potentiometer och sparar det i variaberln potentiometerValue.
    int brightness = map(potentiometerValue, 0, 4095, 0, 255);                                    //Funktionen map tar potentiometerValue och mappar värdet från ett ursprunglit intervall till ett nytt intervall. Ursprungliga intervall [0-4095] till nya intervall [0-255].
    analogWrite(PIN_LED_BLUE, brightness);                                                        //Funktionen analogWrite styr och justerar ljusstyrkanvärdet
    Serial.printf("Potentiometer value: %d, Brightness: %d\n", potentiometerValue, brightness);
  } else {                                                                                        //Om det är inte sant att potentiometer är aktivierad då PWM-styrkan definieras med kommandtolken
    analogWrite(PIN_LED_BLUE, blueLedPower);                                                      //Med funktionen analogWrite styrs och justeras ljusstyrkanvärdet
    Serial.printf("Blue LED power set to: %d\n", blueLedPower);
  }
}
