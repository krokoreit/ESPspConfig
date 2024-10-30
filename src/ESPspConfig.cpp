/**
 * @file ESPspConfig.cpp
 * @author krokoreit (krokoreit@gmail.com)
 * @brief class to handle configuration data in arduino framework
 * @version 1.0.0
 * @date 2024-10-30
 * @copyright Copyright (c) 2024
 * 
 */


#include <ESPspConfig.h>


/*    PUBLIC    PUBLIC    PUBLIC    PUBLIC    

      xxxxxxx   xx    xx  xxxxxxx   xx           xx      xxxxxx 
      xx    xx  xx    xx  xx    xx  xx           xx     xx    xx
      xx    xx  xx    xx  xx    xx  xx           xx     xx      
      xxxxxxx   xx    xx  xxxxxxx   xx           xx     xx      
      xx        xx    xx  xx    xx  xx           xx     xx      
      xx        xx    xx  xx    xx  xx           xx     xx    xx
      xx         xxxxxx   xxxxxxx   xxxxxxxx     xx      xxxxxx 
     

      PUBLIC    PUBLIC    PUBLIC    PUBLIC    */


/**
 * @brief Destroy the ESPspConfig::ESPspConfig object
 * 
 */
ESPspConfig::~ESPspConfig(){
  vTaskDelete(m_loop_task_handle);
}



/*    PRIVATE    PRIVATE    PRIVATE    PRIVATE

      xxxxxxx   xxxxxxx      xx     xx    xx     xx     xxxxxxxx  xxxxxxxx
      xx    xx  xx    xx     xx     xx    xx    xxxx       xx     xx      
      xx    xx  xx    xx     xx     xx    xx   xx  xx      xx     xx      
      xxxxxxx   xxxxxxx      xx      xx  xx   xx    xx     xx     xxxxxxx    
      xx        xx    xx     xx      xx  xx   xxxxxxxx     xx     xx    
      xx        xx    xx     xx       xxxx    xx    xx     xx     xx      
      xx        xx    xx     xx        xx     xx    xx     xx     xxxxxxxx
     

      PRIVATE    PRIVATE    PRIVATE    PRIVATE    */


/**
 * @brief Returns whether we can use the file system.
 * 
 * @return true / false 
 */
bool ESPspConfig::canUseFS()
{
  if (SPIFFS.begin(true))
  {
    return true;
  }
  else
  {
    spLOG_E("ESPspConfig::canUseFS() failed, because SPIFFS.begin() returned false");
    return false;
  }
}

/**
 * @brief Updates the timer for autosave. This function gets called when config was changed.
 * 
 */
void ESPspConfig::onSetChanged()
{
  // delay autosave as we may have multiple values set
  setNextAutosaveTimeMS(millis() + 1500);
}

/**
 * @brief Reads the file content into buffer
 * 
 * @param filename  name of configuration file
 * @param buf       pointer to buffer to read to
 * @param startPos  potiotion in file to start reading from
 * @param maxBytes  maximum number of bytes to read
 * @return size_t   number of bytes read
 */
size_t ESPspConfig::readFile(std::string filename, char* buf, size_t startPos, size_t maxBytes)
{
  std::string filepath = "/" + filename;
  m_fHandle = SPIFFS.open(filepath.c_str(), FILE_READ, true);
  if (!m_fHandle || m_fHandle.isDirectory())
  {
    spLOGF_E("ESPspConfig::readFile() failed to get handle for %s", filepath.c_str());
    return 0;
  }

  if (startPos > 0)
  {
    m_fHandle.seek(startPos);
  }
  
  size_t len = m_fHandle.read((uint8_t*)buf, maxBytes);
  m_fHandle.close();
  return len;
}

/**
 * @brief Writes the file content from buffer
 * 
 * @param filename  name of configuration file
 * @param buf       pointer to buffer to write from
 * @param startPos  potiotion in file to start writing from
 * @param writeBytes  maximum number of bytes to write
 * @return size_t   number of bytes written
 */
size_t ESPspConfig::saveFile(std::string filename, char* buf, size_t startPos, size_t writeBytes)
{
  std::string filepath = "/" + filename;
  m_fHandle = SPIFFS.open(filepath.c_str(), FILE_WRITE, true);
  if (!m_fHandle || m_fHandle.isDirectory())
  {
    spLOGF_E("ESPspConfig::saveFile() failed to get handle for %s", filepath);
    return 0;
  }
  // stop auto save
  setNextAutosaveTimeMS(0);
 
  if (startPos > 0)
  {
    m_fHandle.seek(startPos);
  }
  
  size_t len = m_fHandle.write((uint8_t*)buf, writeBytes);

  m_fHandle.close();
  return len;
}

/**
 * @brief Create loop task if not exisiting.
 * 
 */
void ESPspConfig::ensureLoopTask() {
  if ((m_loop_task_handle == nullptr) && (getAutosave()))
  {
    xTaskCreate(config_loop_task, "spConfig_task", 15*1024, this, 8, &m_loop_task_handle);
  }
}

/**
 * @brief The ever running task, used to regularly check on _loopHandler() and other things
 * 
 * @param arg   pointer to object given to xTaskCreate()
 */
void ESPspConfig::config_loop_task(void* arg) {
  ESPspConfig* pConfig = reinterpret_cast<ESPspConfig*>(arg);

  while (pConfig->getAutosave())
  {
    if (pConfig->changed())
    {
      uint64_t timeMS = pConfig->getNextAutosaveTimeMS();
      if ((timeMS > 0) && (millis() > timeMS))
      {
        pConfig->save();
      }
    }
    // sleep for 1/2 sec
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
  pConfig->m_loop_task_handle = nullptr;
  vTaskDelete(NULL);
}