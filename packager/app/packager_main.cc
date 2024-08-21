// Copyright 2014 Google LLC. All rights reserved.
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://developers.google.com/open-source/licenses/bsd




#include <packager/app/packager_main_def.h>

#if defined(OS_WIN)
#include <codecvt>
#include <functional>
#endif  // defined(OS_WIN)


#if defined(OS_WIN)
// Windows wmain, which converts wide character arguments to UTF-8.
int wmain(int argc, wchar_t* argv[], wchar_t* envp[]) {
  std::unique_ptr<char*[], std::function<void(char**)>> utf8_argv(
      new char*[argc], [argc](char** utf8_args) {
        // TODO(tinskip): This leaks, but if this code is enabled, it crashes.
        // Figure out why. I suspect gflags does something funny with the
        // argument array.
        // for (int idx = 0; idx < argc; ++idx)
        //   delete[] utf8_args[idx];
        delete[] utf8_args;
      });
  std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;

  for (int idx = 0; idx < argc; ++idx) {
    std::string utf8_arg(converter.to_bytes(argv[idx]));
    utf8_arg += '\0';
    utf8_argv[idx] = new char[utf8_arg.size()];
    memcpy(utf8_argv[idx], &utf8_arg[0], utf8_arg.size());
  }

  // Because we just converted wide character args into UTF8, and because
  // std::filesystem::u8path is used to interpret all std::string paths as
  // UTF8, we should set the locale to UTF8 as well, for the transition point
  // to C library functions like fopen to work correctly with non-ASCII paths.
  std::setlocale(LC_ALL, ".UTF8");

  return shaka::main_func::PackagerMain(argc, utf8_argv.get());
}
#else
int main(int argc, char** argv) {
  return shaka::main_func::PackagerMain(argc, argv);
}
#endif  // defined(OS_WIN)
