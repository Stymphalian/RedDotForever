
#include "framework.h"
#include "MainWindow.h"
#include "MidiList.h"
#include "OptionsDialog.h"
#include "Settings.h"
#include <stdio.h>

static char* noteLookup[128] =
{
	"C-1", "C#/Db-1", "D-1", "D#/Eb-1", "E-1", "F-1", "F#/Gb-1", "G-1", "G#/Ab-1", "A-1", "A#/Bb-1", "B-1",
	"C0", "C#/Db0", "D0", "D#/Eb0", "E0", "F0", "F#/Gb0", "G0", "G#/Ab0", "A0", "A#/Bb0", "B0",
	"C1", "C#/Db1", "D1", "D#/Eb1", "E1", "F1", "F#/Gb1", "G1", "G#/Ab1", "A1", "A#/Bb1", "B1",
	"C2", "C#/Db2", "D2", "D#/Eb2", "E2", "F2", "F#/Gb2", "G2", "G#/Ab2", "A2", "A#/Bb2", "B2",
	"C3", "C#/Db3", "D3", "D#/Eb3", "E3", "F3", "F#/Gb3", "G3", "G#/Ab3", "A3", "A#/Bb3", "B3",
	"C4", "C#/Db4", "D4", "D#/Eb4", "E4", "F4", "F#/Gb4", "G4", "G#/Ab4", "A4", "A#/Bb4", "B4",
	"C5", "C#/Db5", "D5", "D#/Eb5", "E5", "F5", "F#/Gb5", "G5", "G#/Ab5", "A5", "A#/Bb5", "B5",
	"C6", "C#/Db6", "D6", "D#/Eb6", "E6", "F6", "F#/Gb6", "G6", "G#/Ab6", "A6", "A#/Bb6", "B6",
	"C7", "C#/Db7", "D7", "D#/Eb7", "E7", "F7", "F#/Gb7", "G7", "G#/Ab7", "A7", "A#/Bb7", "B7",
	"C8", "C#/Db8", "D8", "D#/Eb8", "E8", "F8", "F#/Gb8", "G8", "G#/Ab8", "A8", "A#/Bb8", "B8",
	"C9", "C#/Db9", "D9", "D#/Eb9", "E9", "F9", "F#/Gb9", "G9",
};

static char* controllerLookup[128] =
{
	"Bank Select (coarse)",  // 0
	"Modulation Wheel (coarse)",  // 1
	"Breath controller (coarse)", // 2
	"",
	"Foot Pedal (coarse)",  // 4
	"Portamento Time (coarse)",  // 5
	"Data Entry (coarse)",  // 6
	"Volume (coarse)",  // 7
	"Balance (coarse)",  // 8
	"",
	"Pan position (coarse)",  // 10
	"Expression (coarse)",  // 11
	"Effect Control 1 (coarse)",  // 12
	"Effect Control 2 (coarse)",  // 13
	"",
	"",
	"General Purpose Slider 1",  // 16
	"General Purpose Slider 2",  // 17
	"General Purpose Slider 3",  // 18
	"General Purpose Slider 4",  // 19
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"Bank Select (fine)",  // 32
	"Modulation Wheel (fine)",  // 33
	"Breath controller (fine)",  // 34
	"",
	"Foot Pedal (fine)",  // 36
	"Portamento Time (fine)",  // 37
	"Data Entry (fine)",  // 38
	"Volume (fine)",  // 39
	"Balance (fine)",  // 40
	"",
	"Pan position (fine)",  // 42
	"Expression (fine)",  // 43
	"Effect Control 1 (fine)",  // 44
	"Effect Control 2 (fine)",  // 45
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"Hold Pedal (on/off)",  // 64
	"Portamento (on/off)",  // 65
	"Sustenuto Pedal (on/off)",  // 66
	"Soft Pedal (on/off)",  // 67
	"Legato Pedal (on/off)",  // 68
	"Hold 2 Pedal (on/off)",  // 69
	"Sound Variation",  // 70
	"Sound Timbre",  // 71
	"Sound Release Time",  // 72
	"Sound Attack Time",  // 73
	"Sound Brightness",  // 74
	"Sound Control 6",  // 75
	"Sound Control 7",  // 76
	"Sound Control 8",  // 77
	"Sound Control 9",  // 78
	"Sound Control 10",  // 79
	"General Purpose Button 1 (on/off)",  // 80
	"General Purpose Button 2 (on/off)",  // 81
	"General Purpose Button 3 (on/off)",  // 82
	"General Purpose Button 4 (on/off)",  // 83
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"Effects Level",  // 91
	"Tremulo Level",  // 92
	"Chorus Level",  // 93
	"Celeste Level",  // 94
	"Phaser Level",  // 95
	"Data Button increment",  // 96
	"Data Button decrement",  // 97
	"Non-registered Parameter (fine)",  // 98
	"Non-registered Parameter (coarse)",  // 99
	"Registered Parameter (fine)",  // 100
	"Registered Parameter (coarse)",  // 101
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"All Sound Off",  // 120
	"All Controllers Off",  // 121
	"Local Keyboard (on/off)",  // 122
	"All Notes Off",  // 123
	"Omni Mode Off",  // 124
	"Omni Mode On",  // 125
	"Mono Operation",  // 126
	"Poly Operation",  // 127
};

