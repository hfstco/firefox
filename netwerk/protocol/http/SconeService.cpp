/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/net/SconeService.h"

#include "mozilla/DataMutex.h"
#include "mozilla/Services.h"
#include "mozilla/net/SocketProcessChild.h"
#include "nsHashKeys.h"
#include "nsIObserverService.h"
#include "nsIXULRuntime.h"
#include "nsTArray.h"
#include "nsTHashMap.h"
#include "nsThreadUtils.h"

namespace mozilla::net {

static StaticDataMutex<nsTHashMap<nsUint64HashKey, uint64_t>>
    sSconeThroughputAdvice("sSconeThroughputAdvice");

static void NotifySconeThroughputAdviceChanged(uint64_t aConnectionId,
                                               Maybe<uint64_t> aAdvice) {
  auto notify = [aConnectionId, aAdvice]() {
    if (XRE_IsSocketProcess()) {
      if (SocketProcessChild* child = SocketProcessChild::GetSingleton()) {
        (void)child->SendSconeThroughputAdviceChanged(aConnectionId, aAdvice);
      }
      return;
    }

    nsCOMPtr<nsIObserverService> observerService =
        mozilla::services::GetObserverService();
    if (observerService) {
      nsAutoString connectionId;
      connectionId.AppendInt(aConnectionId);
      observerService->NotifyObservers(
          nullptr, kSconeThroughputAdviceChangedTopic, connectionId.get());
    }
  };

  if (NS_IsMainThread()) {
    notify();
  } else {
    NS_DispatchToMainThread(NS_NewRunnableFunction(
        "NotifySconeThroughputAdviceChanged", std::move(notify)));
  }
}

void SetSconeThroughputAdvice(uint64_t aConnectionId, Maybe<uint64_t> aAdvice) {
  {
    auto throughputAdvice = sSconeThroughputAdvice.Lock();
    if (aAdvice) {
      if (auto entry = throughputAdvice->Lookup(aConnectionId)) {
        if (entry.Data() == *aAdvice) {
          return;
        }
        entry.Data() = *aAdvice;
      } else {
        throughputAdvice->InsertOrUpdate(aConnectionId, *aAdvice);
      }
    } else if (!throughputAdvice->Remove(aConnectionId)) {
      return;
    }
  }

  NotifySconeThroughputAdviceChanged(aConnectionId, aAdvice);
}

Maybe<uint64_t> GetSconeThroughputAdvice(uint64_t aConnectionId) {
  auto throughputAdvice = sSconeThroughputAdvice.Lock();
  if (auto entry = throughputAdvice->Lookup(aConnectionId)) {
    return Some(entry.Data());
  }
  return Nothing();
}

void ClearAllSconeThroughputAdvice() {
  nsTArray<uint64_t> connectionIds;
  {
    auto throughputAdvice = sSconeThroughputAdvice.Lock();
    connectionIds.SetCapacity(throughputAdvice->Count());
    for (auto iter = throughputAdvice->Iter(); !iter.Done(); iter.Next()) {
      connectionIds.AppendElement(iter.Key());
    }
    throughputAdvice->Clear();
  }

  for (uint64_t connectionId : connectionIds) {
    NotifySconeThroughputAdviceChanged(connectionId, Nothing());
  }
}

}  // namespace mozilla::net
