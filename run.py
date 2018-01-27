import subprocess

subprocess.call("g++ ./pile.cc -o ./pile.exe -std=c++14",shell=True)
subprocess.call("pile.exe")