////////////////////////////////////////////////////////////////////////////////

bool TriggerDialog::Go(HWND parent)
{
	return ::DialogBoxParam(
		appInstance, MAKEINTRESOURCE(IDD_TRIGGER), parent, DlgProc,
		(LPARAM) this) != -1;
}

////////////////////////////////////////////////////////////////////////////////

BOOL TriggerDialog::OnMsg(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_INITDIALOG:{
			HICON icon = ::LoadIcon(appInstance, MAKEINTRESOURCE(IDI_ICON));
			::SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)icon);
			return TRUE;
		}break;
		case WM_DESTROY:{
			return 0;
		}break;

		case WM_COMMAND:{
			switch(LOWORD(wParam))
			{
			case IDCANCEL:
			::EndDialog(hwnd, -1);
			return TRUE;
			}
			break;
		}break;			
	}

	return FALSE;
}

////////////////////////////////////////////////////////////////////////////////

void OptionsDialog::Go( HWND parent_, MainWindow* mainWnd_, Settings* settings_)
{
	ASSURE(parent_ != NULL)
	parent = parent_;

	ASSURE(mainWnd_ != NULL)
	mainWnd = mainWnd_;

	ASSURE(settings_ != NULL)
	settings = settings_;

	::DialogBoxParam(
		appInstance, MAKEINTRESOURCE(IDD_OPTIONS), parent, DlgProc,
		(LPARAM) this);
}

////////////////////////////////////////////////////////////////////////////////

BOOL OptionsDialog::OnMsg(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_INITDIALOG:
			OnInitDialog();
			return TRUE;

		case WM_DESTROY:
			OnDestroy();
			return 0;

		case WM_COMMAND:
			switch (LOWORD(wParam))
			{
				case IDC_TRIGGER_RECORD_SET:
					SetTrigger(TRIGGER_RECORD);
					break;

				case IDC_TRIGGER_STOP_SET:
					SetTrigger(TRIGGER_STOP);
					break;

				case IDC_TRIGGER_PLAY_SET:
					SetTrigger(TRIGGER_PLAY);
					break;

				case IDC_TRIGGER_RECORD_CLEAR:
					ClearTrigger(TRIGGER_RECORD);
					break;

				case IDC_TRIGGER_STOP_CLEAR:
					ClearTrigger(TRIGGER_STOP);
					break;

				case IDC_TRIGGER_PLAY_CLEAR:
					ClearTrigger(TRIGGER_PLAY);
					break;

				case IDOK:
				case IDCANCEL:
					::EndDialog(hwnd, 0);
					return TRUE;
			}
			break;
	}

	return FALSE;
}

////////////////////////////////////////////////////////////////////////////////

void OptionsDialog::OnInitDialog()
{
	HICON icon = ::LoadIcon(appInstance, MAKEINTRESOURCE(IDI_ICON));
	::SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM) icon);

	InitDevices();

	Check(IDC_ECHO, settings->echoInput);

	Check(IDC_IGNORE_KEY_PRESSURE,     settings->ignoreKeyPressure);
	Check(IDC_IGNORE_CHANNEL_PRESSURE, settings->ignoreChannelPressure);
	Check(IDC_IGNORE_PROGRAM_CHANGE,   settings->ignoreProgramChange);

	DisplayTrigger(TRIGGER_RECORD);
	DisplayTrigger(TRIGGER_STOP);
	DisplayTrigger(TRIGGER_PLAY);

	triggerDialog = NULL;
	settingTrigger = NO_TRIGGER;
}

////////////////////////////////////////////////////////////////////////////////

void OptionsDialog::InitDevices()
{
	MIDIINCAPS inCaps;
	int numInDevs = ::midiInGetNumDevs();

	for (int t = 0; t < numInDevs; ++t)
	{
		if (MMSYSERR_NOERROR == ::midiInGetDevCaps(t, &inCaps, sizeof(inCaps)))
		{
			::SendDlgItemMessage(hwnd, IDC_INPUT_DEVICE, CB_ADDSTRING, 0,
									(LPARAM) inCaps.szPname);
		}
	}

	if (numInDevs == 0)
	{
		::SendDlgItemMessage(hwnd, IDC_INPUT_DEVICE, CB_ADDSTRING, 0,
								(LPARAM) "No MIDI input devices available...");
	}

	::SendDlgItemMessage(hwnd, IDC_INPUT_DEVICE, CB_SETCURSEL, settings->inDev, 0);

	MIDIOUTCAPS outCaps;
	int numOutDevs = ::midiOutGetNumDevs();

	for (int t = 0; t < numOutDevs; ++t)
	{
		if (MMSYSERR_NOERROR == ::midiOutGetDevCaps(t, &outCaps, sizeof(outCaps)))
		{
			::SendDlgItemMessage(hwnd, IDC_OUTPUT_DEVICE, CB_ADDSTRING, 0,
									(LPARAM) outCaps.szPname);
		}
	}

	if (numOutDevs == 0)
	{
		::SendDlgItemMessage(hwnd, IDC_INPUT_DEVICE, CB_ADDSTRING, 0,
								(LPARAM) "No MIDI output devices available...");
	}

	::SendDlgItemMessage(hwnd, IDC_OUTPUT_DEVICE, CB_SETCURSEL, settings->outDev, 0);
}

