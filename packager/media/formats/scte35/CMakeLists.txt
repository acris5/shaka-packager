# Copyright 2020 Google LLC. All rights reserved.
#
# Use of this source code is governed by a BSD-style
# license that can be found in the LICENSE file or at
# https://developers.google.com/open-source/licenses/bsd


# Copyright 2016 Google LLC. All rights reserved.
#
# Use of this source code is governed by a BSD-style
# license that can be found in the LICENSE file or at
# https://developers.google.com/open-source/licenses/bsd


add_library(scte35 STATIC
 SpliceClass.cpp
 SpliceClass.hpp
 scte35_muxer.cc
 scte35_muxer.h
  )
  
target_link_libraries(scte35
  media_base
  mpd_builder
  )

