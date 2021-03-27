#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SPI.h>

#define TFT_CS 10
#define TFT_RST 9
#define TFT_DC 8

#define PIEZO_BUZZER 6
#define START_PIN 5
#define END_PIN 4
#define FAILED_PIN 3

enum GameState
{
  IN_PROGRESS,
  FAILED,
  SUCCEEDED
};

GameState gameState = GameState::FAILED;

int currentEndPin = 0;
int currentScore = 0;
int maxScore = 0;

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

void updateScreen()
{
  tft.setRotation(3);
  tft.setTextWrap(false);
  tft.fillScreen(ST7735_BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(ST7735_RED);
  tft.setTextSize(2);
  tft.println("");
  tft.println("WireBuzz Game");
  tft.println("");
  tft.print("You");

  switch (gameState)
  {
  case GameState::FAILED:
    tft.println(" failed");
    break;
  case GameState::IN_PROGRESS:
    tft.println("'re playing");
    break;
  case GameState::SUCCEEDED:
    tft.println(" succeeded");
    break;

  default:
    break;
  }

  tft.println("");
  tft.print("Current: ");
  tft.println(currentScore);
  tft.print("Highest: ");
  tft.println(maxScore);
}

void startNewGame()
{
  gameState = GameState::IN_PROGRESS;
  updateScreen();
}

void setup()
{
  pinMode(PIEZO_BUZZER, OUTPUT);
  pinMode(START_PIN, INPUT_PULLUP);
  pinMode(END_PIN, INPUT_PULLUP);
  pinMode(FAILED_PIN, INPUT_PULLUP);

  tft.initR(INITR_BLACKTAB);

  tft.fillScreen(ST7735_BLACK);

  updateScreen();
}

void loop()
{
  switch (gameState)
  {
  case GameState::IN_PROGRESS:
    if (!digitalRead(currentEndPin))
    {
      gameState = GameState::SUCCEEDED;
      tone(PIEZO_BUZZER, 440, 200);
      delay(200);
      tone(PIEZO_BUZZER, 400, 200);
      delay(200);
      tone(PIEZO_BUZZER, 380, 200);
      delay(200);
      currentScore++;
      if (currentScore > maxScore)
      {
        maxScore = currentScore;
      }
      updateScreen();
    }
    else if (!digitalRead(FAILED_PIN))
    {
      gameState = GameState::FAILED;
      tone(PIEZO_BUZZER, 380, 200);
      delay(200);
      tone(PIEZO_BUZZER, 400, 200);
      delay(200);
      tone(PIEZO_BUZZER, 440, 200);
      delay(200);
      currentScore = 0;
      updateScreen();
    }
    break;

  case GameState::FAILED:
  case GameState::SUCCEEDED:
    if (!digitalRead(START_PIN))
    {
      currentEndPin = END_PIN;
      startNewGame();
    }
    else if (!digitalRead(END_PIN))
    {
      currentEndPin = START_PIN;
      startNewGame();
    }
    break;

  default:
    break;
  }
}
