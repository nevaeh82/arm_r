# Mercurial VCS driver for intergration scripts.
include core/environment.mak

# include /vcs-settings.ini

all:
	$(info nothing to do)

vcs-init:
#	%1 Project source URI
#	%2 branch name
#	%3 Workdir

ifeq "$(wildcard $(build_dir))" ""
	$(hg) clone   $(src)#$(trunk) $(build_dir)
else
	$(hg) pull --repository $(build_dir) -b $(trunk)
endif
	$(hg) update --clean --repository $(build_dir) $(trunk)


vcs-export:
#	%1 - Project Source URI
#	%2 - filename in repository
#	%3 - Trunk name.
#	%4 - Revistion
#	%5 - Target

	cd $(build_dir) && $(hg) cat -r$(rev) $(filename) > $(target)


vcs-ls-tag:
#	%1 - Source URI
#	%2 - Tag

	$(hg) tags --repository $(build_dir) |grep $(tag)


vcs-switch-tag:
#	%1 - source URI
#	%2 - Tag
#	%3 - Target directory.

	$(hg) pull --repository $(build_dir)
	$(hg) up -r $(tag) --repository $(target)


vcs-checkout-tag:
#	%1 - source URI
#	%2 - Tag
#	%3 - Target directory.

ifeq "$(wildcard $(build_dir))" ""
	$(hg) clone   $(src)#$(trunk) $(build_dir)
else
	$(hg) pull --repository $(build_dir)
endif
	$(hg) up -r $(tag) --clean --repository $(target)



vcs-checkout-branch:
#	%1 - source URI
#	%2 - Trunk name
#	%3 - Target directory.

ifeq "$(wildcard $(target))" ""
	$(hg) clone $(src)#$(trunk) $(target)
else
	$(hg) pull --repository $(build_dir)
	$(hg) up $(trunk) --clean --repository $(target)
endif
	$(hg) update --clean --repository $(target)


vcs-merged:
#	%1 - Source URI
#	%2 - Trunk name

#	not implemented
	$(hg) log --repository $(build_dir) | $(gawk) "/merged/{print $$3}"


vcs-get-revision:
#	%1 - Source URI
#	%2 - Trunk name

	$(hg) summary --repository $(build_dir) | $(gawk) "/parent:/{print $$2}"


vcs-mktag:
#	%1 - Source URI
#	%2 - Trunk name
#	%3 - Revision
#	%4 - Tag name

	$(hg) tag -f -r $(rev) $(tag) --repository $(build_dir) >$(nul)
	$(hg) push -f --repository $(build_dir) >$(nul)
