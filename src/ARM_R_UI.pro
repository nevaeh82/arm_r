
TEMPLATE = subdirs

include( ARM_R_UI_projects.pri )

ARM_R_view.depends         = ARM_R_viewCore  ARM_R_viewCoreTest
ARM_R_viewCoreTest.depends = ARM_R_viewCore
