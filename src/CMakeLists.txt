set (SOURCES main.c ffdddapplication.c ffdddwindow.c networking.c
	ffdddgiveawaydialog.c)
add_executable (ffddd ${SOURCES} resources.c)

find_package (PkgConfig REQUIRED)

execute_process(
    COMMAND ${PKG_CONFIG_EXECUTABLE} --variable=glib_compile_resources gio-2.0 OUTPUT_VARIABLE GLIB_COMPILE_RESOURCES
    OUTPUT_STRIP_TRAILING_WHITESPACE)

add_custom_command(OUTPUT resources.c
    COMMAND ${GLIB_COMPILE_RESOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/res/ui/ffddd.gresource.xml --target=${CMAKE_CURRENT_BINARY_DIR}/resources.c --sourcedir=${CMAKE_CURRENT_SOURCE_DIR}/res --generate-source)

pkg_check_modules (GTK3 REQUIRED gtk+-3.0)
include_directories (${GTK3_INCLUDE_DIRS})
link_directories (${GTK3_LIBRARY_DIRS})
add_definitions (${GTK3_CFLAGS_OTHER})
target_link_libraries (ffddd ${GTK3_LIBRARIES})
