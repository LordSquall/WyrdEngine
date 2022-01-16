#pragma once

/* Constants */
#define WYRD_PI 3.14159265359

/* Wrapper for a bitwise checker */
#define BIT(x) (1 << x)

/* Wrapper for the Event binding function */
#define WYRD_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1, std::placeholders::_2)

/* Converters */
#define WYRD_DEG_TO_RAD(x) (x * (WYRD_PI / 180.0))