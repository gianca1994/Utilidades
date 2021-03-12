import os,time,signal

def handler(s,f):
    print("mensaje... soy el pid %d"% os.getpid())

def hijo():
    print("Iniciando hijo")
    while True:
        print("Hijo esperando...")
        signal.pause()


signal.signal(signal.SIGUSR1,handler)
retorno=os.fork()
print("pid: %d (soy %d)\n" % (retorno, os.getpid()))

if retorno == 0:
    hijo()
else:
    print("Iniciando padre")
    for item in range(5):
        os.kill(retorno,signal.SIGUSR1)
        time.sleep(1)
    print("Padre matando al hijo...")
    os.kill(retorno,signal.SIGTERM)
    print("Padre terminando...")



        
