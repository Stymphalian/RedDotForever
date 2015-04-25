
#ifndef LABEL_H
#define LABEL_H

class Label : public Subclassed
{
public:
	Label();
	virtual ~Label();

	void SetBold(bool bold);

protected:
	virtual void OnSubclass();
	virtual LRESULT OnReflect(UINT, WPARAM, LPARAM);

protected:
	bool bold;
	HFONT font;
};

#endif // LABEL_H
