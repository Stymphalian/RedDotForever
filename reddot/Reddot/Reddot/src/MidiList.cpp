
#include "framework.h"
#include "MidiList.h"

////////////////////////////////////////////////////////////////////////////////

bool IsRealNoteOn(DWORD msg)
{
	return ((msg & 0xF0) == NOTE_ON) && (((msg >> 16) & 0xFF) != 0);
}

////////////////////////////////////////////////////////////////////////////////

bool IsFakeNoteOff(DWORD msg)
{
	return ((msg & 0xF0) == NOTE_ON) && (((msg >> 16) & 0xFF) == 0);
}

////////////////////////////////////////////////////////////////////////////////

void MidiList::AddEvent(MidiEvent event)
{
	events.push_back(event);
}

////////////////////////////////////////////////////////////////////////////////

int MidiList::CountEvents() const
{
	return events.size();
}

////////////////////////////////////////////////////////////////////////////////

MidiEvent& MidiList::EventAt(int i)
{
	return events[i];
}

////////////////////////////////////////////////////////////////////////////////

void MidiList::Clear()
{
	events.clear();
}

////////////////////////////////////////////////////////////////////////////////

void MidiList::AdjustEventTimes()
{
	// The timestamp is in milliseconds since midiInStart() was called.
	// We adjust the timestamps to be relative to the *first* received
	// "NOTE ON" event, instead. That way we remove any "dead time" at
	// the start of the recording.

	if (events.size() > 0)
	{
		DWORD firstTime = 0;

		for (event_list_t::iterator i = events.begin(); i != events.end(); ++i)
		{
			MidiEvent& event = *i;

			if (IsRealNoteOn(event.msg))     // find the first NOTE ON
			{                                // event (we do not start
				firstTime = event.time;      // on damper pedal, etc)
				break;
			}
		}

		for (event_list_t::iterator i = events.begin(); i != events.end(); ++i)
		{
			MidiEvent& event = *i;

			if (event.time > firstTime)
				event.time -= firstTime;
			else
				event.time = 0;   // events before first NOTE ON
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
