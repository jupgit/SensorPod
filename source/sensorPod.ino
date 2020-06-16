
// DISPLAY OLED
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);



// ENCODER
#include <ESP32Encoder.h>
ESP32Encoder encoder;

// ENCODER PUSH BUTTON
const int encoderButtonPin = 27;     // the number of the pushbutton pin


//MENU SYSTEM
int menu = 11;     /// = MENU 1 / PAG 1
int lastMenu = 1;
int cursorLine = 1;
int cursorRow = 0;
int lastCursorLine = 1;

int lastEncoderCount = 0;
  // set starting count value after attaching
int encoderCount = encoder.getCount();


// LED
const int ledPin =  5;      // the number of the LED pin



void encoderChangeLine() {
  
  /// ENCODER PRA FRENTE
  if ( encoderCount > lastEncoderCount ) {
      cursorLine++;
     }

  /// ENCODER PRA TRAS
  if ( encoderCount < lastEncoderCount ) {
      cursorLine--;
     }


}



// ============================================== SETUP


void setup() {
  Serial.begin(9600);


// DISPLAY OLED

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(1000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();

  // Draw a single pixel in white
  display.drawPixel(64, 32, SSD1306_WHITE);
  display.display();

  delay(500);

  for (int i=1; i<=64; i=i+8) {
   display.drawLine(64-i, 32, 64+i, 32, 1);
   display.display();
  }

   delay(500);

   for (int i=1; i<=40; i=i+=6) {
   display.fillRect(0, 32-i, 128, 32+i, SSD1306_WHITE);
   display.display();
  }   
  display.setCursor(0,40);
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(BLACK, WHITE);        // Draw white text
  display.print(F("SensorPad ioT"));
  
   delay(2000);
   display.display();

   display.clearDisplay();

  display.setTextColor(SSD1306_WHITE);

// ENCODER

  // Enable the weak pull down resistors
  ESP32Encoder::useInternalWeakPullResistors=true;

  // Attache pins for use as encoder pins
  encoder.attachHalfQuad(26, 25);

    
  // set starting count value after attaching
  encoder.setCount(0);

  // initialize the ENCODER pushbutton pin as an input:
  pinMode(encoderButtonPin, INPUT);


// LED

  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin,LOW);

}

// ============================================== LOOP


