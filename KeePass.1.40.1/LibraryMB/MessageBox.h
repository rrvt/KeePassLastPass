// Message Box Function


#pragma once


inline void messageBox(TCchar* msg) {AfxMessageBox(msg, MB_OK, 0);}


void getError(DWORD v, String& err);              // Turn a GetLastError() value into a string



