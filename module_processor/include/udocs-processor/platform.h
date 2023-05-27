/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#pragma once

/*
 * The options are:
 * PLATFORM_WIN64
 * PLATFORM_UNIX
 *
 * The options are mutually exclusive
 * */

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#ifdef _WIN64
#define PLATFORM_WIN64 1
#endif
#elif (__linux__ || __unix__ || defined(_POSIX_VERSION)) \
    && (!__APPLE__ && !__MACH__)
#define PLATFORM_UNIX 1
#endif

#pragma once
#endif

#pragma once
#endif

