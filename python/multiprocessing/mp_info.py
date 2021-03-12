import multiprocessing
from multiprocessing import Process
import os
def child1():
     print("Child 1",os.getpid())
def child2():
        print("Child 2",os.getpid())
if __name__=="__main__":
   print("Parent ID",os.getpid())
   p1=Process(target=child1)
   p2=Process(target=child2)
   p1.start()
   p2.start()
   p1.join()
   alive='Yes' if p1.is_alive() else 'No'
   print("Is p1 alive?",alive)
   alive='Yes' if p2.is_alive() else 'No'
   print("Is p2 alive?",alive)
   p2.join()
   print("We're done")
