// SPIFFSRead.h
#ifndef SPIFFSRead_h
#define SPIFFSRead_h

#include "FS.h"
#include "SPIFFS.h"
#include <string>


class SPIFFSRead {
public:
  static String readFirstFileWithExtension(const String& extension) {
    File root = SPIFFS.open("/");
    if (!root) {
      Serial.println("Failed to open directory");
      return "";
    }
    if (!root.isDirectory()) {
      Serial.println("Not a directory");
      return "";
    }

    File file = root.openNextFile();
    while(file){
      String fileName = file.name();
      if(fileName.endsWith(extension)) {
        String fileContent;
        while(file.available()){
          fileContent += char(file.read());
        }
        file.close();
        return fileContent;
      }
      file = root.openNextFile();
    }
    return ""; // No file found
  }
};

void SPIFFS_check();

#endif