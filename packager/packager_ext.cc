#include <iostream>

#include <optional>
#include "packager_ext.h"
#include <packager/app/packager_main_def.h>
#include <packager/packager.h>

void* Packager_New() {
  std::cout << "[c++ bridge] New Packager" << std::endl;
  auto foo = new shaka::Packager();
  return foo;
}

// Utility function local to the bridge's implementation
shaka::Packager* AsPackager(void* foo) { return reinterpret_cast<shaka::Packager*>(foo); }

void Packager_Destroy(void* foo) {
  std::cout << "[c++ bridge] Destroy Packager" << std::endl;
  AsPackager(foo)->shaka::Packager::~Packager();
}

int Packager_Initialize(void* foo, const char* json_args) {
  std::cout << "[c++ bridge] Initialize Packager" << std::endl;
  std::optional<shaka::PackagingParams> packaging_params = shaka::main_func::PackagerParamsFromJSON(json_args);
  if (!packaging_params)
    return shaka::main_func::kArgumentValidationFailed;
  std::optional<std::vector<shaka::StreamDescriptor>> streams = shaka::main_func::PackagerStreamsFromJSON(json_args);
  if (!streams || streams.value().size() == 0)
    return shaka::main_func::kArgumentValidationFailed;
  shaka::Status status = AsPackager(foo)->shaka::Packager::Initialize(packaging_params.value(), streams.value());
  return status.error_code();
}

int Packager_Run(void* foo) {
  std::cout << "[c++ bridge] Run Packager" << std::endl;
  shaka::Status status = AsPackager(foo)->shaka::Packager::Run();
  return status.error_code();
}

void Packager_Stop(void* foo) {
  std::cout << "[c++ bridge] Stop Packager" << std::endl;
  return AsPackager(foo)->shaka::Packager::Cancel();
}