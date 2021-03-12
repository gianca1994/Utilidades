import sys

while True:
  # output to stdout:
  print("iterando ...")
  try:
    number = input("Ingrese un numero: ")
  except EOFError:
    print("\nBye")
    break
  else:
    number = int(number)
    if number == 0:
      print(sys.stderr + "0 no tiene inverso")
    else:
      print("El inverso de %d es %f" % (number, 1.0/number) )
