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


############################################ Traffic Plots #############################################################

from matplotlib.pyplot import cm

def binned_max(x, y, bins, x_min, x_max, transfer_value=True):
    """
    Retuns y-max values for x-bins.

    Parameters
    ----------
    x : array_like
        This array is divided into bins.
    y : array_like
        This array elements are divided between the bins.
    bins : int
        Number of bins.
    x_min : float
        Min edge of the bins.
    x_max : float
        Max edge of the bins.
    transfer_value : boolean
        If true, value of the previous bin is transfered to the next bin when it empty.

    Return
    ------

    x_bins : array_like
        Binned x array.
    y_bins : array_like
        Max y value for the each bin.
    bin_size : float
        The bin size.
    """

    step = 1. * ( x_max - x_min ) / bins
    edges = [x_min + i * step for i in range(0, bins+1)]

    y_bins = []
    x_bins = []

    for i in range(0, len(edges)-1):

        left = edges[i]
        right = edges[i+1]

        if i == len(edges)-2:
            y_bin = y[(x >= left) * (x <= right)]
        else:
            y_bin = y[(x >= left) * (x < right)]


        if len(y_bin) == 0 and i == 0 and transfer_value:
                y_bins.append(0)
        elif len(y_bin) == 0 and i != 0 and transfer_value:
                y_bins.append(y_bins[-1])
        elif len(y_bin) == 0 and not transfer_value:
            y_bins.append(0)
        else:
            y_bins.append(y_bin.max())

        x_bins.append(0.5*(left + right))

    return numpy.array(x_bins), numpy.array(y_bins), step


def rolling_diff(array, start_val='auto'):
    """
    Compute difference between two neighbor elements of an array.

    Parameters
    ----------
    array : array_like
        1-d array.
    start_val : float
        Start value of the returned array. If 'auto', the start value is calculated automatically.

    Return
    ------
    new_array : array-like
        New array where each element is difference of two elements of the original array.
    """

    if start_val == 'auto':
        new_array = [array[0]]
    else:
        new_array = [start_val]

    for i in range(1, len(array)):
        new_array.append(array[i] - array[i-1])

    return numpy.array(new_array)


