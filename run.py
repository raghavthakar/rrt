#! /usr/bin/python3

import subprocess

def main():
    values = []
    fi = open("runner.txt", "r")
    
    for line in fi:
    
        line_text = line.split(" ")
        values.append(int(line_text[0]))

    print(values) 
    if values:
        outfile = open("./src/output.txt", "w")

        for i in values:
            outfile.write(str(i) + " ")

        outfile.close()

    subprocess.call(["./run.sh"])

main()