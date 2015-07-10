include(coverage.pri)

TEMPLATE = subdirs

SUBDIRS += unit

# We can't run our integration tests yet on other platforms as on arm
# as they expect a working media-hub we don't have yet anywhere else.
system(uname -a | grep arm):SUBDIRS += integration
