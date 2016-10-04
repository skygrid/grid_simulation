import os, glob
Jobs = 1
trace_name = "trace"
out_name = "out.txt"
x = ["2replica", "3replica", "4replica", "5replica", "6replica", "7replica-nonrem", "8replica"]

# for Normal replication
os.system("python InputJobs/Normal-Replication/input_new.py " + str(Jobs))
os.system("python InputJobs/create_storage_content.py")
os.system("./CSim2Sim Platform/platform.xml Platform/deployment.xml simgridoutput/Normal-nonrem/" + out_name +  " --cfg=tracing:yes --cfg=tracing/platform:yes --cfg=tracing/filename:simgridoutput/Normal-nonrem/" + trace_name + " --cfg=maxmin/concurrency_limit:100000 --cfg=storage/max_file_descriptors:220000")
print "okay"

# for 1 replication
os.system("python InputJobs/1Replica/input_new.py " + str(Jobs))
os.system("python InputJobs/create_storage_content.py")
os.system("./CSim2Sim Platform/platform.xml Platform/deployment.xml simgridoutput/1replica-nonrem/" + out_name +  " --cfg=tracing:yes --cfg=tracing/platform:yes --cfg=tracing/filename:simgridoutput/1replica-nonrem/" + trace_name + " --cfg=maxmin/concurrency_limit:100000 --cfg=storage/max_file_descriptors:220000")
print "okay"

for i in range(len(x)):

    os.system("python InputJobs/Diff_replicas/input.py " + str(Jobs) + " " + str(i+2))
    os.system("python InputJobs/create_storage_content.py")

    os.system("./CSim2Sim Platform/platform.xml Platform/deployment.xml simgridoutput/" + x[i] + "/" + out_name +  " --cfg=tracing:yes --cfg=tracing/platform:yes --cfg=tracing/filename:simgridoutput/" + x[i] + "/" +trace_name + " --cfg=maxmin/concurrency_limit:100000 --cfg=storage/max_file_descriptors:220000")
    print "okay"

os.chdir("simgridoutput/")
dirs = glob.glob("*replica*")
print os.system("pwd")
for directory in dirs:
    print directory
    os.system("python clear_trace.py " + directory + "/" + trace_name)