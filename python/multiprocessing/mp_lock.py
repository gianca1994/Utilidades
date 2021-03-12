from multiprocessing import Process, Lock
lock=Lock()
def printer(item):
  lock.acquire()
  try:
      print(item)
  finally:
      lock.release()
if __name__=="__main__":
  items=['nacho','salsa',7]
  for item in items:
     p=Process(target=printer,args=(item,))
     p.start()
