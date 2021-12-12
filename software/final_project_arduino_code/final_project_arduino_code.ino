#include <mbed.h>
#include <mbed_wait_api.h>
#include <rtos.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <SPI.h>       // this is needed for display
#include <Adafruit_ILI9341.h>
#include <Wire.h>      // this is needed for FT6206
#include <Adafruit_FT6206.h>

#include "Nano33BleHID.h"
#include "signal_utils.h"

#include <TensorFlowLite.h>
#include <tensorflow/lite/micro/all_ops_resolver.h>
#include <tensorflow/lite/micro/micro_error_reporter.h>
#include <tensorflow/lite/micro/micro_interpreter.h>
#include <tensorflow/lite/schema/schema_generated.h>
#include <tensorflow/lite/version.h>

#include "model.h"

using namespace rtos;
using namespace mbed;
using namespace std;

// global variables used for TensorFlow Lite (Micro)
tflite::MicroErrorReporter tflErrorReporter;

// pull in all the TFLM ops, you can remove this line and
// only pull in the TFLM ops you need, if would like to reduce
// the compiled size of the sketch.
tflite::AllOpsResolver tflOpsResolver;
//static tflite::MicroMutableOpResolver<4> micro_op_resolver;

const tflite::Model* tflModel = nullptr;
tflite::MicroInterpreter* tflInterpreter = nullptr;
TfLiteTensor* tflInputTensor = nullptr;
TfLiteTensor* tflOutputTensor = nullptr;

// Create a static memory buffer for TFLM, the size may need to
// be adjusted based on the model you are using
char recChar = 'a';
char recChar1 = 'a';
char recChar2 = 'a';
char recChar3 = 'a';
int charIndex1 = 0;
int charIndex2 = 0;
int charIndex3 = 0;
bool flag = false;
constexpr int Widths = 32;
constexpr int tensorArenaSize = 14600;
//#define tensorArenaSize 3000
static uint8_t tensorArena[tensorArenaSize];

// array to map character index to a name
char CHARACTERS[] = {
  '0',
  '1',
  '2',
  '3',
  '4',
  '5',
  '6',
  '7',
  '8',
  '9',
  'A',
  'B',
  'C',
  'D',
  'E',
  'F',
  'G',
  'H',
  'I',
  'J',
  'K',
  'L',
  'M',
  'N',
  'O',
  'P',
  'Q',
  'R',
  'S',
  'T',
  'U',
  'V',
  'W',
  'X',
  'Y',
  'Z',
  'a',
  'b',
  'c',
  'd',
  'e',
  'f',
  'g',
  'h',
  'i',
  'j',
  'k',
  'l',
  'm',
  'n',
  'o',
  'p',
  'q',
  'r',
  's',
  't',
  'u',
  'v',
  'w',
  'x',
  'y',
  'z',
};

#define NUM_CHARACTERS (sizeof(CHARACTERS) / sizeof(CHARACTERS[0]))

bool sendFlag = false;
uint8_t mode = 0; // fast mode 0, slow mode 1
uint8_t thumbwidth = 32;

int8_t oriImage[240][240] = {0};
int8_t downscaledImage[32][32] = {0};

// The FT6206 uses hardware I2C (SCL/SDA)
Adafruit_FT6206 ctp = Adafruit_FT6206();

// The display also uses hardware SPI, plus #9 & #10
#define TFT_CS 10
#define TFT_DC 9
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

// Size of the color selection boxes and the paintbrush size
#define BOXSIZE 40

Semaphore sem(1);
Thread t1;
Thread t2;

Nano33BleKeyboard bleKb("Shining Keyboard");

