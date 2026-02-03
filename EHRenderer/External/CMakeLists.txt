add_library (External STATIC 
	"json.hpp" "tmp.cpp")

target_compile_features(EasyH PUBLIC cxx_std_17)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET EasyH PROPERTY CXX_STANDARD 20)
endif()