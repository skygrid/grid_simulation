/* simgrid_config.h - Results of the configure made visible to user code.   */

/* Copyright (c) 2009-2016. The SimGrid Team. All rights reserved.          */

/* This program is free software; you can redistribute it and/or modify it
 * under the terms of the license (GNU LGPL) which comes with this package. */

#ifndef SIMGRID_PUBLIC_CONFIG_H
#define SIMGRID_PUBLIC_CONFIG_H

/** Define the version numbers of the used header files. See sg_version() to retrieve the version of the dynamic library. */
#define SIMGRID_VERSION_MAJOR 3
#define SIMGRID_VERSION_MINOR 13
#define SIMGRID_VERSION_PATCH 90

/* Version as a single integer. v3.4 is 30400, v3.16.2 is 31602, v42 will be 420000, and so on. */
#define SIMGRID_VERSION (100UL * (100UL * (3) + (13)) + (90))

#define SIMGRID_VERSION_STRING "SimGrid version 3.13.90-prealpha"

#define HAVE_JEDULE     0 /* Was Jedule compiled in?  */
#define HAVE_LUA        0 /* Was the Lua support compiled in? */
#define HAVE_MALLOCATOR 1 /* Were mallocators (object pools) compiled in? */
#define HAVE_MC         0 /* Was the model-checking compiled in? */
#define HAVE_NS3        0 /* Was the NS3 support compiled in? */

#endif /* SIMGRID_PUBLIC_CONFIG_H */
