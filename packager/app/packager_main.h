// Copyright 2014 Google Inc. All rights reserved.
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://developers.google.com/open-source/licenses/bsd
#ifndef APP_MAIN_H_
#define APP_MAIN_H_

//#include <gflags/gflags.h>
#include <packager/packager.h>
#include <optional>

/*DECLARE_bool(dump_stream_info);
DECLARE_bool(licenses);
DECLARE_bool(quiet);
DECLARE_bool(use_fake_clock_for_muxer);
DECLARE_string(test_packager_version);
DECLARE_bool(single_threaded);*/

namespace shaka {
namespace main_func {

enum ExitStatus {
  kSuccess = 0,
  kArgumentValidationFailed,
  kPackagingFailed,
  kInternalError,
};

std::optional<PackagingParams> PackagerParamsFromJSON(
    const char* json_string_params);
std::optional<std::vector<shaka::StreamDescriptor>> PackagerStreamsFromJSON(
    const char* json_string_params);

int PackagerMain(int argc, char** argv);
}  // namespace main_func
}  // namespace shaka

#endif
