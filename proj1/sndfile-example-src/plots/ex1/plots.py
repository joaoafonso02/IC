import argparse
import matplotlib.pyplot as plt

parser = argparse.ArgumentParser()
parser.add_argument("-q", "--quantization",type=int,help="quantization of the samples")
parser.add_argument("-p", "--points",action="store_true",help="use a points graph")
parser.add_argument("-b", "--bars",action="store_true",help="use a bars graph")
args = parser.parse_args()
print(args)


# Read data from text files
with open('../../outch0.data', 'r') as mse_file:
    plt.clf()
    bigdict = dict()
    x_range = range(-32768, 32767, 1)
    for i in x_range:
        bigdict[i] = 0

    for line in mse_file:
        a = line.strip().split("\t")
        bigdict[int(a[0])] = int(a[1])


    # Create subplots
    # plt.figure(figsize=(12, 4))

    x_values = []
    y_values = []
    for i in x_range:
        if bigdict[i] != 0:
            print(i, bigdict[i])
            x_values.append(i)
            y_values.append(bigdict[i])

    # MSE plot
    # plt.subplot(131)
    if args.bars:
        plt.bar(x_values, y_values, width=(32768*2)/(pow(2,args.quantization)), edgecolor='black' )
    else:
        plt.scatter(x_values, y_values, marker='o', s=1)

    plt.xlabel('Frequency')
    plt.ylabel('count')
    plt.title('Channel 0 histogram')

    plt.tight_layout()  # Ensures the plots do not overlap

    # Save or display the plots
    plt.savefig('channel0.png')  # Save the plots to a file
    # plt.show()  # Display the plots on screen

with open('../../outch1.data', 'r') as mse_file:
    plt.clf()
    for line in mse_file:
        a = line.strip().split("\t")
        bigdict[int(a[0])] = int(a[1])


    # Create subplots
    # plt.figure(figsize=(12, 4))

    x_values = []
    y_values = []
    for i in x_range:
        if bigdict[i] != 0:
            print(i, bigdict[i])
            x_values.append(i)
            y_values.append(bigdict[i])

    # MSE plot
    # plt.subplot(131)
    if args.bars:
        plt.bar(x_values, y_values, width=(32768*2)/(pow(2,args.quantization)), edgecolor='black' )
    else:
        plt.scatter(x_values, y_values, marker='o', s=1)

    plt.xlabel('Frequency')
    plt.ylabel('count')
    plt.title('Channel 1 histogram')

    plt.tight_layout()  # Ensures the plots do not overlap

    # Save or display the plots
    plt.savefig('channel1.png')  # Save the plots to a file
    # plt.show()  # Display the plots on screen

with open('../../outmono.data', 'r') as mse_file:
    plt.clf()
    for line in mse_file:
        a = line.strip().split("\t")
        bigdict[int(a[0])] = int(a[1])


    # Create subplots
    # plt.figure(figsize=(12, 4))

    x_values = []
    y_values = []
    for i in x_range:
        if bigdict[i] != 0:
            print(i, bigdict[i])
            x_values.append(i)
            y_values.append(bigdict[i])

    # MSE plot
    # plt.subplot(131)
    if args.bars:
        plt.bar(x_values, y_values, width=(32768*2)/(pow(2,args.quantization)), edgecolor='black' )
    else:
        plt.scatter(x_values, y_values, marker='o', s=1)

    plt.xlabel('Frequency')
    plt.ylabel('count')
    plt.title('Mono Channel histogram')

    plt.tight_layout()  # Ensures the plots do not overlap

    # Save or display the plots
    plt.savefig('mono.png')  # Save the plots to a file
    # plt.show()  # Display the plots on screen

with open('../../outdiff.data', 'r') as mse_file:
    plt.clf()
    for line in mse_file:
        a = line.strip().split("\t")
        bigdict[int(a[0])] = int(a[1])


    # Create subplots
    # plt.figure(figsize=(12, 4))

    x_values = []
    y_values = []
    for i in x_range:
        if bigdict[i] != 0:
            print(i, bigdict[i])
            x_values.append(i)
            y_values.append(bigdict[i])

    # MSE plot
    # plt.subplot(131)
    if args.bars:
        plt.bar(x_values, y_values, width=(32768*2)/(pow(2,args.quantization)), edgecolor='black' )
    else:
        plt.scatter(x_values, y_values, marker='o', s=1)

    plt.xlabel('Frequency')
    plt.ylabel('count')
    plt.title('Difference Channel histogram')

    plt.tight_layout()  # Ensures the plots do not overlap

    # Save or display the plots
    plt.savefig('diff.png')  # Save the plots to a file
    # plt.show()  # Display the plots on screen
