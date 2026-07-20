/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/net/SconeService.h"

#include "mozilla/StaticMutex.h"

namespace mozilla::net {

static StaticMutex sSconeMutex;
static Maybe<uint64_t> sSconeThroughputAdvice MOZ_GUARDED_BY(sSconeMutex);

void SetGlobalSconeThroughputAdvice(Maybe<uint64_t> aAdvice) {
  StaticMutexAutoLock lock(sSconeMutex);
  sSconeThroughputAdvice = aAdvice;
}

Maybe<uint64_t> GetGlobalSconeThroughputAdvice() {
  StaticMutexAutoLock lock(sSconeMutex);
  return sSconeThroughputAdvice;
}

}  // namespace mozilla::net
