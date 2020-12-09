cmake_minimum_required(VERSION 3.12)
project(erses VERSION 1.1.0)

find_package(daq-cmake REQUIRED)

daq_setup_environment()

find_package(ers REQUIRED)     
find_package(nlohmann_json REQUIRED)
#find_package(Pistache REQUIRED)
find_package(cpr)

set(ERSES_DEPENDENCIES ers::ers nlohmann_json::nlohmann_json cpr)

target_link_libraries(erses_src ${CPR_LIBRARIES})
target_include_directories(erses_src ${CPR_INCLUDE_DIRS})

#set(ERSES_DEPENDENCIES ers::ers nlohmann_json::nlohmann_json)


##############################################################################

# Note that when the ers library is linked in, it uses a "::" because
# it's in an external package, not a local CMake target. The semantics
# are <namespace>::<shared library / executable>
daq_add_library(ESStream.cpp LINK_LIBRARIES ${ERSES_DEPENDENCIES})

##############################################################################
daq_add_application( erses_test_program test.cxx TEST LINK_LIBRARIES erses )

##############################################################################
##daq_add_unit_test(ValueWrapper_test)

##############################################################################

# Could add "erses_test_program" to the argument list here, but
# presumably end users wouldn't care about an integration test program
# and just want the shared object library installed

daq_install()



