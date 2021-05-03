#pragma once

/* Wrapper for a bitwise checker */
#define BIT(x) (1 << x)

/* Wrapper for the Event binding function */
#define OSR_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)