
TEMPLATE = subdirs

include( ARM_OD_Server_projects.pri )

ARM_OD_Server.depends         = ARM_OD_ServerCore ARM_OD_ServerCoreTest
ARM_OD_ServerCoreTest.depends = ARM_OD_ServerCore
