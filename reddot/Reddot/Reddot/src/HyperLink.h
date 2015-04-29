
#ifndef HYPERLINK_H
#define HYPERLINK_H

#include <string>

class HyperLink : public Subclassed
{
public:
	HyperLink();
	virtual ~HyperLink();

	void SetURL(const char* url);

protected:
	virtual void OnSubclass();
	virtual LRESULT OnMsg(UINT, WPARAM, LPARAM);
	virtual LRESULT OnReflect(UINT, WPARAM, LPARAM);

protected:
	LRESULT OnReflectCommand(WPARAM wParam, LPARAM lParam);
	LRESULT OnReflectCtlColor(WPARAM wParam, LPARAM lParam);

	void FitWindow();

protected:
	//char* url;
	std::string url;
	HCURSOR cursor;
	HFONT underlineFont;
};

#endif // HYPERLINK_H