////////////////////////////////////////////////////////////////////////////////

void OptionsDialog::OnDestroy()
{
	settings->inDev = ::SendDlgItemMessage(hwnd, IDC_INPUT_DEVICE, CB_GETCURSEL, 0, 0);
	settings->outDev = ::SendDlgItemMessage(hwnd, IDC_OUTPUT_DEVICE, CB_GETCURSEL, 0, 0);

	settings->echoInput = IsChecked(IDC_ECHO);

	settings->ignoreKeyPressure     = IsChecked(IDC_IGNORE_KEY_PRESSURE);
	settings->ignoreChannelPressure = IsChecked(IDC_IGNORE_CHANNEL_PRESSURE);
	settings->ignoreProgramChange   = IsChecked(IDC_IGNORE_PROGRAM_CHANGE);
}

////////////////////////////////////////////////////////////////////////////////

void OptionsDialog::DisplayTrigger(int trigger)
{
	int status = settings->triggerStatus[trigger];
	int value = settings->triggerValue[trigger];

	char buf[128];
	//wchar_t buf[128];
	switch (status)
	{
		case NO_TRIGGER:		
			sprintf(buf, TEXT("(No trigger set)"));
			//swprintf(buf, TEXT("(No trigger set)"));
			break;

		case NOTE_ON:
			sprintf(buf, TEXT("Note On: %s"), noteLookup[value]);
			//swprintf(buf, TEXT("Note On: %s"), noteLookup[value]);
			break;

		case NOTE_OFF:
			sprintf(buf, TEXT("Note Off: %s"), noteLookup[value]);
			//swprintf(buf, TEXT("Note Off: %s"), noteLookup[value]);
			break;

		case CONTROL_CHANGE:
			if (controllerLookup[value][0] != '\0')
				sprintf(buf, TEXT("%s"), controllerLookup[value]);
				//swprintf(buf, TEXT("%s"), controllerLookup[value]);
			else
				sprintf(buf, TEXT("Control: 0x%x"), value);
				//swprintf(buf, TEXT("Control: 0x%x"), value);
			break;

		case KEY_PRESSURE:
			sprintf(buf, TEXT("Key Pressure: %s"), noteLookup[value]);
			//swprintf(buf, TEXT("Key Pressure: %s"), noteLookup[value]);
			break;

		case PROGRAM_CHANGE:
			sprintf(buf, TEXT("Program Change: %d"), value);
			//swprintf(buf, TEXT("Program Change: %d"), value);
			break;

		case PITCH_BEND:
			sprintf(buf, TEXT("Pitch Bend"));
			//swprintf(buf, TEXT("Pitch Bend"));
			break;

		case CHANNEL_PRESSURE:
			sprintf(buf, TEXT("Channel Pressure"));
			//swprintf(buf, TEXT("Channel Pressure"));
			break;

		default:
			sprintf(buf, TEXT("Event: 0x%x, 0x%x"), status, value);
			//swprintf(buf, TEXT("Event: 0x%x, 0x%x"), status, value);
			break;
	}

	UINT id;
	switch (trigger)
	{
		case TRIGGER_RECORD: id = IDC_TRIGGER_RECORD_EDIT; break;
		case TRIGGER_STOP:   id = IDC_TRIGGER_STOP_EDIT;   break;
		case TRIGGER_PLAY:   id = IDC_TRIGGER_PLAY_EDIT;   break;
	}

	::SetWindowText(GetDlgItem(hwnd, id), buf);
}

////////////////////////////////////////////////////////////////////////////////

void OptionsDialog::SetTrigger(int trigger)
{
	ASSURE(triggerDialog == NULL)

	settingTrigger = trigger;
	mainWnd->CaptureTrigger();

	triggerDialog = new TriggerDialog;
	if (!triggerDialog->Go(hwnd))
		mainWnd->CancelCapturingTrigger();

	delete triggerDialog;
	triggerDialog = NULL;
}

////////////////////////////////////////////////////////////////////////////////

void OptionsDialog::TriggerCaptured(int status, int value)
{
	if (triggerDialog != NULL)
		::EndDialog(triggerDialog->hwnd, 0);

	settings->triggerStatus[settingTrigger] = status;
	settings->triggerValue[settingTrigger] = value;
	DisplayTrigger(settingTrigger);
	settingTrigger = -1;
}

////////////////////////////////////////////////////////////////////////////////

void OptionsDialog::ClearTrigger(int trigger)
{
	settings->triggerStatus[trigger] = NO_TRIGGER;
	DisplayTrigger(trigger);
}

////////////////////////////////////////////////////////////////////////////////
