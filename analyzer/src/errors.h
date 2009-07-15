/*!
 * @file
 * @brief Short description.
 *
 * Long description. I would paste some Loren Ipsum rubbish here, but I'm afraid
 * It would stay that way. Not that this comment is by any means ingenious but
 * at least people can understand it.
 */
#pragma once

static const char* ERR_MAINWIN_CREATION = "Main window creation failed.";
static const char* ERR_INVALID_PLUGIN = "File is not a valid plugin:\n%1";
static const char* ERR_LOADED_PLUGIN = "File is already loaded as a plugin:\n%1";
static const char* ERR_NO_SNIFFER = "No sniffer plugin is currently loaded.";

enum MsgType {
	Critical, Error, Warning, Info
};
