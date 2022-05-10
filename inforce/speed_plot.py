import matplotlib.pyplot as plt

data1 = [155.58, 1714.36, 1136.57, 5710.12, 1607.01]
data2 = [3363.96, 2833.64, 3240.33, 1360.56, 5548.84]

x = [0.2, 0.4, 0.6, 0.8, 1.0]
plt.plot(x, data1)
plt.plot(x, data2)
plt.xlabel("gamma", fontsize=15)
plt.ylabel("Episodes per second", fontsize=15)
plt.xticks(x)
plt.show()

mean = lambda l: sum(l) / len(l)
print(mean(data1))
print(mean(data2))
