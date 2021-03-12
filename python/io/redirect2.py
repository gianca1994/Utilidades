import sys

save_stderr = sys.stderr
fh = open("errors.txt","w")
sys.stderr = fh

x = 10 / 0

# return to normal:
sys.stderr = save_stderr

fh.close()

