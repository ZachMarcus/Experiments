import os
import time


def test(callString):
	start = time.time()
	os.system(callString)
	return time.time() - start


def testThread(threadNum, size):
	result = []
	base = "./sieve " + str(threadNum) + " " + str(size)
	for i in range(1,8):
		result.append(test(base))
	return sum(result) / len(result)

if __name__ == "__main__":
	threads = [1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16]
	sizes = []
	results = []
	for x in range(8, 29):
		sizes.append(2**x)
	for t in threads:
		temp = []
		for s in sizes:
			temp.append(testThread(t,s))
		results.append(temp)
	with open('perf.csv','w+') as f:
		f.write('ThreadNum,NumSize,Time\n')
		for t in range(0, len(threads)):
			for s in range(0, len(sizes)):
				f.write(str(threads[t]) + ',' + str(sizes[s]) + ',' + str(results[t][s]) + '\n' )
			






