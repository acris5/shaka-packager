// Copyright 2015 Google Inc. All rights reserved.
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://developers.google.com/open-source/licenses/bsd

#include <packager/media/base/scte35_stream_info.h>

namespace shaka {
namespace media {

SCTE35StreamInfo::SCTE35StreamInfo(int track_id,
                               int32_t time_scale,
                               int64_t duration,
                               Codec codec,
                               const std::string& codec_string,
                               const std::string& codec_config,
                               const std::string& language)
    : StreamInfo(kStreamSCTE35,
                 track_id,
                 time_scale,
                 duration,
                 codec,
                 codec_string,
                 reinterpret_cast<const uint8_t*>(codec_config.data()),
                 codec_config.size(),
                 language,
                 false) {}

SCTE35StreamInfo::~SCTE35StreamInfo() {}

bool SCTE35StreamInfo::IsValidConfig() const {
  return true;
}

std::string SCTE35StreamInfo::ToString() const {
  std::string ret = StreamInfo::ToString();
  return ret + "\n";
}

std::unique_ptr<StreamInfo> SCTE35StreamInfo::Clone() const {
  return std::unique_ptr<StreamInfo>(new SCTE35StreamInfo(*this));
}

}  // namespace media
}  // namespace shaka
