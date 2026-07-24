/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_net_SconeService_h
#define mozilla_net_SconeService_h

#include <cstdint>

#include "mozilla/Maybe.h"

namespace mozilla::net {

inline constexpr char kSconeThroughputAdviceChangedTopic[] =
    "network:scone-throughput-advice-changed";

void SetSconeThroughputAdvice(uint64_t aConnectionId, Maybe<uint64_t> aAdvice);
Maybe<uint64_t> GetSconeThroughputAdvice(uint64_t aConnectionId);

}  // namespace mozilla::net

#endif  // mozilla_net_SconeService_h
