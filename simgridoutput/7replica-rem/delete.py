from IPython.utils import coloransi

import numpy as np
from matplotlib import pyplot as plt


def find_nearest_index(array, value):
    idx = (np.abs(array-value)).argmin()
    return idx


def create_correct_array(dataset_amount, time_array,  end_time):
    time_interval = np.arange(0, end_time, 500)
    true_array = np.array([])
    for i in time_interval:
        index = find_nearest_index(time_array, i)
        true_array = np.append(true_array, dataset_amount[index])

    return true_array



def number_of_datasets(filename1, filename2):
    TIERS = ["CERN", "CNAF", "IN2P3", "RRCKI", "PIC", "RAL", "GRIDKA", "SARA"]
    rtype = np.dtype([('var_name', np.str, 35), ('variable', np.str, 35), ('clock', np.float32),  ('value', np.float32)])
    
    tier_names, variable, timex, number_dset = np.loadtxt(filename1, delimiter=", ", usecols=(1,2,3,6), unpack=True, dtype=rtype)
    tier_names_DAM, variable_DAM, timex_DAM, number_dset_DAM = np.loadtxt(filename2, delimiter=", ", usecols=(1,2,3,6), unpack=True, dtype=rtype)
    
    fig = plt.figure(num=None, figsize=(15, 10), dpi=80, facecolor='w', edgecolor='k')
    fig.suptitle("Number of datasets", fontsize=25)
    
    # FIRST SUBPLOT;  SIMPLE ALGORITHM, DISK SPACE
    cm = plt.get_cmap('gnuplot')
    ax1 = fig.add_subplot(221)
    ax1.set_title("Dataset number on disk space by simple algorithm", fontsize=16)
    ax1.set_color_cycle([cm(1. * i / len(TIERS)) for i in range(len(TIERS))])
    x = []

    time_interval = np.arange(0, timex[-1], 500)

    for i, tier in enumerate(TIERS):
        indexes = np.where(np.logical_and(tier_names == tier, variable == "datasetOnDisk"))
        true_dataset_amount = create_correct_array(number_dset[indexes], timex[indexes], timex[-1])
        x.append(true_dataset_amount)
    stack_1 = ax1.stackplot(time_interval / 86400, x, edgecolor='none')
    plt.setp(ax1.get_xticklabels(), fontsize=14)
    plt.setp(ax1.get_yticklabels(), fontsize=14)
    plt.legend(loc="best", prop={'size':10}, bbox_to_anchor=[0, 1], shadow=True, title="Legend", fancybox=True)
    
    # SECOND SUBPLOT; SIMPLE ALGORITHM, TAPE SPACE
    x = []
    ax2 = fig.add_subplot(222)
    ax2.set_color_cycle([cm(1. * i / len(TIERS)) for i in range(len(TIERS))])
    ax2.set_title("Dataset number on tape by simple algorithm", fontsize=16)
    for i, tier in enumerate(TIERS):
        indexes = np.where(np.logical_and(tier_names == tier, variable == "datasetOnTape"))
        true_dataset_amount = create_correct_array(number_dset[indexes], timex[indexes], timex[-1])
        x.append(true_dataset_amount)
    ax2.stackplot(time_interval / 86400, x, edgecolor='none')
    plt.setp(ax2.get_xticklabels(), fontsize=14)
    plt.setp(ax2.get_yticklabels(), fontsize=14)
    plt.legend(loc="best", prop={'size':10}, bbox_to_anchor=[1, 1],  shadow=True, title="Legend", fancybox=True)
    
    # THIRD SUBPLOT; DAM ALGORITHM, DISK SPACE
    time_interval = np.arange(0, timex_DAM[-1], 500)
    x = []
    ax3 = fig.add_subplot(223, sharex=ax1, sharey=ax1)
    ax3.set_color_cycle([cm(1. * i / len(TIERS)) for i in range(len(TIERS))])

    for i, tier in enumerate(TIERS):
        indexes = np.where(np.logical_and(tier_names_DAM == tier, variable_DAM == "datasetOnDisk"))
        true_dataset_amount = create_correct_array(number_dset_DAM[indexes], timex_DAM[indexes], timex_DAM[-1])
        x.append(true_dataset_amount)

    ax3.stackplot(time_interval / 86400, x, edgecolor='none')
    ax3.set_title("Dataset number on disk by DAM algorithm", fontsize=16)
    plt.setp(ax3.get_xticklabels(), fontsize=14)
    plt.setp(ax3.get_yticklabels(), fontsize=14)
    
    # FOURTH SUBPLOT; DAM ALGORITHM, TAPE SPACE
    x = []
    ax4 = fig.add_subplot(224, sharex=ax2, sharey=ax2)
    ax4.set_color_cycle([cm(1. * i / len(TIERS)) for i in range(len(TIERS))])
    for i, tier in enumerate(TIERS):
        indexes = np.where(np.logical_and(tier_names_DAM == tier, variable_DAM == "datasetOnTape"))
        true_dataset_amount = create_correct_array(number_dset_DAM[indexes], timex_DAM[indexes], timex_DAM[-1])
        x.append(true_dataset_amount)
    ax4.stackplot(time_interval / 86400, x, edgecolor='none')
    ax4.set_title("Dataset number on tape by DAM algorithm", fontsize=16)
    plt.setp(ax4.get_xticklabels(), fontsize=14)
    plt.setp(ax4.get_yticklabels(), fontsize=14)
    
    fig.text(0.06, 0.5, 'Number of datasets', ha='center', va='center', rotation='vertical', fontsize=22)
    fig.text(0.5, 0.04, 'Time, days', ha='center', va='center', fontsize=22)
    fig.legend(stack_1, TIERS, 'upper right')
    
    plt.savefig("dataset.png")


