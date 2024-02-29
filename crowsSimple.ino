#define pass() (void)NULL

const uint8_t A = 2;
const uint8_t Latch_Clock = 3;
const uint8_t Shift_Clock = 4;
const uint8_t GND = 5;
const uint8_t V5 = 6;
const uint8_t V2p5 = 7;

uint8_t str[99];
uint8_t ax[4], ay[3];
uint8_t mt_reset = 0; uint8_t mt_cs1 = 0; uint8_t mt_cs2 = 0; uint8_t node[3]; uint8_t NOD;
bool err = false;

void shiftOutMSBFIRST (uint8_t dataPin, uint8_t clockPin, uint8_t* val)
{
    for (uint8_t i = 0; i <= 15; i++)  
    {
        digitalWrite(dataPin, val[i]);
        digitalWrite(clockPin, HIGH);
        digitalWrite(clockPin, LOW);       
    }
}

void x_Decode(uint8_t* x)
{
  uint8_t temp = x[0];
  switch ( temp ) 
  {
    case 0:
      x[0] = 0; x[1] = 0; x[2] = 0; x[3] = 0;
      break;
    case 1:
      x[0] = 1; x[1] = 0; x[2] = 0; x[3] = 0;
      break;
    case 2:
      x[0] = 0; x[1] = 1; x[2] = 0; x[3] = 0;
      break;
    case 3:
      x[0] = 1; x[1] = 1; x[2] = 0; x[3] = 0;
      break;
    case 4:
      x[0] = 0; x[1] = 0; x[2] = 1; x[3] = 0;
      break;
    case 5:
      x[0] = 1; x[1] = 0; x[2] = 1; x[3] = 0;
      break;
    case 12:
      x[0] = 0; x[1] = 1; x[2] = 1; x[3] = 0;
      break;
    case 13:
      x[0] = 1; x[1] = 1; x[2] = 1; x[3] = 0;
      break;
    case 6:
      x[0] = 0; x[1] = 0; x[2] = 0; x[3] = 1;
      break;
    case 7:
      x[0] = 1; x[1] = 0; x[2] = 0; x[3] = 1;
      break;
    case 8:
      x[0] = 0; x[1] = 1; x[2] = 0; x[3] = 1;
      break;
    case 9:
      x[0] = 1; x[1] = 1; x[2] = 0; x[3] = 1;
      break;
    case 10:
      x[0] = 0; x[1] = 0; x[2] = 1; x[3] = 1;
      break;
    case 11:
      x[0] = 1; x[1] = 0; x[2] = 1; x[3] = 1;
      break;
    case 14:
      x[0] = 0; x[1] = 1; x[2] = 1; x[3] = 1;
      break;
    case 15:
      x[0] = 1; x[1] = 1; x[2] = 1; x[3] = 1;
      break;
    default:
      Serial.print("X not in [0..15] (+1)");
  }
}

void y_Decode(uint8_t* y)
{
  uint8_t temp = y[0];
  switch ( temp )
  {
    case 0:
      y[0] = 0; y[1] = 0; y[2] = 0;
      break;
    case 1:
      y[0] = 1; y[1] = 0; y[2] = 0;
      break;
    case 2:
      y[0] = 0; y[1] = 1; y[2] = 0;
      break;
    case 3:
      y[0] = 1; y[1] = 1; y[2] = 0;
      break;
    case 4:
      y[0] = 0; y[1] = 0; y[2] = 1;
      break;
    case 5:
      y[0] = 1; y[1] = 0; y[2] = 1;
      break;
    case 6:
      y[0] = 0; y[1] = 1; y[2] = 1;
      break;
    case 7:
      y[0] = 1; y[1] = 1; y[2] = 1;
      break;
     default:
      Serial.print("Y not in [0..7] (+1)");
          
  }
}

void setup() 
{
  pinMode(A, OUTPUT);
  pinMode(Latch_Clock, OUTPUT);
  pinMode(Shift_Clock, OUTPUT);
  pinMode(GND, OUTPUT);
  pinMode(V5, OUTPUT);
  pinMode(V2p5, OUTPUT);
  Serial.begin(115200);
}

