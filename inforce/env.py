from random import sample

STANDARD_RANDOM_WALK = {
    "shape": (5, 5),
    "actions": {"left": [0, -1], "up": [-1, 0], "right": [0, 1], "down": [1, 0]},
    "rewards": {
        "BORDER": -1, # Reward for hitting the border: do not move the agent and
                      # give it this reward instead
        "GOAL": 100,
        "MOVE": -1, # Action to a normal cell also costs something
        "PIT": -100, # Covered with dirt, unpleasant to be in, but does not
                    # restart the agent to the origin
    },
    "agent_pos": 0,
}
STANDARD_RANDOM_WALK["goal"] = STANDARD_RANDOM_WALK["shape"][0]\
                             * STANDARD_RANDOM_WALK["shape"][1] - 1
STANDARD_RANDOM_WALK["pits"] = sample(range(1, STANDARD_RANDOM_WALK["goal"]), 8)
                                      #STANDARD_RANDOM_WALK["shape"][0])

class RandomWalk:
    """Environment provides lists of states and actions,
    rewards and the gamma factor. Probability of transition is not implemented
    so every action deterministically leads to a state.
    """

    def __init__(self, shape, goal, pits, actions, rewards, agent_pos):
        """
        Parameters
        ----------
        shape : tuple, optional
            size of the random walk board
        goal : int
            position of the goal
        pits : list of ints
            positions of the pits
        actions : dict
            allowed actions and their coordinates
        rewards : dict
            rewards per each possible cell/action
        agent_pos : int
            initial position of the agent
        """
        self.shape = shape
        self.goal = goal
        self.pits = pits
        self.actions = actions
        self.rewards = rewards # Rewards
        self.agent_pos = agent_pos # Initially at agent_pos

        self.states = list(range(shape[0] * shape[1]))

    def __repr__(self):
        cell_str = {"AGENT_POS": "^", "CELL": ".", "GOAL": "o", "PIT": "x"}
        # Normal cells
        ans = [cell_str["CELL"] for _ in self.states]
        ans[self.goal] = cell_str["GOAL"]
        for pit in self.pits:
            ans[pit] = cell_str["PIT"]
        ans[self.agent_pos] = cell_str["AGENT_POS"]

        return "\n".join(["  ".join(
            ans[i * self.shape[1]:(i + 1) * self.shape[1]])
            for i in range(self.shape[0])])

    def move_agent(self, pos):
        self.agent_pos = pos

    def move(self, A, move_agent=False):
        R = self.rewards["MOVE"]
        S = self.agent_pos + self.actions[A][0] * self.shape[1]\
          + self.actions[A][1]

        # Prohibit top action for the top row, bottom action for the bottom row,
        # left action for the left column, right action for the right column
        if S < 0 or S >= len(self.states)\
                 or (self.agent_pos % self.shape[1] == 0 and self.actions[A][1] < 0)\
                 or ((self.agent_pos + 1) % self.shape[1] == 0 and self.actions[A][1] > 0):
            R = self.rewards["BORDER"]
            S = self.agent_pos
        elif S == self.goal:
            R = self.rewards["GOAL"]
        elif S in self.pits:
            R = self.rewards["PIT"]

        if move_agent: self.move_agent(S)
        return R, S

    def done(self):
        return self.agent_pos == self.goal

if __name__ == "__main__":
    env = RandomWalk(**STANDARD_RANDOM_WALK)
    print("Start:")
    print(env)
    exit()
    for action in ["left", "down", "down", "down", "right", "up", "right",
                   "right", "right", "down"]:
        R, S = env.move(action)
        env.move_agent(S)
        print("{} ({}):".format(action, R))
        print(env)
