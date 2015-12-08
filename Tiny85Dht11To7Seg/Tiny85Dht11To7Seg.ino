#include "DHT.h"

#define dhtPin 0 //2
#define dhtType DHT11

DHT dht(dhtPin, dhtType);

int ledPin = 1; //13;

// Pins connected to Shift Registers
int latchPin = 2; // 8;
int clockPin = 4; //12;
int dataPin = 3; //11;

// Led symbols
char ledSymbols[] = {
  '0',  
  '1',
  '2',
  '3',
  '4',
  '5',
  '6',
  '7',
  '8',
  '9',
  '-',
  ' ',
  'C',
  'c',
  'D',
  'd',
  'H',
  'h',
  'P',
  'p',
  'r',
  'R',
  'i',
  'I',
  'B',
  'b',
  'e',
  'E',
  'o',
  'O',
  'l',
  'L',
  'T',
  't',
  'a',
  'A',
  'U',
  'u',
  'S',
  's',
  'Y',
  'y',
  'f',
  'F',
};

int ledCodes[] = {
  238, // '0',  
  130, // '1',
  220, // '2',
  214, // '3',
  178, // '4'
  118, // '5',
  126, // '6',
  194, // '7',
  254, // '8',
  246, // '9',
  16, // '-',
  0, //
  108, // 'C',
  28, // 'c',
  158, // 'D',
  158, // 'd',
  186, // 'H',
  58, // 'h',
  248, // 'P',
  248, // 'p',
  24, // 'r',
  24, // 'R',
  8, // 'i',
  40, // 'I',
  62, // 'B',
  62, // 'b',
  124, // 'e',
  124, // 'E'
  30, // 'o',
  238, // 'O',
  44, // 'l',
  44, // 'L',
  60, // 't',
  60, // 'T',
  250, // 'a',
  250, // 'A',
  174, //'U',
  14, // 'u'
  118, // 'S',
  118, // 's',
  182, // 'Y',
  182, // 'y',
  120, // 'f',
  120, // 'F',
};

void writeTicker( String Ticker, int Speed = 3 ) {
  int i, Length, Code, Delay;
  
  Delay = (int)(1000 / Speed);
  Length = Ticker.length();
  
  for ( i=0; i<Length; i++ ) {
    Code = getCodeByChar( Ticker[ i ] );
    
    if ( i < Length - 2 && Ticker[ i+1 ] == '.' ) {
      Code = Code + 1;
      i = i + 1;
    }
    
    writeInt( Code );
    delay( Delay );
  }
}

void setup() {
  //set pins to output because they are addressed in the main loop
  pinMode(ledPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  digitalWrite(latchPin, 0);
  
  dht.begin();

  blink( 1 );

  displayReset();

  blink( 1 );
}

void loop() {
  String Buffer;
  
  float t = dht.readTemperature();
  
  if ( isnan(t) ) {
    displayReset();
    writeTicker( "read t dht err" );
  } else {
    Buffer = String((int)t);

    writeTicker( "    ", 6 );
    writeTicker( Buffer + "t", 6 );
  }
  
  delay( 10000 );
  
  float h = dht.readHumidity();
  
  if ( isnan(h) ) {
    displayReset();
    writeTicker( "read H dht err" );
  } else {
    Buffer = String((int)h);
    
    writeTicker( "    ", 6 );
    writeTicker( Buffer + "h", 6 );
  }
  
  delay( 10000 );
}

int getCodeByChar( char needle ) {
  int i, N;

  N = sizeof( ledSymbols );

  for ( i=0; i<N; i++ ) {
    if ( ledSymbols[ i ] == needle ) {
      return ledCodes[ i ];
    }
  }

  return 0;
}

void displayReset() {
  digitalWrite(latchPin, 0);
  delay(200);
  digitalWrite(latchPin, 1);
  writeInt(0);
  writeInt(0);
  writeInt(0);
  writeInt(0);
}

void writeInt( int data ) {
  shiftOut(dataPin, clockPin, MSBFIRST, data);
}

void blink( int n ) {
  for (int j = 0; j < n; j++) {
    digitalWrite(ledPin, HIGH);
    delay(300);
    digitalWrite(ledPin, LOW);
    delay(100);
  }
}