void loop() {

  encoderCount = encoder.getCount();

/////////////////////////////////////////////////
//  (MENU): MENU 11 - Menu 1 / Pagina 1
/////////////////////////////////////////////////
  

if (menu == 11) {

      printMenu(" MENU", " Setup", " Perform", " Blahblah");

//// MENU:

  // Chama função que muda linha de acordo com o Encoder
  encoderChangeLine();

  /// LIMITES DO MENU
   if (cursorLine < 1) { cursorLine = 1; }              // MININUM LINE = 1
   if (cursorLine > 3) { cursorLine = 1; menu = 12; }   // MAXIMUM LINE = 3 // GOTO PG 2

  // POSICIONA SETA MENU  (cada linha y = 0, 16, 32, 48)
  
  display.setCursor(0,(cursorLine*16));
  display.write(16);                  // SETA PARA direita COD 16
  lastEncoderCount = encoder.getCount();

  display.display();

// LED Feedback: pisca ao trocar de linha
   if (lastCursorLine!=cursorLine) {flashLED(1);}


lastCursorLine = cursorLine;


  // VERIFICA SE O BOTAO FOI APERTADO
  // APERTADO = 0 // SOLTO = 1 ( GND quando aperta )
  
  if  (digitalRead(encoderButtonPin) == 0)  {

    //if (cursorLine == 1) { menu = 21; }  // linha 1 > GO TO MENU 21
    if (cursorLine == 2) { menu = 31; }  // linha 2 > GO TO MENU 31
    if (cursorLine == 3) { menu = 41; }  // linha 3 > GO TO MENU 41

   flashLED(3);
   cursorLine = 1;
    
    }  


}




/////////////////////////////////////////////////
//  (MENU): MENU 12 - Menu 1 / Pagina 2
/////////////////////////////////////////////////
  

if (menu == 12) {

    printMenu(" MENU", " AAA", " BBB", " CCC");


//// MENU:

// Chama função que muda linha de acordo com o Encoder
  encoderChangeLine();

  /// LIMITES DO MENU
   if (cursorLine < 1) { cursorLine = 3; menu = 11; }  // MININUM LINE = 1
   if (cursorLine > 3) { cursorLine = 3; }  // MAXIMUM LINE = 3


// LED Feedback: pisca ao trocar de linha
   if (lastCursorLine!=cursorLine) {flashLED(1); lastCursorLine = cursorLine;}

  // POSICIONA SETA MENU  (cada linha y = 0, 16, 32, 48)
  display.setCursor(0,(cursorLine*16));
  display.write(16);                  // SETA PARA direita COD 16
  lastEncoderCount = encoder.getCount();

  display.display();

  // VERIFICA SE O BOTAO FOI APERTADO
  // APERTADO = 0 // SOLTO = 1 ( GND quando aperta )
  if  (digitalRead(encoderButtonPin) == 0)  {

    //if (cursorLine == 1) { menu = 21; }  // linha 1 > GO TO MENU 
    //if (cursorLine == 2) { menu = 31; }  // linha 2 > GO TO MENU 
    if (cursorLine == 3) { menu = 11; }  // linha 3 > GO TO MENU 

   flashLED(3);
   cursorLine = 1;
    
    }  

}


/////////////////////////////////////////////////
//  (PERFORM): MENU 31 - Menu 3 / Pagina 1
/////////////////////////////////////////////////
  

if (menu == 31) {

     printMenu(" Perform", " Test", " PontosG", "");

//// MENU:

// Chama função que muda linha de acordo com o Encoder
  encoderChangeLine();

  /// LIMITES DO MENU
   if (cursorLine < 0) { cursorLine = 0; }  // MININUM LINE = 0
   if (cursorLine > 2) { cursorLine = 2; }  // MAXIMUM LINE = 2


// LED Feedback: pisca ao trocar de linha
   if (lastCursorLine!=cursorLine) {flashLED(1); lastCursorLine = cursorLine;}

  // POSICIONA SETA MENU  (cada linha y = 0, 16, 32, 48)
  display.setCursor(0,(cursorLine*16));
  if ( cursorLine == 0 ) {display.write(17);} else {display.write(16);}                 // SETA PARA direita COD 16
  lastEncoderCount = encoder.getCount();

  display.display();

  // VERIFICA SE O BOTAO FOI APERTADO
  // APERTADO = 0 // SOLTO = 1 ( GND quando aperta )
  if  (digitalRead(encoderButtonPin) == 0)  {

    if (cursorLine == 0) { menu = 11; }  // linha 1 > GO TO MENU 
    if (cursorLine == 1) { menu = 41; }  // linha 1 > GO TO MENU 
    if (cursorLine == 2) { menu = 41; }  // linha 2 > GO TO MENU 

   flashLED(3);
   cursorLine = 1;
    
    }  

}


/////////////////////////////////////////////////
//  (MENU): MENU 41 - Menu 4 / Pagina 1
/////////////////////////////////////////////////


if (menu == 41) {

    printMenu(" Blaahhhh", "", "", "");

  display.display();


  delay(2000);
  menu = 11;

  
  flashLED(3);
  cursorLine = 1;

} 

}




//====================================
//==   F   U   N   C   O   E   S   ==
//====================================


void flashLED(int LEDflashes) {
 
    
    for (int i = 0; i<LEDflashes; i++) {
      digitalWrite(ledPin,HIGH);
      delay(100);
      digitalWrite(ledPin,LOW);
      delay(100);

    }
}


void printMenu (String line0, String line1, String line2, String line3) {

  
  display.clearDisplay();
  display.cp437(true);         // Use full 256 char 'Code Page 437' font

  display.setCursor(0,0);
  display.drawLine(0,15,128,15,1);

  display.setTextSize(2);             // 

  display.println(line0); 
  display.println(line1);
  display.println(line2);
  display.println(line3);


  
}
