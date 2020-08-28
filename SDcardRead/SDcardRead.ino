#include <SPI.h>
#include <SD.h>
#include <stdio.h>
#include <string.h>
File myFile;
char readData[2] = {'\0', '\0'};
char readBuffer[100] = {0};

double data_queue[100][3];

char *start_idx, *end_idx;
char float_buffer[100];

int totalSize = 0;
int count = 0;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.print("Initializing SD card...");
  delay(1000);
  if (!SD.begin(10)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");
  // open the file for reading:
  
}
void loop() {
  myFile = SD.open("test.txt");
  if (myFile) {
    Serial.println("test.txt:");
    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      readData[0] = myFile.read();
      if (readData[0] == '\n') {
//        myFile.close();

        start_idx = strchr(readBuffer, 'X');
        if (start_idx != NULL) {
          start_idx = start_idx + 1;
          end_idx = strchr(start_idx, ' ');
          if (end_idx != NULL) {
            strncpy(float_buffer, start_idx, end_idx - start_idx);
            data_queue[count][0] = atof(float_buffer);
            Serial.print("string x = ");
            Serial.println(float_buffer);
            
            Serial.print("double x = ");
            Serial.println(data_queue[count][0]);
            float_buffer[0] = '\0';
          }
        }
        start_idx = strchr(readBuffer, 'Y');
        if (start_idx != NULL) {
          start_idx = start_idx + 1;
          end_idx = strchr(start_idx, ' ');
          if (end_idx != NULL) {
            strncpy(float_buffer, start_idx, end_idx - start_idx);
            float_buffer[end_idx - start_idx] = '\0';
            data_queue[count][1] = atof(float_buffer);
            Serial.print("string y = ");
            Serial.println(float_buffer);
            Serial.print("double y = ");
            Serial.println(data_queue[count][1]);
            float_buffer[0] = '\0';
          }
        }
        count++;
        readBuffer[0] = '\0';
      }
      strcat(readBuffer, readData);


    }
    // close the file:
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
}
