PROJECT_ROOT_DIRECTORY = $$PWD/..
CONFIG += debug_and_release
OUTDIR = $$PROJECT_ROOT_DIRECTORY/destdir/outputs

CONFIG(debug, debug|release) {
    DESTDIR = $$PROJECT_ROOT_DIRECTORY/destdir/build/debug/
    OBJECTS_DIR =   $$OUTDIR/debug/obj
    MOC_DIR =       $$OUTDIR/debug/moc
    RCC_DIR =       $$OUTDIR/debug/rcc
    UI_DIR =        $$OUTDIR/debug/ui
    LIBS +=         "-L$$DESTDIR"
    COPYDIR =       $$DESTDIR

} else {
    DESTDIR = $$PROJECT_ROOT_DIRECTORY/destdir/build/release/
    OBJECTS_DIR =   $$OUTDIR/release/obj
    MOC_DIR =       $$OUTDIR/release/moc
    RCC_DIR =       $$OUTDIR/release/rcc
    UI_DIR =        $$OUTDIR/release/ui
    LIBS +=         "-L$$DESTDIR"
    COPYDIR =       $$DESTDIR
}
