import os, glob
Jobs = 1
out_name = "out"
trace_name = "trace"
x = ["2replica", "3replica", "4replica", "5replica", "6replica", "7replica-nonrem", "8replica"]

# for Normal replication
os.system("python InputJobs/Normal-Replication/input_new.py " + str(Jobs))
os.system("python InputJobs/create_storage_content.py")
for t_type in ["", "_DAM"]:
    os.system("./CSim2Sim Platform/platform.xml Platform/deployment.xml simgridoutput/Normal-nonrem/" + out_name + t_type + ".txt" +  " --cfg=tracing:yes --cfg=tracing/platform:yes --cfg=tracing/filename:simgridoutput/Normal-nonrem/" + trace_name + t_type + " --cfg=maxmin/concurrency_limit:100000 --cfg=storage/max_file_descriptors:220000")
    print("okay" + t_type)

# for 1 replication
os.system("python InputJobs/1Replica/input_new.py " + str(Jobs))
os.system("python InputJobs/create_storage_content.py")
for t_type in ["", "_DAM"]:
    os.system("./CSim2Sim Platform/platform.xml Platform/deployment.xml simgridoutput/1replica-nonrem/" + out_name + t_type + ".txt"  +  " --cfg=tracing:yes --cfg=tracing/platform:yes --cfg=tracing/filename:simgridoutput/1replica-nonrem/" + trace_name + t_type + " --cfg=maxmin/concurrency_limit:100000 --cfg=storage/max_file_descriptors:220000")
    print("okay" + t_type)

for i in range(len(x)):

    os.system("python InputJobs/Diff_replicas/input.py " + str(Jobs) + " " + str(i+2))
    os.system("python InputJobs/create_storage_content.py")

    for t_type in ["", "_DAM"]:
        os.system("./CSim2Sim Platform/platform.xml Platform/deployment.xml simgridoutput/" + x[i] + "/" + out_name + t_type + ".txt"  +  " --cfg=tracing:yes --cfg=tracing/platform:yes --cfg=tracing/filename:simgridoutput/" + x[i] + "/" + trace_name + t_type + " --cfg=maxmin/concurrency_limit:100000 --cfg=storage/max_file_descriptors:220000")
        print("okay" + t_type + " " + x[i])


os.chdir("simgridoutput/")
dirs = glob.glob("*replica*")
dirs.append("Normal-nonrem")
print os.system("pwd")

for directory in dirs:
    print directory
    for t_type in ["", "_DAM"]:
        os.system("python clear_trace.py " + directory + "/" + trace_name + t_type)