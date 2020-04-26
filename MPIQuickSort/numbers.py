import random

random.seed(7)

f = open("1m_nums.txt", "w")
for i in range(1000000):
	x = random.randrange(0,1000000)
	f.write(str(x) + ' ')
f.close()