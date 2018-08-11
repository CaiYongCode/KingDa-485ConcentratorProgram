/*************************************************
 *
 * Debug implementation of "__write" that redirects to use DCC
 *
 * Copyright 2008 IAR Systems.  All rights reserved.
 *
 * $Revision: 24126 $
 *
 **************************************************/

#include <yfuns.h>

#include "JLINKDCC.h"

__interwork size_t __write(int handle, const unsigned char *buf, size_t size)
{

  JLINKDCC_SendOnChannel(0, (unsigned char*) buf, size);

  return size;
}

