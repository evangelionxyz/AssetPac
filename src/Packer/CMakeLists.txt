add_executable(AssetPac
    asset_packer.cpp
    asset.cpp
    main.cpp
)

target_include_directories(AssetPac PRIVATE 
    ${THIRD_PARTY}/stb_image
    ${INCLUDE_DIR}
)

message(${CMAKE_SOURCE_DIR})

add_custom_command(TARGET AssetPac POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/resources/"
        "$<TARGET_FILE_DIR:AssetPac>/resources"
    COMMENT "Copying Resources"
)
