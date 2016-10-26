import os

os.system("./CSimSim Platform/platform.xml Platform/deployment.xml simgridoutput/Normal-rem/out.txt simple"
          " --cfg=tracing:yes --cfg=tracing/platform:yes --cfg=tracing/filename:simgridoutput/Normal-rem/trace")

os.system("./CSimSim Platform/platform.xml Platform/deployment.xml simgridoutput/Normal-rem/out_DAM.txt DAM"
          " --cfg=tracing:yes --cfg=tracing/platform:yes --cfg=tracing/filename:simgridoutput/Normal-rem/trace_DAM")

os.system("./CsimSim Platform/platfrom.xml Platform/deployment.xml simgridoutput/Normal-nonrem/out.txt simple"
          " --cfg=tracing:yes --cfg=tracing/platform:yes --cfg=tracing/filename:/simgridoutput/Normal-nonrem/trace")

os.system("./CsimSim Platform/platfrom.xml Platform/deployment.xml simgridoutput/Normal-nonrem/out.txt simple"
          " --cfg=tracing:yes --cfg=tracing/platform:yes --cfg=tracing/filename:/simgridoutput/Normal-nonrem/trace")
