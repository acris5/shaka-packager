// Copyright 2017 Google Inc. All rights reserved.
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://developers.google.com/open-source/licenses/bsd

#include <packager/mpd/base/event_stream.h>

#include <cmath>
#include <string>

#include <absl/log/check.h>
#include <packager/mpd/base/mpd_options.h>
#include <packager/mpd/base/mpd_utils.h>

namespace shaka {


EventStream::EventStream(  const MpdOptions& mpd_options,
                             uint32_t* counter)
    : events_counter_(counter),
      mpd_options_(mpd_options) {
  DCHECK(counter);
}

EventStream::~EventStream() {}


// Creates a copy of <EventStream> xml element, iterate thru all the
// <Scte35Event> (child) elements and add them to the copy.
// Set all the attributes first and then add the children elements 
std::optional<xml::XmlNode> EventStream::GetXml() {
  xml::XmlNode events_stream("EventStream");
  if (!events_stream.SetStringAttribute("schemeIdUri","urn:scte:scte35:2014:xml+bin") ||
      !events_stream.SetStringAttribute("timescale","90000")) { //TODO: get timeScale from mpdOptions
      return std::nullopt;
    }   
  for (const auto& Scte35Event_pair : events_map_) {
    const auto& Scte35Event = Scte35Event_pair.second;
    // <Event id="15549" presentationTime="1693988723911" duration="170000">
          //<Signal xmlns="http://www.scte.org/schemas/35/2016">
         //   <Binary>/DAlAAAAAAAAAAAAFAUAADy9f+//H6aDRgAA6XWgAAMR/wAAiIAGQQ==</Binary>
        //  </Signal>
     //   </Event>
    xml::XmlNode event("Event");
    xml::XmlNode signal("Signal");
    xml::XmlNode binary("Binary"); 
    //TODO: hex needed in Uppercase form
    binary.SetContent(Scte35Event.cue_data);//TODO: string should be text-encoded
    if (!event.SetStringAttribute("id",std::to_string(Scte35Event_pair.first)) ||
        !event.SetStringAttribute("presentationTime",std::to_string(Scte35Event.timestamp)) ||
        !event.SetStringAttribute("duration",std::to_string(Scte35Event.duration)) ||
        !signal.SetStringAttribute("xmlns","http://www.scte.org/schemas/35/2016") ||
        !signal.AddChild(std::move(binary)) ||
        !event.AddChild(std::move(signal)) ||
        !events_stream.AddChild(std::move(event))){
          return std::nullopt;
        }    
  }
  return events_stream;
}

void EventStream::AddNewSCTE35Event(int64_t start_time,
                                                  int64_t duration, const std::string& cue_data) {
  if (duration >= 0){
    //add scte35event to map
    const uint32_t event_id = (*events_counter_)++;
    events_map_[event_id] = {start_time, duration, cue_data};
  }
  //TODO: delete outdated scte35 events
}

/*const std::list<Scte35Event*> EventStream::GetSCTE35Events() const {
  std::list<Scte35Event*> Scte35Events;
  for (const auto& Scte35Event_pair : Scte35Event_map_) {
    Scte35Events.push_back(Scte35Event_pair.second.get());
  }
  return Scte35Events;
}*/





}  // namespace shaka
