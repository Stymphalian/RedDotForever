
#ifndef ABOUT_BOX_H
#define ABOUT_BOX_H

#include "Label.h"
#include "HyperLink.h"

class AboutBox : public Dialog
{
public:
	void Go(HWND parent);

protected:
	virtual BOOL OnMsg(UINT, WPARAM, LPARAM);

private:
	void OnInitDialog();

private:
	Label titleLabel;
	HyperLink webLink;
	HyperLink donateLink;
};

#endif // ABOUT_BOX_H
