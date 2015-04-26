
#ifndef MIDI_LIST_H
#define MIDI_LIST_H

#include <vector>

#define NOTE_OFF          0x80
#define NOTE_ON           0x90
#define KEY_PRESSURE      0xa0
#define CONTROL_CHANGE    0xb0
#define PROGRAM_CHANGE    0xc0
#define CHANNEL_PRESSURE  0xd0
#define PITCH_BEND        0xe0

bool IsRealNoteOn(DWORD msg);
bool IsFakeNoteOff(DWORD msg);

struct MidiEvent
{
	DWORD msg;
	DWORD time;
};

class MidiList
{
public:
	void AddEvent(MidiEvent event);
	int CountEvents() const;
	MidiEvent& EventAt(int i);
	void Clear();

	void AdjustEventTimes();

private:
	typedef std::vector<MidiEvent> event_list_t;
	event_list_t events;
};

#endif // MIDI_LIST_H
