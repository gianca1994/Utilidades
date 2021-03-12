import signal, os

def handler(signum, frame):
    print('Signal handler called with signal', signum)

# Set the signal handler and a 5-second alarm
signal.signal(signal.SIGALRM, handler)
signal.alarm(5)
signal.pause()
signal.alarm(0)          # Disable the alarm

