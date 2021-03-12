import multiprocessing
from multiprocessing import Process, current_process
import os
def child1():
     print(current_process().name)
def child2():
         print(current_process().name)
if __name__=="__main__":
   print("Parent ID",os.getpid())
   p1=Process(target=child1,name='Child 1')
   p2=Process(target=child2,name='Child 2')
   p1.start()
   p2.start()
   p1.join()
   p2.join()
   print("We're done")
