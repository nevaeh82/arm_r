# Define satellite names for TARGET if need manual copy
#


TARGET_PDB := $(DESTDIR_TARGET:.dll=.pdb)
TARGET_EXP := $(DESTDIR_TARGET:.dll=.exp)
TARGET_LIB_MSVC  := $(DESTDIR_TARGET:.dll=.lib)
TARGET_LIB_MINGW := $(addsuffix .a, $(dir $(DESTDIR_TARGET))/lib$(notdir $(basename $(DESTDIR_TARGET) )))