class TrafficPlots(object):

    def __init__(self, sim_trace_data):
        """
        Generates data transfer plots.

        Parameters
        ----------

        sim_trace_data : pandas.DataFrame
            Simulation trace data.
        """

        self.sim_trace_data = sim_trace_data

    def lhcopn_cumulative_data_transfer(self):
        """
        Generates LHCOPN cumulative data transfer plots by links.
        :return:
        """


        tier_0_1_links = ["CERN-CNAF-LHCOPN-001",
                          "CERN-GRIDKA-LHCOPN-001",
                          "CERN-IN2P3-LHCOPN-001",
                          "CERN-PIC-LHCOPN-001",
                          "CERN-RAL-LHCOPN-001",
                          "CERN-SARA-LHCOPN-001",
                          "CERN-RRCK1-LHCOPN-001",
                          "CNAF-GRIDKA-LHCOPN-001",
                          "GRIDKA-IN2P3-LHCOPN-001",
                          "GRIDKA-SARA-LHCOPN-001"]

        tier_0_1_links_short = ["CERN-CNAF",
                                "CERN-GRIDKA",
                                "CERN-IN2P3",
                                "CERN-PIC",
                                "CERN-RAL",
                                "CERN-SARA",
                                "CERN-RRCK1",
                                "CNAF-GRIDKA",
                                "GRIDKA-IN2P3",
                                "GRIDKA-SARA"]

        variable_name = 'traffic'

        link_traffic_arr = []

        time_min = 0
        time_max = self.sim_trace_data.icol(4).values.max()


        for link in tier_0_1_links:

            link_data = self.sim_trace_data[(self.sim_trace_data.Object.values == link)*\
                                            (self.sim_trace_data.Variable.values == variable_name)]

            link_time = link_data.EndObservTime.values
            link_traffic = link_data.VarValue.values / 10.**12 # TB

            time_bins, traffic_bins, _ = binned_max(link_time, link_traffic, 100, time_min, time_max)
            time_bins = time_bins / 86400.# days

            link_traffic_arr.append(traffic_bins)



        colors = cm.Set1(numpy.linspace(0, 1, len(tier_0_1_links)))
        labels = tier_0_1_links_short

        plt.figure(num=None, figsize=(12, 8), dpi=300, facecolor='w', edgecolor='k')
        plt.stackplot(time_bins, link_traffic_arr, alpha=1, colors=colors, edgecolor='0')
        plt.title("LHCOPN Cumulative Data Transfer by Link", fontsize=25)
        plt.xlabel('Time, days', fontsize=25)
        plt.ylabel('TB', fontsize=25)
        plt.xlim(time_bins.min(), time_bins.max())
        plt.xticks(fontsize=25)
        plt.yticks(fontsize=25)
        plt.grid(linewidth=1)
        plt.legend(labels, prop={'size':16}, framealpha=0.5, ncol=4, loc='upper center', bbox_to_anchor=(0.5, -0.15),
                  fancybox=True)
        plt.show()

    def lhcopn_data_transfer_rate(self):
        """
        Generates LHCOPN data transfer rate plots by links.
        :return:
        """

        tier_0_1_links = ["CERN-CNAF-LHCOPN-001",
                          "CERN-GRIDKA-LHCOPN-001",
                          "CERN-IN2P3-LHCOPN-001",
                          "CERN-PIC-LHCOPN-001",
                          "CERN-RAL-LHCOPN-001",
                          "CERN-SARA-LHCOPN-001",
                          "CERN-RRCK1-LHCOPN-001",
                          "CNAF-GRIDKA-LHCOPN-001",
                          "GRIDKA-IN2P3-LHCOPN-001",
                          "GRIDKA-SARA-LHCOPN-001"]

        tier_0_1_links_short = ["CERN-CNAF",
                                "CERN-GRIDKA",
                                "CERN-IN2P3",
                                "CERN-PIC",
                                "CERN-RAL",
                                "CERN-SARA",
                                "CERN-RRCK1",
                                "CNAF-GRIDKA",
                                "GRIDKA-IN2P3",
                                "GRIDKA-SARA"]

        variable_name = 'traffic'

        link_traffic_arr = []

        time_min = 0
        time_max = self.sim_trace_data.icol(4).values.max()


        for link in tier_0_1_links:

            link_data = self.sim_trace_data[(self.sim_trace_data.Object.values == link)*\
                                            (self.sim_trace_data.Variable.values == variable_name)]

            link_time = link_data.EndObservTime.values
            link_traffic = link_data.VarValue.values / 10.**9 # TB

            time_bins, traffic_bins, bin_size = binned_max(link_time, link_traffic, 100, time_min, time_max)
            time_bins = time_bins / 86400.# days

            traffic_bins_diff = 1. * rolling_diff(traffic_bins, start_val='auto') / bin_size

            link_traffic_arr.append(traffic_bins_diff)



        colors = cm.Set1(numpy.linspace(0, 1, len(tier_0_1_links)))
        labels = tier_0_1_links_short

        plt.figure(num=None, figsize=(12, 8), dpi=300, facecolor='w', edgecolor='k')
        plt.stackplot(time_bins, link_traffic_arr, alpha=1, colors=colors, edgecolor='0')
        plt.title("LHCOPN Data Transfer Rate by Link", fontsize=25)
        plt.xlabel('Time, days', fontsize=25)
        plt.ylabel('GB / s', fontsize=25)
        plt.xlim(time_bins.min(), time_bins.max())
        plt.xticks(fontsize=25)
        plt.yticks(fontsize=25)
        plt.grid(linewidth=1)
        plt.legend(labels, prop={'size':16}, framealpha=0.5, ncol=4, loc='upper center', bbox_to_anchor=(0.5, -0.15),
                  fancybox=True)
        plt.show()