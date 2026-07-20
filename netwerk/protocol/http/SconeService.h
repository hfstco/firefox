/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_net_SconeService_h
#define mozilla_net_SconeService_h

#include <cstdint>

#include "mozilla/Maybe.h"

namespace mozilla::net {

// Stores the most recently received SCONE throughput advice in this process.
void SetGlobalSconeThroughputAdvice(Maybe<uint64_t> aAdvice);
Maybe<uint64_t> GetGlobalSconeThroughputAdvice();

}  // namespace mozilla::net

#endif  // mozilla_net_SconeService_h
