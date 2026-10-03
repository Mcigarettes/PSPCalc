cmake_minimum_required(VERSION 3.11)

project(PSPCalc)

add_executable(${PROJECT_NAME}
    src/main.c
    src/input.c
    src/ui_text.c
    src/ui_element.c
    src/ui_button.c
    src/ui_label.c
    src/ui_container.c
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    pspgu
    pspge
    pspdisplay
    pspctrl
    pspdebug
)

create_pbp_file(
    TARGET ${PROJECT_NAME}
    ICON_PATH NULL
    BACKGROUND_PATH NULL
    PREVIEW_PATH NULL
    TITLE "PSP Calculator"
    VERSION 01.00
)