void func1(){
  int16_t x_old = 0;
  int16_t y_old = 0;
  int old_time, now;
  old_time = millis();
  bool writtenFlag = false;

  // get the TFL representation of the model byte array
  tflModel = tflite::GetModel(model);
  if (tflModel->version() != TFLITE_SCHEMA_VERSION) {
    Serial.println("Model schema mismatch!");
    while (1);
  }

  Serial.println("Initialized Successful 2");

  // Create an interpreter to run the model
  tflInterpreter = new tflite::MicroInterpreter(tflModel, tflOpsResolver, tensorArena, tensorArenaSize, &tflErrorReporter);

  Serial.println("Initialized Successful 2.5");
  // Allocate memory for the model's input and output tensors
  tflInterpreter->AllocateTensors();

  Serial.println("Initialized Successful 2.7");

  // Get pointers for the model's input and output tensors
  tflInputTensor = tflInterpreter->input(0);
  tflOutputTensor = tflInterpreter->output(0);

  Serial.println("Initialized Successful 3");
  
  while(1){
    
  sem.acquire();
  // Retrieve a point  
  TS_Point p = ctp.getPoint();
  if (p.z == 0 and writtenFlag){
     now = millis();
     if (now - old_time > 150){

      writtenFlag = false;

      for (int i = 1; i < 31; i++){
        for (int j = 1; j < 31; j++){
          for (int p = 0; p < 8; p++){
            for (int q = 0; q < 8; q++){
              if (oriImage[(i-1)*8+p][(j-1)*8+q] == 1){
                downscaledImage[i-1][j-1] = 1;
                downscaledImage[i-1][j] = 1;
                downscaledImage[i-1][j+1] = 1;
                downscaledImage[i][j-1] = 1;
                downscaledImage[i][j] = 1;
                downscaledImage[i][j+1] = 1;
                downscaledImage[i+1][j-1] = 1;
                downscaledImage[i+1][j] = 1;
                downscaledImage[i+1][j+1] = 1;
                p = p + 8;
                q = q + 8;
              }
            }
          }
        }
      }

// Inference
      for (int y = 0; y < Widths; y++) {  
        for (int x = 0; x < Widths; x++) {
          if (downscaledImage[y][x] == 1){
            tflInputTensor->data.int8[y * Widths + x] = 127;
          }
          else{
          tflInputTensor->data.int8[y * Widths + x] = -128;}
        }
      }

      
      
      TfLiteStatus invokeStatus = tflInterpreter->Invoke();

      int8_t score1 = -128;
      int8_t score2 = -128;
      int8_t score3 = -128;
      for (int i = 0; i < NUM_CHARACTERS; i++) {
        int8_t curr_category_score = tflOutputTensor->data.uint8[i];
        if (curr_category_score > score1){
          score1 = curr_category_score;
          charIndex1 = i;
        }
        else if (curr_category_score > score2){
          score2 = curr_category_score;
          charIndex2 = i;
        }
        else if (curr_category_score > score3){
          charIndex3 = i;
          score3 = curr_category_score;
        }
      }
      recChar1 = CHARACTERS[charIndex1];
      recChar2 = CHARACTERS[charIndex2];
      recChar3 = CHARACTERS[charIndex3];

      if (mode == 0){
          recChar = recChar1;
          sendFlag = true;
      }
      else if (mode == 1){
        tft.setRotation(1);
        tft.fillRect(0, 0, 80, 50, ILI9341_BLUE);
        tft.drawRect(0, 0, 80, 50, ILI9341_WHITE);
        tft.setCursor(30, 10);
        tft.setTextSize(5);
        tft.print(recChar1);
        tft.fillRect(0, 50, 80, 50, ILI9341_BLUE);
        tft.drawRect(0, 50, 80, 50, ILI9341_WHITE);
        tft.setCursor(30, 60);
        tft.setTextSize(5);
        tft.print(recChar2);
        tft.fillRect(0, 100, 80, 50, ILI9341_BLUE);
        tft.drawRect(0, 100, 80, 50, ILI9341_WHITE);
        tft.setCursor(30, 110);
        tft.setTextSize(5);
        tft.print(recChar3);
        tft.setRotation(0);
        bool selected = false;
        now = millis();
        old_time = now;
        p.x = 240;
        p.y = 320;
        while(!selected){
          now = millis();
          p = ctp.getPoint();
          p.x = map(p.x, 0, 240, 240, 0);
          p.y = map(p.y, 0, 320, 320, 0);
          if (now - old_time > 50){
            old_time = now;
            if (p.y < 80 && p.x > 190){
              recChar = recChar1;
              sendFlag = true;
              selected = true;
            }
            else if ((p.y < 80) && (p.x > 140)){
              recChar = recChar2;
              sendFlag = true;
              selected = true;
            }
            else if (p.y < 80 && (p.x > 90)){
              recChar = recChar3;
              sendFlag = true;
              selected = true;
            }
            else if (p.y < 80 && (p.x > 45)){
            }                           
            else if (p.y < 80 && (p.x > 0)){
              selected = true;
            }             
          }            
        }
      }
      for(int i = 1; i<=240; i++){
        for (int j = 1; j<=240; j++){
          if (oriImage[240 - i][j] == 1){
            oriImage[240-i][j] = 0;
            tft.drawPixel(i, j+80, ILI9341_BLACK);
          }
        }
      }
      for(int i = 0; i<32; i++){
        for(int j = 0; j<32; j++){
          downscaledImage[i][j] = 0;
        }
      }
     }
     
  }
  else{
  if (p.x != 0){

    // flip it around to match the screen.
    p.x = map(p.x, 0, 240, 240, 0);
    p.y = map(p.y, 0, 320, 320, 0);
    
  if (p.y < 80){
      now = millis();
      if ((p.x <45) && (now - old_time > 50)){
        old_time = now;
        tft.setRotation(1);
        tft.drawRect(0, 195, 80, 45, ILI9341_WHITE);
        wait_us(1000);
        tft.drawRect(0, 195, 80, 45, ILI9341_RED);
        tft.setRotation(0);
        recChar = 8;
        sendFlag = true;
      }
      else if ((p.x > 45 && p.x < 90) && (now - old_time > 50)){
        old_time = now;
        tft.setRotation(1);
        tft.setCursor(30, 165); tft.setTextSize(3);
        if (mode == 1){
          tft.fillRect(0, 150, 80, 45, ILI9341_GREEN);
          tft.print("F");
          mode = 0;
        }
        else if (mode == 0){
          tft.fillRect(0, 150, 80, 45, ILI9341_YELLOW);
          tft.print("S");
          mode = 1;
        }
        tft.setRotation(0);
      }
  }
    else{
      now = millis();
      if ((now - old_time < 30)){
          
          tft.startWrite();
          tft.writeAndRecordLine(x_old, y_old, p.x, p.y, ILI9341_RED, oriImage);
          tft.endWrite();
          writtenFlag = true;
          
          x_old = p.x;
          y_old = p.y;
        }
      else{
          x_old = p.x;
          y_old = p.y;
        }
      old_time = now;
      }
      
      wait_us(10);
    }
  }
    sem.release();
  }
}

