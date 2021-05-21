################################################################################
#
# medInria

# Copyright (c) INRIA 2013. All rights reserved.

# See LICENSE.txt for details in the root of the sources or:
# https://github.com/medInria/medInria-public/blob/master/LICENSE.txt

# This software is distributed WITHOUT ANY WARRANTY; without even
# the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
# PURPOSE.
#
################################################################################

set(TARGET_NAME medCore)

#required for querying the GPU, can be removed once using Qt 5.3
#using QOpenGLFunctions::glGetString(), as well as the {OPENGL_LIBRARIES} below.
find_package(OpenGL REQUIRED)

## #############################################################################
## List Sources
## #############################################################################

list_source_files(${TARGET_NAME}
  ${CMAKE_CURRENT_SOURCE_DIR}
  legacy/data
  legacy/database
  legacy/data/annotationData
  legacy/gui
  legacy/gui/commonWidgets
  legacy/gui/database
  legacy/gui/factories
  legacy/gui/lookUpTables
  legacy/gui/settingsWidgets
  legacy/gui/toolboxes
  legacy/gui/viewContainers
  legacy/process
  legacy/views
  legacy/views/interactors
  legacy/views/navigators
  legacy/parameters
  legacy/resources
  data
  process
  process/arithmetic_operation
  process/diffusion_processes
  process/morphomath_operation
  process/mask_image
  process/single_filter
  parameter
  parameter/tools
  view
  view/tools
  job
  )

list_header_directories_to_include(${TARGET_NAME}
  ${${TARGET_NAME}_HEADERS}
  )

## #############################################################################
## add library
## #############################################################################

add_library(${TARGET_NAME} SHARED
  ${${TARGET_NAME}_CFILES}
  ${${TARGET_NAME}_QRC}
  )

## #############################################################################
## include directorie.
## #############################################################################

target_include_directories(${TARGET_NAME}
  PUBLIC ${${TARGET_NAME}_INCLUDE_DIRS}
  )

## #############################################################################
## Link
## #############################################################################

target_link_libraries(${TARGET_NAME}
  #medCoreLegacy
  Qt5::Core
  Qt5::Widgets
  Qt5::OpenGL
  Qt5::Gui
  Qt5::Sql
  Qt5::Test
  Qt5::Xml
  ${OPENGL_LIBRARIES}
  dtkCoreSupport
  dtkGuiSupport
  dtkLog
  dtkComposer
  )

if (WIN32) #TODO is that for the memory management thing ?
  target_link_libraries(${TARGET_NAME} psapi.lib)
endif()

## #############################################################################
## install
## #############################################################################

set_lib_install_rules(${TARGET_NAME}
  ${${TARGET_NAME}_HEADERS}
  )
  