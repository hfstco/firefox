/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

// Experimental, non-standard API. SCONE is defined by
// https://datatracker.ietf.org/doc/draft-ietf-scone-protocol/.
[Exposed=Window, SecureContext, Pref="network.http.http3.scone.enabled"]
interface Scone : EventTarget {
  readonly attribute unsigned long long? throughputAdvice;
  attribute EventHandler onchange;
};
