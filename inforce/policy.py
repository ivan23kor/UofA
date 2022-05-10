from random import choice, random

Q = {""}

class Soft():
    """Epsilon-greedy policy """

    def __init__(self, epsilon=0):
        """
        Parameters
        ----------
        actions : dict
            allowed actions
        states : list
            list of possible states
        epsilon : float
            number within [0, 1] specifying percentage of time the
            action shouild be taken randomly
        """

        self.epsilon = epsilon

    def decide(self, Q_S):
        """Choose action according to the soft policy"""
        if random() <= self.epsilon:
            return (choice(list(Q_S.keys())), self.epsilon / len(Q_S))
        else:
            return (max(Q_S, key=Q_S.get), 1 - self.epsilon +
                                           self.epsilon / len(Q_S))

    def __str__(self):
        return "{}-greedy policy with value function:\n{}".format(self.epsilon,
                                                                  self.Q)

class FixedSoft(Soft):
    def __init__(self, epsilon):
        super().__setattr__('epsilon', epsilon)

    def __setattr__(self, name, value):
        raise ValueError("Can't change '{}' in an instance of {}".format(name,
            self.__class__.__name__))

class Greedy(FixedSoft):
    """Greedy policy, 0% random actions"""
    def __init__(self):
        super().__init__(0.0)

class Random(FixedSoft):
    """Random policy, 100% random actions"""
    def __init__(self):
        super().__init__(1.0)

if __name__ == "__main__":
    Q = {0: {"left": 1.0, "right": 0.0}, 1: {"left": 0.0, "right": 1.0}}
    p = Soft(0.2)
    r = Random()

    def mini_test(p, s, n=10):
        print("For state {}:".format(s), end=" ")
        for _ in range(10):
            print(p.decide(Q[s]), end=", ")
        print()

    print("soft:")
    mini_test(p, 0.)
    mini_test(p, 1.)
    print("random:")
    mini_test(r, 0.)
    mini_test(r, 1.)