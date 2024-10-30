/**
 * @file ESPspConfig.h
 * @author krokoreit (krokoreit@gmail.com)
 * @brief class to handle configuration data in arduino framework
 * @version 1.0.0
 * @date 2024-10-30
 * @copyright Copyright (c) 2024
 * 
 *  this library extends upon the spConfigBase library and implements arduino specific file handling
 *  There is also an additional autosave functionality, which is turned off by default.
 * 
 */


/**
 * Version history:
 * v1.0.0   original develoment for platformio / arduino in an ESP32 project
 *  
 */


#ifndef ESPSPCONFIG_H
#define ESPSPCONFIG_H

#include <Arduino.h>
#include <SPIFFS.h>

#include <spLogHelper.h>
#include <spConfigBase.h>


class ESPspConfig : public spConfigBase {
  private:
    File m_fHandle;
    TaskHandle_t m_loop_task_handle = nullptr;

    // override virtuals
    bool canUseFS();
    void onSetChanged();
    size_t readFile(std::string filename, char* buf, size_t startPos, size_t maxBytes);
    size_t saveFile(std::string filename, char* buf, size_t startPos, size_t writeBytes);
    void ensureLoopTask();

    static void config_loop_task(void* arg);

  public:
    ~ESPspConfig();

};





#endif // ESPSPCONFIG_H


