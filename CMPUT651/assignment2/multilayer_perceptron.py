from settings import BATCH_SIZE, HIDDEN_SIZES, LEARNING_RATE, VOCAB_SIZE
import numpy as np

EPSILON = 1e-13

class MLP():
    """Multi-layer Perceptron with sigmoid activation"""
    def __init__(self, batch_size=BATCH_SIZE, hidden_sizes=HIDDEN_SIZES, input_size=VOCAB_SIZE,
                 learning_rate=LEARNING_RATE):
        """
        Args:
            hidden_sizes - list containing the number of neurons in each hidden
        layer. This argument determines the number of such layers as well
            input_size - dimensionality of input vector space
            learning_rate - rate of learning
        """
        super().__init__()

        # Constants
        self.batch_size = batch_size
        self.learning_rate = learning_rate
        self.num_layers = len(hidden_sizes)

        # Weights and biases
        self.Ws, self.bs = [], []
        for inp, out in zip([input_size] + hidden_sizes[:-1], hidden_sizes):
            self.Ws.append(np.random.uniform(-0.5, 0.5, (inp, out)))
            self.bs.append(np.random.uniform(-0.5, 0.5, self.Ws[-1].shape[-1]))

        # Used in forward pass
        self.zs, self.ys = [0] * self.num_layers, [0] * self.num_layers

        # Used in backprop - derivatives of loss with respect to outputs at each
        # layer. Each of them is of shape (batch_size x hidden_size)
        self.dJ_dys = [0.0] * self.num_layers

    def _sigmoid(self, x):
        return 1. / (1. + np.exp(-x))

    def _sigmoid_deriv(self, x):
        return self._sigmoid(x) * (1 - self._sigmoid(x))

    def forward_pass(self, x):
        out = x
        for i, (W, b) in enumerate(zip(self.Ws, self.bs)):
            self.zs[i] = out.dot(W) + b                   #(batch_size, W[i].columns)
            out = self._sigmoid(self.zs[i])  #(batch_size, W[i].columns)
            out[np.where(out == 1.0)] = 1.0 - EPSILON
            out[np.where(out == 0.0)] = EPSILON
            self.ys[i] = out
        return out

    def fit(self, x, y):
        """ Updates weights and biases using backprop"""

        # Store temporary values during the forward pass
        self.forward_pass(x)

        # Each of them has shape (batch_size x hidden_size)
        sigmoid_derivs = [self._sigmoid_deriv(z) for z in self.zs]

        # First derivative is given by G.O.D.
        self.dJ_dys[-1] = (y[:, None] - self.ys[-1]) \
                        / (self.ys[-1] * (self.ys[-1] - 1)) # (batch_size x 1)
        # Backpropagate derivative from layer "i + 1" to "i"
        for i in range(len(self.dJ_dys) - 2, -1, -1):
            self.dJ_dys[i] = np.dot(self.dJ_dys[i + 1] * sigmoid_derivs[i + 1],
                                    self.Ws[i + 1].T) # (batch_size x 200)

        # Inputs to each layer are used in dJ/dy -> dJ/dW conversion
        inputs = [x] + self.ys[:-1]
        # Update weights and biases converting dJ/dy -> dJ/dW on the way
        for i in range(len(self.Ws)):
            self.Ws[i] -= self.learning_rate * inputs[i].T.dot(self.dJ_dys[i] \
                        * sigmoid_derivs[i]) / self.batch_size
            self.bs[i] -= self.learning_rate * np.mean(self.dJ_dys[i]
                                                     * sigmoid_derivs[i])

    def score(self, x, y):
        """ Accuracy"""
        y_pred = np.array(self.forward_pass(x) >= 0.5, dtype=np.int)
        return np.average(y_pred.reshape(-1) == y)
