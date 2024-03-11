#define PIN_LED_RED 2                                   //Definera som konstant global variabler med ett namn och GPIO nummer.
#define PIN_LED_GREEN 4
#define PIN_LED_BLUE 5

void setup() {

  Serial.begin(921600);                                 //Etablera en serial communication med samma baudhastighet tillsammans med arduino IDE.
  pinMode(PIN_LED_RED, OUTPUT);                         //Konfigurera GPIO pinnen som utgång för att skicka data och tända eller släcka lampa som resultat.
  pinMode(PIN_LED_GREEN, OUTPUT);
  pinMode(PIN_LED_BLUE, OUTPUT);

}

void loop() {
  
  //RED
  digitalWrite(PIN_LED_RED, HIGH);                      //Tända röda lampa. GPIO sätts till hög (HIGH) spänningsnivå.
  delay(2000);                                          //Vänta med två sekunder att försätta med loopen.
  float current_red = 5.0 / 220.0;                      //Variabel för ström beräkning genom lampan.
  Serial.print("RED LED STRÖM:\tI = U/R = 5/220 = ");   //Det som ska synas på Serial monitor
  Serial.print(current_red, 3);                         //Variabel med tre decimaler.
  Serial.println(" A");

  //GREEN
  digitalWrite(PIN_LED_GREEN, HIGH);
  delay(1000);                                           //Vänta med en sekund
  float current_green = 5.0 / 1000.0;                      //Variabel för ström beräkning genom lampan.
  Serial.print("GREEN LED STRÖM:I = U/R = 5/1000 = ");
  Serial.print(current_green, 3);
  Serial.println(" A");

  //BLUE
  digitalWrite(PIN_LED_BLUE, HIGH);
  delay(500);                                             //Vänta med halv sekund
  float current_blue = 5.0 / 1000.0;
  Serial.print("BLUE LED STRÖM:\tI = U/R = 5/1000 = ");
  Serial.print(current_blue, 3);
  Serial.println(" A");

  digitalWrite(PIN_LED_RED, LOW);                         //Släcka röda lampan. GPIO sätts till låg (LOW) spänningsnivå
  digitalWrite(PIN_LED_GREEN, LOW);
  digitalWrite(PIN_LED_BLUE, LOW);
  delay(1000);                                            //Vänta med en sekund. Efter loop börjar igen.

}