def cumulative_input_and_output_per_site(filename1, filename2):
    TIERS = ["CERN", "CNAF", "IN2P3", "RRCKI", "PIC", "RAL", "GRIDKA", "SARA"]
    rtype = np.dtype([('var_name', np.str, 35), ('variable', np.str, 35), ('clock', np.float32), ('value', np.float32)])

    tier_names, variable, timex, traffic = np.loadtxt(filename1, delimiter=", ", usecols=(1, 2, 3, 6), unpack=True,
                                                          dtype=rtype)
    tier_names_DAM, variable_DAM, timex_DAM, traffic_DAM = np.loadtxt(filename2, delimiter=", ",
                                                                          usecols=(1, 2, 3, 6), unpack=True,
                                                                          dtype=rtype)

    fig = plt.figure(num=None, figsize=(15, 10), dpi=80, facecolor='w', edgecolor='k')
    fig.suptitle("Cumulative input and output traffic per site", fontsize=25)

    # FIRST SUBPLOT;  SIMPLE ALGORITHM, DISK SPACE
    cm = plt.get_cmap('inferno')
    ax1 = fig.add_subplot(221)
    ax1.set_title("Input by simple algorithm", fontsize=16)
    ax1.set_color_cycle([cm(1. * i / len(TIERS)) for i in range(len(TIERS))])
    x = []

    time_interval = np.arange(0, timex[-1], 500)

    for i, tier in enumerate(TIERS):
        indexes = np.where(np.logical_and(tier_names == tier, variable == "inputData"))
        true_dataset_amount = create_correct_array(traffic[indexes], timex[indexes], timex[-1]) / 10**12
        x.append(true_dataset_amount)
    stack_1 = ax1.stackplot(time_interval / 86400, x, edgecolor='none')
    plt.setp(ax1.get_xticklabels(), fontsize=14)
    plt.setp(ax1.get_yticklabels(), fontsize=14)
    plt.legend(loc="best", prop={'size': 10}, bbox_to_anchor=[0, 1], shadow=True, title="Legend", fancybox=True)

    # SECOND SUBPLOT; SIMPLE ALGORITHM, TAPE SPACE
    x = []
    ax2 = fig.add_subplot(222)
    ax2.set_color_cycle([cm(1. * i / len(TIERS)) for i in range(len(TIERS))])
    ax2.set_title("Output simple algorithm", fontsize=16)
    for i, tier in enumerate(TIERS):
        indexes = np.where(np.logical_and(tier_names == tier, variable == "outputData"))
        true_dataset_amount = create_correct_array(traffic[indexes], timex[indexes], timex[-1]) / 10**12
        x.append(true_dataset_amount)
    ax2.stackplot(time_interval / 86400, x, edgecolor='none')
    plt.setp(ax2.get_xticklabels(), fontsize=14)
    plt.setp(ax2.get_yticklabels(), fontsize=14)
    plt.legend(loc="best", prop={'size': 10}, bbox_to_anchor=[1, 1], shadow=True, title="Legend", fancybox=True)

    # THIRD SUBPLOT; DAM ALGORITHM, DISK SPACE
    time_interval = np.arange(0, timex_DAM[-1], 500)
    x = []
    ax3 = fig.add_subplot(223, sharex=ax1, sharey=ax1)
    ax3.set_color_cycle([cm(1. * i / len(TIERS)) for i in range(len(TIERS))])

    for i, tier in enumerate(TIERS):
        indexes = np.where(np.logical_and(tier_names_DAM == tier, variable_DAM == "inputData"))
        true_dataset_amount = create_correct_array(traffic_DAM[indexes], timex_DAM[indexes], timex_DAM[-1]) / 10**12
        x.append(true_dataset_amount)

    ax3.stackplot(time_interval / 86400, x, edgecolor='none')
    ax3.set_title("Input by DAM algorithm", fontsize=16)
    plt.setp(ax3.get_xticklabels(), fontsize=14)
    plt.setp(ax3.get_yticklabels(), fontsize=14)

    # FOURTH SUBPLOT; DAM ALGORITHM, TAPE SPACE
    x = []
    ax4 = fig.add_subplot(224, sharex=ax2, sharey=ax2)
    ax4.set_color_cycle([cm(1. * i / len(TIERS)) for i in range(len(TIERS))])
    for i, tier in enumerate(TIERS):
        indexes = np.where(np.logical_and(tier_names_DAM == tier, variable_DAM == "outputData"))
        true_dataset_amount = create_correct_array(traffic_DAM[indexes], timex_DAM[indexes], timex_DAM[-1]) / 10**12
        x.append(true_dataset_amount)
    ax4.stackplot(time_interval / 86400, x, edgecolor='none')
    ax4.set_title("Output by DAM algorithm", fontsize=16)
    plt.setp(ax4.get_xticklabels(), fontsize=14)
    plt.setp(ax4.get_yticklabels(), fontsize=14)

    fig.text(0.06, 0.5, 'Traffic, TB', ha='center', va='center', rotation='vertical', fontsize=22)
    fig.text(0.5, 0.04, 'Time, days', ha='center', va='center', fontsize=22)
    fig.legend(stack_1, TIERS, 'upper right')

    plt.savefig("ciaops.png")


if __name__ == '__main__':
    number_of_datasets("trace.csv", "trace.csv")
    cumulative_input_and_output_per_site("trace.csv", "trace.csv")

