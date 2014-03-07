include core/settings.mak

.PHONY:: $(trunk_version_file) $(current_version_file)
.IGNORE:: $(current_version_file) $(trunk_version_file)

$(trunk_version_file): $(buildinfo_src_path)
	@sed "s/set //g" < $<  >$@

$(current_version_file):
	@$(svn) export $(buldinfo_dst_uri) $@.tmp >$(nul)
	@sed "s/set //g" < $@.tmp  >$@
	@$(RM) $@.tmp
