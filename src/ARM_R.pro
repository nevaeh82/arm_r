
TEMPLATE = subdirs
BUILD_ALL = $$(BUILD_ALL)

include( ARM_R_UI.pri )
include( ARM_R_Server.pri )

ARM_R_viewCore.depends = Graphics_ipp ARM_R_Common
ARM_R_view.depends         = ARM_R_viewCore Graphics_ipp ARM_R_viewCoreTest
ARM_R_viewCoreTest.depends = ARM_R_viewCore
ARM_R_ServerCore.depends = ARM_R_Common
ARM_R_Server.depends         = ARM_R_ServerCore ARM_R_ServerCoreTest
ARM_R_ServerCoreTest.depends = ARM_R_ServerCore
