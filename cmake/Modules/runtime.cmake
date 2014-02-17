macro(configure_msvc_runtime)
  if(MSVC)
    # Default to statically-linked runtime.
    set(MSVC_DYNAMIC_RUNTIME OFF CACHE BOOL "Use dynamic runtime")
    # Set compiler options.
    set(variables
      CMAKE_C_FLAGS_DEBUG
      CMAKE_C_FLAGS_MINSIZEREL
      CMAKE_C_FLAGS_RELEASE
      CMAKE_C_FLAGS_RELWITHDEBINFO
      CMAKE_CXX_FLAGS_DEBUG
      CMAKE_CXX_FLAGS_MINSIZEREL
      CMAKE_CXX_FLAGS_RELEASE
      CMAKE_CXX_FLAGS_RELWITHDEBINFO
    )
    if(NOT ${MSVC_DYNAMIC_RUNTIME})
      message(STATUS
        "MSVC -> forcing use of statically-linked runtime."
      )
      remove_definitions(-D_AFXDLL -D_AFXEXT)
      foreach(variable ${variables})
        if(${variable} MATCHES "/MD")
          string(REGEX REPLACE "/MD" "/MT" ${variable} "${${variable}}")
        endif()
      endforeach()
    else()
      message(STATUS
        "MSVC -> forcing use of dynamically-linked runtime."
      )
      add_definitions(-D_AFXDLL -D_AFXEXT)
      foreach(variable ${variables})
        if(${variable} MATCHES "/MT")
          string(REGEX REPLACE "/MT" "/MD" ${variable} "${${variable}}")
        endif()
      endforeach()
    endif()
  endif()
endmacro()
