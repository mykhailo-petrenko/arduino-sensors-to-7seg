//**************************************************************//
//  Name    : shiftOutCode, Hello World                         //
//  Author  : Carlyn Maw,Tom Igoe                               //
//  Date    : 25 Oct, 2006                                      //
//  Version : 1.0                                               //
//  Notes   : Code for using a 74HC595 Shift Register           //
//          : to count from 0 to 255                            //
//****************************************************************

int ledPin = 13;

// Pins connected to Shift Registers
int latchPin = 8;
int clockPin = 12;
int dataPin = 11;

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
  'H',
  'h',
  'P',
  'p',
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
  186, // 'H',
  58, // 'h',
  248, // 'P',
  248, // 'p',
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

String command;

void setup() {
  Serial.begin(9600);

  delay( 1000 );
  
  //set pins to output because they are addressed in the main loop
  pinMode(ledPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  digitalWrite(latchPin, 0);

  blink( 1 );

  displayInitial();

  blink( 1 );
}

void loop() {
  int code;
  
  while(Serial.available() > 0) {
//    command = Serial.parseInt();
    command = Serial.readString();

    Serial.println(command[0]);

    if (command.length() >= 0 ) {
      displayReset();
      code = getCodeByChar( command[0] );
      writeInt( code );
//      writeToAll( command );
      
      blink( 1 );
    }
  }
}

void writeTemperature( int Temperature ) {
// ' ' if >=0
// '-' if < 0
// 2 symbol of temp
// 't' or C or c ???
}

void writeHumidity( int Humidity ) {
// 3 symbols ( ' ' and 2 if < 100 ) and ( '  ' and 1 if < 10 )  
// 'h' or 'H'
}

void writePressure( int Pressure ) {
  // 3 symbols
  // 'P'
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
}

void displayInitial() {
  displayReset();

  writeToAll( 0 );
}

void writeInt( int data ) {
  shiftOut(dataPin, clockPin, MSBFIRST, data);
}

void writeToAll( int data ) {
  writeInt( data );
  writeInt( data );
  writeInt( data );
  writeInt( data );
}

void blink( int n ) {
  for (int j = 0; j < n; j++) {
    digitalWrite(ledPin, HIGH);
    delay(300);
    digitalWrite(ledPin, LOW);
    delay(100);
  }
}

