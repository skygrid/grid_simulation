import sys
import random
from scipy import stats
import numpy as np

'''
REPLICA DICTIONARY JobType:[At CERN1 yes or no\
                            At CERN0 yes or no\
                            Number of replicas at Tier1 disk\
                            Number of replicas at Tier1 tape]
'''

job_amount = int(sys.argv[1])
print job_amount

# VARIABLE FOR POISSON DISTRIBUTION
lyambda = 0.0031

Locations = ["CERN", "CNAF", "GRIDKA", "IN2P3", "PIC", "RAL", "SARA", "RRCKI"]
Location_DISK = ["CNAF1", "GRIDKA1", "IN2P31", "PIC1", "RAL1", "SARA1", "RRCKI1"]
Location_TAPE = ["CNAF0", "GRIDKA0", "IN2P30", "PIC0", "RAL0", "SARA0", "RRCKI0"]
NULL_REPLICA = "1,1,1,1,1,1,1,1,CERN0,+"

types = ["MCSIMULATION", "USER", "DATASTRIPPING", "DATARECONSTRUCTION", "MERGE", "WGPRODUCTION", "MCRECONSTRUCTION", "TURBO", "MCStripping", "MCMERGE"]
FLOP_SIZE_BY_TYPE = {"USER":0.34, "DATASTRIPPING":0.24, "MERGE":0.07, "MCStripping":0.03, "DATARECONSTRUCTION":0.56, "TURBO":0.03,  "MCRECONSTRUCTION":0.55, "WGPRODUCTION":0.22, "MCMERGE":0.17, "MCSIMULATION":1.07}
INPUT_SIZE_BY_TYPE = {"USER":31710*10**6, "DATASTRIPPING":4273*10**6, "MERGE":2072*10**6, "MCStripping":9897*10**6, "DATARECONSTRUCTION":2887*10**6, "TURBO":2788*10**6,  "MCRECONSTRUCTION":4429*10**6, "WGPRODUCTION":10**9, "MCMERGE":3857*10**6, "UNKNOWN":0, "MCSIMULATION":0, "TEST":0.1*10**9}
OUTPUT_SIZE_BY_TYPE = {"USER":79*10**6, "DATASTRIPPING":1.5*10**9, "MERGE":5*10**9, "MCStripping":1.5*10**9, "DATARECONSTRUCTION":2*10**9, "TURBO":1*10**9,  "MCRECONSTRUCTION":2*10**9, "WGPRODUCTION":1*10**9, "MCMERGE":5*10**9, "UNKNOWN":0, "MCSIMULATION":0.5*10**9, "TEST":0.1*10**9}
REPLICA_BY_TYPE = {"USER":[0, 0, 3, 0], "DATASTRIPPING":NULL_REPLICA, "MERGE":[1, 1, 3, 1], "MCStripping":NULL_REPLICA, "DATARECONSTRUCTION":NULL_REPLICA, "TURBO":NULL_REPLICA,  "MCRECONSTRUCTION":NULL_REPLICA, "WGPRODUCTION":NULL_REPLICA, "MCMERGE":[1, 1, 2, 1], "UNKNOWN":NULL_REPLICA, "MCSIMULATION":NULL_REPLICA, "TEST":NULL_REPLICA}

int_types = range(0, 10)
types_pk = (0.507, 0.196, 0.108, 0.096, 0.058, 0.015, 0.005, 0.005, 0.004, 0.004)
types_custm = stats.rv_discrete(name='custm', values=(int_types, types_pk)).rvs(size=job_amount)
mc_indexes = np.where(types_custm == 0)


xtimes_array = np.arange(0, job_amount, 1)
xtimes_array = np.delete(xtimes_array, mc_indexes)
inp_array = np.zeros(job_amount).astype(dtype="|S64")
out_array = np.arange(0, job_amount, 1).astype(dtype="|S64")

LOCATION_STR = np.zeros(job_amount).astype(dtype="|S64")
STORAGE_TYPES = np.zeros(job_amount).astype(dtype="|S64")
NREPIN = np.zeros(job_amount).astype(dtype="|S64")
BYTE_SIZE = np.zeros(job_amount).astype(dtype="|S64")


def my_func():
	for i in range(len(xtimes_array)):
		# MC or Reconst
		if (types_custm[i] == 0) | (types_custm[i] == 3):
			depth = 3
		# Stripping or MCStripping
		elif (types_custm[i] == 2) | (types_custm[i] == 8):
			depth = 2
		# Merge or MCMerge
		elif (types_custm[i] == 4) | (types_custm[i] == 9):
			depth = 1
		else:
			depth = 1
		name = "dataset" + str(i)
		if fill_array(name, depth) == "x":
			break


