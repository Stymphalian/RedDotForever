
#ifndef SETTINGS_H
#define SETTINGS_H

#define NO_TRIGGER  -1

enum
{
	TRIGGER_RECORD = 0,
	TRIGGER_PLAY,
	TRIGGER_STOP,
	NUM_TRIGGERS
};

class Settings
{
public:
	Settings();

	void Load();
	void Save();

public:
	int inDev;
	int outDev;
	bool echoInput;
	bool ignoreKeyPressure;
	bool ignoreChannelPressure;
	bool ignoreProgramChange;

	int triggerStatus[NUM_TRIGGERS];
	int triggerValue[NUM_TRIGGERS];
};

#endif // SETTINGS_H
