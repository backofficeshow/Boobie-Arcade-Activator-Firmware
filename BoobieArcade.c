//ARCADE SWITCH
//Andrew Armstrong 19/11/2016

#include <16F688.h>
// C Optimised for the CCS PIC Compiler
#fuses INTRC_IO,NOWDT,PUT, NOPROTECT,NOMCLR,NOFCMEN,BROWNOUT_SW//NOMCLR
#use delay(clock=8000000) //Not using the crystal

#define USER_PINS 12
#define high_start 0x32      //For timer was 32 for about a second
#define long_start 0x24C
#define deBounced 0x07

unsigned int32 high_count = high_start;            //For timer
unsigned int32 long_count = long_start;            //For long timer
unsigned char tick = 0;
unsigned char longtick = 0;
unsigned char buttonPressed = 0;

void setpin(BYTE pin, BYTE value)
{
   switch (pin)
   {
      case 0: if (value != 0) {output_high(45);}else{output_low(45);} break;
      case 1: if (value != 0) {output_high(44);}else{output_low(44);} break;
      //case 2: if (value != 0) {output_high(43);}else{output_low(43);} break;
      //case 3: if (value != 0) {output_high(61);}else{output_low(61);} break;
      //case 4: if (value != 0) {output_high(60);}else{output_low(60);} break;
      case 5: if (value != 0) {output_high(59);}else{output_low(59);} break;
      case 6: if (value != 0) {output_high(40);}else{output_low(40);} break;
      case 7: if (value != 0) {output_high(41);}else{output_low(41);} break;
      case 8: if (value != 0) {output_high(42);}else{output_low(42);} break;
      case 9: if (value != 0) {output_high(56);}else{output_low(56);} break;
      case 10: if (value != 0) {output_high(57);}else{output_low(57);} break;
      case 11: if (value != 0) {output_high(58);}else{output_low(58);} break;
   }
}

BYTE readpin(BYTE pin)
{
   switch (pin)
   {
      case 0: return(input(45)); break;
      case 1: return(input(44)); break;
      case 2: return(input(43)); break;
      //case 3: return(input(61)); break;
      //case 4: return(input(60)); break;
      case 5: return(input(59)); break;
      case 6: return(input(40)); break;
      case 7: return(input(41)); break;
      case 8: return(input(42)); break;
      case 9: return(input(56)); break;
      case 10: return(input(57)); break;
      case 11: return(input(58)); break;
   }
}

resetClock()
{
   high_count = high_start;
}

resetLongClock()
{
   long_count = long_start;
}

#int_rtcc
clock_isr()
{  
   high_count --;      
   if (high_count == 0)
   {
      resetClock();
      tick=1;
   }
   
   long_count --;
   if (long_count == 0)
   {
      resetLongClock();
      longtick=1;
   }
   if (readpin(0) == 0)
   {
      if (buttonPressed < deBounced)
      {
         buttonPressed++;  //buttonPressed will contain a cycle value useful for debugging
      }
   }
   return 0;
}

waittick()
{
   resetClock();
   tick = 0;
   while(tick == 0)
   {
      //Stay busy in here!
   }
}

waitLongtick()
{
   resetLongClock();
   longtick = 0;
   while(longtick == 0)
   {
      //Stay busy in here!
   }
}

void alloffImmediate()
{
   setpin(1,0);
   setpin(5,0); 
   setpin(6,0); 
   setpin(7,0); 
   setpin(8,0);
   setpin(9,0); 
   setpin(10,0);
   setpin(11,0);
}

void alloff()
{
   setpin(1,0); waitLongtick();
   setpin(5,0); waitLongtick();
   setpin(6,0); waitLongtick();
   setpin(7,0); waitLongtick();
   setpin(8,0); waitLongtick();
   setpin(9,0); waitLongtick();
   setpin(10,0); waitLongtick();
   setpin(11,0); waitLongtick();
}

void allon()
{
   setpin(1,1); waitLongtick();
   setpin(5,1); waitLongtick();
   setpin(6,1); waitLongtick();
   setpin(7,1); waitLongtick();
   setpin(8,1); waitLongtick();
   setpin(9,1); waitLongtick();
   setpin(10,1); waitLongtick();
   setpin(11,1); waitLongtick();
}

void main()
{
   unsigned int16 status=0;
   
   set_rtcc(0);
   setup_counters(RTCC_INTERNAL, RTCC_DIV_64);
   enable_interrupts(INT_RDA);
   enable_interrupts(INT_RTCC);
   enable_interrupts(GLOBAL);

   alloffImmediate();

   while (TRUE)
   {
   
  
      if (buttonPressed == deBounced)
      {
         if (status == 0)
         {
            status = 1; //We are going to turn all the outputs on
            allon();
         }
         else
         {
            status = 0; //We are going to turn all the outputs off
            alloff();
         }
         while (readpin(0) == 0)
         {
            //lets just hang here until the switch is released!
         }
         buttonPressed = 0x00;
      }
   }   
} 

