from settings import BATCH_SIZE, TRAIN_SIZE, VOCAB_SIZE
from time import time
import math
import numpy as np
import os

def read_dir(path):
    """ Reads files from the specified directory and tokenizes comments"""
    result = []
    cwd = os.getcwd()
    os.chdir(path)
    for filename in os.listdir():
        with open(filename, 'r') as f:
            result.append(f.read().strip().lower().split())
    os.chdir(cwd)
    return result

def build_vocab(comments):
    """ Pack comment words into two dictionaries
    Returns:
        id2word, word2id for top VOCAB_SIZE (2000) words
    """
    word2id = {}
    for comment in comments:
        for word in comment:
            word2id[word] = word2id.get(word, 0) + 1
    top2000 = sorted(word2id.items(), key=lambda kv: kv[1], reverse=True)[:VOCAB_SIZE]
    id2word = [kv[0] for kv in top2000]
    word2id = dict(top2000)
    return id2word, word2id

def shuffle_inplace(x, y=None, seed=42):
    """ Shuffles input array(s) in place"""
    np.random.seed(seed)
    np.random.shuffle(x)
    if y is None: return
    np.random.seed(seed)
    np.random.shuffle(y)

def shuffle_ninplace(arr, seed=42):
    """ Shuffle not in place
    Shuffles preserving the input array, returns shuffled array
    """
    res = arr.copy()
    np.random.seed(seed)
    np.random.shuffle(res)
    return res

def make_train_val(train_pos, train_neg):
    """ Mixes inputs array and splits them into train and val
    Args:
        train_pos, train_neg - positive/negative comments from the train dir of IMDB dataset
    Returns:
        (X_train, y_train), (X_val, y_val)
        X - python lists of words
    """
    train_pos = shuffle_ninplace(train_pos)
    train_neg = shuffle_ninplace(train_neg)
    
    X_train = np.concatenate((train_pos[:TRAIN_SIZE // 2], train_neg[:TRAIN_SIZE // 2]))
    y_train = np.concatenate((np.ones(TRAIN_SIZE // 2), np.zeros(TRAIN_SIZE // 2)))
    shuffle_inplace(X_train, y_train)
    
    X_val = np.concatenate((train_pos[TRAIN_SIZE // 2:], train_neg[TRAIN_SIZE // 2:]))
    y_val = np.concatenate((np.ones(len(X_val) // 2), np.zeros(len(X_val) // 2)))
    shuffle_inplace(X_val, y_val)
    
    return (X_train, y_train), (X_val, y_val)

def make_test(test_pos, test_neg):
    X_test = np.concatenate((test_pos, test_neg))
    y_test = np.concatenate((np.ones(len(test_pos)), np.zeros(len(test_neg))))
    shuffle_inplace(X_test, y_test)
    return X_test, y_test

def featurize(comments, id2word, word2id):
    """ Convert tokenized comments to one-hot vectors

    Args:
        comments - array of comments, each comment is an array of words.
    Unexpected behavior if comments is a list of words! Pass a list containing
    one comment in this case
    """
    result = np.zeros((len(comments), VOCAB_SIZE))
    print("Iterations: ", end="")
    for i, c in enumerate(comments):
        np.put(result[i], [id2word.index(x) for x in c if x in word2id], 1)
        if i % 1000 == 0: print(i, end=", ")
    print()
    return result
    # return np.array([[1 if w in c else 0 for w in word2id] for c in comments])

def read_aclImdb(data_path):
    """ Reads comments, tokenizes, splits into train, val, test"""

    # Read from disk
    train_pos = read_dir(os.path.join(data_path, "train/pos"))
    train_neg = read_dir(os.path.join(data_path, "train/neg"))
    test_pos = read_dir(os.path.join(data_path, "test/pos"))
    test_neg = read_dir(os.path.join(data_path, "test/neg"))

    # Build vocabulary
    (id2word, word2id) = build_vocab(train_pos + train_neg)

    # Convert comments to one-hot vectors
    train_pos = featurize(train_pos, id2word, word2id)
    # print(train_pos.shape)
    train_neg = featurize(train_neg, id2word, word2id)
    # print(train_neg.shape)
    test_pos = featurize(test_pos, id2word, word2id)
    # print(test_pos.shape)
    test_neg = featurize(test_neg, id2word, word2id)
    # print(test_neg.shape)

    # Shuffle and split into train, val and test
    (X_train, y_train), (X_val, y_val) = make_train_val(train_pos, train_neg)
    (X_test, y_test) = make_test(test_pos, test_neg)

    return (X_train, y_train), (X_val, y_val), (X_test, y_test),\
           (id2word, word2id)

def batches(x, y):
    if len(x) != len(y):
        raise ValueError("Number of samples in x and y differs")
    shuffle_inplace(x, y) # random order every invocation
    for i in range(0, len(x), BATCH_SIZE):
        yield np.array(x[i:i + BATCH_SIZE]), np.array(y[i:i + BATCH_SIZE])
