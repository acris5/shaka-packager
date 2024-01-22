// Copyright 2017 Google Inc. All rights reserved.
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://developers.google.com/open-source/licenses/bsd

#include <packager/media/base/scte35_event.h>

#include <algorithm>
#include <functional>


namespace shaka {
namespace media {


SCTE35Event::SCTE35Event(const std::string& id,
                       int64_t start_time,
                       int64_t duration) 
    : id_(id),
      start_time_(start_time),
      duration_(duration){}

int64_t SCTE35Event::EndTime() const {
  return start_time_ + duration_;
}

}  // namespace media
}  // namespace shaka
