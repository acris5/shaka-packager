// Copyright 2020 Google LLC. All rights reserved.
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://developers.google.com/open-source/licenses/bsd

#ifndef PACKAGER_MEDIA_FORMATS_SCTE35_MUXER_H_
#define PACKAGER_MEDIA_FORMATS_SCTE35_MUXER_H_

#include <packager/media/base/muxer.h>
#include <packager/media/base/media_handler.h>
#include <packager/media/base/scte35_stream_info.h>

namespace shaka {
namespace media {
namespace scte35 {

class Scte35Muxer : public Muxer {
 public:
  explicit Scte35Muxer(const MuxerOptions& options);
  ~Scte35Muxer() override;

 private:
  // Muxer implementation overrides.
  Status InitializeMuxer() override;
  Status Finalize() override;
  Status AddScte35Event(size_t stream_id, const Scte35Event& sample) override;
  Status FinalizeSegment(size_t stream_id,
                         const SegmentInfo& segment_info) override;

  Status InitializeStream(SCTE35StreamInfo* stream);
  Status AddScte35EventInternal(const Scte35Event& sample);
  /// Writes the buffered samples to the file with the given name.  This should
  /// also clear any buffered samples.
  Status WriteToFile(const std::string& filename, uint64_t* size) ;

  int64_t total_duration_ms_ = 0;
  int64_t last_cue_ms_ = 0;
  uint32_t segment_index_ = 0;
};


}  // namespace scte35
}  // namespace media
}  // namespace shaka

#endif  // PACKAGER_MEDIA_FORMATS_SCTE35_MUXER_H_
