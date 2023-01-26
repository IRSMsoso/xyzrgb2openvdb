import random


num = 10000
min_range = -1
max_range = 1

f = open("output.txt", "w")
f.write(str(num) + "\n")
for i in range(num):
    f.write(str(random.uniform(min_range, max_range)) + " " + str(random.uniform(min_range, max_range)) + " " + str(random.uniform(min_range, max_range)) + " " + str(random.randint(0, 255)) + " " + str(random.randint(0, 255)) + " " + str(random.randint(0, 255)) + "\n")
f.close()
