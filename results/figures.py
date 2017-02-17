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


    def _base_plotter(self, sim_array, real_array, sim_object_names, real_object_names, title, xlabel, ylabel):
        """
        Base plotter for other plots.

        Parameters
        ----------
        sim_array : array-like
            Simulation data to plot.
        real_array : array-like
            Real data to plot.
        sim_object_names : array-like
            List of object names. Simulation data will be split between these objects and plotted separately.
            If None, the data will not be split.
        real_object_names : array-like
            List of object names. Real data will be split between these objects and plotted separately.
            If None, the data will not be split.
        title : string
            Title of the figure.
        xlabel : string
            Label of X-axis of the figure.
        ylabel : string
            Label of Y-axis of the figure.
        """

        if sim_object_names == None or real_object_names == None:

            min_val = min(sim_array.min(), real_array.min())
            max_val = max(sim_array.max(), real_array.max())

            bins = numpy.linspace(min_val, max_val, 101)

            plt.figure(figsize=(12, 8))
            plt.hist((real_array), bins=bins, histtype='stepfilled', linewidth=2, label='Real', color='r', alpha=0.5)
            plt.hist((sim_array), bins=bins, histtype='stepfilled', linewidth=2, label='Simulation', color='b', alpha=0.5)

            plt.title(title, size=20)
            plt.xlabel(xlabel, size=20)
            plt.ylabel(ylabel, size=20)
            plt.xticks(size=20)
            plt.yticks(size=20)
            plt.grid(b=1)
            plt.legend(loc='best', prop={'size':15})
            plt.show()


        else:

            unique_object_names = numpy.unique(list(sim_object_names) + list(real_object_names))

            for one_object in unique_object_names:

                plt.figure(figsize=(12, 8))

                sim_obj_array = (sim_array)[sim_object_names == one_object]
                real_obj_array = (real_array)[real_object_names == one_object]

                min_val = min(sim_obj_array.min(), real_obj_array.min())
                max_val = max(sim_obj_array.max(), real_obj_array.max())

                bins = numpy.linspace(min_val, max_val, 101)

                plt.hist(real_obj_array,
                         bins=bins,
                         label='Real',
                         normed=False,
                         alpha=0.5,
                         color='r',
                         histtype='stepfilled',
                         linewidth=2)

                plt.hist(sim_obj_array,
                         bins=bins,
                         label='Simulation',
                         normed=False,
                         alpha=0.5,
                         color='b',
                         histtype='stepfilled',
                         linewidth=2)

                plt.title(one_object + ' ' + title, size=20)
                plt.xlabel(xlabel, size=20)
                plt.ylabel(ylabel, size=20)
                plt.xticks(size=20)
                plt.yticks(size=20)
                plt.grid(b=1)
                plt.legend(prop={'size':15}, loc='best')
                plt.show()


    def job_exec_time(self):
        """
        Generates job execution time plot.
        Job execution time = job end execution time - job start execution time
        """

        sim_exec_time = self.sim_out_data.TimeEndExec.values - self.sim_out_data.TimestartExec.values
        sim_exec_time = sim_exec_time / 1000.
        real_exec_time = self.real_out_data.EndExecTime.values.astype(numpy.float) - self.real_out_data.StartExecTime.values.astype(numpy.float)
        real_exec_time = real_exec_time / 1000.

        self._base_plotter(sim_array=sim_exec_time,
                           real_array=real_exec_time,
                           sim_object_names=None,
                           real_object_names=None,
                           title='Job Execution Time Distribution',
                           xlabel='Job Execution Time, ks',
                           ylabel='Number of jobs')


    def job_exec_time_by_jobtype(self):
        """
        Generates job execution time plots by the job types.
        Job execution time = job end execution time - job start execution time
        """

        sim_exec_time = self.sim_out_data.TimeEndExec.values - self.sim_out_data.TimestartExec.values
        sim_exec_time = sim_exec_time / 1000.
        sim_job_types = self.sim_out_data.Type.values

        real_exec_time = self.real_out_data.EndExecTime.values.astype(numpy.float) - self.real_out_data.StartExecTime.values.astype(numpy.float)
        real_exec_time = real_exec_time / 1000.
        real_job_types = self.real_out_data.JobType.values


        self._base_plotter(sim_array=sim_exec_time,
                           real_array=real_exec_time,
                           sim_object_names=sim_job_types,
                           real_object_names=real_job_types,
                           title='Job Execution Time Distribution',
                           xlabel='Job Execution Time, ks',
                           ylabel='Number of jobs')


    def job_wait_time(self):
        """
        Generates job wait time plot.
        Job wait time = job start execution time - job submission time
        """

        sim_wait_time = self.sim_out_data.TimestartExec.values - self.sim_out_data.Timestart.values
        sim_wait_time = numpy.log10(sim_wait_time)

        real_wait_time = self.real_out_data.StartExecTime.values.astype(numpy.float) - self.real_out_data.SubmissionTime.values.astype(numpy.float)
        real_wait_time = numpy.log10(real_wait_time)

        self._base_plotter(sim_array=sim_wait_time,
                           real_array=real_wait_time,
                           sim_object_names=None,
                           real_object_names=None,
                           title='Job Wait Time Distribution',
                           xlabel='Job Wait Time, log10(s)',
                           ylabel='Number of jobs')


    def job_wait_time_by_jobtype(self):
        """
        Generates job wait time plots by the job types.
        Job wait time = job start execution time - job submission time
        """


        sim_wait_time = self.sim_out_data.TimestartExec.values - self.sim_out_data.Timestart.values
        sim_wait_time = numpy.log10(sim_wait_time)
        sim_job_types = self.sim_out_data.Type.values

        real_wait_time = self.real_out_data.StartExecTime.values.astype(numpy.float) - self.real_out_data.SubmissionTime.values.astype(numpy.float)
        real_wait_time = numpy.log10(real_wait_time)
        real_job_types = self.real_out_data.JobType.values


        self._base_plotter(sim_array=sim_wait_time,
                           real_array=real_wait_time,
                           sim_object_names=sim_job_types,
                           real_object_names=real_job_types,
                           title='Job Wait Time Distribution',
                           xlabel='Job Wait Time, log10(s)',
                           ylabel='Number of jobs')


    def job_life_time(self):
        """
        Generates job life time plot.
        Job life time = job end execution time - job submission time
        """

        sim_life_time = self.sim_out_data.TimeEndExec.values - self.sim_out_data.Timestart.values
        sim_life_time = numpy.log10(sim_life_time)
        real_life_time = self.real_out_data.EndExecTime.values.astype(numpy.float) - self.real_out_data.SubmissionTime.values.astype(numpy.float)
        real_life_time = numpy.log10(real_life_time)


        self._base_plotter(sim_array=sim_life_time,
                           real_array=real_life_time,
                           sim_object_names=None,
                           real_object_names=None,
                           title='Job Life Time Distribution',
                           xlabel='Job Life Time, log10(s)',
                           ylabel='Number of jobs')


    def job_life_time_by_jobtype(self):
        """
        Generates job life time plots by the job types.
        Job life time = job end execution time - job submission time
        """


        sim_life_time = self.sim_out_data.TimeEndExec.values - self.sim_out_data.Timestart.values
        sim_life_time = numpy.log10(sim_life_time)
        sim_job_types = self.sim_out_data.Type.values

        real_life_time = self.real_out_data.EndExecTime.values.astype(numpy.float) - self.real_out_data.SubmissionTime.values.astype(numpy.float)
        real_life_time = numpy.log10(real_life_time)
        real_job_types = self.real_out_data.JobType.values

        self._base_plotter(sim_array=sim_life_time,
                           real_array=real_life_time,
                           sim_object_names=sim_job_types,
                           real_object_names=real_job_types,
                           title='Job Life Time Distribution',
                           xlabel='Job Life Time, log10(s)',
                           ylabel='Number of jobs')


    def job_cpu_time(self):
        """
        Generates job cpu time plot.
        """

        sim_cpu_time = self.sim_out_data.TimeEndExec.values - self.sim_out_data.TimestartExec.values
        sim_cpu_time = sim_cpu_time / 1000.
        real_cpu_time = self.real_out_data.TotalCPUTime.values.astype(numpy.float)
        real_cpu_time = real_cpu_time / 1000.

        self._base_plotter(sim_array=sim_cpu_time,
                           real_array=real_cpu_time,
                           sim_object_names=None,
                           real_object_names=None,
                           title='Job CPU Time Distribution',
                           xlabel='Job CPU Time, ks',
                           ylabel='Number of jobs')


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


    def _base_plotter(self, object_names, variable_name, labels, title, xlabel, ylabel, var_miltiplier, cumulative=True):
        """
        Base plotter for other plots.

        Parameters
        ----------
        object_names : array-like
            List of object names for which curves will be plotted.
        variable_name : string
            Name of a variable which will be Y-axis of the plot. X-axis is time.
        labels : array-like
            List of labels for the objects.
        title : string
            Title of the figure.
        xlabel : string
            Label of X-axis of the figure.
        ylabel : string
            Label of Y-axis of the figure.
        var_miltiplier :
            Multiplier of the plotted variable.
        cumulative : boolean
            If True, cumulative plot will be generated.
        """

        if len(object_names) == 0:
            print "No data to plot."
            return

        object_variable_arr = []

        time_min = 0
        time_max = self.sim_trace_data.icol(4).values.max()


        for one_object in object_names:

            object_data = self.sim_trace_data[(self.sim_trace_data.Object.values == one_object)*\
                                            (self.sim_trace_data.Variable.values == variable_name)]

            object_time = object_data.EndObservTime.values
            object_variable = object_data.VarValue.values * var_miltiplier

            time_bins, variable_bins, bin_size = binned_max(object_time, object_variable, 100, time_min, time_max)
            time_bins = time_bins / 86400.# days


            if cumulative:
                object_variable_arr.append(variable_bins)
            else:
                variable_bins_diff = 1. * rolling_diff(variable_bins, start_val='auto') / bin_size
                object_variable_arr.append(variable_bins_diff)


        colors = cm.Set1(numpy.linspace(0, 1, len(object_names)))

        if labels == 'None':
            labels = object_names

        plt.figure(num=None, figsize=(12, 8), dpi=300, facecolor='w', edgecolor='k')
        plt.stackplot(time_bins, object_variable_arr, alpha=1, colors=colors, edgecolor='0')
        plt.title(title, fontsize=25)
        plt.xlabel(xlabel, fontsize=25)
        plt.ylabel(ylabel, fontsize=25)
        plt.xlim(time_bins.min(), time_bins.max())
        plt.xticks(fontsize=25)
        plt.yticks(fontsize=25)
        plt.grid(linewidth=1)
        plt.legend(labels, prop={'size':16}, framealpha=0.5, ncol=4, loc='upper center', bbox_to_anchor=(0.5, -0.15),
                  fancybox=True)
        plt.show()

    def lhcopn_cumulative_data_transfer(self):
        """
        Generates LHCOPN cumulative data transfer plots by links.
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

        self._base_plotter(object_names = tier_0_1_links,
                           variable_name='traffic',
                           labels = tier_0_1_links_short,
                           title = "LHCOPN Cumulative Data Transfer by Link",
                           xlabel = 'Time, days',
                           ylabel = 'TB',
                           var_miltiplier = 1. / 10.**12,
                           cumulative=True)

    def lhcopn_data_transfer_rate(self):
        """
        Generates LHCOPN data transfer rate plots by links.
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

        self._base_plotter(object_names = tier_0_1_links,
                           variable_name='traffic',
                           labels = tier_0_1_links_short,
                           title = "LHCOPN Data Transfer Rate by Link",
                           xlabel = 'Time, days',
                           ylabel = 'GB / s',
                           var_miltiplier = 1. / 10.**9,
                           cumulative=False)

    def total_cumulative_data_transfer(self):
        """
        Generates total cumulative data transfer.
        """

        variable_name='traffic'
        object_names = numpy.unique(self.sim_trace_data[self.sim_trace_data.Variable == variable_name].Object.values)

        self._base_plotter(object_names=object_names,
                           variable_name=variable_name,
                           labels = 'None',
                           title = "Cumulative Data Transfer by Link",
                           xlabel = 'Time, days',
                           ylabel = 'TB',
                           var_miltiplier = 1. / 10.**12,
                           cumulative=True)

    def input_cumulative_data_transfer(self):
        """
        Generates input cumulative data transfer.
        """

        variable_name='inputData'
        object_names = numpy.unique(self.sim_trace_data[self.sim_trace_data.Variable == variable_name].Object.values)

        self._base_plotter(object_names=object_names,
                           variable_name=variable_name,
                           labels = 'None',
                           title = "Input Cumulative Data Transfer by Link",
                           xlabel = 'Time, days',
                           ylabel = 'TB',
                           var_miltiplier = 1. / 10.**12,
                           cumulative=True)

    def output_cumulative_data_transfer(self):
        """
        Generates output cumulative data transfer.
        """

        variable_name='outputData'
        object_names = numpy.unique(self.sim_trace_data[self.sim_trace_data.Variable == variable_name].Object.values)

        self._base_plotter(object_names=object_names,
                           variable_name=variable_name,
                           labels = 'None',
                           title = "Output Cumulative Data Transfer by Link",
                           xlabel = 'Time, days',
                           ylabel = 'TB',
                           var_miltiplier = 1. / 10.**12,
                           cumulative=True)

    def number_of_datasets_ondisk(self):
        """
        Generates number of datasets on disk.
        """

        variable_name='datasetOnDisk'
        object_names = numpy.unique(self.sim_trace_data[self.sim_trace_data.Variable == variable_name].Object.values)

        self._base_plotter(object_names=object_names,
                           variable_name=variable_name,
                           labels = 'None',
                           title = "Number of Datasets on Disk",
                           xlabel = 'Time, days',
                           ylabel = 'Number',
                           var_miltiplier = 1.,
                           cumulative=True)

    def number_of_datasets_ontape(self):
        """
        Generates number of datasets on tape.
        """

        variable_name='datasetOnTape'
        object_names = numpy.unique(self.sim_trace_data[self.sim_trace_data.Variable == variable_name].Object.values)

        self._base_plotter(object_names=object_names,
                           variable_name=variable_name,
                           labels = 'None',
                           title = "Number of Datasets on Tape",
                           xlabel = 'Time, days',
                           ylabel = 'Number',
                           var_miltiplier = 1.,
                           cumulative=True)