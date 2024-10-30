# ESPspConfig Library

This library provides the C++ ESPspConfig class for managing configuration values as well as read them from / save them to configuration files. This is specifically for use with the ESP32 in platformio and Arduino framework. 

When using the read() and save() functions, the configuration values will be read from / saved to a 'config.ini' file in the current folder. If available, values from the 'config-default.ini' file will be read before trying to read from 'config.ini'. Such a default configuration allows for an optional method of specifying factory defaults for settings to be changed on user level in the application. 
It also serves as an option to change the value of 'variables' without touching code, i.e. no need to recompile, just amend the default config value and restart.

Set optional path, change the extension ('ini') or the filenames for 'config' and 'config-default' to determine the location and names of files used.

Autosave is disabled by default, but when enabled, the ESPspConfig object will check for changes to the configuration values and saves them automatically.

The .cpp files in the /examples folder demonstrate the various options to use the functions.

This library depend on the following libraries
- spConfig library ([available here](https://github.com/krokoreit/spConfig.git))
- spLogHelper library ([available here](https://github.com/krokoreit/spLogHelper.git)).
- spObjectStore library ([available here](https://github.com/krokoreit/spObjectStore.git)),

It is not necessary to download or install these dependencies when using platformio, as its library dependency finder should automatically handle this.

<br>


Enjoy

&emsp;krokoreit  
&emsp;&emsp;&emsp;<img src="assets/krokoreit-01.svg" width="140"/>









## Usage & API

### Configuration Object
Include library and create object
```cpp
#include <ESPspConfig.h>
ESPspConfig config;
```
Typically an application program will start with running the read() function to load configuration data from file. However, this is not a requirement, as you can set() and get() values without any files involved.

In case of using other than the standard file names, extension and location, use the following functions before calling read() or save():
```cpp
config.setConfigFilename("newName");
config.setConfigDefaultFilename("newDefaultName");
config.setConfigFileExtension("newExtension");
config.setConfigFilePath("newPath");
```

</br>

### Config File Format

The configuration files are regular text files, with each line representing either a section or a key=value pair, e.g.
```cpp
[system]
startCounterToday=6
startCounterTotal=10
todayString=20240615
```

The 'config-default' file must have at least one [section] and allows for
  - comments (starting with '#' or ';')
  - whitespaces before or after section, key or value entries
  - keys without values (as either key only or with '=', but no value).

Note that 'config' file will be built from 'config-default' as well as entries made with
application code and therefore does not allow for comments or whitespace, which will be 
stripped out when reading the content. However, other manual changes to the 'config' file
will be preserved until reset() is used.

</br>

### API

#### Functions
* [setValue()](#setvalue-functions)  
* [getConfigValue()](#getconfigvalue-function)  
* [get...()](#get-functions)  
* [exists()](#exists-function)  
* [changed()](#changed-function)  
* [reset()](#reset-function)  
* [read() and save()](#read-and-save-functions)  
* [setAutosave() and getAutosave()](#setautosave-and-getautosave-functions)  
* [setConfigFilename() and getConfigFilename()](#setconfigfilename-and-getconfigfilename-functions)  
* [setConfigDefaultFilename() and getConfigDefaultFilename()](#setconfigdefaultfilename-and-getconfigdefaultfilename-functions)  
* [setConfigFileExtension() and getConfigFileExtension()](#setconfigfileextension-and-getconfigfileextension-functions)  
* [setConfigFilePath() and getConfigFilePath()](#setconfigfilepath-and-getconfigfilepath-functions)

#### setValue() Functions
```cpp
void setValue(const char* section, const char* key, const char* value);
void setValue(const char* section, const char* key, int32_t value);
void setValue(const char* section, const char* key, uint32_t value);
void setValue(const char* section, const char* key, int64_t value);
void setValue(const char* section, const char* key, uint64_t value);
void setValue(const char* section, const char* key, double value);
void setValue(const char* section, const char* key, bool value);
```
All values can be stored via these functions with their section and key parameters followed by the value. The value can be of type const char*, int32_t, uint32_t, int64_t, uint64_t, double or bool.

<div style="text-align: right"><a href="#functions">&#8679; back up to list of functions</a></div>


#### getConfigValue() Function
```cpp
spConfigValue* getConfigValue(const char* section, const char* key);
```
Returns an spConfigValue object, which allows access to the value via its as...() functions (e.g. asString(), asInt32()). There is normally no need to work with spConfigValue objects directly, as the following get...() functions of ESPspConfig provide easier access to the values stored.

<div style="text-align: right"><a href="#functions">&#8679; back up to list of functions</a></div>

#### get...() Functions
```cpp
const char* getCStr(const char* section, const char* key, const char* defaultValue = "");
std::string getString(const char* section, const char* key, const char* defaultValue = "");
int32_t getInt32(const char* section, const char* key, int32_t defaultValue = 0);
int32_t getUInt32(const char* section, const char* key, uint32_t defaultValue = 0);
int64_t getInt64(const char* section, const char* key, int64_t defaultValue = 0);
int64_t getUInt64(const char* section, const char* key, uint64_t defaultValue = 0);
double getDouble(const char* section, const char* key, double defaultValue = 0.0);
bool getBool(const char* section, const char* key, bool defaultValue = false);
```
Getting values requires to use the getter function suitable for the returned type wanted. While a compiler may be able to cast some return values to the type needed in program code, it is generally best to use the appropriate function matching the type desired.

All function have an optional defaultValue parameter to specify an return value in case the requested value does not exist, i.e. no entry for the section - key combination. Otherwise, the function will return either "", 0 or false, when the entry is not found.

<div style="text-align: right"><a href="#functions">&#8679; back up to list of functions</a></div>

#### exists() Function
```cpp
bool exists(const char* section, const char* key);
```
To determine whether a value exists with these section and key parameters. 

<div style="text-align: right"><a href="#functions">&#8679; back up to list of functions</a></div>

#### changed() Function
```cpp
bool changed();
```
Returns whether the config object holds any changed and not yet saved values. As such it may act as an indicator for the need to save() the content. If autosave is set to true, the changed() status will automatically return to false after the next autosave is performed.

<div style="text-align: right"><a href="#functions">&#8679; back up to list of functions</a></div>

#### reset() Function
```cpp
void reset();
```
Reset the whole config to either 'config-default.ini' as the factory defaults or - if no default file exists - to an empty list.

<div style="text-align: right"><a href="#functions">&#8679; back up to list of functions</a></div>

#### read() and save() Functions
```cpp
void read();
void save();
```
The config data are read by trying to parse first the 'config-default.ini' file and then the 'config.ini' file, whereby each file is only parsed when it exists and default values with the same section and key will be replaced by those in the 'config.ini' file.

To avoid unnecessary file operation, the saving of the config data with save() will only be done when data have changed. Thus, prior checking of changed() is not needed for that purpose. The ESPspConfig object also has an autosave functionality, which is turned off by default and can be enabled with setAutosave(true).

<div style="text-align: right"><a href="#functions">&#8679; back up to list of functions</a></div>

#### setAutosave() and getAutosave() Functions
```cpp
bool setAutosave(bool autosave);
bool getAutosave();
```
Setting autosave to true will automatically check for any changes made to configuration values and then saves them to file. This means there is no need to call the save() function in the program code after amending configuration values. However, when the program is making many changes in a row, it may be better to set autosave to false during that time. When setting autosave to true again, these amended values will then be automatically saved to file.

<div style="text-align: right"><a href="#functions">&#8679; back up to list of functions</a></div>

#### setConfigFilename() and getConfigFilename() Functions
```cpp
void setConfigFilename(std::string newName);
std::string getConfigFilename();
```
To change the standard 'config' filename to use a different one and to retrieve the currently used file name. This can also be used to temporarily change the name in order to make a (backup) copy of the configuration values.

<div style="text-align: right"><a href="#functions">&#8679; back up to list of functions</a></div>

#### setConfigDefaultFilename() and getConfigDefaultFilename() Functions
```cpp
void setConfigDefaultFilename(std::string newName);
std::string getConfigDefaultFilename();
```
Setting the name of the file with default configuration values (i.e. replacing the standard 'config-default' with 'newName') and retrieving the currently used name.

<div style="text-align: right"><a href="#functions">&#8679; back up to list of functions</a></div>

#### setConfigFileExtension() and getConfigFileExtension() Functions
```cpp
void setConfigFileExtension(std::string newExtension);
std::string getConfigFileExtension();
```
Replacing the standard 'ini' extention and retrieving the current extension used.

<div style="text-align: right"><a href="#functions">&#8679; back up to list of functions</a></div>

#### setConfigFilePath() and getConfigFilePath() Functions
```cpp
void setConfigFilePath(std::string newPath);
std::string getConfigFilePath();
```
Replace the default empty string with a path to be used for read() and save(). The newPath can be absolute or relative and trailing slashes will be added if not present. The only condition is that the folder given by newPath exists and the application has rights to read and write.  

<div style="text-align: right"><a href="#functions">&#8679; back up to list of functions</a></div>

</br>

## License
MIT license  
Copyright &copy; 2024 by krokoreit
