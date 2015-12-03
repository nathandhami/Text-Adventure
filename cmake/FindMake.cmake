find_program(MAKE_EXECUTABLE NAMES make
    PATH_SUFFIXES bin
    DOC "make build and dependency management tool"
)
 
include(FindPackageHandleStandardArgs)
 
find_package_handle_standard_args(Make DEFAULT_MSG
    MAKE_EXECUTABLE
)
 
mark_as_advanced(MAKE_EXECUTABLE)

