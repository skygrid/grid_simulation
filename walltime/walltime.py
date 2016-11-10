import matplotlib.pyplot as plt
import numpy as np
from matplotlib.patches import Rectangle
import sys
file = open("xxx.csv", "w")

original_types = ["MCSIMULATION", "USER", "DATASTRIPPING", "DATARECONSTRUCTION", "MERGE", "MCRECONSTRUCTION", "TURBO", "MCStripping", "MCMERGE"]
file.write("epoch," + ",".join(original_types) + "\n")
digital_types = range(10)
my_dict = dict(zip(original_types, digital_types))
rtype = np.dtype([('clock', np.float32), ('clock2', np.float32), ('type', np.str, 25)])

time_start, time_end, types = np.loadtxt(sys.argv[1], delimiter=",", skiprows=1, usecols=(3,5,7), unpack=True, dtype=rtype)
wall_time = time_end - time_start
days = (int(time_end[-1] / 86400)+1)

hist, bins = np.histogram(time_end, bins=range(0, 86400*days, 86400))

zipped_wt = zip(wall_time, types)
chunked = np.split(zipped_wt, hist.cumsum(), axis=0)

timex = 0

for day_array in chunked:
	file.write(str(timex) + ",")
	for my_type in original_types:
		indexes = np.where(day_array[:, 1] == my_type)
		new_array = day_array[indexes][:, 0]
		new_array = np.array(map(np.float, new_array))
		if len(new_array) > 0:
			average = np.average(new_array)
		else: average = 0
		file.write(str(round(average/3600, 3)) + ",")
	file.write("\n")
	timex += 1

file.close()

time, t1, t2, t3, t4, t5, t6, t7, t8, t9 = np.loadtxt("xxx.csv", usecols=range(10), skiprows=1, unpack=True, delimiter=",")
l = []
original_types = ["MCSIMULATION", "USER", "DATASTRIPPING", "DATARECONSTRUCTION", "MERGE", "MCRECONSTRUCTION", "TURBO", "MCStripping", "MCMERGE"]
for i in range(1, 10):
	l.append(eval("t" + str(i)))


fig, ax = plt.subplots()
stack_coll = ax.stackplot(time, l, edgecolor="none")
proxy_rects = [Rectangle((0, 0), 1, 1, fc=pc.get_facecolor()[0]) for pc in stack_coll]
ax.legend(proxy_rects, original_types)
plt.show()
