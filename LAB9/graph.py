import glob, os, sys
import matplotlib.pyplot as plt

folder = sys.argv[1]
imagepath = "graphs"
try:
	os.stat("graphs")
except:
	os.mkdir("graphs")
os.chdir(folder)
respath = "../"+folder
i = 0
for file in glob.glob("*"):
	filename = file+".png"
	print(filename)
	X, Y = [], []
	plt.close("all")
	for line in open(file, 'r'):
		values = [float(s) for s in line.split()]
		X.append(values[0])
		Y.append(values[1])
	i = i+1
	fig = plt.figure(i)
	plt.ylabel('Congestion Window')
	plt.xlabel('Packet')
	plt.plot(X, Y)
	os.chdir("../graphs")
	plt.savefig(filename)
	#plt.show()
	plt.close(fig)
	os.chdir(respath)