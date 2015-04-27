#include <iostream>
#include <fstream>

#include "framework.h"
#include "defs.h"
#include "Settings.h"

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
	inDev = ReadRegistryInt(HKEY_CURRENT_USER, REGISTRY_KEY, SETTING_INPUT_DEV, 0);
	outDev = ReadRegistryInt(HKEY_CURRENT_USER, REGISTRY_KEY, SETTING_OUTPUT_DEV, 0);

	echoInput = ReadRegistryInt(HKEY_CURRENT_USER, REGISTRY_KEY, SETTING_ECHO_INPUT, 0) != 0;

	ignoreKeyPressure = ReadRegistryInt(HKEY_CURRENT_USER, REGISTRY_KEY, SETTING_IGNORE_KEY_PRESSURE, 1) != 0;
	ignoreChannelPressure = ReadRegistryInt(HKEY_CURRENT_USER, REGISTRY_KEY, SETTING_IGNORE_CHANNEL_PRESSURE, 1) != 0;
	ignoreProgramChange = ReadRegistryInt(HKEY_CURRENT_USER, REGISTRY_KEY, SETTING_IGNORE_PROGRAM_CHANGE, 0) != 0;

	triggerStatus[TRIGGER_RECORD] = ReadRegistryInt(HKEY_CURRENT_USER, REGISTRY_KEY, SETTING_TRIGGER_RECORD_STATUS, -1);
	triggerValue[TRIGGER_RECORD] = ReadRegistryInt(HKEY_CURRENT_USER, REGISTRY_KEY, SETTING_TRIGGER_RECORD_VALUE, 0);
	triggerStatus[TRIGGER_PLAY] = ReadRegistryInt(HKEY_CURRENT_USER, REGISTRY_KEY, SETTING_TRIGGER_PLAY_STATUS, -1);
	triggerValue[TRIGGER_PLAY] = ReadRegistryInt(HKEY_CURRENT_USER, REGISTRY_KEY, SETTING_TRIGGER_PLAY_VALUE, 0);
	triggerStatus[TRIGGER_STOP] = ReadRegistryInt(HKEY_CURRENT_USER, REGISTRY_KEY, SETTING_TRIGGER_STOP_STATUS, -1);
	triggerValue[TRIGGER_STOP] = ReadRegistryInt(HKEY_CURRENT_USER, REGISTRY_KEY, SETTING_TRIGGER_STOP_VALUE, 0);
}

////////////////////////////////////////////////////////////////////////////////

void Settings::Save()
{
	 WriteRegistryInt(HKEY_CURRENT_USER, REGISTRY_KEY, SETTING_INPUT_DEV, inDev);
	 WriteRegistryInt(HKEY_CURRENT_USER, REGISTRY_KEY, SETTING_OUTPUT_DEV, outDev);

	 WriteRegistryInt(HKEY_CURRENT_USER, REGISTRY_KEY, SETTING_ECHO_INPUT, echoInput ? 1 : 0);

	 WriteRegistryInt(HKEY_CURRENT_USER, REGISTRY_KEY, SETTING_IGNORE_KEY_PRESSURE, ignoreKeyPressure ? 1 : 0);
	 WriteRegistryInt(HKEY_CURRENT_USER, REGISTRY_KEY, SETTING_IGNORE_CHANNEL_PRESSURE, ignoreChannelPressure ? 1 : 0);
	 WriteRegistryInt(HKEY_CURRENT_USER, REGISTRY_KEY, SETTING_IGNORE_PROGRAM_CHANGE, ignoreProgramChange ? 1 : 0);

	 WriteRegistryInt(HKEY_CURRENT_USER, REGISTRY_KEY, SETTING_TRIGGER_RECORD_STATUS, triggerStatus[TRIGGER_RECORD]);
	 WriteRegistryInt(HKEY_CURRENT_USER, REGISTRY_KEY, SETTING_TRIGGER_RECORD_VALUE, triggerValue[TRIGGER_RECORD]);
	 WriteRegistryInt(HKEY_CURRENT_USER, REGISTRY_KEY, SETTING_TRIGGER_PLAY_STATUS, triggerStatus[TRIGGER_PLAY]);
	 WriteRegistryInt(HKEY_CURRENT_USER, REGISTRY_KEY, SETTING_TRIGGER_PLAY_VALUE, triggerValue[TRIGGER_PLAY]);
	 WriteRegistryInt(HKEY_CURRENT_USER, REGISTRY_KEY, SETTING_TRIGGER_STOP_STATUS, triggerStatus[TRIGGER_STOP]);
	 WriteRegistryInt(HKEY_CURRENT_USER, REGISTRY_KEY, SETTING_TRIGGER_STOP_VALUE, triggerValue[TRIGGER_STOP]);
}

////////////////////////////////////////////////////////////////////////////////
