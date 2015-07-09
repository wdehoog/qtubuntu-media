include(coverage.pri)

TEMPLATE = subdirs

SUBDIRS += \
# FIXME: Temporarily disable integration tests until we can figure out
#        how to have them only run on phone targets
#integration \
    unit