void setup() {
 
  Serial.begin(115200);

  Serial.println(F("Cap Touch Paint!"));
  
  tft.begin();

  if (! ctp.begin(40)) {  // pass in 'sensitivity' coefficient
    Serial.println(F("Couldn't start FT6206 touchscreen controller"));
    while (1);
  }

  Serial.println(F("Capacitive touchscreen started"));
  
  tft.fillScreen(ILI9341_BLACK);
  
  // make the color selection boxes
  tft.fillRect(0, 0, BOXSIZE*6, 80, ILI9341_BLUE);
  tft.setRotation(1);
  tft.drawRect(0, 0, 80, 50, ILI9341_WHITE);
  tft.drawRect(0, 50, 80, 50, ILI9341_WHITE);
  tft.drawRect(0, 100, 80, 50, ILI9341_WHITE);
  tft.fillRect(0, 150, 80, 45, ILI9341_GREEN);
  tft.fillRect(0, 195, 80, 45, ILI9341_RED);
  tft.setTextColor(ILI9341_WHITE);  
  tft.setCursor(30, 165); tft.setTextSize(3);
  tft.println("F");
  tft.setCursor(12, 215); tft.setTextSize(1);
  tft.println("Backspace");
  tft.setRotation(0);

  Serial.println("Initialized Successful 1");
  
  t1.start(func1);
  Serial.println("Thread 1 initialized");
  bleKb.initialize();
  MbedBleHID_RunEventThread();

}

void loop(){

    // When disconnected, we animate the builtin LED to indicate the device state.
  if (bleKb.connected() == false) {
    animateLED(LED_BUILTIN, (bleKb.has_error()) ? 118 
                                                : 1185);

    return;
  }
  
  sem.acquire();

  auto *kb = bleKb.hid();

  if (sendFlag == true){
    kb->sendCharacter(recChar);
    sendFlag = false;
  }

    sem.release();

  wait_us(20);
  
}
