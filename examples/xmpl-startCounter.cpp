/**
 * example code for ESPspConfig library
 * 
 * 
 */

#include <Arduino.h>

#include <ESPspConfig.h>

#include "FS.h"
#include "SPIFFS.h"



ESPspConfig config;


void setup() {

  Serial.begin(115200);

  std::string devName("Example");
  std::string reason = "(power on)";
  esp_reset_reason_t resetReason = esp_reset_reason();
  if ((esp_reset_reason() != ESP_RST_POWERON) && (esp_reset_reason() != ESP_RST_SW))
  {
    reason = "restarting after event";
  }
  Serial.printf("\n\nStarting: %s ... %s\n", devName.c_str(), reason.c_str());

  uint32_t startCounterTotal = 0;
  uint32_t startCounterToday = 0;

  // config
  config.setAutosave(true);
  config.read();

  time_t now = time(nullptr);
  tm* local_tm = localtime(&now);
  char todayString[9];
  sprintf(todayString, "%4d%02d%02d", local_tm->tm_year + 1900, local_tm->tm_mon + 1, local_tm->tm_mday);

  Serial.printf("todayString is %s\n", todayString);

  // get config values
  if (strcmp(todayString, config.getCStr("system", "todayString", "none")) == 0)
  {
    startCounterToday = config.getInt32("system", "startCounterToday", startCounterToday);
    startCounterToday++;
  }
  else
  {
    config.setValue("system", "todayString", todayString);
    startCounterToday = 1;
  }
  config.setValue("system", "startCounterToday", startCounterToday);
  Serial.printf("Start Counter: %i\n", startCounterToday);
  
  startCounterTotal = config.getInt32("system", "startCounterTotal", startCounterTotal);
  startCounterTotal++;
  config.setValue("system", "startCounterTotal", startCounterTotal);
  Serial.printf("Total Start Counter: %i\n", startCounterTotal);

  Serial.printf("done\n");

}

void loop() {  
  delay(5000);
}
