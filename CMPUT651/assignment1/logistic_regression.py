from settings import LEARNING_RATE, N_OUTPUTS, VOCAB_SIZE
import numpy as np

class LogisticRegression(object):
    """docstring for LogisticRegression"""
    def __init__(self, vocab_size=VOCAB_SIZE):
        super().__init__()
        self.W = np.random.rand(vocab_size, N_OUTPUTS) - 0.5
        self.b = np.random.rand(1, N_OUTPUTS) - 0.5

    def _sigmoid(self, x):
        return 1. / (1. + np.exp(-x))

    def predict_proba(self, x):
        # (vocab_size x batch_size) x (vocab_size x 1) = (vocab_size x 1)
        z = (x.dot(self.W) + self.b)
        return self._sigmoid(z.reshape(-1))

    def fit(self, x, y):
        # average of (vocab_size x batch_size) x (batch_size x 1) = (vocab_size)
        W_grad = x.T.dot(self.predict_proba(x) - y).reshape(VOCAB_SIZE, 1)
        # average of (batch_size x 1) = (batch_size)
        b_grad = np.mean(self.predict_proba(x) - y)
        self.W -= W_grad * LEARNING_RATE / x.shape[0]
        self.b -= b_grad * LEARNING_RATE

    def loss(self, y, y_proba):
        """ Cross-entropy loss
        Args:
            y - ground truth labels
            y_proba - probabilities for the samples
        """
        return -np.mean(y * np.log(y_proba) + (1 - y) * np.log(1 - y_proba))

    def score(self, x, y):
        """ Accuracy"""
        y_pred = np.array(self.predict_proba(x) >= 0.5, dtype=np.int)
        return np.average(y_pred == y)
