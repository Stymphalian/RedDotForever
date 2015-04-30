
#include "framework.h"
#include "MidiList.h"
#include "MidiWriter.h"

#define TICKS_PER_BEAT    96
#define BEATS_PER_MINUTE  120

////////////////////////////////////////////////////////////////////////////////

MidiWriter::MidiWriter()
{
	file = NULL;
}

////////////////////////////////////////////////////////////////////////////////

uint32 MidiWriter::MillisecondsToTicks(uint32 ms)
{
	return (uint32) ((uint64) ms * BEATS_PER_MINUTE * TICKS_PER_BEAT / 60000);
}

////////////////////////////////////////////////////////////////////////////////

bool MidiWriter::WriteFourCC(char a, char b, char c, char d)
{
	char fourcc[4] = { a, b, c, d };

	return 1 == fwrite(fourcc, 4, 1, file);
}

////////////////////////////////////////////////////////////////////////////////

bool MidiWriter::Write32Bit(uint32 val)
{
	uint8 buf[4];
	buf[0] = (val >> 24) & 0xFF;
	buf[1] = (val >> 16) & 0xFF;
	buf[2] = (val >>  8) & 0xFF;
	buf[3] =  val        & 0xFF;

	return 1 == fwrite(buf, 4, 1, file);
}

////////////////////////////////////////////////////////////////////////////////

bool MidiWriter::Write16Bit(uint16 val)
{
	uint8 buf[2];
	buf[0] = (val >> 8) & 0xFF;
	buf[1] = val & 0xFF;

	return 1 == fwrite(buf, 2, 1, file);
}

////////////////////////////////////////////////////////////////////////////////

void MidiWriter::WriteByte(uint8 val)
{
	if (1 == fwrite(&val, 1, 1, file))
		++byteCount;
}

////////////////////////////////////////////////////////////////////////////////

void MidiWriter::WriteVarLength(uint32 val)
{
	// create a buffer of the values to write
	// initialize to the first 7 bits of the value.
	// The 8th bit is a flag bit which we use to inform
	// us to stop writing out bits.
	uint32 buffer = val & 0x7F;

	// while the val still has values in the
	// higher byte ranges, continue to add the bits
	// into the buffer.
	while ((val >>= 7) != 0)
	{	
		// shift the buffer over by 8 bits in order
		// to make room for the next byte.
		buffer <<= 8;

		// write the 7 bits into the buffer
		// set the 8th bit to 1. A '1' in the 8th position 
		// is used as a flag later to tell us that there are more bits to write
		buffer |= ((val & 0x7F) | 0x80);
	}

	while (true)
	{
		// keep writing out bytes, until we reach the byte where the flag
		// bit is not set.
		WriteByte(buffer);
		if (buffer & 0x80){
			// shift the buffer back to write out the next 7 bits.
			buffer >>= 8;
		}else{
			// once the 8th bit is '0' we know that
			// we have written the last byte; therefore we can now exit.
			break;
		}

	}
}

////////////////////////////////////////////////////////////////////////////////

void MidiWriter::WriteTrack()
{
	WriteFourCC('M', 'T', 'r', 'k');

	long lengthPos = ftell(file);  // remember this to
	Write32Bit(0);                 // temporarily write a chunk length

	byteCount = 0;
	uint32 oldTime = 0;
	uint32 newTime;

	// meta event
	WriteVarLength(0);                          // tempo change	
	WriteByte(0xFF);                            // to set our BPM
	WriteByte(0x51);
	WriteByte(0x03);
	uint32 val = 60000000 / BEATS_PER_MINUTE;   // meta event
	WriteByte(val >> 16);
	WriteByte(val >> 8);
	WriteByte(val);

	for (int i = 0; i < events->CountEvents(); ++i)
	{
		MidiEvent& event = events->EventAt(i);

		newTime = MillisecondsToTicks(event.time);  // store delta time;
		WriteVarLength(newTime - oldTime);          // first event is
		oldTime = newTime;                          // always 0 already

		uint8 byte1 = (uint8) ((event.msg      ) & 0xFF);
		uint8 byte2 = (uint8) ((event.msg >> 8 ) & 0xFF);
		uint8 byte3 = (uint8) ((event.msg >> 16) & 0xFF);

		switch (byte1 & 0xF0)
		{
			case NOTE_OFF:
			case NOTE_ON:
			case KEY_PRESSURE:
			case CONTROL_CHANGE:
			case PITCH_BEND:
				WriteByte(byte1);
				WriteByte(byte2);
				WriteByte(byte3);
				break;

			case PROGRAM_CHANGE:
			case CHANNEL_PRESSURE:
				WriteByte(byte1);
				WriteByte(byte2);
				break;
		}
	}

	//meta event to denote the end of the track
	WriteVarLength(0);
	WriteByte(0xFF);   // the end-of-track
	WriteByte(0x2F);   // marker is required
	WriteByte(0x00);

	// go back to the beginning of the track and write out the byte length
	fseek(file, lengthPos, SEEK_SET);
	Write32Bit(byteCount);
	fseek(file, 0, SEEK_END);
}

////////////////////////////////////////////////////////////////////////////////

bool MidiWriter::WriteMidiFile(MidiList* events_, const char* filename)
{
	file = fopen(filename, "wb");
	if (file == NULL)
		return false;

	events = events_;

	WriteFourCC('M', 'T', 'h', 'd');
	Write32Bit(6);
	Write16Bit(0);                // format 0
	Write16Bit(1);                // one track
	Write16Bit(TICKS_PER_BEAT);
	WriteTrack();

	fclose(file);
	file = NULL;

	return true;
}

////////////////////////////////////////////////////////////////////////////////
