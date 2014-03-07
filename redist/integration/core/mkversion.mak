
checkout_ci:
	$(info ------------------------------ checkout CI  ---------------------- )
	$(svn) co $(ci) $(build_ci)

mkversion_ci: checkout_ci
	$(info ------------------------------ configure CI version ---------------------- )
	$(MAKE) -f core/make-build-info.mak $(current_version_file)

mkversion_trunk: checkout_source_trunk
	$(info ------------------------------ configure version ---------------------- )
	$(MAKE) -f core/make-build-info.mak $(trunk_version_file)
