#!/usr/local/bin/python3.9
import time
import sys


write = 'Number of arguments:' + str(len(sys.argv)) + 'arguments.\n' + 'Argument List:' + str(sys.argv)

f = open("cgi_post_test_file", "w")
f.write(write)
f.close()