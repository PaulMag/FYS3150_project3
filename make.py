# "How does one use this crap?" - Vedad Hodzic

import os,sys
os.system('c++ -c *.cpp')
os.system('c++ -o %s.exe -larmadillo -lblas -llapack *.o' % sys.argv[1])
os.system('rm *.o')
