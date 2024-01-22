// Copyright 2020 Google Inc. All rights reserved.
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://developers.google.com/open-source/licenses/bsd

#include <packager/media/formats/mp2t/es_parser_scte35.h>

#include <packager/media/base/bit_reader.h>
#include <packager/media/base/scte35_stream_info.h>
#include <packager/media/base/timestamp.h>
#include <packager/media/formats/mp2t/mp2t_common.h>
#include <packager/media/formats/scte35/SpliceClass.hpp>
#include <sstream>
#include <iostream>
#include <iomanip>

namespace shaka {
namespace media {
namespace mp2t {
    namespace {


template <typename T>
constexpr T bit(T value, const size_t bit_pos) {
  return (value >> bit_pos) & 0x1;
}
    }



std::string uint8_to_hex_string(const uint8_t *v, const size_t s) {
  std::stringstream ss;

  ss << std::hex << std::setfill('0');

  for (uint64_t i = 0; i < s; i++) {
    ss << std::hex << std::setw(2) << static_cast<int>(v[i]);
  }

  return ss.str();
}

bool ParseSCTE35Descriptor(const uint8_t* descriptor,
                           const size_t size,
                           std::unordered_map<uint16_t, std::string>& result) {
  BitReader reader(descriptor, size);
  RCHECK(reader.SkipBits(8));

  size_t data_size;
  RCHECK(reader.ReadBits(8, &data_size));
  RCHECK(data_size + 2 <= size);
  //result.emplace(0, std::move("ru"));
  return true;
}

EsParserSCTE35::EsParserSCTE35(const uint32_t pid,
                               const NewStreamInfoCB& new_stream_info_cb,
                               const EmitTextSampleCB& emit_sample_cb,
                               const EmitSCTE35EventCB& emit_scte35_event_cb,
                               const uint8_t* descriptor,
                               const size_t descriptor_length)
    : EsParser(pid),
      new_stream_info_cb_(new_stream_info_cb),
      emit_sample_cb_(emit_sample_cb),
      emit_scte35_event_cb_(emit_scte35_event_cb),
      sent_info_(true),//Will send info in mp2t_media_parser immediately after scte35_parser creation 
      sent_test_(0),
      last_pts_(0) {};

bool EsParserSCTE35::Parse(const uint8_t* buf,
                           int size,
                           int64_t pts,
                           int64_t dts) {
  if (!sent_info_) {
    sent_info_ = true;
    auto info = std::make_shared<SCTE35StreamInfo>(
        pid(), kMpeg2Timescale, kInfiniteDuration, kCodecSCTE35, "", "", "");
   
    new_stream_info_cb_(info);
  }

  return ParseInternal(buf, size, pts);
}

bool EsParserSCTE35::Flush() {
    return true;
}

void EsParserSCTE35::Reset() {
  sent_info_ = false;
}


bool EsParserSCTE35::ParseInternal(const uint8_t* data,
                                   const size_t size,
                                   const int64_t pts) {
  BitReader reader(data, size);
  RCHECK(reader.SkipBits(8));
  std::vector<std::string> lines;
    //LOG(INFO) << "SCTE35 parse run internal: "<<size;
    
   scte35::CSpliceClass objLocal;
    //may be check also unsigned char*
    std::string s(data, data + size);
    std::string h(uint8_to_hex_string(data, size));
    //std::string sBinData = const_cast<char*>(reinterpret_cast<const char *>(ts_packet.payload()));
    //LOG(INFO) <<  "SCTE35 ParseBinary:" << h <<std::endl;
    std::shared_ptr<SCTE35Event> event;
    try {
    if (objLocal.ParseHexa(h)) {
      if (objLocal.m_SInfoData.splice_command_type !=
          scte35::scte35Cmd::splice_null) {
        // LOG(INFO)<<  "SCTE35 text: "<< objLocal.GetText(false)<<std::endl;
        if (objLocal.m_SInfoData.splice_command_type == scte35::scte35Cmd::splice_insert) {
          if (objLocal.m_objSpliceInsert.splice_event_cancel_indicator == 0) {
            if (objLocal.m_objSpliceInsert.out_of_network_indicator) {
              // Emit Cue-Out
              if (objLocal.m_objSpliceInsert.splice_immediate_flag) {
                if (objLocal.m_objSpliceInsert.duration_flag) {
                  event = std::make_shared<SCTE35Event>(
                      h, pts,
                      objLocal.m_objSpliceInsert.m_breakD.duration);
                  emit_scte35_event_cb_(event);
                  LOG(INFO) << "emit event SCTE35 Out" << std::endl;
                }
              } else {
                if (objLocal.m_objSpliceInsert.duration_flag)
                  if (objLocal.m_objSpliceInsert.program_splice_flag == 1 &&
                      objLocal.m_objSpliceInsert.m_spliceT
                              .time_specified_flag == 1) {
                    event = std::make_shared<SCTE35Event>(
                        h,
                        objLocal.m_objSpliceInsert.m_spliceT.pts_time,
                        objLocal.m_objSpliceInsert.m_breakD.duration);
                    emit_scte35_event_cb_(event);
                    LOG(INFO) << "emit event SCTE35 Out" << std::endl;
                  }
              }
            } else {
              // Emit Cue-in
              if (objLocal.m_objSpliceInsert.splice_immediate_flag) {
                // Negative duration means Cue-in
                event = std::make_shared<SCTE35Event>(
                    "scte35spliceinsertIn",
                    objLocal.m_objSpliceInsert.splice_immediate_flag
                        ? pts
                        : objLocal.m_objSpliceInsert.m_spliceT.pts_time,
                    -1);
                emit_scte35_event_cb_(event);
                LOG(INFO) << "emit event SCTE35 In" << std::endl;
              }
            }
          }
        } else {
          LOG(INFO)<< pts << "SCTE35 text: "<< objLocal.GetText(false)<<std::endl;
        }
      } else {
        sent_test_++;
         if (sent_test_ > 10) {
          sent_test_ = 0;
        LOG(INFO) << pts << " SCTE35..." << std::endl;
         }
      }
    
     /* //sample scte35 test inserts to manifest
     sent_test_++;
      if (sent_test_ > 50) {
        sent_test_ = 0;
        if (!test_sent_out_) {
          event = std::make_shared<SCTE35Event>(h, pts,
                                                90000 * 100);
          test_sent_out_ = true;
        } else {
          event =
              std::make_shared<SCTE35Event>(h, pts, -1);
          test_sent_out_ = false;
        }
        emit_scte35_event_cb_(event);
        std::cout << "emit event SCTE35 test" << std::endl;
      }*/ 
    }
    } catch (...) {
      LOG(ERROR)<<"Error during SCTE35 parse"<<std::endl;
    }
      /*
      std::stringstream CSpliceClass::GetText_Splice_Insert(const bool& bIfSingleLine) {
	std::stringstream slocalStr_1;
	std::string sSepLocal = sNextLine;
	if (bIfSingleLine) {
		sSepLocal = sCommaOnly;
	}

	slocalStr_1 << "splice_insert()" << sSepLocal;

	slocalStr_1 << "splice_event_id=" << m_objSpliceInsert.splice_event_id << sSepLocal;
	slocalStr_1 << "splice_event_cancel_indicator=" << m_objSpliceInsert.splice_event_cancel_indicator << sSepLocal;
	slocalStr_1 << "reserved_1=" << m_objSpliceInsert.reserved_1 << sSepLocal;

	if (m_objSpliceInsert.splice_event_cancel_indicator == 0) {
		slocalStr_1 << "out_of_network_indicator=" << m_objSpliceInsert.out_of_network_indicator << sSepLocal;
		slocalStr_1 << "program_splice_flag=" << m_objSpliceInsert.program_splice_flag << sSepLocal;
		slocalStr_1 << "duration_flag=" << m_objSpliceInsert.duration_flag << sSepLocal;
		slocalStr_1 << "splice_immediate_flag=" << m_objSpliceInsert.splice_immediate_flag << sSepLocal;
		slocalStr_1 << "reserved_2=" << m_objSpliceInsert.reserved_2 << sSepLocal;
		
		if (m_objSpliceInsert.program_splice_flag == 1 && m_objSpliceInsert.splice_immediate_flag == 0) {
			slocalStr_1 << "time_specified_flag=" << m_objSpliceInsert.m_spliceT.time_specified_flag << sSepLocal;

			if(m_objSpliceInsert.m_spliceT.time_specified_flag == 1) {
				slocalStr_1 << "reserved_1=" << m_objSpliceInsert.m_spliceT.reserved_1 << sSepLocal;
				slocalStr_1 << "pts_time=" << m_objSpliceInsert.m_spliceT.pts_time << sSepLocal;
			}
			else {
				slocalStr_1 << "reserved_2=" << m_objSpliceInsert.m_spliceT.reserved_2 << sSepLocal;
			}
		}

		if (m_objSpliceInsert.program_splice_flag == 0) {
			slocalStr_1 << "component_count=" << m_objSpliceInsert.component_count << sSepLocal;
			if (m_objSpliceInsert.component_count > 0) {
				for (uint32_t i = 0;i < m_objSpliceInsert.component_count;i++) {
					slocalStr_1 << "sin_comp["<<i+1<<"]"<< sSepLocal;
					if (m_objSpliceInsert.splice_immediate_flag == 0) {
						slocalStr_1 << "time_specified_flag=" << m_objSpliceInsert.v_ChildComps[i].m_spliceT.time_specified_flag << sSepLocal;
						if (m_objSpliceInsert.v_ChildComps[i].m_spliceT.time_specified_flag==1) {
							slocalStr_1 << "reserved_1=" << m_objSpliceInsert.v_ChildComps[i].m_spliceT.reserved_1 << sSepLocal;
							slocalStr_1 << "pts_time=" << m_objSpliceInsert.v_ChildComps[i].m_spliceT.pts_time << sSepLocal;
						} else {
							slocalStr_1 << "reserved_2=" << m_objSpliceInsert.v_ChildComps[i].m_spliceT.reserved_2 << sSepLocal;
						}
					}
				}
			}
		}
		if (m_objSpliceInsert.duration_flag == 1) {
			slocalStr_1 << "auto_return=" << m_objSpliceInsert.m_breakD.auto_return << sSepLocal;
			slocalStr_1 << "reserved=" << m_objSpliceInsert.m_breakD.reserved << sSepLocal;
			slocalStr_1 << "duration=" << m_objSpliceInsert.m_breakD.duration << sSepLocal;
		}

		slocalStr_1 << "unique_program_id=" << m_objSpliceInsert.unique_program_id << sSepLocal;
		slocalStr_1 << "avail_num=" << m_objSpliceInsert.avail_num << sSepLocal;
		slocalStr_1 << "avails_expected=" << m_objSpliceInsert.avails_expected << sSepLocal;

	}
	return slocalStr_1;
}
      */

  /*while (reader.bits_available()) {
    uint8_t data_unit_id;
    RCHECK(reader.ReadBits(8, &data_unit_id));

    uint8_t data_unit_length;
    RCHECK(reader.ReadBits(8, &data_unit_length));

    if (data_unit_length != 44) {
      LOG(ERROR) << "Bad SCTE35 data length";
      break;
    }

    

    RCHECK(reader.SkipBits(16));

    uint16_t address_bits;
    RCHECK(reader.ReadBits(16, &address_bits));

    const uint8_t packet_nr =
        (bit(address_bits, 8) + 2 * bit(address_bits, 6) +
         4 * bit(address_bits, 4) + 8 * bit(address_bits, 2) +
         16 * bit(address_bits, 0));
    const uint8_t* data_block = reader.current_byte_ptr();
    RCHECK(reader.SkipBytes(40));

    std::string display_text;
    if (ParseDataBlock(pts, data_block, packet_nr)) {
      lines.emplace_back(std::move(display_text));
    }
  }

  if (lines.empty()) {
    return true;
  }
*/

  return true;
}

bool EsParserSCTE35::ParseDataBlock(const int64_t pts,
                                    const uint8_t* data_block,
                                    const uint8_t packet_nr) {
  if (packet_nr == 0) {
    last_pts_ = pts;
    BitReader reader(data_block, 32);

    return true;
  }
  return true;
                                    }


}  // namespace mp2t
}  // namespace media
}  // namespace shaka
