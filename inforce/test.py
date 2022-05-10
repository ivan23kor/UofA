import argparse
import pickle
from control import MC
from env import RandomWalk, STANDARD_RANDOM_WALK
from policy import Greedy, Soft, Random

def parse():
    parser = argparse.ArgumentParser()
    parser.add_argument("algorithm", choices=["weighted", "disc"])
    parser.add_argument("episodes", type=int)
    parser.add_argument("gamma", type=float)
    return parser.parse_args()

if __name__ == "__main__":
    args = parse()
    algorithm = args.algorithm
    episodes = args.episodes
    gamma = args.gamma

    # Init env
    params = STANDARD_RANDOM_WALK
    params["pits"] = [4, 9, 14]
    #params["pits"] = [5, 6, 11, 16, 22, 23]
    #params["pits"] = [5, 6, 11, 16, 22, 23]
    env = RandomWalk(**params)
    print(env)

    # Init policies for control
    p, b_soft, b_random = Greedy(), Soft(0.2), Random()
    # Init control
    control = MC(env, gamma)

    # Train and plot
    algorithm = "Weighted importance sampling" if args.algorithm == "weighted" else "Discounting-aware importance sampling"
    with open("{}_{}_{}.pickle".format(algorithm[:5], episodes, gamma), "wb") as f:
        res = control.eval(p, b_soft, num_avg=3,
                           episodes=episodes, algorithm=algorithm)
        pickle.dump(res, f)
