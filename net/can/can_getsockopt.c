/****************************************************************************
 * net/can/can_setsockopt.c
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <sys/time.h>
#include <stdint.h>
#include <errno.h>
#include <assert.h>
#include <debug.h>

#include <netpacket/can.h>

#include <nuttx/net/net.h>
#include <nuttx/net/can.h>

#include "socket/socket.h"
#include "utils/utils.h"
#include "can/can.h"

#ifdef CONFIG_NET_CANPROTO_OPTIONS

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: can_getsockopt
 *
 * Description:
 *   can_getsockopt() retrieves the value for the option specified by the
 *   'option' argument for the socket specified by the 'psock' argument.  If
 *   the size of the option value is greater than 'value_len', the value
 *   stored in the object pointed to by the 'value' argument will be silently
 *   truncated. Otherwise, the length pointed to by the 'value_len' argument
 *   will be modified to indicate the actual length of the 'value'.
 *
 *   See <sys/socket.h> a complete list of values for the socket-level
 *   'option' argument.  Protocol-specific options are are protocol specific
 *   header files (such as netpacket/can.h for the case of the CAN protocol).
 *
 * Input Parameters:
 *   psock     Socket structure of the socket to query
 *   level     Protocol level to set the option
 *   option    identifies the option to get
 *   value     Points to the argument value
 *   value_len The length of the argument value
 *
 * Returned Value:
 *   Returns zero (OK) on success.  On failure, it returns a negated errno
 *   value to indicate the nature of the error.  See psock_getsockopt() for
 *   the complete list of appropriate return error codes.
 *
 ****************************************************************************/

int can_getsockopt(FAR struct socket *psock, int option,
                   FAR void *value, FAR socklen_t *value_len)
{
  FAR struct can_conn_s *conn;
  int ret;
  int count = 0;

  DEBUGASSERT(psock != NULL && value != NULL && value_len != NULL &&
              psock->s_conn != NULL);
  conn = (FAR struct can_conn_s *)psock->s_conn;

  if (psock->s_type != SOCK_RAW)
    {
      nerr("ERROR:  Not a RAW CAN socket\n");
      return -ENOTCONN;
    }

  switch (option)
    {
      case CAN_RAW_FILTER:
        if (*value_len % sizeof(struct can_filter) != 0)
          {
            ret = -EINVAL;
          }

        if (value_len > CAN_RAW_FILTER_MAX * sizeof(struct can_filter))
          {
            ret = -EINVAL;
          }

        count = *value_len / sizeof(struct can_filter);

        /* FIXME pass filter to driver */

        break;

      case CAN_RAW_ERR_FILTER:
        break;

      case CAN_RAW_LOOPBACK:
        break;

      case CAN_RAW_RECV_OWN_MSGS:
        break;

      case CAN_RAW_FD_FRAMES:
        break;

      case CAN_RAW_JOIN_FILTERS:
        break;

      default:
        nerr("ERROR: Unrecognized RAW CAN socket option: %d\n", option);
        ret = -ENOPROTOOPT;
        break;
    }

  return ret;
}

#endif /* CONFIG_NET_CANPROTO_OPTIONS */