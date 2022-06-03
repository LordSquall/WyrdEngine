#pragma once

/* Constants */
#define WYRD_PI 3.14159265359
#define WYRD_PIF 3.14159265359f

/* Wrapper for a bitwise checker */
#define BIT(x) (1 << x)

/* Wrapper for the Event binding function */
#define WYRD_BIND_FN(fn) std::bind(&fn, this, std::placeholders::_1)
#define WYRD_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1, std::placeholders::_2)

/* Converters */
#define WYRD_DEG_TO_RAD(x) (x * (WYRD_PI / 180.0))
#define WYRD_DEG_TO_RADF(x) (x * (WYRD_PIF / 180.0f))
