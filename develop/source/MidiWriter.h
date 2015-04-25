
#ifndef MIDI_WRITER_H
#define MIDI_WRITER_H

class MidiList;

class MidiWriter
{
public:
	MidiWriter();

	bool WriteMidiFile(MidiList* events, const char* filename);

private:
	uint32 MillisecondsToTicks(uint32 ms);
	bool WriteFourCC(char a, char b, char c, char d);
	bool Write32Bit(uint32 val);
	bool Write16Bit(uint16 val);
	void WriteByte(uint8 val);
	void WriteVarLength(uint32 val);
	void WriteTrack();

private:
	FILE* file;
	uint32 byteCount;
	MidiList* events;
};

#endif // MIDI_WRITER_H