def fill_array(dataset_name, depth):
	depth -= 1
	global xtimes_array
	if depth < 0:
		return
	if len(xtimes_array) == 0:
		return "x"
	mu = random.randint(20, 40)
	sigma = random.randint(20, 40)
	z = random.randint(20, 60)

	amount_dataset = stats.binom.rvs(random.randint(10, 20), p=0.3)

	if len(xtimes_array) <= amount_dataset:
		amount_dataset = len(xtimes_array)
	if len(xtimes_array) <= z:
		z = len(xtimes_array)
	if amount_dataset > z:
		amount_dataset = z

	x = np.linspace(0, 50, z)
	prob = stats.norm.pdf(x, loc=mu, scale=sigma)
	prob = prob / sum(prob)

	inp_indexes = np.sort(np.random.choice(xtimes_array[:z], size=amount_dataset, replace=False, p=prob))
	xtimes_array = np.setdiff1d(xtimes_array, inp_indexes)

	random.shuffle(Locations)
	if len(inp_indexes) > 0:
		data_size = 11.2 * np.random.normal(INPUT_SIZE_BY_TYPE[types[types_custm[inp_indexes[0]]]], 0.15*INPUT_SIZE_BY_TYPE[types[types_custm[inp_indexes[0]]]], 1)[0] if INPUT_SIZE_BY_TYPE[types[types_custm[inp_indexes[0]]]] else 0

		for item in inp_indexes:
			inp_array[item] = dataset_name
			out_array[item] = "out_dataset" + "_" + str(item)

			NREpIn = 10
			location_str = ",".join(Locations) + ",CERN," + str(Locations[0])
			storage_types = "1,1,1,1,1,1,1,1,0,0"

			LOCATION_STR[item] = location_str
			STORAGE_TYPES[item] = storage_types
			NREPIN[item] = NREpIn
			BYTE_SIZE[item] = data_size

			#fill_array(out_array[item], depth)


my_func()


# TASKS COMES OVER A TIME
times_array = (np.cumsum(stats.poisson.rvs(lyambda, size=job_amount))).astype(float) * 86400
frequency, z = np.histogram(times_array)
frequency = frequency[np.nonzero(frequency)]
big_noize = []
for item in frequency:
	noize = np.sort(np.random.uniform(low=0, high=10000, size=item))
	big_noize.append(noize)
big_noize = np.hstack(np.array(big_noize))
print len(big_noize)
print len(times_array)
times_array += big_noize


f = open("input.csv", "w")
f.write("Name, Type, TimeOfSubmission, Flops-Size, InputFileName, InputSize, NRep, Location1, Location2, Location3, Location4, Location5, Location6, Location7, Location8, Location9, Location10, Storage_T1, Storage_T2, Storage_T3, Storage_T4, Storage_T5, Storage_T6, Storage_T7, Storage_T8, Storage_T9, Storage_T10, OutPut-Name, Output-Size, NRepOut, OutLoc1, OutLoc2, OutLoc3, OutLoc4, OutLoc5, OutLoc6, OutLoc7, OutLoc7, OutLoc9, OutLoc10\n")



for i in range(job_amount):
	name = "Job" + str(i)
	cpu_size = 86400 * 10**9 * np.random.normal(FLOP_SIZE_BY_TYPE[types[types_custm[i]]], 0.15 * FLOP_SIZE_BY_TYPE[types[types_custm[i]]], 1)[0]
	out_size = np.random.normal(OUTPUT_SIZE_BY_TYPE[types[types_custm[i]]], 0.15*OUTPUT_SIZE_BY_TYPE[types[types_custm[i]]], 1)[0]

	random.shuffle(Location_DISK)
	random.shuffle(Location_TAPE)

	if types[types_custm[i]] == "MCSIMULATION":
		dataset_name = "none"
		locs = "0,0,0,0,0,0,0,0,0,0"
		storage_types = "none,none,none,none,none,none,none,none,none,none"
		byte_size = 0
		out_dataset = "mc_out_data" + str(i)
		nrepin = 0
	else:
		dataset_name = inp_array[i]
		locs = LOCATION_STR[i]
		storage_types = STORAGE_TYPES[i]
		byte_size = BYTE_SIZE[i]
		out_dataset = out_array[i]
		nrepin = NREPIN[i]

	NREpOut = 10
	string = "Job" + str(i) + "," + types[types_custm[i]] + "," + str(times_array[i]) + "," + str(cpu_size) + "," + dataset_name + "," + str(byte_size) + "," + str(nrepin) + "," + locs + "," + storage_types + "," + out_dataset + "," + str(out_size) + "," + str(NREpOut) + "," + ",".join(Location_DISK) + ",CERN1,CERN0,+" + "\n"
	f.write(string)

f.close()

