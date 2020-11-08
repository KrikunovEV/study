import gym
import numpy as np
from enum import Enum
import seaborn as sn
import matplotlib.pyplot as plt


class EnvType(Enum):
    Deterministic = 1
    Stochastic = 2


class IterationType(Enum):
    Policy = 1
    Value = 2


def direction(index):
    if index == 0:
        return r'$\leftarrow$'
    elif index == 1:
        return '↓'
    elif index == 2:
        return r'$\rightarrow$'
    elif index == 3:
        return '↑'
    return 'unknown'


class FrozenLakeSolver:
    def __init__(self, env_type: EnvType, iteration_type: IterationType, gamma: float, eps: float):
        self.env = gym.make('FrozenLake-v0', is_slippery=False if env_type == EnvType.Deterministic else True)
        self.env.render()
        self.nA = self.env.unwrapped.nA
        self.nS = self.env.unwrapped.nS
        self.P_raw = self.env.unwrapped.P
        print(f'Количество действий: {self.nA}\nКоличество состояний: {self.nS}')

        if iteration_type == IterationType.Policy:
            self.__policy_iteration(eps=eps, gamma=gamma)
        elif iteration_type == IterationType.Value:
            self.__value_iteration(eps=eps, gamma=gamma)

        self.__plot(env_type=env_type, iteration_type=iteration_type)

    def __policy_iteration(self, eps: float, gamma: float):

        self.V = np.zeros(self.nS)
        self.policy = np.zeros(self.nS)
        policy_stable = False

        while not policy_stable:

            # policy evaluation
            while True:
                delta = 0
                for s in range(self.nS):
                    v_prev = self.V[s]
                    sum = 0
                    for p in self.P_raw[s][self.policy[s]]:  # p, s', reward, terminal
                        sum += p[0] * (p[2] + gamma * self.V[p[1]])
                    self.V[s] = sum
                    delta = np.maximum(delta, np.abs(v_prev - self.V[s]))

                if delta < eps:
                    break

            # policy improvement
            policy_stable = True
            for s in range(self.nS):
                a_prev = self.policy[s]
                values = []
                for a in range(self.nA):
                    sum = 0
                    for p in self.P_raw[s][a]:  # p, s', reward, terminal
                        sum += p[0] * (p[2] + gamma * self.V[p[1]])
                    values.append(sum)
                self.policy[s] = np.argmax(values)
                if self.policy[s] != a_prev:
                    policy_stable = False
                    break

    def __value_iteration(self, eps: float, gamma: float):

        # Compute V
        self.V = np.zeros(self.nS)
        while True:
            delta = 0
            for s in range(self.nS):
                v_prev = self.V[s]
                values = []
                for a in range(self.nA):
                    sum = 0
                    for p in self.P_raw[s][a]:  # p, s', reward, terminal
                        sum += p[0] * (p[2] + gamma * self.V[p[1]])
                    values.append(sum)
                self.V[s] = np.max(values)
                delta = np.maximum(delta, np.abs(v_prev - self.V[s]))

            if delta < eps:
                break

        # Find Policy
        self.policy = np.zeros(self.nS)
        for s in range(self.nS):
            values = []
            for a in range(self.nA):
                sum = 0
                for p in self.P_raw[s][a]:  # p, s', reward, terminal
                    sum += p[0] * (p[2] + gamma * self.V[p[1]])
                values.append(sum)
            self.policy[s] = np.argmax(values)

    def __plot(self, env_type: EnvType, iteration_type: IterationType):
        map_labels = ['S', 'F', 'F', 'F', 'F', 'H', 'F', 'H', 'F', 'F', 'F', 'H', 'H', 'F', 'F', 'G']

        fig, ax = plt.subplots(2, 2, figsize=(16, 9))
        fig.suptitle(f'V and policy visualization ({env_type.name}, {iteration_type.name} iteration)', fontsize=16)

        ax[0][0].set_title('V function')
        V_labels = np.array([f'{np.around(v, 2)} ({l})' for v, l in zip(self.V, map_labels)])
        sn.heatmap(self.V.reshape(4, 4), annot=V_labels.reshape(4, 4), fmt='', cmap='Blues',
                   xticklabels=False, yticklabels=False, ax=ax[0][0], square=True, cbar=False)

        ax[0][1].set_title('Policy')
        p_labels = np.array([f'{direction(p)} ({l})' for p, l in zip(self.policy, map_labels)])
        sn.heatmap(self.V.reshape(4, 4), annot=p_labels.reshape(4, 4), fmt='', cmap='Reds',
                   xticklabels=False, yticklabels=False, ax=ax[0][1], square=True, cbar=False)

        ax[1][0].set_title('V function flatten')
        V_labels = np.array([f'{np.around(v, 2)}' for v in self.V])
        sn.heatmap(self.V.reshape(1, 16), annot=V_labels.reshape(1, 16), fmt='', cmap='Blues',
                   xticklabels=map_labels, yticklabels=False, ax=ax[1][0], square=True, cbar=False)

        ax[1][1].set_title('Policy flatten')
        p_map = np.zeros((self.nS, self.nA))
        p_map[np.arange(self.nS), self.policy.astype(np.int_)] = self.V
        sn.heatmap(p_map.T, annot=False, cmap='Reds',
                   xticklabels=map_labels, yticklabels=[direction(i) + f'({i})' for i in range(self.nA)], ax=ax[1][1],
                   square=True, cbar=False)

        fig.tight_layout(rect=(0, 0, 1., 0.98))
        plt.yticks(rotation=0)
        plt.savefig(f'{env_type.name}_{iteration_type.name}_Iteration.png')
        plt.show()


solver = FrozenLakeSolver(env_type=EnvType.Stochastic, iteration_type=IterationType.Policy, gamma=0.9, eps=0.0001)
