// Copyright 2020 Google LLC. All rights reserved.
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://developers.google.com/open-source/licenses/bsd

#include <packager/media/formats/scte35/scte35_muxer.h>

#include <packager/macros/status.h>
#include <iostream>

namespace shaka {
namespace media {
namespace scte35 {

Scte35Muxer::Scte35Muxer(const MuxerOptions& options) : Muxer(options) {}
Scte35Muxer::~Scte35Muxer() {}

Status Scte35Muxer::InitializeStream(SCTE35StreamInfo* stream) {
  LOG(INFO)<<"Scte35Muxer::InitializeStream"<<std::endl;
  return Status::OK;
}

Status Scte35Muxer::AddScte35Event(size_t stream_id, const Scte35Event& sample) {
  LOG(INFO)<<" Scte35Muxer::AddScte35Event"<<std::endl;
  return Status::OK;
}

Status Scte35Muxer::WriteToFile(const std::string& filename, uint64_t* size) {
  LOG(INFO)<<" Scte35Muxer::WriteToFile"<<std::endl;
  return Status::OK;
}

Status Scte35Muxer::InitializeMuxer() {
LOG(INFO)<<" Scte35Muxer::InitializeMuxer"<<std::endl;
  last_cue_ms_ = 0;
  return Status::OK;
}

Status Scte35Muxer::Finalize() {
  //const float duration_ms = static_cast<float>(total_duration_ms_);
  //float duration_seconds = duration_ms / 1000;
  LOG(INFO)<<" Scte35Muxer::Finalize"<<std::endl;

  return Status::OK;
}


Status Scte35Muxer::FinalizeSegment(size_t stream_id,
                                  const SegmentInfo& segment_info) {
  total_duration_ms_ += segment_info.duration;
LOG(INFO)<<" Scte35Muxer::FinalizeSegm"<<std::endl;
  return Status::OK;
}

}  // namespace Scte35
}  // namespace media
}  // namespace shaka
