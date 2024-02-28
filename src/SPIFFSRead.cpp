#include "FS.h"
#include "SPIFFS.h"
#include "SPIFFSRead.h"



#if 0
String SPIFFSRead::readFirstFileWithExtension(const String &extension) {
    if (!SPIFFS.begin(true)) {
        Serial.println("SPIFFSのマウントに失敗しました。");
        return "";
    }

    File root = SPIFFS.open("/");
    File file = root.openNextFile();
    while (file) {
        String fileName = file.name();
        if (fileName.endsWith(extension)) {
            Serial.println("読み込み中: " + fileName);
            String fileContent;
            while (file.available()) {
                fileContent += (char)file.read();
            }
            file.close();
            return fileContent;
        }
        file = root.openNextFile();
    }
    return ""; // ファイルが見つからなかった場合
}
#endif




void SPIFFS_check() {
  //M5.begin();
  Serial.begin(115200);
  if (!SPIFFS.begin(true)) {
    Serial.println("SPIFFSのマウントに失敗しました");
    return;
  }

  Serial.println("SPIFFS内のファイル:");
  File root = SPIFFS.open("/");
  File file = root.openNextFile();
  while(file){
    Serial.print("ファイル名: ");
    Serial.println(file.name());
    // ファイルの内容を読み出して表示
    while(file.available()){
      Serial.write(file.read());
    }
    file = root.openNextFile();
  }
}