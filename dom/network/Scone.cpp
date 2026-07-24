/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "Scone.h"

#include "mozilla/Services.h"
#include "mozilla/dom/SconeBinding.h"
#include "mozilla/net/SconeService.h"
#include "nsIObserverService.h"

namespace mozilla::dom::network {

NS_IMPL_CYCLE_COLLECTION_CLASS(Scone)

NS_IMPL_CYCLE_COLLECTION_UNLINK_BEGIN_INHERITED(Scone, DOMEventTargetHelper)
  tmp->Shutdown();
  NS_IMPL_CYCLE_COLLECTION_UNLINK_WEAK_REFERENCE
NS_IMPL_CYCLE_COLLECTION_UNLINK_END

NS_IMPL_CYCLE_COLLECTION_TRAVERSE_BEGIN_INHERITED(Scone, DOMEventTargetHelper)
NS_IMPL_CYCLE_COLLECTION_TRAVERSE_END

NS_INTERFACE_MAP_BEGIN_CYCLE_COLLECTION(Scone)
  NS_INTERFACE_MAP_ENTRY(nsIObserver)
  NS_INTERFACE_MAP_ENTRY(nsISupportsWeakReference)
NS_INTERFACE_MAP_END_INHERITING(DOMEventTargetHelper)

NS_IMPL_ADDREF_INHERITED(Scone, DOMEventTargetHelper)
NS_IMPL_RELEASE_INHERITED(Scone, DOMEventTargetHelper)

Scone::Scone(nsPIDOMWindowInner* aWindow, uint64_t aConnectionId,
             Maybe<uint64_t> aThroughputAdvice)
    : DOMEventTargetHelper(aWindow),
      mConnectionId(aConnectionId),
      mThroughputAdvice(aThroughputAdvice) {
  nsCOMPtr<nsIObserverService> observerService =
      mozilla::services::GetObserverService();
  if (observerService) {
    observerService->AddObserver(this, net::kSconeThroughputAdviceChangedTopic,
                                 true);
  }
}

Scone::~Scone() { Shutdown(); }

JSObject* Scone::WrapObject(JSContext* aCx, JS::Handle<JSObject*> aGivenProto) {
  return Scone_Binding::Wrap(aCx, this, aGivenProto);
}

Nullable<uint64_t> Scone::GetThroughputAdvice() const {
  Nullable<uint64_t> result;
  if (mThroughputAdvice) {
    result.SetValue(mThroughputAdvice.value());
  }
  return result;
}

void Scone::Shutdown() {
  if (mShutdown) {
    return;
  }
  mShutdown = true;

  nsCOMPtr<nsIObserverService> observerService =
      mozilla::services::GetObserverService();
  if (observerService) {
    observerService->RemoveObserver(this,
                                    net::kSconeThroughputAdviceChangedTopic);
  }
}

void Scone::DisconnectFromOwner() {
  Shutdown();
  DOMEventTargetHelper::DisconnectFromOwner();
}

NS_IMETHODIMP Scone::Observe(nsISupports* aSubject, const char* aTopic,
                             const char16_t* aData) {
  if (!strcmp(aTopic, net::kSconeThroughputAdviceChangedTopic)) {
    nsresult rv;
    uint64_t connectionId = nsDependentString(aData).ToInteger64(&rv);
    if (NS_SUCCEEDED(rv) && connectionId == mConnectionId) {
      Update(net::GetSconeThroughputAdvice(mConnectionId), true);
    }
  }
  return NS_OK;
}

void Scone::Update(Maybe<uint64_t> aAdvice, bool aNotify) {
  if (mShutdown) {
    return;
  }

  mThroughputAdvice = aAdvice;
  if (aNotify) {
    DispatchTrustedEvent(u"change"_ns);
  }
}

}  // namespace mozilla::dom::network
