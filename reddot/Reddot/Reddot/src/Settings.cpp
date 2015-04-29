#include <iostream>
#include <fstream>

#include "framework.h"
#include "defs.h"
#include "Settings.h"
#include "ini_reader/IniReader.h"

#define SETTING_FILENAME                 "settings.ini"
#define SETTING_ECHO_INPUT               "EchoInput"
#define SETTING_INPUT_DEV                "InputDevice"
#define SETTING_OUTPUT_DEV               "OutputDevice"
#define SETTING_IGNORE_KEY_PRESSURE      "IgnoreKeyPressure"
#define SETTING_IGNORE_CHANNEL_PRESSURE  "IgnoreChannelPressure"
#define SETTING_IGNORE_PROGRAM_CHANGE    "IgnoreProgramChange"

#define SETTING_TRIGGER_RECORD_STATUS    "TriggerRecordStatus"
#define SETTING_TRIGGER_RECORD_VALUE     "TriggerRecordValue"
#define SETTING_TRIGGER_PLAY_STATUS      "TriggerPlayStatus"
#define SETTING_TRIGGER_PLAY_VALUE       "TriggerPlayValue"
#define SETTING_TRIGGER_STOP_STATUS      "TriggerStopStatus"
#define SETTING_TRIGGER_STOP_VALUE       "TriggerStopValue"

////////////////////////////////////////////////////////////////////////////////

Settings::Settings()
{
   inDev = 0;
   outDev = 0;
   echoInput = true;
   ignoreKeyPressure = true;
   ignoreChannelPressure = true;
   ignoreProgramChange = true;

   for (int t = 0; t < NUM_TRIGGERS; ++t)
   {
      triggerStatus[t] = NO_TRIGGER;
      triggerValue[t] = 0;
   }
}

////////////////////////////////////////////////////////////////////////////////

void Settings::Load()
{
	if(_iniReader.Load(SETTING_FILENAME) == false){
		// TODO: Error handling
		return;
	}

   inDev  = _iniReader.GetInt(SETTING_INPUT_DEV ,"",0);
   outDev = _iniReader.GetInt(SETTING_OUTPUT_DEV,"",0);

   echoInput = _iniReader.GetInt(SETTING_ECHO_INPUT,"",0) != 0;

   ignoreKeyPressure      = _iniReader.GetInt(SETTING_IGNORE_KEY_PRESSURE    ,"",1) != 0;
   ignoreChannelPressure  = _iniReader.GetInt(SETTING_IGNORE_CHANNEL_PRESSURE,"",1) != 0;
   ignoreProgramChange    = _iniReader.GetInt(SETTING_IGNORE_PROGRAM_CHANGE  ,"",0) != 0;

   triggerStatus[TRIGGER_RECORD]  = _iniReader.GetInt( SETTING_TRIGGER_RECORD_STATUS,"",-1);
   triggerValue[TRIGGER_RECORD]   = _iniReader.GetInt( SETTING_TRIGGER_RECORD_VALUE ,"", 0);
   triggerStatus[TRIGGER_PLAY]    = _iniReader.GetInt( SETTING_TRIGGER_PLAY_STATUS  ,"",-1);
   triggerValue[TRIGGER_PLAY]     = _iniReader.GetInt( SETTING_TRIGGER_PLAY_VALUE   ,"", 0);
   triggerStatus[TRIGGER_STOP]    = _iniReader.GetInt( SETTING_TRIGGER_STOP_STATUS  ,"",-1);
   triggerValue[TRIGGER_STOP]     = _iniReader.GetInt( SETTING_TRIGGER_STOP_VALUE   ,"", 0);
}

////////////////////////////////////////////////////////////////////////////////

void Settings::Save()
{
    _iniReader.SetInt(SETTING_INPUT_DEV, inDev);
    _iniReader.SetInt(SETTING_OUTPUT_DEV, outDev);

    _iniReader.SetInt(SETTING_ECHO_INPUT, echoInput ? 1 : 0);

    _iniReader.SetInt(SETTING_IGNORE_KEY_PRESSURE,ignoreKeyPressure ? 1 : 0);
    _iniReader.SetInt(SETTING_IGNORE_CHANNEL_PRESSURE, ignoreChannelPressure ? 1 : 0);
    _iniReader.SetInt(SETTING_IGNORE_PROGRAM_CHANGE,ignoreProgramChange ? 1 : 0);

    _iniReader.SetInt(SETTING_TRIGGER_RECORD_STATUS, triggerStatus[TRIGGER_RECORD]);
    _iniReader.SetInt(SETTING_TRIGGER_RECORD_VALUE, triggerValue[TRIGGER_RECORD]);
    _iniReader.SetInt(SETTING_TRIGGER_PLAY_STATUS, triggerStatus[TRIGGER_PLAY]);
    _iniReader.SetInt(SETTING_TRIGGER_PLAY_VALUE, triggerValue[TRIGGER_PLAY]);
    _iniReader.SetInt(SETTING_TRIGGER_STOP_STATUS, triggerStatus[TRIGGER_STOP]);
    _iniReader.SetInt(SETTING_TRIGGER_STOP_VALUE, triggerValue[TRIGGER_STOP]);

	 if(_iniReader.Save(SETTING_FILENAME) == false){
		 //TODO: Error handling
	 }   
}

////////////////////////////////////////////////////////////////////////////////
