#
# Output directory rules
#


BuildDir = $$SolutionDir/build

DESTDIR  = $$BuildDir/bin/$$SPEC
LIBS    += -L$$BuildDir/bin/$$SPEC

UI_DIR         = $$BuildDir/obj/$$SPEC/$$TARGET
OBJECTS_DIR    = $$BuildDir/obj/$$SPEC/$$TARGET
MOC_DIR        = $$BuildDir/obj/$$SPEC/$$TARGET

CONFIG(debug, debug|release) {
 OBJECTS_DIR = $$OBJECTS_DIR/debug
}
CONFIG(release, debug|release) {
 OBJECTS_DIR = $$OBJECTS_DIR/release
}


