import sys
import random
from scipy import stats

'''
REPLICA DICTIONARY JobType:[At CERN1 yes or no\
                            At CERN0 yes or no\
                            Number of replicas at Tier1 disk\
                            Number of replicas at Tier1 tape]
'''

job_amount = int(sys.argv[1])
print job_amount

Locations = ["CERN", "CNAF", "GRIDKA", "IN2P3", "PIC", "RAL", "SARA", "RRCKI"]
Location_DISK = ["CNAF1", "GRIDKA1", "IN2P31", "PIC1", "RAL1", "SARA1", "RRCKI1"]
Location_TAPE = ["CNAF0", "GRIDKA0", "IN2P30", "PIC0", "RAL0", "SARA0", "RRCKI0"]
NULL_REPLICA = [0, 0, 0, 0]

xk = (4, 3, 2)  # Amount of locations data has
pk = (0.7, 0.2, 0.1)  # Probabilities that data has such amount of data
custm = stats.rv_discrete(name='custm', values=(xk, pk))
prob_array_input = custm.rvs(size=job_amount)
prob_array_output = custm.rvs(size=job_amount)

types = ["USER", "DATASTRIPPING", "MERGE", "MCStripping", "DATARECONSTRUCTION", "TURBO",  "MCRECONSTRUCTION", "WGPRODUCTION", "MCMERGE", "UNKNOWN", "MCSIMULATION", "TEST"]
FLOP_SIZE_BY_TYPE = {"USER":0.34, "DATASTRIPPING":0.24, "MERGE":0.07, "MCStripping":0.03, "DATARECONSTRUCTION":0.56, "TURBO":0.03,  "MCRECONSTRUCTION":0.55, "WGPRODUCTION":0.22, "MCMERGE":0.17, "UNKNOWN":0.0, "MCSIMULATION":1.07, "TEST":0.1}
INPUT_SIZE_BY_TYPE = {"USER":31710*10**6, "DATASTRIPPING":4273*10**6, "MERGE":2072*10**6, "MCStripping":9897*10**6, "DATARECONSTRUCTION":2887*10**6, "TURBO":2788*10**6,  "MCRECONSTRUCTION":4429*10**6, "WGPRODUCTION":10**9, "MCMERGE":3857*10**6, "UNKNOWN":0, "MCSIMULATION":0, "TEST":0.1*10**9}
OUTPUT_SIZE_BY_TYPE = {"USER":79*10**6, "DATASTRIPPING":1.5*10**9, "MERGE":5*10**9, "MCStripping":1.5*10**9, "DATARECONSTRUCTION":2*10**9, "TURBO":1*10**9,  "MCRECONSTRUCTION":2*10**9, "WGPRODUCTION":1*10**9, "MCMERGE":5*10**9, "UNKNOWN":0, "MCSIMULATION":0.5*10**9, "TEST":0.1*10**9}
REPLICA_BY_TYPE = {"USER":[0, 0, 3, 0], "DATASTRIPPING":NULL_REPLICA, "MERGE":[1, 1, 3, 1], "MCStripping":NULL_REPLICA, "DATARECONSTRUCTION":NULL_REPLICA, "TURBO":NULL_REPLICA,  "MCRECONSTRUCTION":NULL_REPLICA, "WGPRODUCTION":NULL_REPLICA, "MCMERGE":[1, 1, 2, 1], "UNKNOWN":NULL_REPLICA, "MCSIMULATION":NULL_REPLICA, "TEST":NULL_REPLICA}

int_types = range(0,12)
types_pk = (0.061, 0.175, 0.061, 0.005, 0.205, 0.046, 0.006, 0.025, 0.0, 0.0, 0.393, 0.023)
types_custmx = stats.rv_discrete(name='custm', values=(int_types, types_pk))
types_custm = types_custmx.rvs(size=job_amount)



f = open("input.csv", "w")
f.write("Name, Type, Flops-Size, InputFileName, InputSize, NRep, Location1, Location2, Location3, Location4, Storage_T1, Storage_T2, Storage_T3, Storage_T4, OutPut-Name, Output-Size, NRepOut, OutLoc1, OutLoc2, OutLoc3, OutLoc4, OutLoc5, OutLoc6\n")



for i in range(job_amount):
    random.shuffle(Locations)
    input_file_name = "inp_" + str(i) + "." + types[types_custm[i]]
    output_file_name = "out_" + str(i) + "." + types[types_custm[i]]
    name = "Job" + str(i)
    cpu_size = 86400 * 10**9 * random.uniform(0.85*FLOP_SIZE_BY_TYPE[types[types_custm[i]]], 1.15*FLOP_SIZE_BY_TYPE[types[types_custm[i]]])
    data_size = random.uniform(0.85*INPUT_SIZE_BY_TYPE[types[types_custm[i]]], 1.15*INPUT_SIZE_BY_TYPE[types[types_custm[i]]])
    out_size = random.uniform(0.85*OUTPUT_SIZE_BY_TYPE[types[types_custm[i]]], 1.15*OUTPUT_SIZE_BY_TYPE[types[types_custm[i]]])

    NREpIn = prob_array_input[i]
    if types[types_custm[i]] == 'MCSIMULATION':
        location_str = "0,0,0,0"
        storage_types = "none,none,none,none"
        NREpIn = 0
    else:
        if NREpIn == 2:
            location_str = ",".join(Locations[:prob_array_input[i]]) + ",0,0"
            storage_types = "1,0,none,none"
        elif NREpIn == 3:
            location_str = ",".join(Locations[:prob_array_input[i]]) + ",0"
            storage_types = "1,1,0,none"
        else:
            location_str = ",".join(Locations[:prob_array_input[i]])
            storage_types = "1,1,1,0"

    random.shuffle(Location_DISK)
    random.shuffle(Location_TAPE)

    NREpOut = sum(REPLICA_BY_TYPE[types[types_custm[i]]])
    if types[types_custm[i]] == "MERGE":
        output_locations = "CERN1,CERN0," + ",".join(Location_DISK[:3]) + "," + Location_TAPE[0]
    elif types[types_custm[i]] == "MCMERGE":
        output_locations = "CERN1,CERN0," + ",".join(Location_DISK[:2]) + "," + Location_TAPE[0] + ",0"
    elif types[types_custm[i]] == "USER":
        output_locations = ",".join(Location_DISK[:3]) + ",0,0,0"
    else:
        output_locations= "0,0,0,0,0,0"

    string = "Job" + str(i) + "," + types[types_custm[i]] + "," + str(cpu_size) + "," + input_file_name + "," + str(data_size) + "," + str(NREpIn) + "," + location_str + "," + storage_types + "," + output_file_name + "," + str(out_size) + "," + str(NREpOut) + "," + output_locations+ "\n"
    f.write(string)

f.close()

