import matplotlib.pyplot as plt
import pickle

def plot_one_gamma(gamma):
    with open("pickles/5x5_Weigh_100000_{}.pickle".format(gamma), "rb") as f:
        data = pickle.load(f)
    with open("pickles/5x5_Disco_100000_{}.pickle".format(gamma), "rb") as f:
        disco = pickle.load(f)

    plt.plot(data, linestyle="dashed")
    plt.plot(disco)
    plt.xlabel("Episode number", fontsize=15)
    plt.ylabel("Mean squared error, Q vs Q*", fontsize=15)
    plt.savefig("plots/five_{}.png".format(gamma))

if __name__ == "__main__":
    for gamma in [0.4, 0.7, 1.0]:
        plot_one_gamma(gamma)