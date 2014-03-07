# Subversion VCS driver for intergration scripts.
include core/environment.mak

.PHONY: vcs-ls-tag vcs-ls-branch
.IGNORE: vcs-ls-tag vcs-ls-branch
.SILENT: vcs-ls-tag vcs-ls-branch

all:
	$(info nothing to do)

vcs-init:
#	%1 Project source URI
#	%2 branch name
#	%3 Workdir

vcs-export:
#	src     - Project Source URI
#	filename - filename in repository
#	trunk    - Trunk name.
#	rev      - Revistion
#	target   - Target

	$(svn) export -r$(rev)  $(src)/$(trunk)/$(filename)  $(target) >$(nul) 2>$(nul)


vcs-ls-tag:
#	%1 - Source URI
#	%2 - Tag
	@$(svn) ls  $(src)/tags/$(tag) 2>$(nul)

vcs-ls-branch:
#	%1 - Source URI
#	%2 - Branch
	@$(svn) ls  $(src)/branches/$(branch) 2>$(nul)


vcs-switch-tag:
#	%1 - source URI
#	%2 - Tag
#	%3 - Target directory.

	$(svn) sw --force $(src)/tags/$(tag) $(target)
	$(svn) up $(target)

vcs-checkout-tag:
#	%1 - source URI
#	%2 - Tag
#	%3 - Target directory.

	$(svn) co --force $(src)/tags/$(tag) $(target)


vcs-checkout-branch:
#	%1 - source URI
#	%2 - Trunk name
#	%3 - Target directory.

	$(info Checkout $(src)/$(trunk) to  $(target))
	@$(svn) co --force $(src)/$(trunk) $(target)


vcs-merged:
#	%1 - Source URI
#	%2 - Trunk name
	@$(svn) pg svn:mergeinfo $(src)/$(trunk) | $(sed) -re "s/\/([A-Za-z0-9]+)\/branches\/(.*):.*/\2/g"

vcs-get-revision:
#	%1 - Source URI
#	%2 - Trunk name

ifeq "$(OS)" "Windows_NT"
	@$(svn) info $(src)/$(trunk) | $(gawk) "/Revision:/{ print $$2}"
else
	@$(svn) info $(src)/$(trunk) | $(gawk) '/Revision:/{ print $$2}'
endif


vcs-mktag:
#	%1 - Source URI
#	%2 - Trunk name
#	%3 - Revision
#	%4 - Tag name

	$(svn) cp  -m "tag $(tag) created" $(src)/$(trunk)@$(rev) $(src)/tags/$(tag)

vcs-mkbranch:
#	%1 - Source URI
#	%2 - Trunk name
#	%3 - Revision
#	%4 - Branch name

	$(svn) cp  -m "branch $(branch) created" $(src)/$(trunk)@$(rev) $(src)/branches/$(branch)
