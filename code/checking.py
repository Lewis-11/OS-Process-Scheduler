nuestroFile=open("out_par.txt", "r")
origFile=open("out.txt", "r")

ourLines=nuestroFile.readlines()
theirLines=origFile.readlines()

if not len(ourLines) == len(theirLines):
    print("out.txt and out_seq_nuestro.txt dont have the same # of lines")
else:
    for i in range(len(ourLines)):
        ourdata=ourLines[i].split(" ")
        theirdata=theirLines[i].split(" ")
        for j in range(5):
            diff= float(ourdata[j])-float(theirdata[j])
            if diff<0:
                diff = diff*(-1)
            if diff>5.0:
                print("problem found: " + ourdata[j] + " when orig was: " + theirdata[j] + " in line: " + str(i+1))
                break


