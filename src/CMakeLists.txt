add_library(SRC STATIC
    OBuffer.cpp
    S2T.cpp
    PtrTable.cpp
)


target_include_directories(SRC PUBLIC ../includes)