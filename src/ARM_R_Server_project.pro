
TEMPLATE = subdirs

include( ARM_R_Server_projects.pri )

ARM_R_Server.depends         = ARM_R_ServerCore ARM_R_ServerCoreTest
ARM_R_ServerCoreTest.depends = ARM_R_ServerCore
