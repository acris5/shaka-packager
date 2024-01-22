// Copyright 2020 Google Inc. All rights reserved.
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://developers.google.com/open-source/licenses/bsd

#ifndef PACKAGER_MEDIA_FORMATS_MP2T_ES_PARSER_SCTE35_H_
#define PACKAGER_MEDIA_FORMATS_MP2T_ES_PARSER_SCTE35_H_

#include <unistd.h>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

#include <packager/media/formats/mp2t/es_parser.h>

namespace shaka {
namespace media {
namespace mp2t {

class EsParserSCTE35 : public EsParser {
 public:
  EsParserSCTE35(const uint32_t pid,
                   const NewStreamInfoCB& new_stream_info_cb,
                   const EmitTextSampleCB& emit_sample_cb,
                   const  EmitSCTE35EventCB& emit_scte35_event_cb,
                   const uint8_t* descriptor,
                   const size_t descriptor_length);

  EsParserSCTE35(const EsParserSCTE35&) = delete;
  EsParserSCTE35& operator=(const EsParserSCTE35&) = delete;

  bool Parse(const uint8_t* buf, int size, int64_t pts, int64_t dts) override;
  bool Flush() override;
  void Reset() override;

 private:
 

  bool ParseInternal(const uint8_t* data, const size_t size, const int64_t pts);
  bool ParseDataBlock(const int64_t pts,
                      const uint8_t* data_block,
                      const uint8_t packet_nr);
 
  
  NewStreamInfoCB new_stream_info_cb_;
  EmitTextSampleCB emit_sample_cb_;
  EmitSCTE35EventCB emit_scte35_event_cb_;
  bool sent_info_;
  int sent_test_;
 bool test_sent_out_ = false;
  int64_t last_pts_;
};

}  // namespace mp2t
}  // namespace media
}  // namespace shaka

#endif  // PACKAGER_MEDIA_FORMATS_MP2T_ES_PARSER_SCTE35_H_
