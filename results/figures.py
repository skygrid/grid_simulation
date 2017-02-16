__author__ = 'mikhail91'

import numpy
import pandas
import matplotlib.pyplot as plt


class JobTimePlots(object):

    def __init__(self, sim_out_data, real_out_data):
        """
        This class generates different job time plots.

        Parameters
        ----------
        sim_out_data : pandas.DataFrame
            Simulation output data of job times.
        real_out_data : pandas.DataFrame
            Real job times data.
        """

        self.sim_out_data = sim_out_data
        self.real_out_data = real_out_data


    def job_exec_time(self):
        """
        Generates job execution time plot.
        Job execution time = job end execution time - job start execution time
        """

        sim_exec_time = self.sim_out_data.TimeEndExec.values - self.sim_out_data.TimestartExec.values
        sim_exec_time = sim_exec_time / 1000.
        real_exec_time = self.real_out_data.EndExecTime.values.astype(numpy.float) - self.real_out_data.StartExecTime.values.astype(numpy.float)
        real_exec_time = real_exec_time / 1000.

        min_val = min(sim_exec_time.min(), real_exec_time.min())
        max_val = max(sim_exec_time.max(), real_exec_time.max())

        bins = numpy.linspace(min_val, max_val, 101)

        plt.figure(figsize=(12, 8))
        plt.hist((real_exec_time), bins=bins, histtype='stepfilled', linewidth=2, label='Real', color='r', alpha=0.5)
        plt.hist((sim_exec_time), bins=bins, histtype='stepfilled', linewidth=2, label='Simulation', color='b', alpha=0.5)

        plt.title('Job Execution Time Distribution', size=20)
        plt.xlabel('Job Execution Time, ks', size=20)
        plt.ylabel('Number of jobs', size=20)
        plt.xticks(size=20)
        plt.yticks(size=20)
        plt.grid(b=1)
        plt.legend(loc='best', prop={'size':15})
        plt.show()


    def job_exec_time_by_jobtype(self):
        """
        Generates job execution time plots by the job types.
        Job execution time = job end execution time - job start execution time
        """

        sim_exec_time = self.sim_out_data.TimeEndExec.values - self.sim_out_data.TimestartExec.values
        sim_job_types = self.sim_out_data.Type.values

        real_exec_time = self.real_out_data.EndExecTime.values.astype(numpy.float) - self.real_out_data.StartExecTime.values.astype(numpy.float)
        real_job_types = self.real_out_data.JobType.values

        unique_job_types = numpy.unique(list(sim_job_types) + list(real_job_types))


        for job_type in unique_job_types:

            plt.figure(figsize=(12, 8))

            sim_job_time = (sim_exec_time)[sim_job_types == job_type] / 1000.
            real_job_time = (real_exec_time)[real_job_types == job_type] / 1000.

            min_val = min(sim_job_time.min(), real_job_time.min())
            max_val = max(sim_job_time.max(), real_job_time.max())

            bins = numpy.linspace(min_val, max_val, 101)

            plt.hist(sim_job_time,
                     bins=bins,
                     label='Simulation',
                     normed=False,
                     alpha=0.5,
                     color='b',
                     histtype='stepfilled',
                     linewidth=2)

            plt.hist(real_job_time,
                     bins=bins,
                     label='Real',
                     normed=False,
                     alpha=0.5,
                     color='r',
                     histtype='stepfilled',
                     linewidth=2)

            plt.title(job_type + ' Job Execution Time Distribution', size=20)
            plt.xlabel('Job Execution Time, ks', size=20)
            plt.ylabel('Number of jobs', size=20)
            plt.xticks(size=20)
            plt.yticks(size=20)
            plt.grid(b=1)
            plt.legend(prop={'size':15}, loc='best')
            plt.show()


    def job_wait_time(self):
        """
        Generates job wait time plot.
        Job wait time = job start execution time - job submission time
        """

        sim_wait_time = self.sim_out_data.TimestartExec.values - self.sim_out_data.Timestart.values
        sim_wait_time = numpy.log10(sim_wait_time)

        real_wait_time = self.real_out_data.StartExecTime.values.astype(numpy.float) - self.real_out_data.SubmissionTime.values.astype(numpy.float)
        real_wait_time = numpy.log10(real_wait_time)


        min_val = min(sim_wait_time.min(), real_wait_time.min())
        max_val = max(sim_wait_time.max(), real_wait_time.max())

        bins = numpy.linspace(min_val, max_val, 101)

        plt.figure(figsize=(12, 8))
        plt.hist((real_wait_time), bins=bins, histtype='stepfilled', linewidth=2, label='Real', color='r', alpha=0.5)
        plt.hist((sim_wait_time), bins=bins, histtype='stepfilled', linewidth=2, label='Simulation', color='b', alpha=0.5)

        plt.title('Job Wait Time Distribution', size=20)
        plt.xlabel('Job Wait Time, log10(s)', size=20)
        plt.ylabel('Number of jobs', size=20)
        plt.xticks(size=20)
        plt.yticks(size=20)
        plt.grid(b=1)
        plt.legend(loc='best', prop={'size':15})
        plt.show()


    def job_wait_time_by_jobtype(self):
        """
        Generates job wait time plots by the job types.
        Job wait time = job start execution time - job submission time
        """


        sim_wait_time = self.sim_out_data.TimestartExec.values - self.sim_out_data.Timestart.values
        sim_job_types = self.sim_out_data.Type.values

        real_wait_time = self.real_out_data.StartExecTime.values.astype(numpy.float) - self.real_out_data.SubmissionTime.values.astype(numpy.float)
        real_job_types = self.real_out_data.JobType.values

        unique_job_types = numpy.unique(list(sim_job_types) + list(real_job_types))



        for job_type in unique_job_types:

            plt.figure(figsize=(12, 8))

            sim_job_time = (sim_wait_time)[sim_job_types == job_type]
            sim_job_time = numpy.log10(sim_job_time)
            real_job_time = (real_wait_time)[real_job_types == job_type]
            real_job_time = numpy.log10(real_job_time)

            min_val = min(sim_job_time.min(), real_job_time.min())
            max_val = max(sim_job_time.max(), real_job_time.max())

            bins = numpy.linspace(min_val, max_val, 101)

            plt.hist(sim_job_time,
                     bins=bins,
                     label='Simulation',
                     normed=False,
                     alpha=0.5,
                     color='b',
                     histtype='stepfilled',
                     linewidth=2)

            plt.hist(real_job_time,
                     bins=bins,
                     label='Real',
                     normed=False,
                     alpha=0.5,
                     color='r',
                     histtype='stepfilled',
                     linewidth=2)

            plt.title(job_type + ' Job Wait Time Distribution', size=20)
            plt.xlabel('Job Wait Time, log10(s)', size=20)
            plt.ylabel('Number of jobs', size=20)
            plt.xticks(size=20)
            plt.yticks(size=20)
            plt.grid(b=1)
            plt.legend(prop={'size':15}, loc='best')
            plt.show()


    def job_life_time(self):
        """
        Generates job life time plot.
        Job life time = job end execution time - job submission time
        """

        sim_life_time = self.sim_out_data.TimeEndExec.values - self.sim_out_data.Timestart.values
        sim_life_time = numpy.log10(sim_life_time)
        real_life_time = self.real_out_data.EndExecTime.values.astype(numpy.float) - self.real_out_data.SubmissionTime.values.astype(numpy.float)
        real_life_time = numpy.log10(real_life_time)

        min_val = min(sim_life_time.min(), real_life_time.min())
        max_val = max(sim_life_time.max(), real_life_time.max())

        bins = numpy.linspace(min_val, max_val, 101)

        plt.figure(figsize=(12, 8))
        plt.hist((real_life_time), bins=bins, histtype='stepfilled', linewidth=2, label='Real', color='r', alpha=0.5)
        plt.hist((sim_life_time), bins=bins, histtype='stepfilled', linewidth=2, label='Simulation', color='b', alpha=0.5)

        plt.title('Job Life Time Distribution', size=20)
        plt.xlabel('Job Life Time, log10(s)', size=20)
        plt.ylabel('Number of jobs', size=20)
        plt.xticks(size=20)
        plt.yticks(size=20)
        plt.grid(b=1)
        plt.legend(loc='best', prop={'size':15})
        plt.show()


    def job_life_time_by_jobtype(self):
        """
        Generates job life time plots by the job types.
        Job life time = job end execution time - job submission time
        """


        sim_life_time = self.sim_out_data.TimeEndExec.values - self.sim_out_data.Timestart.values
        sim_job_types = self.sim_out_data.Type.values

        real_life_time = self.real_out_data.EndExecTime.values.astype(numpy.float) - self.real_out_data.SubmissionTime.values.astype(numpy.float)
        real_job_types = self.real_out_data.JobType.values

        unique_job_types = numpy.unique(list(sim_job_types) + list(real_job_types))


        for job_type in unique_job_types:

            plt.figure(figsize=(12, 8))

            sim_job_time = (sim_life_time)[sim_job_types == job_type]
            sim_job_time = numpy.log10(sim_job_time)
            real_job_time = (real_life_time)[real_job_types == job_type]
            real_job_time = numpy.log10(real_job_time)

            min_val = min(sim_job_time.min(), real_job_time.min())
            max_val = max(sim_job_time.max(), real_job_time.max())

            bins = numpy.linspace(min_val, max_val, 101)

            plt.hist(sim_job_time,
                     bins=bins,
                     label='Simulation',
                     normed=False,
                     alpha=0.5,
                     color='b',
                     histtype='stepfilled',
                     linewidth=2)

            plt.hist(real_job_time,
                     bins=bins,
                     label='Real',
                     normed=False,
                     alpha=0.5,
                     color='r',
                     histtype='stepfilled',
                     linewidth=2)

            plt.title(job_type + ' Job Life Time Distribution', size=20)
            plt.xlabel('Job Life Time, log10(s)', size=20)
            plt.ylabel('Number of jobs', size=20)
            plt.xticks(size=20)
            plt.yticks(size=20)
            plt.grid(b=1)
            plt.legend(prop={'size':15}, loc='best')
            plt.show()


    def job_cpu_time(self):
        """
        Generates job cpu time plot.
        """


        sim_cpu_time = self.sim_out_data.TimeEndExec.values - self.sim_out_data.TimestartExec.values
        sim_cpu_time = sim_cpu_time / 1000.
        real_cpu_time = self.real_out_data.TotalCPUTime.values.astype(numpy.float)
        real_cpu_time = real_cpu_time / 1000.


        min_val = min(sim_cpu_time.min(), real_cpu_time.min())
        max_val = max(sim_cpu_time.max(), real_cpu_time.max())

        bins = numpy.linspace(min_val, max_val, 101)

        plt.figure(figsize=(12, 8))
        plt.hist((real_cpu_time), bins=bins, histtype='stepfilled', linewidth=2, label='Real', color='r', alpha=0.5)
        plt.hist((sim_cpu_time), bins=bins, histtype='stepfilled', linewidth=2, label='Simulation', color='b', alpha=0.5)

        plt.title('Job CPU Time Distribution', size=20)
        plt.xlabel('Job CPU Time, ks', size=20)
        plt.ylabel('Number of jobs', size=20)
        plt.xticks(size=20)
        plt.yticks(size=20)
        plt.grid(b=1)
        plt.legend(loc='best', prop={'size':15})
        plt.show()