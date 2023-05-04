
import time
import sys
import os


print ("Content-type:text/html\r\n\r\n")
print ('<html>')
print ('<head>')
print ('<title>Hello World - First CGI Program</title>')
print ('</head>')
print ('<body>')
print ('<h2>Hello World! This is my first CGI program</h2>')
print ('Number of arguments:', len(sys.argv), 'arguments.')
print ('Argument List:', str(sys.argv))
os.environ["TEST_VAR"] = "go4expert.com"
for name, value in os.environ.items():
         print ('%s\t= %s <br/>' % (name, value))
# print ('environment variables:')
# env_var = os.environ[1]
# print('Currently working in ', env_var, 'environment')
print ('</body>')
print ('</html>')
