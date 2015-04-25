
#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "Clock.h"
#include "MidiList.h"
#include "Settings.h"
#include "thread.h"

class OptionsDialog;

class MainWindow : public Dialog
{
public:
	void Go();

	void CaptureTrigger();  // for OptionsDialog
	void CancelCapturingTrigger();

protected:
	virtual BOOL OnMsg(UINT, WPARAM, LPARAM);

private:
	void OnInitDialog();
	void OnDestroy();
	void OnRecord();
	void OnPlay();
	void OnStop();
	void OnSave();
	void OnEdit();
	void OnOptions();
	void OnHelp();
	void OnAbout();
	void OnQuit();
	void OnTimer();

	void OpenMidi();
	void CloseMidi();
	void RestartMidi();
	static void CALLBACK LaunchMidiInProc(HMIDIIN, UINT, DWORD, DWORD, DWORD);
	void MidiInProc(HMIDIIN midiIn, UINT msg, DWORD param1, DWORD param2);
	void ReceiveMsg(DWORD msg, DWORD time);
	void IdleMsg(DWORD msg, DWORD time);
	void PlayingMsg(DWORD msg, DWORD time);

	bool StartRecording();
	void StopRecording();
	void RecordingMsg(DWORD msg, DWORD time);
	bool AcceptMsg(DWORD msg);

	bool IsTrigger(int trigger, DWORD msg);
	void TriggerMsg(DWORD msg, DWORD time);

	bool StartPlaying();
	void StopPlaying();
	static DWORD WINAPI LaunchPlayThread(void* param);
	DWORD PlayThread();
	void OnPlaybackDone();

	void UpdateControls();

private:
	Settings settings;

	int store;
	int state;

	HMIDIIN inHandle;
	HMIDIOUT outHandle;

	MidiList events;

	Clock theClock;
	DWORD startTime;   // just for the clock during recording
	bool firstNoteOn;

	thread_t playThread;
	bool playing;

	OptionsDialog* optionsDialog;
};

#endif // MAIN_WINDOW_H
