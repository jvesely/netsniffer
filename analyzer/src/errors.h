#pragma once

#define ERR_MAINWIN_CREATION "Main window creation failed."
#define ERR_INVALID_SNIFFER "File is not a valid sniffer plugin:\n%1"
#define ERR_INVALID_ENGINE "File is not a valid recognizing engine."
#define ERR_LOADED_PLUGIN "File is already loaded as a plugin:\n%1"
#define ERR_NO_SNIFFER "No sniffer plugin is currently loaded."

enum MsgType {
	Critical, Error, Warning, Info
};
