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

  // config
  config.setConfigFilename("application");
  config.setConfigDefaultFilename("default");
  config.setConfigFileExtension("conf");
  config.read();

  Serial.printf("configuration values read:\n");
  int32_t maybeInt32 = config.getInt32("maybe-overridden", "maybeInt32", 1111);
  Serial.printf("   maybeInt32: %i\n", maybeInt32);
  std::string maybeString = config.getString("maybe-overridden", "maybeString", "no maybeString found");
  Serial.printf("   maybeString: %s\n", maybeString.c_str());
  int32_t stableInt32 = config.getInt32("not-overridden", "stableInt32", 2222);
  Serial.printf("   stableInt32: %i\n", stableInt32);
  std::string stableString = config.getString("not-overridden", "stableString", "no stableString found");
  Serial.printf("   stableString: %s\n", stableString.c_str());
  Serial.printf("\nrename 'application.conf' and then 'default.conf' and see how the output changes.\n...\n");

  Serial.printf("done\n");

}

void loop() {  
  delay(5000);
}