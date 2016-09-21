#include <SPI.h>
#include <menu.h>//menu macros and objects
#include <genericKeyboard.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <menuGFX.h>
//-------KEYS--------
#define keyboard_AnalogInput A6
#define btnUP     menu::downCode
#define btnDOWN   menu::upCode
#define btnLEFT   menu::escCode
#define btnENTER  menu::enterCode
#define btnNONE   -1
//-------TFT---------
#define tftCS 10
#define dc    8
#define rst   7
//-------VARS--------
int old_button;
bool runMenu=false;

byte futesStart = 2;
byte futesStop = 60;
byte tartasStart = 2;
byte tartasStop = 150;

byte temperature = 70;
byte histeresis = 2;

byte menuTimeout = 60;
//-------------------

Adafruit_ST7735 tft(tftCS, dc, rst);
menuGFX gfx(tft);

//aux function
bool nothing() {}

bool pauseMenu() {
  runMenu=false;
  return true;
  }

//---------MENU TREE---------

CHOOSE(temperature,temp,"Homerseklet",
    VALUE("60",60,updTemp),
    VALUE("65",65,updTemp),
    VALUE("70",70,updTemp),
    VALUE("75",75,updTemp),
    VALUE("80",80,updTemp)
);

CHOOSE(histeresis,temp,"Histeresis",
    VALUE("2",2,updTemp),
    VALUE("4",4,updTemp),
    VALUE("6",6,updTemp)
);   

MENU(timerMenu,"Idozites",
    FIELD(futesStart,"Futes Start","s",1,20,1,1,nothing),
    FIELD(futesStop,"Futes Stop","s",1,200,2,1,nothing),
    FIELD(tartasStart,"Tartas Start","s",1,20,1,1,updTimer),
    FIELD(tartasStop,"Tartas Stop","s",1,500,10,1,updTimer)
);

MENU(options,"Beallitasok",
    OP("MenuTimer",updOptions),
    OP("Frissit seb",updOptions),
    OP("Mentes",updOptions)
);

MENU(mainMenu,"Main menu",
    SUBMENU(timerMenu),
    SUBMENU(temp),
    SUBMENU(options),
    OP("Exit",pauseMenu)
);

//---------------------------

int readKeyboard() {
  int button, button2, pressed_button;  
  button = getButton();
  if (button != old_button) {
      delay(50);        // debounce
      button2 = getButton();

      if (button == button2) {
         old_button = button;
         pressed_button = button;
         if(button != 0) {
           if(button == 1) return btnLEFT;
           if(button == 2) return btnUP;
           if(button == 3) return btnDOWN;
           if(button == 4) return btnENTER;
         }
      }
  }else{
    return btnNONE;
  }
}

genericKeyboard mykb(readKeyboard);

int getButton() {
  int i, z, button;
  int sum = 0;

  for (i=0; i < 4; i++) {
     sum += analogRead(keyboard_AnalogInput);
  }
  z = sum / 4;
  if (z > 1000) button = 0;                                           
  else if (z >= 0 && z < 20) button = 1; //LEFT                    
  else if (z > 50 && z < 100) button = 2; //UP                
  else if (z > 210 && z < 250) button = 3; //DOWN                
  else if (z > 150 && z < 190) button = 4; //RIGHT           
  else button = 0;

  return button;
}

void mainScreen() {
  runMenu=false;
  tft.fillScreen(BLACK); 
}

void updTimer() {}

void updTemp() {}

void updOptions() {}

void saveEEPROM() {}

void setup() {
  Serial.begin(9600);
  SPI.begin();
  tft.initR(INITR_BLACKTAB);
  tft.setRotation(1);
  tft.setTextWrap(false);
  tft.setTextColor(ST7735_RED,ST7735_BLACK);
  tft.setTextSize(2);
  gfx.resX*=2;//update resolution after font size change
  gfx.resY*=2;//update resolution after font size change
  gfx.maxX=12;
  gfx.maxY=6;
  gfx.bgColor=SILVER;
  tft.fillScreen(GREEN);

}

void loop() {
  if (runMenu) mainMenu.poll(gfx,mykb);
  else if (readKeyboard()==menu::enterCode) {
    runMenu=true;
    mainMenu.redraw(gfx,mykb);
  } else {
    mainScreen();
  }
  
}
