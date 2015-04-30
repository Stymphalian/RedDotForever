
#include "framework.h" // appInstance
#include <mmsystem.h>
#include "defs.h"
#include "MainWindow.h"
#include "MidiWriter.h"
#include "AboutBox.h"
#include "OptionsDialog.h"

#define TIMER_ID  1

enum
{
	STORE_EMPTY = 0,
	STORE_UNSAVED,
	STORE_SAVED,
};

enum
{
	STATE_IDLE = 0,
	STATE_RECORDING,
	STATE_PLAYING,
	STATE_CAPTURING_TRIGGER
};

const UINT MSG_PLAYBACK_DONE = ::RegisterWindowMessage(
	TEXT("MSG_PLAYBACK_DONE-{602728F1-B21C-45fc-819E-9B732F49ED18}"));

////////////////////////////////////////////////////////////////////////////////

void MainWindow::Go()
{
	::DialogBoxParam(
		appInstance, 					// the window handle holding the dialog
		MAKEINTRESOURCE(IDD_DIALOG),	// dialog template to use
		 NULL, 							// no parent
		 DlgProc,						// the callback to run for this dialog
		(LPARAM) this);					// user-defined parameter
}

////////////////////////////////////////////////////////////////////////////////

BOOL MainWindow::OnMsg(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_COMMAND:
		case WM_CTLCOLORSTATIC:
		{
			if ((HWND) lParam == theClock.hwnd)
				return theClock.Reflect(msg, wParam, lParam);

			break;
		}
	}

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
				case IDCANCEL:
					OnQuit();
					return TRUE;

				case IDC_RECORD:
					OnRecord();
					return 0;

				case IDC_STOP:
					OnStop();
					return 0;

				case IDC_PLAY:
					OnPlay();
					return 0;

				case IDC_SAVE:
					OnSave();
					return 0;

				case IDC_EDIT:
					OnEdit();
					return 0;

				case IDC_OPTIONS:
					OnOptions();
					return 0;

				case IDC_MANUAL:
					OnHelp();
					return 0;

				case IDC_ABOUT:
					OnAbout();
					return 0;
			}
			break;

		case WM_TIMER:
			OnTimer();
			break;

		default:
			if (msg == MSG_PLAYBACK_DONE)
			{
				OnPlaybackDone();
				return TRUE;
			}
			break;
	}

	return FALSE;
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::OnInitDialog()
{
	settings.Load();

	store = STORE_EMPTY;
	state = STATE_IDLE;

	inHandle = NULL;
	outHandle = NULL;

	HICON icon = ::LoadIcon(appInstance, MAKEINTRESOURCE(IDI_ICON));
	::SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM) icon);

	// Create Icon resources
	HICON iconRecord  = ::LoadIcon(appInstance, MAKEINTRESOURCE(IDI_RECORD));
	HICON iconStop    = ::LoadIcon(appInstance, MAKEINTRESOURCE(IDI_STOP));
	HICON iconPlay    = ::LoadIcon(appInstance, MAKEINTRESOURCE(IDI_PLAY));
	HICON iconSave    = ::LoadIcon(appInstance, MAKEINTRESOURCE(IDI_SAVE));
	HICON iconEdit    = ::LoadIcon(appInstance, MAKEINTRESOURCE(IDI_EDIT));
	HICON iconOptions = ::LoadIcon(appInstance, MAKEINTRESOURCE(IDI_OPTIONS));

	// send messages to ourselve telling ourselve to set the specified icons on
	// the GUI
	::SendDlgItemMessage(hwnd, IDC_RECORD, BM_SETIMAGE, IMAGE_ICON, (LPARAM) iconRecord);
	::SendDlgItemMessage(hwnd, IDC_STOP, BM_SETIMAGE, IMAGE_ICON, (LPARAM) iconStop);
	::SendDlgItemMessage(hwnd, IDC_PLAY, BM_SETIMAGE, IMAGE_ICON, (LPARAM) iconPlay);
	::SendDlgItemMessage(hwnd, IDC_SAVE, BM_SETIMAGE, IMAGE_ICON, (LPARAM) iconSave);
	::SendDlgItemMessage(hwnd, IDC_EDIT, BM_SETIMAGE, IMAGE_ICON, (LPARAM) iconEdit);
	::SendDlgItemMessage(hwnd, IDC_OPTIONS, BM_SETIMAGE, IMAGE_ICON, (LPARAM) iconOptions);


	theClock.Subclass(hwnd, IDC_CLOCK);
	theClock.SetState(CLOCK_INACTIVE);

	// we are currenlty in the idle state, so update the UI to show the
	// the state to be IDLE, the store/song stored is currenty
	UpdateControls();

	::SendMessage(hwnd, DM_SETDEFID, (WPARAM) IDC_RECORD, 0);
	::SetFocus(::GetDlgItem(hwnd, IDC_RECORD));

	optionsDialog = NULL;

	OpenMidi();
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::OnDestroy()
{
	switch (state)
	{
		case STATE_RECORDING:
			StopRecording();
			break;

		case STATE_PLAYING:
			StopPlaying();
			thread_join(playThread);  // call this one manually now
			OnPlaybackDone();         // (message loop is dead)
			break;
	}

	trace(__FILEW__, __LINE__,L"%s",unicode_utils::utf8_to_utf16( GetCurrentDir(),NULL).c_str());
	settings.Save();
	CloseMidi();
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::OnRecord()
{
	if (state != STATE_RECORDING)
	{
		if (StartRecording())
		{
			state = STATE_RECORDING;
			store = STORE_UNSAVED;

			UpdateControls();
			::SendMessage(hwnd, DM_SETDEFID, (WPARAM) IDC_STOP, 0);
			::SetFocus(::GetDlgItem(hwnd, IDC_STOP));
		}
	}
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::OnPlay()
{
	if (state != STATE_PLAYING)
	{
		if (StartPlaying())
		{
			state = STATE_PLAYING;

			UpdateControls();
			::SendMessage(hwnd, DM_SETDEFID, (WPARAM) IDC_STOP, 0);
			::SetFocus(::GetDlgItem(hwnd, IDC_STOP));
		}
	}
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::OnStop()
{
	if (state == STATE_RECORDING)
	{
		StopRecording();
		state = STATE_IDLE;

		UpdateControls();
		::SendMessage(hwnd, DM_SETDEFID, (WPARAM) IDC_RECORD, 0);
		::SetFocus(::GetDlgItem(hwnd, IDC_RECORD));
	}
	else if (state == STATE_PLAYING)
	{
		StopPlaying();
	}
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::OnSave()
{
	/*char* filename = FilePicker(
		hwnd, true, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "", NULL,
		"MIDI Files (*.mid)\0*.mid\0All Files (*.*)\0*.*\0\0", "mid");*/

	// TODO: Get rid of this hack which creates s UTF-16 filter string
	std::string filename = FilePicker(
		hwnd, true, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, "", NULL,
		L"MIDI Files (*.mid)\0*.mid\0All Files (*.*)\0*.*\0\0","mid");


	if (!filename.empty())
	{		
		MidiWriter writer;
		if (!writer.WriteMidiFile(&events, filename.c_str()))
			::MessageBox(hwnd, L"Could not create MIDI file.", NULL, MB_OK | MB_ICONSTOP);
		
		store = STORE_SAVED;
		UpdateControls();
	}
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::OnEdit()
{
	::MessageBox(hwnd, L"This feature is not available yet.", NULL, MB_OK | MB_ICONINFORMATION);
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::OnOptions()
{
	optionsDialog = new OptionsDialog;
	optionsDialog->Go(hwnd, this, &settings);
	delete optionsDialog;
	optionsDialog = NULL;

	RestartMidi();   // in case the MIDI devices have changed
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::OnHelp()
{
	std::string dir = GetCurrentDir();
	if(dir.length()  != 0){
		char url[MAX_PATH + 1];
		sprintf(url, "file://%s/%s", dir, MANUAL_PATH);
		OpenUrl(url);
	}
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::OnAbout()
{
	AboutBox box;
	box.Go(hwnd);
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::OnQuit()
{
	::EndDialog(hwnd, FALSE);
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::OnTimer()
{
	if (firstNoteOn || (state == STATE_PLAYING))
		theClock.SetTime(timeGetTime() - startTime);
	else
		theClock.Blink();
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::OpenMidi()
{
	// We don't give an error message if we fail to open MIDI IN or OUT.
	// If it fails, handle will be NULL and we always check on that later.

	ASSURE(inHandle == NULL)
	ASSURE(outHandle == NULL)

	if (MMSYSERR_NOERROR == ::midiInOpen(
			&inHandle, settings.inDev, (DWORD) LaunchMidiInProc, (DWORD) this, CALLBACK_FUNCTION))
	{
		if (MMSYSERR_NOERROR != ::midiInStart(inHandle))
		{
			::midiInClose(inHandle);
			inHandle = NULL;
		}
	}

	::midiOutOpen(&outHandle, settings.outDev, 0, 0, 0);
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::CloseMidi()
{
	if (outHandle != NULL)
	{
		::midiOutReset(outHandle);
		::midiOutClose(outHandle);
		outHandle = NULL;
	}

	if (inHandle != NULL)
	{
		::midiInReset(inHandle);
		::midiInClose(inHandle);
		inHandle = NULL;
	}
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::RestartMidi()
{
	CloseMidi();
	OpenMidi();
}

////////////////////////////////////////////////////////////////////////////////

void CALLBACK MainWindow::LaunchMidiInProc(
	HMIDIIN midiIn, UINT msg, DWORD instance, DWORD param1, DWORD param2)
{
	((MainWindow*) instance)->MidiInProc(midiIn, msg, param1, param2);
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::MidiInProc(
	HMIDIIN midiIn, UINT msg, DWORD param1, DWORD param2)
{
	switch (msg)
	{
		case MIM_DATA:
			ReceiveMsg(param1, param2);
			break;

		case MIM_ERROR:
			break;

		case MIM_LONGDATA:
			break;

		case MIM_LONGERROR:
			break;
	}
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::ReceiveMsg(DWORD msg, DWORD time)
{
	switch (state)
	{
		case STATE_IDLE:
			IdleMsg(msg, time);
			break;

		case STATE_PLAYING:
			PlayingMsg(msg, time);
			break;

		case STATE_RECORDING:
			RecordingMsg(msg, time);
			break;

		case STATE_CAPTURING_TRIGGER:
			TriggerMsg(msg, time);
			break;
	}
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::IdleMsg(DWORD msg, DWORD time)
{
	if (optionsDialog == NULL)
	{
		if (IsTrigger(TRIGGER_RECORD, msg))
			OnRecord();
		else if (IsTrigger(TRIGGER_PLAY, msg) && (store != STORE_EMPTY))
			OnPlay();
	}
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::PlayingMsg(DWORD msg, DWORD time)
{
	if (IsTrigger(TRIGGER_STOP, msg))
		OnStop();

	/*else if (IsTrigger(TRIGGER_PLAY, msg))
	{
		OnStop();      this doesn't work because we play in another
		OnPlay();      thread -- not a big deal, not an important feature
	}*/
}

////////////////////////////////////////////////////////////////////////////////

bool MainWindow::StartRecording()
{
	ASSURE(state == STATE_IDLE)

	events.Clear();

	if (inHandle == NULL)
	{
		::MessageBox(hwnd, TEXT("Could not open MIDI input for recording.\r\n\r\nMake sure you have selected the correct MIDI input\r\nin the Options window.\r\n\r\nClose all other open MIDI applications. They may be\r\nusing the same MIDI input."), NULL, MB_OK | MB_ICONSTOP);
		return false;
	}

	if (settings.echoInput && (outHandle == NULL))
	{
		::MessageBox(hwnd, TEXT("Could not open MIDI output for playback.\r\nYou can still record."), NULL, MB_OK | MB_ICONEXCLAMATION);
	}

	firstNoteOn = false;
	theClock.SetState(CLOCK_WAITING);
	::SetTimer(hwnd, TIMER_ID, 200, NULL);

	return true;
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::StopRecording()
{
	ASSURE(state == STATE_RECORDING)

	events.AdjustEventTimes();

	::KillTimer(hwnd, TIMER_ID);
	theClock.SetState(CLOCK_INACTIVE);
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::RecordingMsg(DWORD msg, DWORD time)
{
	if (IsTrigger(TRIGGER_STOP, msg))
	{
		OnStop();
	}
	/*else if (IsTrigger(TRIGGER_RECORD, msg))
	{
		OnStop();       this is a nice feature, but inconsistent with how
		OnRecord();     the GUI works (need to press STOP first before you
		                can RECORD again), so disabled for now...
	}*/
	else
	{
		//TRACE((__FILE__,__LINE__,"MSG %x", msg & 0xFF))

		if (!firstNoteOn && IsRealNoteOn(msg))
		{
			firstNoteOn = true;
			theClock.SetState(CLOCK_RECORDING);
			startTime = timeGetTime();
		}

		if (AcceptMsg(msg))
		{
			MidiEvent m;
			m.msg = msg;
			m.time = time;
			events.AddEvent(m);

			if (settings.echoInput && (outHandle != NULL))
				::midiOutShortMsg(outHandle, msg);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////

bool MainWindow::AcceptMsg(DWORD msg)
{
	switch (msg & 0xF0)
	{
		case NOTE_OFF:
		case NOTE_ON:
		case CONTROL_CHANGE:
		case PITCH_BEND:
			return true;

		case KEY_PRESSURE:
			return !settings.ignoreKeyPressure;

		case CHANNEL_PRESSURE:
			return !settings.ignoreChannelPressure;

		case PROGRAM_CHANGE:
			return !settings.ignoreProgramChange;
	}

	return false;
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::CaptureTrigger()
{
	ASSURE(state == STATE_IDLE)

	state = STATE_CAPTURING_TRIGGER;
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::CancelCapturingTrigger()
{
	ASSURE(state == STATE_CAPTURING_TRIGGER)

	state = STATE_IDLE;
}

////////////////////////////////////////////////////////////////////////////////

bool MainWindow::IsTrigger(int trigger, DWORD msg)
{
	if (settings.triggerStatus[trigger] == NO_TRIGGER)
		return false;

	int status = (int)  (msg       & 0xF0);
	int value  = (int) ((msg >> 8) & 0xFF);

	// Some devices send a "Note Off" event as Note On with velocity 0.
	// We should not confuse this with a *real* Note On event trigger!
	if (IsFakeNoteOff(msg) && (settings.triggerStatus[trigger] == NOTE_ON))
		return false;

	switch (status)
	{
		case NOTE_OFF:           // look at status AND value
		case NOTE_ON:
		case CONTROL_CHANGE:
		case KEY_PRESSURE:
		case PROGRAM_CHANGE:
			return (status == settings.triggerStatus[trigger])
				&& (value == settings.triggerValue[trigger]);

		case PITCH_BEND:         // just look at status
		case CHANNEL_PRESSURE:
			return status == settings.triggerStatus[trigger];
	}

	return (status == settings.triggerStatus[trigger])
		&& (value == settings.triggerValue[trigger]);
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::TriggerMsg(DWORD msg, DWORD time)
{
	ASSURE(optionsDialog != NULL)

	int status = (int)  (msg        & 0xF0);
	int value  = (int) ((msg >>  8) & 0xFF);

	switch (status)
	{
		case NOTE_OFF:
		case NOTE_ON:
		case CONTROL_CHANGE:
		case KEY_PRESSURE:
		case PROGRAM_CHANGE:
			state = STATE_IDLE;
			optionsDialog->TriggerCaptured(status, value);
			break;

		case PITCH_BEND:
		case CHANNEL_PRESSURE:
			state = STATE_IDLE;
			optionsDialog->TriggerCaptured(status, 0);
			break;
	}
}

////////////////////////////////////////////////////////////////////////////////

bool MainWindow::StartPlaying()
{
	ASSURE(state == STATE_IDLE)

	if (outHandle == NULL)
	{
		::MessageBox(hwnd, TEXT("Could not open MIDI output for playback.\r\n\r\nMake sure you have selected the correct MIDI output\r\nin the Options window.\r\n\r\nClose all other open MIDI applications. They may be\r\nusing the same MIDI output."), NULL, MB_OK | MB_ICONSTOP);
		return false;
	}

	theClock.SetState(CLOCK_PLAYING);
	startTime = timeGetTime();
	::SetTimer(hwnd, TIMER_ID, 200, NULL);

	playing = true;
	thread_init(playThread, LaunchPlayThread, this);
	thread_resume(playThread);
	return true;
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::StopPlaying()
{
	ASSURE(state == STATE_PLAYING)

	playing = false;  // tell thread to stop
}

////////////////////////////////////////////////////////////////////////////////

DWORD WINAPI MainWindow::LaunchPlayThread(void* param)
{
	return ((MainWindow*) param)->PlayThread();
}

////////////////////////////////////////////////////////////////////////////////

DWORD MainWindow::PlayThread()
{
	DWORD startTime = timeGetTime();

	for (int i = 0; i < events.CountEvents(); ++i)
	{
		MidiEvent& event = events.EventAt(i);

		// When the user presses STOP, we have to immediately abort playback.
		// To do this, we check every 20 milliseconds if we're still good.

		while (playing && (timeGetTime() - startTime + 20 < event.time))
			::Sleep(20);

		if (!playing)
			break;

		while (timeGetTime() - startTime < event.time)
			; // "busy loop" for the last few milliseconds

		//TRACE((__FILE__,__LINE__,"too late: %u", timeGetTime() - startTime - event.time))

		::midiOutShortMsg(outHandle, event.msg);
	}

	::midiOutReset(outHandle);  // turn off all notes

	::PostMessage(hwnd, MSG_PLAYBACK_DONE, 0, 0);

	return 0;
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::OnPlaybackDone()
{
	thread_done(playThread);
	playThread = NULL;

	state = STATE_IDLE;

	::KillTimer(hwnd, TIMER_ID);
	theClock.SetState(CLOCK_INACTIVE);

	UpdateControls();
	::SendMessage(hwnd, DM_SETDEFID, (WPARAM) IDC_PLAY, 0);
	::SetFocus(::GetDlgItem(hwnd, IDC_PLAY));
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::UpdateControls()
{
	switch (state)
	{
		case STATE_RECORDING:
			SetDlgItemText(hwnd, IDC_STATUS, TEXT("Recording..."));

			EnableDlgItem(hwnd, IDC_RECORD, false);
			EnableDlgItem(hwnd, IDC_STOP, true);
			EnableDlgItem(hwnd, IDC_PLAY, false);
			EnableDlgItem(hwnd, IDC_SAVE, false);
			EnableDlgItem(hwnd, IDC_EDIT, false);
			EnableDlgItem(hwnd, IDC_OPTIONS, false);
			break;

		case STATE_PLAYING:
			SetDlgItemText(hwnd, IDC_STATUS, TEXT("Playing..."));

			EnableDlgItem(hwnd, IDC_RECORD, false);
			EnableDlgItem(hwnd, IDC_STOP, true);
			EnableDlgItem(hwnd, IDC_PLAY, false);
			EnableDlgItem(hwnd, IDC_SAVE, false);
			EnableDlgItem(hwnd, IDC_EDIT, false);
			EnableDlgItem(hwnd, IDC_OPTIONS, false);
			break;

		default:
			EnableDlgItem(hwnd, IDC_RECORD, true);
			EnableDlgItem(hwnd, IDC_STOP, false);
			EnableDlgItem(hwnd, IDC_OPTIONS, true);

			switch (store)
			{
				case STORE_EMPTY:
					SetDlgItemText(hwnd, IDC_STATUS, TEXT("(Nothing recorded yet)"));

					EnableDlgItem(hwnd, IDC_PLAY, false);
					EnableDlgItem(hwnd, IDC_SAVE, false);
					EnableDlgItem(hwnd, IDC_EDIT, false);
					break;

				case STORE_UNSAVED:
					SetDlgItemText(hwnd, IDC_STATUS, TEXT("Recording is NOT saved yet!"));

					EnableDlgItem(hwnd, IDC_PLAY, true);
					EnableDlgItem(hwnd, IDC_SAVE, true);
					EnableDlgItem(hwnd, IDC_EDIT, true);
					break;

				case STORE_SAVED:
					SetDlgItemText(hwnd, IDC_STATUS, TEXT("Saved"));

					EnableDlgItem(hwnd, IDC_PLAY, true);
					EnableDlgItem(hwnd, IDC_SAVE, true);
					EnableDlgItem(hwnd, IDC_EDIT, true);
					break;
			}
	}
}

////////////////////////////////////////////////////////////////////////////////
