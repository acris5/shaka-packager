// Copyright 2017 Google Inc. All rights reserved.
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://developers.google.com/open-source/licenses/bsd
//
/// All the methods that are virtual are virtual for mocking.

#ifndef PACKAGER_MPD_BASE_EVENT_STREAM_H_
#define PACKAGER_MPD_BASE_EVENT_STREAM_H_

#include <stdint.h>

#include <list>
#include <map>
#include <memory>
#include <set>
#include <vector>

#include <packager/mpd/base/xml/xml_node.h>

namespace shaka {

class MediaInfo;
class Representation;

struct ContentProtectionElement;
struct MpdOptions;

struct Scte35Event {
    int64_t timestamp;
    int64_t duration;
    std::string cue_data;
};
/// EventStream class provides methods to add Events 
/// to the EventStream element.
class EventStream {
 public:
  
  virtual ~EventStream();

 /// Add an scte35 event to the eventStream.
  /// @param start_time is the start time for the scte35 event, in units of the
  ///        stream's time scale.
  /// @param duration is the duration of the segment, in units of the stream's
  ///        time scale. In the low latency case, this duration is that of the
  ///        first chunk because the full duration is not yet known.
  virtual void AddNewSCTE35Event(int64_t start_time,
                             int64_t duration,
                             const std::string& cue_data);


  /// Makes a copy of EventStream xml element with its children SCTE35Events
  /// @return On success returns a non-NULL scoped_xml_ptr. Otherwise returns a
  ///         NULL scoped_xml_ptr.
  std::optional<xml::XmlNode> GetXml();



  // Return the list of Representations in this EventStream.
  //const std::list<Scte35Event*> GetScte35Events() const;

 
 protected:
  /// @param mpd_options is the options for this MPD.
  /// @param mpd_type is the type of this MPD.
  /// @param events_counter is a Counter for assigning ID numbers to
  ///        Events. It can not be NULL.
  EventStream(const MpdOptions& mpd_options,
                uint32_t* events_counter);

 private:
  EventStream(const EventStream&) = delete;
  EventStream& operator=(const EventStream&) = delete;

  friend class Period;
  //friend class EventStreamTest;



  // This maps Representations (IDs) to a list of start times of the segments.
  // e.g.
  // If Representation 1 has start time 0, 100, 200 and Representation 2 has
  // start times 0, 200, 400, then the map contains:
  // 1 -> [0, 100, 200]
  // 2 -> [0, 200, 400]
  typedef std::map<uint32_t, std::list<int64_t>> EventsTimeline;


  // representation_id => Representation map. It also keeps the representations_
  // sorted by default.
  std::map<uint32_t, Scte35Event> events_map_;

  uint32_t* const events_counter_;

  const MpdOptions& mpd_options_;



  // Keeps track of event start times of Events.
  // For static MPD, this will not be cleared, all the event start times are
  // stored in this. This should not out-of-memory for a reasonable length
  // video and reasonable subsegment length.
  // For dynamic MPD, the entries are deleted (see
  // CheckDynamicSegmentAlignment() implementation comment) because storing the
  // entire timeline is not reasonable and may cause an out-of-memory problem.
  EventsTimeline events_start_times_;

};

}  // namespace shaka

#endif  // PACKAGER_MPD_BASE_EVENT_STREAM_H_
