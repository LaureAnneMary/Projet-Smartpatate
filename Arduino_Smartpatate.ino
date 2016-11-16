//****************************************************************************************
// Illutron take on Disney style capacitive touch sensor using only passives and Arduino
// Dzl 2012
//****************************************************************************************


//                              10n
// PIN 9 --[10k]-+-----10mH---+--||-- OBJECT
//               |            |
//              3.3k          |
//               |            V 1N4148 diode
//              GND           |
//                            |
//Analog 0 ---+------+--------+
//            |      |
//          100pf   1MOmhm
//            |      |
//           GND    GND



#define SET(x,y) (x |=(1<<y))        //-Bit set/clear macros
#define CLR(x,y) (x &= (~(1<<y)))           // |
#define CHK(x,y) (x & (1<<y))               // |
#define TOG(x,y) (x^=(1<<y))                //-+



#define N 160  //How many frequencies

float results[N];            //-Filtered result buffer
float freq[N];            //-Filtered result buffer
int sizeOfArray = N;

int ledRouge = 3;
int ledVerte = 2;
int buzzer = 10;
int Max = 0;
int indiceBoucle = 0;
   

void setup()
{
  
  
  TCCR1A=0b10000010;        //-Set up frequency generator
  TCCR1B=0b00011001;        //-+
  ICR1=110;
  OCR1A=55;

  pinMode(9,OUTPUT);        //-Signal generator pin
  pinMode(8,OUTPUT);        //-Sync (test) pin
  pinMode(ledVerte, OUTPUT);
  Serial.begin(115200);

  for(int i=0;i<N;i++)      //-Preset results
    results[i]=0;         //-+
}

void loop()
{
  unsigned int d;

  int counter = 0;
  for(unsigned int d=0;d<N;d++)
  {
    int v=analogRead(0);    //-Read response signal
    CLR(TCCR1B,0);          //-Stop generator
    TCNT1=0;                //-Reload new frequency
    ICR1=d;                 // |
    OCR1A=d/2;              //-+
    SET(TCCR1B,0);          //-Restart generator

    results[d]=results[d]*0.5+(float)(v)*0.5; //Filter results
   
    freq[d] = d;
    
 //   plot(v,0);              //-Display
 //   plot(results[d],1);
  // delayMicroseconds(1);
  }

Effet(results);
//PlottArray(1,freq,results); 
 

  TOG(PORTB,0);            //-Toggle pin 8 after each sweep (good for scope)
}


void Effet(float freq12[160])
{ 
  Max = 0;
  for(indiceBoucle=0; indiceBoucle <158; indiceBoucle++)
    {
      if(freq12[indiceBoucle+1]>freq12[Max])
        {
          Max = indiceBoucle+1;
        
        } 
          
      
    }
  
  
  if (Max <= 38)
  {
    digitalWrite(ledVerte, LOW);  
  }
  else if (Max > 38 && Max<55)
  {
    digitalWrite(ledVerte, HIGH);
  }
  if ( Max>55 ) {
    digitalWrite(ledVerte, HIGH);
    delay (500);
    digitalWrite(ledVerte, LOW);
    delay (500);
    digitalWrite(ledVerte, HIGH);
    
    
  }
  Serial.println(Max);

  }

