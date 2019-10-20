//LTC 2400 24bit ADC Converter Example code
//By Razvan C.

int Debug;

// Variables needed for conversion
  byte DataRead[31];     // This is an array that will store the 32 bits of data read from the ADC  
  unsigned long counts;  // This will store the counts value read from the ADC
  float volt;            // This will store the final voltage value, that is converted from the counts; 32 bits
  float v_ref;       // Reference Voltage

//  Variables needed for ADC
  bool sign;  // High if Vin is positive, Low  if Vin negative
  bool EOC;   // End Of Conversion
  bool EXR;   // Extended Range bit; Low if Vin is within 0 and Vref. High otherwise

#define LTC_CS   A4       // LTC2400 Chip Select Pin, Active Low
#define LTC_SDO  14      // LTC2400 SDO Select Pin  
#define LTC_SCK  15       // LTC2400 SCK Select Pin  
#define F0 A5             // LTC2400 50Hz/60Hz Filter Select; Low for 60Hz rejection, High for 50Hz

void setup() 
{
// Pin settings
  pinMode (LTC_CS,OUTPUT);  // When CS is High, any CLK pulses are ignored
  digitalWrite(LTC_CS,HIGH);

  pinMode(F0, OUTPUT); 
  digitalWrite(F0, HIGH);  // Set for 50 Hz Filter rejection; Set LOW for 60 Hz rejection

  pinMode (LTC_SDO,INPUT);
  pinMode (LTC_SCK,OUTPUT); //Data is captured on rising edge of CLK, and shifted out on falling edge 
  digitalWrite(LTC_SCK,LOW); 

  Serial.begin(9600);

  v_ref = 5.0;
  Debug  = 1;   // THis turn On Debug messages

  if (Debug == 1)
  {
   Serial.println("LTC2400 ADC Test");
  }
 
 }

 /********************************************************************/

 void loop() 
 {
  delay(5);
  digitalWrite(LTC_CS,LOW);  // Start ADC conversion
  delay(170);  // Conversion @ 50Hz rejection takes 160ms. Wait 170 just to make sure everything is settled
  
  if (digitalRead(LTC_SDO) == LOW)   // if it's low, the ADC has finished the conversion and data can  be read from it
   {
     Serial.println("Raw bits are:");
     for (int i=0; i<32; i++)
      {
        delay(5);
        digitalWrite(LTC_SCK,HIGH);
        delay(5);
        DataRead[i] = digitalRead(LTC_SDO);  //Data from ADC is shifted out MSB first, so DataRead[0] will have the MSB, DataRead[23] will have the LSB
        digitalWrite(LTC_SCK,LOW);
        delay(5);        
        Serial.print(DataRead[i]);
        
      }  
       counts = 0x00000000;

       counts |= DataRead[27]; // Convert bool array, to form counts reading. MSB from DataRead[0] goes to location 24 of the "counts" which is a 32 bit variable
       counts |= DataRead[26] << 1;
       counts |= DataRead[25] << 2;
       counts |= DataRead[24] << 3;
       counts |= DataRead[23] << 4;
       counts |= DataRead[22] << 5;
       counts |= DataRead[21] << 6;
       counts |= DataRead[20] << 7;
       counts |= DataRead[19] << 8;
       counts |= DataRead[18] << 9;
       counts |= DataRead[17] << 10;
       counts |= DataRead[16] << 11;
       counts |= DataRead[15] << 12;
       counts |= DataRead[14] << 13;
       counts |= DataRead[13] << 14;
       counts |= DataRead[12] << 15;
       counts |= DataRead[11] << 16;
       counts |= DataRead[10] << 17;
       counts |= DataRead[9] << 18;
       counts |= DataRead[8] << 19;
       counts |= DataRead[7] << 20;
       counts |= DataRead[6] << 21;
       counts |= DataRead[5] << 22;
       counts |= DataRead[4] << 23;   
       
         // Yeah, I know it's weird and if you look at it it doesn't make sense, but apparently this is the only way I could make the IDE spit out the results I needed.     
         

       counts &= 0x007FFFFF;  // Somehow, without this mask, despite having it set to "0" at the srat, I get bits 31-28 all 1s   
     
     Serial.println("");
     Serial.println("Result is:");
     Serial.println(counts, BIN);
     Serial.println(counts);
     volt = counts * 0.0000002980;
     Serial.println(volt, 6);

     digitalWrite(LTC_CS,HIGH);  // Put ADC to Sleep
     delay(10);
   }
   
 }

 
 
