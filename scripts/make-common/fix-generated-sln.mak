include ../../scripts/common.mak

all:
	$(info using: make -f fix-generated-sln.mak fix BuildDir=path/to/build SlnFile=path/to/project.sln)
	$(error stop)


fix:
	$(sed) "s/$(call escape-slash,$(BuildDir))\///g" $(SlnFile) | \
		$(sed) "s/ProjectConfiguration/ProjectConfigurationPlatforms/g" |\
		$(sed) "s/SolutionConfiguration/SolutionConfigurationPlatforms/g" |\
		$(sed) "s/ConfigName.0/Debug|Win32/g" |\
		$(sed) "s/ConfigName.1/Release|Win32/g" > $(SlnFile).tmp
	$(mv) -f $(SlnFile).tmp $(SlnFile)
