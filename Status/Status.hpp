// Copyright (C) 2010, Aret N Carlsen (aretcarlsen@autonomoustools.com).
// Status codes, typically used for enumerated returns.
// Licensed under GPLv3 and later versions. See license.txt or <http://www.gnu.org/licenses/>.


#pragma once

namespace Status {
// Status codes
  typedef uint8_t Status_t;
// Successful / Acceptable
  static const Status_t Status__Good = 0;
// Rejected / Aborted
  static const Status_t Status__Bad = 1;
// Busy / In Progress / Unavailable
  static const Status_t Status__Busy = 2;
// Completed / Finished
  static const Status_t Status__Complete = 3;
}

