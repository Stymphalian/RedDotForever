
#"RED DOT FOREVER" DEVELOPMENT NOTES

For version 1.04, 8-sept-2007

For version 1.04.1, 25-april-2015


#HOW TO BUILD:

- 	No idea what is being used to build this right now... MSVC, but I don't
	know what version/anything. Hold tight i'll get back to you..

-	As of version 1.03, Red Dot is compiled with Code::Blocks and MinGW.
	(http://www.codeblocks.org)

	But you can probably get it to compile with most modern C++ compilers.

	I used ResEd to edit the resources. (http://www.radasm.com/resed/)

-	The installer was built with Inno Setup 5. Two batch files are provided to
	automate this process (makeinstaller.bat and cleaninstaller.bat), although
	you may need to tweak them to work on your system.


#BUGS:

-	Some users have reported that Red Dot gives the error message: "Cannot open
	MIDI input" when they try to record something. It turns out that in these
	cases Red Dot somehow crashed before and that this first copy was still in
	memory, holding on to the MIDI In device.

	I have seen something like this on my own computer, where Red Dot locked up
	and not even Task Manager was able to kill it. Then I noticed my USB MIDI
	device's light had gone out on my USB hub. I plugged it out and in again,
	and Red Dot became responsive again. Weird.

	I don't know if this is a bug in the code, or strange behavior from Windows.
	I also don't know if the incident on my computer is similar to what happened
	with these other users.

# CHANGE LIST:
_28-april-2015_

-	Converted application for mostly unicode/utf-8 support
-	Got rid of using the Windows registry.Now uses a settings.ini file.

_25-april-2015_

- 	Added Unicode/UTF-8 support?
- 	Started writing tests. Using ("Catch")[https://github.com/philsquared/Catch] framework

# GOALS:

-	Unicode/UTF-8 support
-	Remove registry dependency
-	New GUI library
-	Tests
-	Cross-platform (Windows/Linux)