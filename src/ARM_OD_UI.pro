
TEMPLATE = subdirs

include( ARM_OD_UI_projects.pri )

ARM_OD_view.depends         = ARM_OD_viewCore  ARM_OD_viewCoreTest
ARM_OD_viewCoreTest.depends = ARM_OD_viewCore
