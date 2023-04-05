#!/usr/local/bin/python3.9
import time
import sys


print ("Content-type:text/html\r\n\r\n")
print ('<html>')
print ('<head>')
print ('<title>Hello World - First CGI Program</title>')
print ('</head>')
print ('<body>')
print ('<h2>Hello World! This is my first CGI program</h2>')
print ('Number of arguments:', len(sys.argv), 'arguments.')
print ('Argument List:', str(sys.argv))
print ('</body>')
print ('</html>')
