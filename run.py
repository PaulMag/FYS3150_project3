from sys import argv
from os import system

if len(argv) >= 4: # solve and write data
    system("mkdir data/%s" % argv[3])
    system("./main.exe %s %s %s" % (argv[1], argv[2], argv[3]))

    flag = True
    if len(argv) >= 5:
        if argv[4] == "no":
            flag = False

    if flag == True: # also automatically plot results
        system("python plot.py %s &" % argv[3])

else: # solve without writing data
    system("./main.exe %s %s 0" % (argv[1], argv[2]))
