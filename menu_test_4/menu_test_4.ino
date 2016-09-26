#include <SPI.h>
#include <EEPROMex.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <menu.h>//menu macros and objects
#include <genericKeyboard.h>
#include <menuFields.h>
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
bool screenCleared = 0;

byte futesStart = 2;
byte futesStop = 60;
byte tartasStart = 2;
byte tartasStop = 30;
byte temperature = 70;
byte histeresis = 2;

byte menuTimeout = 60;
byte activeProfil = 1;
//-------EEPROM Adresses------------
byte addrfStart1 = 10;
byte addrfStop1 = 20;
byte addrtStart1 = 30;
byte addrtStop1 = 40;
byte addrTemp1 = 50;
byte addrHis1 = 60;
byte addrMenuhide = 70;
byte addrProfil = 80;

byte addrfStart2 = 90;
byte addrfStop2 = 100;
byte addrtStart2 = 110;
byte addrtStop2 = 120;
byte addrTemp2 = 130;
byte addrHis2 = 140;

byte addrfStart3 = 150;
byte addrfStop3 = 160;
byte addrtStart3 = 170;
byte addrtStop3 = 180;
byte addrTemp3 = 190;
byte addrHis3 = 200;

//-------------------

Adafruit_ST7735 tft(tftCS, dc, rst);
menuGFX gfx(tft);

/* Colors: BLACK, WHITE, RED, YELLOW, CYAN, MAGENTA, BRIGHT_RED, LIGHT_GREY, 
DARK_GREY */

//aux function
bool nothing() {}

bool pauseMenu() {
  runMenu=false;
  screenCleared = 0;
  return true;
  }

bool loadProfil1() {
 activeProfil = 1;
 EEPROM.write(addrProfil,activeProfil);
 futesStart = EEPROM.read(addrfStart1);
 futesStop = EEPROM.read(addrfStop1);
 tartasStart = EEPROM.read(addrtStart1);
 tartasStop = EEPROM.read(addrtStop1);
 temperature = EEPROM.read(addrTemp1);
 histeresis = EEPROM.read(addrHis1);
 return true;
}
bool loadProfil2() {
 activeProfil = 2;
 EEPROM.write(addrProfil,activeProfil);
 futesStart = EEPROM.read(addrfStart2);
 futesStop = EEPROM.read(addrfStop2);
 tartasStart = EEPROM.read(addrtStart2);
 tartasStop = EEPROM.read(addrtStop2);
 temperature = EEPROM.read(addrTemp2);
 histeresis = EEPROM.read(addrHis2);
 return true;
}
bool loadProfil3() {
 activeProfil = 3;
 EEPROM.write(addrProfil,activeProfil);
 futesStart = EEPROM.read(addrfStart3);
 futesStop = EEPROM.read(addrfStop3);
 tartasStart = EEPROM.read(addrtStart3);
 tartasStop = EEPROM.read(addrtStop3);
 temperature = EEPROM.read(addrTemp3);
 histeresis = EEPROM.read(addrHis3);
 return true;
}

//---------Functions---------
void mainScreen() {
  runMenu=false;
  tft.setTextColor(ST7735_GREEN,ST7735_BLACK);
  tft.setCursor(0, 0); 
  tft.print("Profil: ");
  tft.println(activeProfil);
  tft.setCursor(0, 20); 
  tft.print("fSta: ");
  tft.println(futesStart);
  tft.setCursor(0, 40); 
  tft.print("fSto: ");
  tft.println(futesStop);
  tft.setCursor(0, 60); 
  tft.print("tSto: ");
  tft.println(tartasStart);
  tft.setCursor(0, 80); 
  tft.print("tSto: ");
  tft.println(tartasStop);
}

