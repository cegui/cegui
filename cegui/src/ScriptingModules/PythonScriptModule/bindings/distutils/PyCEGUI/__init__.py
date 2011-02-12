import os
import os.path

# atrocious and unholy!
def get_my_path():
    import fake
    return os.path.dirname(str(fake).split()[3][1:])

libpath = os.path.abspath(get_my_path())
print "libpath =", libpath
os.environ['PATH'] = libpath + ";" + os.environ['PATH']

from PyCEGUI import *
