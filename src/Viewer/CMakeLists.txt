add_executable(PacViewer
    main.cpp
)

target_include_directories(PacViewer PRIVATE 
    ${THIRD_PARTY}/stb_image
    ${INCLUDE_DIR}
)
