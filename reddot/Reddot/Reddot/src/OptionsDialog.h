
#ifndef OPTIONS_DIALOG_H
#define OPTIONS_DIALOG_H

class MainWindow;
class Settings;

class TriggerDialog: public Dialog
{
public:
	bool Go(HWND parent);

protected:
	virtual BOOL OnMsg(UINT, WPARAM, LPARAM);
};

class OptionsDialog : public Dialog
{
public:
	void Go(HWND parent, MainWindow* mainWnd, Settings* settings);
	void TriggerCaptured(int status, int value);

protected:
	virtual BOOL OnMsg(UINT, WPARAM, LPARAM);

private:
	void OnInitDialog();
	void InitDevices();
	void OnDestroy();

	void SetTrigger(int trigger);
	void DisplayTrigger(int trigger);
	void ClearTrigger(int trigger);

private:
	MainWindow* mainWnd;
	Settings* settings;
	TriggerDialog* triggerDialog;
	int settingTrigger;
};

#endif // OPTIONS_DIALOG_H