void loop() 
{

  for (uint8_t i = 0; i <= 98; i++)  
  {
    str[i] = '\0';
  }
  
  if(Serial.available() > 0)
  {
    
    for (uint8_t i = 0; i <= 3; i++)  // reading first 4 character 
    {
      str[i] = Serial.read();
      delay(100);
    }
    
    if ((str[0]=='C') && (str[1]=='M') && (str[2]== 'T'))  // commutate
    {
      Serial.println("~CMT~ indeed");  
      mt_cs1 = 1;
      mt_cs2 = 0;
      ax[0] = Serial.parseInt();
      ay[0] = Serial.parseInt();
      ax[0]--;
      ay[0]--;
      
      if ((ax[0] > 15) || (ay[0]) > 7) // wrong address
      {
        for (int i = 0; i<= 98; i++)
        {
          if (Serial.available() > 0)
          {
          Serial.read();
          }
        }
          Serial.print("err x = ");
          Serial.print(ax[0]);
          Serial.print("   y = ");
          Serial.print(ay[0]);
          Serial.print("\t");
          mt_cs1 = 0;
          mt_cs2 = 0;
          mt_reset = 0;
          ax[0] = 0;
          ay[0] = 0;
          err = true;
      }

      x_Decode(&ax[0]);
      y_Decode(&ay[0]);

      str[0] = 0;
      str[1] = node[2];
      str[2] = node[1];
      str[3] = node[0];
      str[4] = mt_cs2;
      str[5] = mt_cs1;
      str[6] = 0;                //mt_strobe
      str[7] = mt_reset;
      str[8] = 1;
      str[9] = ay[2];
      str[10] = ay[1];
      str[11] = ay[0];
      str[12] = ax[3];
      str[13] = ax[2];
      str[14] = ax[1];
      str[15] = ax[0];
      
      for(uint8_t i = 0; i <= 15; i++)
      Serial.print(str[i]);
      Serial.print("\t");

      (err != true) ? shiftOutMSBFIRST(A, Shift_Clock, &str[0]) : pass();
      digitalWrite(Latch_Clock, HIGH);
      delay(10);
      digitalWrite(Latch_Clock, LOW);
      delay(1);
      
      str[6] = 1;
      (err != true) ? shiftOutMSBFIRST(A, Shift_Clock, &str[0]) : pass();
      digitalWrite(Latch_Clock, HIGH);
      delay(10);
      digitalWrite(Latch_Clock, LOW);
      delay(1);
      
      str[6] = 0;
      (err != true) ? shiftOutMSBFIRST(A, Shift_Clock, &str[0]) : pass();
      digitalWrite(Latch_Clock, HIGH);
      delay(10);
      digitalWrite(Latch_Clock, LOW);
      delay(1);
      

      Serial.println("~CMT~ complete");
      mt_cs1 = 0;
      err = false;
    }
    
    if ((str[0]=='S') && (str[1]=='T') && (str[2]=='T'))   // START
    {
      Serial.println("~STT~ indeed");
      Serial.println("~STT~ complete");
    }
    
    if ((str[0]=='S') && (str[1]=='T') && (str[2]=='P'))  // STOP
    {
      Serial.println("~STP~ indeed");
      Serial.println("~STP~ complete");
      digitalWrite(GND, HIGH);
      digitalWrite(V5, HIGH);
      digitalWrite(V2p5, HIGH);
    }

    if ((str[0]=='R') && (str[1]=='S') && (str[2]=='T'))  // RESET
    { 
      Serial.println("~RST~ indeed");
      node[0] = 0; node[1] = 0; node[2] = 0;   

      digitalWrite(V2p5, LOW);
      digitalWrite(V5, LOW);
      digitalWrite(GND, LOW);
         
      str[0] = 0;
      str[1] = node[2];
      str[2] = node[1];
      str[3] = node[0];
      str[4] = 1;
      str[5] = 1;
      str[6] = 0;                //mt_strobe
      str[7] = 1;
      str[8] = 0;
      str[9] = 0;
      str[10] = 0;
      str[11] = 0;
      str[12] = 0;
      str[13] = 0;
      str[14] = 0;
      str[15] = 0;

      shiftOutMSBFIRST(A, Shift_Clock, &str[0]);
      digitalWrite(Latch_Clock, HIGH);
      delay(1);
      digitalWrite(Latch_Clock, LOW);
      delay(1);

      str[7] = 0;
      shiftOutMSBFIRST(A, Shift_Clock, &str[0]);
      digitalWrite(Latch_Clock, HIGH);
      delay(1);
      digitalWrite(Latch_Clock, LOW);
      delay(1);

      str[4] = 0; str[5] = 0;

      shiftOutMSBFIRST(A, Shift_Clock, &str[0]);
      digitalWrite(Latch_Clock, HIGH);
      delay(1);
      digitalWrite(Latch_Clock, LOW);
      delay(1);
      

      Serial.println("~RST~ complete");
    }

    if ((str[0]=='M') && (str[1]=='S') && (str[2]=='R'))  //MEASURE
    {
      Serial.println("~MSR~ indeed");  
      mt_cs1 = 0;
      mt_cs2 = 1;
      ax[0] = Serial.parseInt();
      ay[0] = Serial.parseInt();
      ax[0]--;
      ay[0]--;
      
      if ((ax[0] > 15) || (ay[0]) > 7) // wrong address
      {
        for (int i = 0; i<= 98; i++)
        {
          if (Serial.available() > 0)
          {
          Serial.read();
          }
        }
          Serial.print("err x = ");
          Serial.print(ax[0]);
          Serial.print("   y = ");
          Serial.print(ay[0]);
          Serial.print("\t");
          mt_cs1 = 0;
          mt_cs2 = 0;
          mt_reset = 0;
          ax[0] = 0;
          ay[0] = 0;
          err = true;
      }

      x_Decode(&ax[0]);
      y_Decode(&ay[0]);

      str[0] = 0;
      str[1] = node[2];
      str[2] = node[1];
      str[3] = node[0];
      str[4] = mt_cs2;
      str[5] = mt_cs1;
      str[6] = 0;                //mt_strobe
      str[7] = mt_reset;
      str[8] = 1;
      str[9] = ay[2];
      str[10] = ay[1];
      str[11] = ay[0];
      str[12] = ax[3];
      str[13] = ax[2];
      str[14] = ax[1];
      str[15] = ax[0];
      
      for(uint8_t i = 0; i <= 15; i++)
      Serial.print(str[i]);
      Serial.print("\t");

      (err != true) ? shiftOutMSBFIRST(A, Shift_Clock, &str[0]) : pass();
      digitalWrite(Latch_Clock, HIGH);
      delay(10);
      digitalWrite(Latch_Clock, LOW);
      delay(1);
      
      str[6] = 1;
      (err != true) ? shiftOutMSBFIRST(A, Shift_Clock, &str[0]) : pass();
      digitalWrite(Latch_Clock, HIGH);
      delay(10);
      digitalWrite(Latch_Clock, LOW);
      delay(1);
      
      str[6] = 0;
      (err != true) ? shiftOutMSBFIRST(A, Shift_Clock, &str[0]) : pass();
      digitalWrite(Latch_Clock, HIGH);
      delay(10);
      digitalWrite(Latch_Clock, LOW);
      delay(1);
      

      Serial.println("~MSR~ complete");
      mt_cs2 = 0;
      err = false;
      
    }

    if ((str[0]=='N') && (str[1]=='O') && (str[2]=='D'))  //fixing nodes
    {
      Serial.println("~NOD~ indeed");
      NOD = Serial.parseInt();
      NOD --;
      if (Serial.available() > 0)
      {
        for (uint8_t i = 0; i<= 99; i++)
        {
          Serial.read();
        }
       
      }
      node[NOD] = 1;
      Serial.print(node[0]); Serial.print("\t"); Serial.print(node[1]); Serial.print("\t"); Serial.print(node[2]); Serial.print("\t");


      str[0] = 0;
      str[1] = node[2];
      str[2] = node[1];
      str[3] = node[0];
      str[4] = 0;
      str[5] = 0;
      str[6] = 0;                //mt_strobe
      str[7] = 0;
      str[8] = 0;
      str[9] = 0;
      str[10] = 0;
      str[11] = 0;
      str[12] = 0;
      str[13] = 0;
      str[14] = 0;
      str[15] = 0;

      for (uint8_t i = 0; i <= 15; i++)
      {
        Serial.print(str[i]);
      }
      Serial.print("\t");

      shiftOutMSBFIRST(A, Shift_Clock, &str[0]);
      digitalWrite(Latch_Clock, HIGH);
      delay(10);
      digitalWrite(Latch_Clock, LOW);
      delay(1);
      
      Serial.println("~NOD~ complete");
    }
    
  }
  
}
