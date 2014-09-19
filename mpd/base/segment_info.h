// Copyright 2014 Google Inc. All rights reserved.
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://developers.google.com/open-source/licenses/bsd

#ifndef MPD_BASE_SEGMENT_INFO_H_
#define MPD_BASE_SEGMENT_INFO_H_

namespace edash_packager {
/// Container for keeping track of information about a segment.
/// Used for keeping track of all the segments used for generating MPD with
/// dynamic  profile.
struct SegmentInfo {
  uint64 start_time;
  uint64 duration;
  // This is the number of times same duration segments are repeated not
  // inclusive. In other words if this is the only one segment that starts at
  // |start_time| and has |duration| but none others have |start_time| * N and
  // |duration|, then this should be set to 0. The semantics is the same as S@r
  // in the DASH MPD spec.
  uint64 repeat;
};
}  // namespace edash_packager

#endif  // MPD_BASE_SEGMENT_INFO_H_