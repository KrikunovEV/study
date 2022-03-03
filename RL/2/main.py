import gym
import numpy as np
from enum import Enum
import seaborn as sn
import matplotlib.pyplot as plt


class EnvType(Enum):
    Deterministic = 1
    Stochastic = 2


class PolicyInit(Enum):
    Random = 1
    Optimal = 2


class FrozenLakeSolver:
    def __init__(self, env_type: EnvType, policy_init: PolicyInit, gamma: float, eps: float):
        self.env = gym.make('FrozenLake-v0', is_slippery=False if env_type == EnvType.Deterministic else True)
        self.env.render()
        self.nA = self.env.unwrapped.nA
        self.nS = self.env.unwrapped.nS
        print(f'Количество действий: {self.nA}\nКоличество состояний: {self.nS}')

        self.__policy_initialization(env_type=env_type, policy_init=policy_init)
        self.__compute_P_and_R(P_raw=self.env.unwrapped.P)
        self.__evaluation(eps=eps, gamma=gamma)
        self.__plot(env_type=env_type, policy_init=policy_init)

    def __plot(self, env_type: EnvType, policy_init: PolicyInit):
        map_labels = ['S', 'F', 'F', 'F', 'F', 'H', 'F', 'H', 'F', 'F', 'F', 'H', 'H', 'F', 'F', 'G']
        map_mask = np.array([False, False, False, False, False, True, False, True, False, False, False, True, True,
                             False, False, True])

        fig, ax = plt.subplots(1, 2, figsize=(15, 6))
        fig.suptitle(f'V function visualization ({env_type.name}, {policy_init.name})', fontsize=16)

        V = self.V_direct.copy()
        ax[0].set_title('Direct evaluation')
        labels = np.array([f'{np.around(v, 6)} ({l})' for v, l in zip(V, map_labels)]).reshape(4, 4)
        sn.heatmap(V.reshape(4, 4), annot=labels, fmt='', cmap='Blues', xticklabels=False, yticklabels=False,
                   ax=ax[0], square=True)

        V = self.V_iterative.copy()
        ax[1].set_title('Iterative evaluation')
        labels = np.array([f'{np.around(v, 6)} ({l})' for v, l in zip(V, map_labels)]).reshape(4, 4)
        sn.heatmap(V.reshape(4, 4), annot=labels, fmt='', cmap='Blues', xticklabels=False, yticklabels=False, ax=ax[1],
                   square=True)

        plt.savefig(f'{env_type.name}_{policy_init.name}.png')

    def __policy_initialization(self, env_type: EnvType, policy_init: PolicyInit):

        if env_type == EnvType.Deterministic and policy_init == PolicyInit.Random:
            self.policy = np.zeros((self.nS, self.nA))
            self.policy[np.arange(self.nS), np.random.randint(0, self.nA, size=self.nS)] = 1.0

        elif env_type == EnvType.Stochastic and policy_init == PolicyInit.Random:
            self.policy = np.full((self.nS, self.nA), fill_value=1. / self.nA)

        elif env_type == EnvType.Deterministic and policy_init == PolicyInit.Optimal:
            self.policy = np.array([
                [0., 0., 0., 1., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.],  # left
                [0., 0., 1., 0., 1., 0., 1., 0., 0., 0., 1., 0., 0., 0., 0., 0.],  # down
                [1., 1., 0., 0., 0., 0., 0., 0., 1., 1., 0., 0., 0., 1., 1., 0.],  # right
                [0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.]   # top
            ])
            self.policy = self.policy.T

        elif env_type == EnvType.Stochastic and policy_init == PolicyInit.Optimal:
            self.policy = np.array([
                [0.25, 0.33, 0.25, 0.34, 0.33, 0., 0.0, 0., 0.33, 0.33, 0.33, 0., 0., 0.00, 0.25, 0.],  # left
                [0.25, 0.00, 0.25, 0.00, 0.34, 0., 0.5, 0., 0.00, 0.33, 0.34, 0., 0., 0.33, 0.25, 0.],  # down
                [0.25, 0.34, 0.25, 0.33, 0.00, 0., 0.0, 0., 0.34, 0.34, 0.00, 0., 0., 0.34, 0.25, 0.],  # right
                [0.25, 0.33, 0.25, 0.33, 0.33, 0., 0.5, 0., 0.33, 0.00, 0.33, 0., 0., 0.33, 0.25, 0.]  # top
            ])
            self.policy = self.policy.T

    def __compute_P_and_R(self, P_raw):
        # P and R computation according to policy
        self.P = np.zeros((self.nS, self.nS))  # P[s', s]
        self.R = np.zeros(self.nS)
        for s in range(self.nS):
            for a in range(self.nA):
                for p in P_raw[s][a]:  # p, s', reward, terminal
                    self.R[s] += self.policy[s, a] * p[0] * p[2]
                    self.P[p[1], s] += self.policy[s, a] * p[0]

    def __evaluation(self, eps: float, gamma: float):
        # P has dimensionality of [s', s] where s' is the next state
        # Thus P is right operand to compute over s'

        # Direct computation
        self.V_direct = np.dot(self.R, np.linalg.inv(np.identity(self.nS) - gamma * self.P))

        # Iterative computation
        self.V_iterative = np.zeros(self.nS)
        while True:
            old_v = self.V_iterative.copy()
            self.V_iterative = self.R + gamma * np.dot(self.V_iterative, self.P)
            if np.sum(np.abs(self.V_iterative - old_v)) < eps:
                break


solver = FrozenLakeSolver(env_type=EnvType.Deterministic, policy_init=PolicyInit.Random, gamma=0.99, eps=0.00001)
