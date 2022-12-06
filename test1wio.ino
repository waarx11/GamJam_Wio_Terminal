// TODO :
// Mur
// Collision (écran+ mur)
// déplacement fantôme

// F3ATUR3 :
// Faux/vrai mur (une fois découvert effacer) + espace en dehors map
// Un fantome au bout d'un temps aléatoire glitch à travers les murs (redraw vrai mur à chaque loop)

#include"TFT_eSPI.h"
TFT_eSPI tft;

#define TFT_BLACK       0x0000      /*   0,   0,   0 */
#define TFT_NAVY        0x000F      /*   0,   0, 128 */
#define TFT_DARKGREEN   0x03E0      /*   0, 128,   0 */
#define TFT_DARKCYAN    0x03EF      /*   0, 128, 128 */
#define TFT_MAROON      0x7800      /* 128,   0,   0 */
#define TFT_PURPLE      0x780F      /* 128,   0, 128 */
#define TFT_OLIVE       0x7BE0      /* 130, 128,   0 */
#define TFT_LIGHTGREY   0xC618      /* 192, 192, 192 */
#define TFT_DARKGREY    0x7BEF      /* 128, 128, 128 */
#define TFT_BLUE        0x001F      /*   0,   0, 255 */
#define TFT_GREEN       0x07E0      /*   0, 255,   0 */
#define TFT_CYAN        0x07FF      /*   0, 255, 255 */
#define TFT_RED         0xF800      /* 255,   0,   0 */
#define TFT_MAGENTA     0xF81F      /* 255,   0, 255 */
#define TFT_YELLOW      0xFFE0      /* 255, 255,   0 */
#define TFT_WHITE       0xFFFF      /* 255, 255, 255 */
#define TFT_ORANGE      0xFDA0      /* 255, 180,   0 */

#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

#define PAS 10
#define THINCKNESS 15
#define W 15
#define H 15
#define SCREEN_HEIGHT 240
#define SCREEN_WIDTH 320

int16_t COLORPAC=TFT_YELLOW;
int16_t COLORF1=TFT_RED;
int16_t COLORF2=TFT_GREEN ;
int16_t COLORF3=TFT_PURPLE;
int16_t COLORWALL=TFT_BLUE ;

int16_t wallsInside[10][4]={
        {60, 65, 50, THINCKNESS},
        {135, 65, 50, THINCKNESS},
        {60, 240, 50, THINCKNESS},
        {135, 240, 50, THINCKNESS},
        {170, 110, THINCKNESS, 100},
        {70, 150, 80, THINCKNESS},
        {115, 195, THINCKNESS, THINCKNESS},
        {70, 195, THINCKNESS, THINCKNESS},
        {115, 105, THINCKNESS, THINCKNESS},
        {70, 105, THINCKNESS, THINCKNESS}
    };

int16_t wallsOutside[4][4] = {
    {H*2, W*2, H*2, SCREEN_WIDTH-(W*2)},
    {H*2, W*2, SCREEN_HEIGHT-(H*2), W*2},
    {SCREEN_HEIGHT-(H*2), SCREEN_WIDTH-(W*2), SCREEN_HEIGHT-(H*2), W*2},
    {SCREEN_HEIGHT-(H*2), SCREEN_WIDTH-(W*2), H*2, SCREEN_WIDTH-(W*2)}
};

int16_t STATE=0; //0 - BEGIN | 1 - START | 2 - LOOSE | 3 - WIN

int16_t x=(W*2)+5;
int16_t y=(H*2)+5;

int16_t posFantome[3][2] = {
      {SCREEN_HEIGHT-(H*2)-18, (W*2)+5},
      {SCREEN_HEIGHT-(H*2)-18, SCREEN_WIDTH-(W*2)-18},
      {H*5-40, SCREEN_WIDTH-50}
};

void right()
{   
    bouger(RIGHT);
}

void left()
{   
    bouger(LEFT);
}

void up()
{   
    bouger(UP);
}

void down()
{    
    bouger(DOWN);
}
/*
H*2, W*2                        H*2, SCREEN_WIDTH-(W*2)

SCREEN_HEIGHT-(H*2), W*2        SCREEN_HEIGHT-(H*2), SCREEN_WIDTH-(W*2)

*/

void bouger(int16_t direc){
    tft.fillRect(x,y,W,H,TFT_BLACK); //effacer
    bool bouge=false;
    for(int i=0;i<10;i++){
        if (x < wallsInside[i][0] + wallsInside[i][2] &&
            x + W > wallsInside[i][0] &&
            y < wallsInside[i][1]+ wallsInside[i][3] &&
            H + y > wallsInside[i][1]) {
            tft.fillRect(x,y,W,H,COLORPAC);
            return ;
        }

    // collision détectée !
    }
    if(direc==DOWN  && x-PAS>W*2 ){
        x-=PAS;
        bouge=true;
    }
    else if(direc==UP && x+PAS<SCREEN_HEIGHT-(H*2)-5 ){
        x+=PAS;
        bouge=true;
    }
    else if(direc==RIGHT && y+PAS <SCREEN_WIDTH-(W*2)-5){
        y+=PAS;
        bouge=true;
    }
    else if(direc==LEFT && y-PAS>H*2 ) {
        y-=PAS;
        bouge=true;
    }
    if(bouge){
        fuirPac(1,COLORF2,direc);
        if(abs(y-posFantome[1][1])<=20 && abs(x-posFantome[1][0])<=20){
            STATE=3;
        }
        
    }
    tft.fillRect(x,y,W,H,COLORPAC);
}