bool saveProfil1() {
  EEPROM.write(addrProfil,activeProfil);
  EEPROM.write(addrMenuhide,menuTimeout);
  EEPROM.write(addrfStart1,futesStart);
  EEPROM.write(addrfStop1,futesStop);
  EEPROM.write(addrtStart1,tartasStart);
  EEPROM.write(addrtStop1,tartasStop);
  EEPROM.write(addrTemp1,temperature);
  EEPROM.write(addrHis1,histeresis);
  return true;
  }

bool saveProfil2() {
  EEPROM.write(addrProfil,activeProfil);
  EEPROM.write(addrMenuhide,menuTimeout);
  EEPROM.write(addrfStart2,futesStart);
  EEPROM.write(addrfStop2,futesStop);
  EEPROM.write(addrtStart2,tartasStart);
  EEPROM.write(addrtStop2,tartasStop);
  EEPROM.write(addrTemp2,temperature);
  EEPROM.write(addrHis2,histeresis);
  return true;
  }

bool saveProfil3() {
  EEPROM.write(addrProfil,activeProfil);
  EEPROM.write(addrMenuhide,menuTimeout);
  EEPROM.write(addrfStart3,futesStart);
  EEPROM.write(addrfStop3,futesStop);
  EEPROM.write(addrtStart3,tartasStart);
  EEPROM.write(addrtStop3,tartasStop);
  EEPROM.write(addrTemp3,temperature);
  EEPROM.write(addrHis3,histeresis);
  return true;
  }

//---------MENU TREE---------

CHOOSE(temperature,temp,"Temp",
    VALUE("60",60,nothing),
    VALUE("65",65,nothing),
    VALUE("70",70,nothing),
    VALUE("75",75,nothing),
    VALUE("80",80,nothing)
);

CHOOSE(histeresis,hister,"Toler.",
    VALUE("2",2,nothing),
    VALUE("4",4,nothing),
    VALUE("6",6,nothing)
);   

MENU(timerMenu,"Idozites",
    FIELD(futesStart,"fStart","s",1,20,1,1,nothing),
    FIELD(futesStop,"fStop","s",1,200,10,1,nothing),
    FIELD(tartasStart,"tStart","s",1,20,1,1,nothing),
    FIELD(tartasStop,"tStop","p",1,30,10,1,nothing)
);

MENU(loadProfil,"Profilok",
    OP("Profil 1",loadProfil1),
    OP("Profil 2",loadProfil2),
    OP("Profil 3",loadProfil3)
);

MENU(saveProfil,"Mentes",
    OP("Profil 1",saveProfil1),
    OP("Profil 2",saveProfil2),
    OP("Profil 3",saveProfil3)
);

MENU(options,"Beallitasok",
    SUBMENU(loadProfil),
    OP("MenuTimer",nothing),
    OP("Frissit seb",nothing),
    SUBMENU(saveProfil),
    OP("Firmware",nothing)
);

MENU(subMenu,"Homerseklet",
    SUBMENU(temp),
    SUBMENU(hister)
);

MENU(mainMenu,"Main menu",
    SUBMENU(timerMenu),
    SUBMENU(subMenu),
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
  gfx.maxX=13;
  gfx.maxY=7;
  gfx.bgColor=RGB565(125,125,125);
  tft.fillScreen(BLACK);
  menuTimeout = EEPROM.read(addrMenuhide);
  activeProfil = EEPROM.read(addrProfil);
  if (activeProfil == 1 ) loadProfil1();
  if (activeProfil == 2 ) loadProfil2();
  if (activeProfil == 3 ) loadProfil3();
}

void loop() {
  if (runMenu) { 
    mainMenu.poll(gfx,mykb);
  }  else if (readKeyboard()==menu::enterCode) {
    runMenu=true;
    tft.setTextColor(ST7735_RED,ST7735_BLACK);
    mainMenu.redraw(gfx,mykb);
    mainMenu.poll(gfx,mykb);
  } else {
    if (screenCleared == 0) {
      tft.fillScreen(BLACK); 
      screenCleared = 1;
    }
    mainScreen();
  }
  
}
