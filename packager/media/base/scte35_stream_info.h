// Copyright 2015 Google Inc. All rights reserved.
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://developers.google.com/open-source/licenses/bsd

#ifndef PACKAGER_MEDIA_BASE_SCTE35_STREAM_INFO_H_
#define PACKAGER_MEDIA_BASE_SCTE35_STREAM_INFO_H_

#include <packager/media/base/stream_info.h>
#include <packager/media/base/scte35_event.h>

#include <map>
#include <string>

namespace shaka {
namespace media {


class SCTE35StreamInfo : public StreamInfo {
 public:
  /// No encryption supported.
  /// @param track_id is the track ID of this stream.
  /// @param time_scale is the time scale of this stream.
  /// @param duration is the duration of this stream.
  /// @param codec is the media codec.
  /// @param codec_string is the codec in string format.
  /// @param codec_config is configuration for this text stream. This could be
  ///        the metadata that applies to all the samples of this stream. This
  ///        may be empty.
  /// @param language is the language of this stream. This may be empty.
  SCTE35StreamInfo(int track_id,
                 int32_t time_scale,
                 int64_t duration,
                 Codec codec,
                 const std::string& codec_string,
                 const std::string& codec_config,
                 const std::string& language);

  ~SCTE35StreamInfo() override;

  bool IsValidConfig() const override;

  std::string ToString() const override;
  std::unique_ptr<StreamInfo> Clone() const override;
  

 private:
  

  // Allow copying. This is very light weight.
};

}  // namespace media
}  // namespace shaka

#endif  // PACKAGER_MEDIA_BASE_SCTE35_STREAM_INFO_H_
