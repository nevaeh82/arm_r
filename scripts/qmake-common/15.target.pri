
#
#  target selector
#


CONFIG(debug, debug|release) {
        unix: TARGET = $$join(TARGET,,,d)
        win32: TARGET = $$join(TARGET,,,d)
}
