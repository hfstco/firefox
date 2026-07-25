/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_dom_network_Scone_h
#define mozilla_dom_network_Scone_h

#include "mozilla/DOMEventTargetHelper.h"
#include "mozilla/Maybe.h"
#include "mozilla/dom/Nullable.h"
#include "nsIObserver.h"
#include "nsWeakReference.h"

namespace mozilla::dom::network {

class Scone final : public DOMEventTargetHelper,
                    public nsIObserver,
                    public nsSupportsWeakReference {
 public:
  NS_DECL_ISUPPORTS_INHERITED
  NS_DECL_CYCLE_COLLECTION_CLASS_INHERITED(Scone, DOMEventTargetHelper)
  NS_DECL_NSIOBSERVER

  Scone(nsPIDOMWindowInner* aWindow, uint64_t aConnectionId,
        Maybe<uint64_t> aThroughputAdvice);

  JSObject* WrapObject(JSContext* aCx,
                       JS::Handle<JSObject*> aGivenProto) override;

  Nullable<uint64_t> GetThroughputAdvice() const;

  IMPL_EVENT_HANDLER(change)

  void Shutdown();
  void DisconnectFromOwner() override;

 private:
  ~Scone();

  void Update(Maybe<uint64_t> aAdvice, bool aNotify);

  const uint64_t mConnectionId;
  Maybe<uint64_t> mThroughputAdvice;
  bool mRegistered = false;
  bool mShutdown = false;
};

}  // namespace mozilla::dom::network

#endif  // mozilla_dom_network_Scone_h
