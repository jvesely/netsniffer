#pragma once

static const char* ERR_MAINWIN_CREATION = "Main window creation failed.";
static const char* ERR_INVALID_PLUGIN = "File is not a valid plugin:\n%1";
static const char* ERR_LOADED_PLUGIN = "File is already loaded as a plugin:\n%1";
static const char* ERR_NO_SNIFFER = "No sniffer plugin is currently loaded.";

enum MsgType {
	Critical, Error, Warning, Info
};
