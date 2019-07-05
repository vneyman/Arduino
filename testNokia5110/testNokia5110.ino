/*

  HelloWorld.pde
  
  "Hello World!" example code.
  
  >>> Before compiling: Please remove comment from the constructor of the 
  >>> connected graphics display (see below).
  
  Universal 8bit Graphics Library, https://github.com/olikraus/u8glib/
  
  Copyright (c) 2012, olikraus@gmail.com
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification, 
  are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice, this list 
    of conditions and the following disclaimer.
    
  * Redistributions in binary form must reproduce the above copyright notice, this 
    list of conditions and the following disclaimer in the documentation and/or other 
    materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
  CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  
  
*/


#include "U8glib.h"

// setup u8g object, please remove comment from one of the following constructor calls
// IMPORTANT NOTE: The following list is incomplete. The complete list of supported 
// devices with all constructor calls is here: https://github.com/olikraus/u8glib/wiki/device

U8GLIB_PCD8544 u8g(13, 11, 10, 9, 8);    // SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9, Reset = 8
unsigned int counterInt = 0;
char counterChar[10];

void setup(void) {
  Serial.begin(115200);
  Serial.println("starting test of Nokia 5110");

  //set font
  //u8g.setFont(u8g_font_profont12);
  //u8g.setFont(u8g_font_4x6);
  u8g.setFont(u8g_font_5x7);
  //u8g.setFont(u8g_font_5x8);
  //u8g.setFont(u8g_font_6x10);
  
  // flip screen, if required
  //u8g.setRot180();
  //u8g.setRot90();
  //u8g.setRot270();
  
  Serial.print(F("getMode():\t")); Serial.println(u8g.getMode());
  //pinMode(8, OUTPUT);
}

void loop(void) {
  // picture loop
  u8g.firstPage();  
  do {
    counter();
    draw();
  } while( u8g.nextPage() );
  
  // rebuild the picture after some delay
  delay(50);
}

void draw(void) {
  // graphic commands to redraw the complete screen should be placed here  
  
  u8g.drawStr( 0, 7, "Hello World!"); //(x,y) => (column,row)
  u8g.drawRFrame(0, 12, 84, 20, 3); //(x,y,width, height, radius of edge)
  u8g.drawStr( 3, 24, "Val's Nokia 5110"); //locations is x,y
  
  u8g.drawCircle(70, 36, 12, U8G_DRAW_ALL); //(x,y,radius, option)
  u8g.setPrintPos(65, 40); //x,y
  u8g.print(counterInt);
}

void counter(){
  counterInt++;

  if(counterInt > 300){
      counterInt = 0;
    }
  }
