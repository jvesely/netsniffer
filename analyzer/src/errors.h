#pragma once

#define ERR_MAINWIN_CREATION "Main window creation failed."
#define ERR_INVALID_PLUGIN "File is not a valid plugin:\n%1"
#define ERR_LOADED_PLUGIN "File is already loaded as a plugin:\n%1"
#define ERR_NO_SNIFFER "No sniffer plugin is currently loaded."

enum MsgType {
	Critical, Error, Warning, Info
};