void bougerFantome(int16_t direc,int f, int16_t colorF){
    tft.fillRect(posFantome[f][0],posFantome[f][1],W,H,TFT_BLACK); //effacer
    if(f==1){
     for(int i=0;i<10;i++){
        if (posFantome[f][0] < wallsInside[i][0] + wallsInside[i][2] &&
            posFantome[f][0]+ W > wallsInside[i][0] &&
            posFantome[f][1] < wallsInside[i][1]+ wallsInside[i][3] &&
            H + posFantome[f][1] > wallsInside[i][1]) {
            tft.fillRect(posFantome[f][0],posFantome[f][1],W,H,colorF);
            return ;
        }
    }
    // collision détectée !
    }
    if(direc==DOWN  && posFantome[f][0]-PAS>W*2 ){
        posFantome[f][0]-=PAS;
    }
    else if(direc==UP && posFantome[f][0]+PAS<SCREEN_HEIGHT-(H*2)-15  ){
        posFantome[f][0]+=PAS;
    }
    else if(direc==RIGHT && posFantome[f][1]+PAS <SCREEN_WIDTH-(W*2)-15){
        posFantome[f][1]+=PAS;
    }
    else if(direc==LEFT && posFantome[f][1]-PAS>H*2 ) {
        posFantome[f][1]-=PAS;
    }
    tft.fillRect(posFantome[f][0],posFantome[f][1],W,H,colorF);
}

void pousuivrePac(int f, int colorF){
    if(abs(y-posFantome[f][1])<=20 && abs(x-posFantome[f][0])<=20){
        STATE=2;
    }
    if(posFantome[f][1]>y )
        bougerFantome(LEFT,f, colorF);
    else if(posFantome[f][1]!=y)
        bougerFantome(RIGHT,f, colorF);
    if(posFantome[f][0]>x  )
        bougerFantome(DOWN,f, colorF);
    else if(posFantome[f][0]!=x)
        bougerFantome(UP,f, colorF);
}

void fuirPac(int f, int colorF,int direc){
   if(direc==RIGHT)
        bougerFantome(LEFT,f, colorF);
    else if(direc==LEFT) 
        bougerFantome(RIGHT,f, colorF);
    else if(direc==UP)
        bougerFantome(DOWN,f, colorF);
    else
        bougerFantome(UP,f, colorF);
}

void randomPac(int f, int colorF){
    bougerFantome(random(1, 5),f, colorF);
    if(abs(y-posFantome[f][1])<=20 && abs(x-posFantome[f][0])<=20){
        STATE=random(2,4);
    }
}

void createGhost()
{    
    tft.fillRect(posFantome[0][0],posFantome[0][1],W,H,COLORF1);
    tft.fillRect(posFantome[1][0],posFantome[1][1],W,H,COLORF2);
    tft.fillRect(posFantome[2][0],posFantome[2][1],W,H,COLORF3);
}

void createPacman(){
    tft.fillRect(x,y,W,H,COLORPAC);
}


void dessinerCadre(){
    for(int16_t i=0; i<4; i++){
      tft.drawLine(wallsOutside[i][0],wallsOutside[i][1],wallsOutside[i][2],wallsOutside[i][3],COLORWALL);
    }
}

void dessinerMurs(){
    for(int16_t i=0; i<10; i++){
      tft.drawRect(wallsInside[i][0],wallsInside[i][1],wallsInside[i][2],wallsInside[i][3],COLORWALL);
    }
}

void initialisation(){
    dessinerCadre();
    dessinerMurs();
    createGhost();
    createPacman();
}

void setup() {
    tft.begin();
    tft.setRotation(0);
    Serial.begin(115200);
    tft.setTextSize(4);
    pinMode(WIO_5S_UP, INPUT_PULLUP);
    pinMode(WIO_5S_DOWN, INPUT_PULLUP);
    pinMode(WIO_5S_LEFT, INPUT_PULLUP);
    pinMode(WIO_5S_RIGHT, INPUT_PULLUP);
    pinMode(WIO_5S_PRESS, INPUT_PULLUP);  
    pinMode(WIO_KEY_C, INPUT_PULLUP);
    
    waitInput("Press Start",25);

    attachInterrupt(digitalPinToInterrupt(WIO_5S_UP), down, FALLING);
    attachInterrupt(digitalPinToInterrupt(WIO_5S_DOWN), up, FALLING);
    attachInterrupt(digitalPinToInterrupt(WIO_5S_LEFT), right, FALLING);
    attachInterrupt(digitalPinToInterrupt(WIO_5S_RIGHT), left, FALLING);

    initialisation();
} 

void waitInput(String msg, int16_t y){
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);
    tft.drawString(msg,y,100);
    while (STATE != 1)
    {
        if (digitalRead(WIO_KEY_C) == LOW) {
            STATE=1;
                               
        }
    }
    tft.fillScreen(TFT_BLACK);
    tft.setRotation(0);
}

void loop() {
    /*-------------BEGIN-----------------*/
    if(STATE == 1){
        dessinerCadre();
        dessinerMurs();
        pousuivrePac(0, COLORF1);
        randomPac(2, COLORF3);
    }
    /*-------------PERDU-----------------*/
    if(STATE == 2) {
        waitInput("Game Over",50);
        NVIC_SystemReset();
    }
    /*-------------WIN-----------------*/
    if(STATE == 3){
        waitInput("You Win",75);
        NVIC_SystemReset();
    }
    delay(400);
}
