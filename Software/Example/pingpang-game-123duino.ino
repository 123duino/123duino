#include "U8glib.h"

#define WIDTH 128
#define HEIGHT 64
#define CHAR_WIDTH 8
#define CHAR_HEIGHT 16

//U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE|U8G_I2C_OPT_DEV_0);
U8GLIB_SSD1306_128X64 u8g(6, 8, 7, 9, 4);  // SW SPI Com: SCK = 6, MOSI = 8, CS = 7, A0 = 9, RESET = 11 for 123duino
//Source code from https://github.com/MagicCube/game-platform-arduino/blob/master/pingpang-game.ino


int paddleSize = 3;
int paddleWidth = CHAR_WIDTH * paddleSize;
int ballRadius = 2;

int speakerPin = 12;        //For 123duino
const int buttonPin_R = 0;    // the number of the pushbutton pin for 123duino
const int buttonPin_L = 1;     // the number of the pushbutton pin for 123duino


bool gameOver;
int paddleX;
int paddleY;
double ballX;
double ballY;
double ballAccX;
double ballAccY;

void beep(){
    analogWrite(speakerPin, 1000);
    delay(50);
    analogWrite(speakerPin, 0);
}

bool hitTest()
{
    if (ballY >= paddleY && ballY <= paddleY + ballRadius * 2)
    {
        if (ballX >= paddleX && ballX < paddleX + paddleWidth)
        {
            ballAccY = -ballAccY;
            double pos = (ballX - (paddleX)) / paddleX;
            if (pos > 0.4 && pos < 0.6)
            {
                ballAccX = random(0, 8) / 10 - 0.4;
                ballAccY;
            }
            else if (pos <= 0.4)
            {
                ballAccX = ballAccX * (1 + random(0, 8) / 10);
                ballAccY *= 1.1;
            }
            else if (pos >= 0.6)
            {
                ballAccX = ballAccX * (1 + random(0, 8) / 10);
                ballAccY *= 1.1;
            }
            Serial.println(ballAccX);
            return true;
        }
    }
    return false;
}

double renderGameOver()
{
    u8g.setFont(u8g_font_unifont);
    u8g.setFontPosTop();
    u8g.drawStr((WIDTH - 9 * CHAR_WIDTH) / 2, 22, "GAME OVER");
}

void render(void)
{
    ballX += ballAccX;
    ballY += ballAccY;
    if (hitTest())
    {
        beep();
        return;
    }
    else if (ballY > HEIGHT - ballRadius)
    {
        // GAME OVER
        gameOver = true;
    }
    else if (ballY < 0)
    {
        // Hit ceiling
        ballY = 0;
        ballAccY = -ballAccY;
        ballAccX = ballAccX;
        beep();
    }
    
    if (ballX < 0)
    {
        ballX = 0;
        ballAccX = -ballAccX;
        beep();
    }
    else if (ballX > WIDTH - ballRadius)
    {
        ballX = WIDTH - ballRadius;
        ballAccX = -ballAccX;
        beep();
    }

    if (ballAccX > 1)
    {
        ballAccX = 0.5;
    }
    else if (ballAccX < -1)
    {
        ballAccX = -0.5;
    }

    u8g.drawStr(paddleX, paddleY, "xxxx");
    u8g.drawDisc(ballX, ballY, ballRadius);
}

void reset()
{
    paddleX = 6 * CHAR_WIDTH;
    paddleY = HEIGHT - CHAR_HEIGHT;
    ballX = WIDTH / 2;
    ballY = 0;
    ballAccX = random(0, 8) / 10 - 0.8;
    ballAccY = 0.33;
    gameOver = false;
}

void setup(void)
{
    Serial.begin(9600);
    Serial.println("Hello");
    
    // Setup display
    u8g.setColorIndex(1);
    u8g.setFont(u8g_font_unifont);
    u8g.setFontPosTop();
    
    // Setup left/right key
    pinMode(buttonPin_R, INPUT_PULLUP);
    pinMode(buttonPin_L, INPUT_PULLUP);
  
    //pinMode(buttonPin_R, INPUT);
    digitalWrite(buttonPin_R, HIGH);
    //pinMode(buttonPin_L, INPUT);
    digitalWrite(buttonPin_L, HIGH);
    attachInterrupt(digitalPinToInterrupt(buttonPin_L), []() {
        if (!gameOver)
        {
            paddleX = constrain(paddleX - CHAR_WIDTH, 0, WIDTH - paddleWidth);
        }
        else
        {
            reset();
        }
    }, FALLING);
    attachInterrupt(digitalPinToInterrupt(buttonPin_R), []() {
        if (!gameOver)
        {
            paddleX = constrain(paddleX + CHAR_WIDTH, 0, WIDTH - paddleWidth);
        }
        else
        {
            reset();
        }
    }, FALLING);

    // Reset variables
    reset();
}

void loop(void)
{
    u8g.firstPage();
    do
    {
        if (!gameOver)
        {
            render();
        }
        else
        {
            renderGameOver();
        }
    }
    while(u8g.nextPage());
}

