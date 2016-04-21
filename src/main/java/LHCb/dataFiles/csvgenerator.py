import random

Locations = ['Tier1_1','Tier1_2', 'Tier1_3', 'Tier1_4', 'Tier1_5', 'Tier1_6', 'Tier1_7']
numbers = [i for i in range(7)]
f = open("data.csv", 'w')
f.write("Task, ExecutionSize, DataSize, Location1, Location2, Location3\n")
for i in range(100):
    s = ""
    random.shuffle(numbers)
    for j in range(7):
        x = numbers[j]
        s += Locations[x] + ","
    f.write("Task" + str(i) + "," + str(random.randint(50000000, 1000000000)) +"," + str(random.randint(100, 300)) + "," + s + "\n")
f.close()

