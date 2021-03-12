import sys

save_stderr = sys.stderr
fh = open("/tmp/errors.txt","w")
sys.stderr = fh

print ("printing to error.txt", file=sys.stderr)

# return to normal:
sys.stderr = save_stderr

fh.close()

