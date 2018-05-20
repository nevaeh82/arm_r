QT += core xml
TARGET   = Graphics_ipp
TEMPLATE = lib
ProjectDir=$$PWD
CONFIG += staticlib

include(project.pri)
include(tbb.pri)
include(ipp.pri)

SOURCES +=  \
    $$PWD/modules/core/strings.cpp \
    $$PWD/modules/filesystem/filesystem.cpp \
    $$PWD/modules/gui/multi_graphics/multi_graphics.cpp \
    $$PWD/modules/gui/multi_graphics/mg_settings.cpp \
    $$PWD/modules/gui/multi_graphics/components_relation.cpp \
    $$PWD/modules/gui/multi_graphics_sonogram_interface/sonogram_material.cpp \
    $$PWD/modules/gui/multi_graphics_sonogram_interface/sonogram_interface.cpp \
    $$PWD/modules/gui/multi_graphics_spectrum_interface/spectrum_material.cpp \
    $$PWD/modules/gui/multi_graphics_spectrum_interface/mg_spectrum_interface.cpp \
    $$PWD/modules/log/log.cpp \
    $$PWD/modules/gui/multi_graphics/labels/no_visual_labels_manager.cpp \
    $$PWD/modules/gui/multi_graphics/labels/mg2_drawer.cpp \
    $$PWD/modules/gui/multi_graphics/labels/mg1_label_instrument_item.cpp \
    $$PWD/modules/gui/multi_graphics/labels/mg1_drawer.cpp

HEADERS  += \
    stdafx.h \
    sample_types.h \
    $$PWD/modules/core/templates_inl.h \
    $$PWD/modules/core/strings_inl.h \
    $$PWD/modules/core/strings.h \
    $$PWD/modules/core/storage_inl.h \
    $$PWD/modules/core/preprocessor.h \
    $$PWD/modules/core/memory_inl.h \
    $$PWD/modules/filesystem/filesystem.h \
    $$PWD/modules/filesystem/file_inl.h \
    $$PWD/modules/gui/multi_graphics/multi_graphics.h \
    $$PWD/modules/gui/multi_graphics/mg_settings.h \
    $$PWD/modules/gui/multi_graphics/mg_fastlab_style.h \
    $$PWD/modules/gui/multi_graphics/components_relation.h \
    $$PWD/modules/gui/multi_graphics_sonogram_interface/sonogram_material.h \
    $$PWD/modules/gui/multi_graphics_sonogram_interface/sonogram_interface.h \
    $$PWD/modules/gui/multi_graphics_spectrum_interface/spectrum_material.h \
    $$PWD/modules/gui/multi_graphics_spectrum_interface/mg_spectrum_interface.h \
    $$PWD/modules/log/log.h \
    $$PWD/modules/windows/error_reporting_policy_inl.h \
    $$PWD/modules/windows/dll_loader.h \
    $$PWD/sample_types.h \
    $$PWD/modules/gui/multi_graphics/labels/no_visual_labels_manager.h \
    $$PWD/modules/gui/multi_graphics/labels/mg2_drawer.h \
    $$PWD/modules/gui/multi_graphics/labels/mg1_label_instrument_item.h \
    $$PWD/modules/gui/multi_graphics/labels/mg1_drawer.h

OTHER_FILES += \
    $$PWD/modules/core/storage_inl.h.autoexp.dat

RESOURCES += \
    $$PWD/modules/gui/multi_graphics/grafic.qrc \
    $$PWD/modules/gui/utilities/button_list.qrc \
    $$PWD/modules/gui/multi_graphics/labels/labels_resource.qrc

DEFINES += UNICODE
DEFINES += _UNICODE
