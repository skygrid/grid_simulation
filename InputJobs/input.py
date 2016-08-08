import pandas as pd
import random
from scipy import stats

job_amount = 1500

Locations = ["CERN", "CNAF", "GRIDKA", "IN2P3", "PIC", "RAL", "SARA", "RRCKI"]

xk = (3, 2, 1)  # Amount of locations data has
pk = (0.7, 0.2, 0.1)  # Probabilities that data has such amount of data
custm = stats.rv_discrete(name='custm', values=(xk, pk))
prob_array_input = custm.rvs(size=job_amount)
prob_array_output = custm.rvs(size=job_amount)

types = ["USER", "DATASTRIPPING", "MERGE", "MCStripping", "DATARECONSTRUCTION", "TURBO",  "MCRECONSTRUCTION", "WGPRODUCTION", "MCMERGE", "UNKNOWN", "MCSIMULATION", "TEST"]
int_types = range(0,12)
types_pk = (0.061, 0.175, 0.061, 0.005, 0.205, 0.046, 0.006, 0.025, 0.0, 0.0, 0.393, 0.023)
types_custmx = stats.rv_discrete(name='custm', values=(int_types, types_pk))
types_custm = types_custmx.rvs(size=job_amount)



f = open("../input.csv", "w")
f.write("Name, Type, Flops-Size, InputFileName, InputSize, NRep, Storage_T, Location1, Location2, Location3, OutPut-Name, Output-Size, NRepOut, OutLoc1, OutLoc2, OutLoc3\n")



for i in range(job_amount):
    random.shuffle(Locations)
    input_file_name = "inp_" + str(i) + "." + types[types_custm[i]]
    output_file_name = "out_" + str(i) + "." + types[types_custm[i]]
    name = "Job" + str(i)
    cpu_size = random.randint(100, 200) * 1073741824
    NREpIn = prob_array_input[i]
    NREpOut = 3 #prob_array_output[count]
    if types[types_custm[i]] == "MCSIMULATION":
        out_size = 1 * 1073741824 # GB
        data_size = 0
    else:
        out_size = 1 * 1073741824 # GB
        data_size = random.choice([1,2,3]) * 1073741824
    if types[types_custm[i]] == 'MCSIMULATION':
        location_str = "0,0,0"
        NREpIn = 0
    else:
        if NREpIn == 1:
            location_str = ",".join(Locations[:prob_array_input[i]]) + ",0,0"
        elif NREpIn == 2:
            location_str = ",".join(Locations[:prob_array_input[i]]) + ",0"
        else:
            location_str = ",".join(Locations[:prob_array_input[i]])
    random.shuffle(Locations)
    output_locations = ",".join(Locations[:3])
    string = "Job" + str(i) + "," + types[types_custm[i]] + "," + str(cpu_size) + "," + input_file_name + "," + str(data_size) + "," + str(NREpIn) + "," + str(random.randint(0,1)) + "," + location_str + "," + output_file_name + "," + str(out_size) + "," + str(NREpOut) + "," + output_locations+ "\n"
    f.write(string)

f.close()

