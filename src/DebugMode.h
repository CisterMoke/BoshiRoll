#pragma once

enum DebugMode
{
	DEBUG_OFF = 0x0,
	DEBUG_INFO = 0x1,
	DEBUG_DRAW = 0x2,
	DEBUG_ALL = DEBUG_INFO + DEBUG_DRAW
};
