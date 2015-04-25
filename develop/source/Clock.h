
#ifndef CLOCK_H
#define CLOCK_H

enum
{
	CLOCK_INACTIVE = 0,
	CLOCK_WAITING,
	CLOCK_RECORDING,
	CLOCK_PLAYING
};

class Clock : public Subclassed
{
public:
	Clock();
	virtual ~Clock();

	void SetTime(uint32 millis);
	void SetState(int state);
	void Blink();

protected:
	virtual void OnSubclass();
	virtual LRESULT OnReflect(UINT, WPARAM, LPARAM);

protected:
	HFONT font;
	HBRUSH brush;
	int state;
	bool blink;
};

#endif // CLOCK_H
