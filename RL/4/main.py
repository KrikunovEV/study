import gym
import numpy as np
from enum import Enum
import seaborn as sn
import matplotlib.pyplot as plt


class EnvType(Enum):
    Deterministic = 1
    Stochastic = 2


class PolicyType(Enum):
    Random = 1,
    Optimal = 2


class SolverType(Enum):
    FirstVisit = 1
    EveryVisit = 2


class FrozenLakeSolver:
    def __init__(self, env_type: EnvType, policy_type: PolicyType, gamma: float, episodes: int):
        self.env = gym.make('FrozenLake-v0', is_slippery=False if env_type == EnvType.Deterministic else True)
        self.nA = self.env.action_space.n
        self.nS = self.env.observation_space.n
        self.P_raw = self.env.unwrapped.P
        self.gamma = gamma
        self.episodes = episodes
        self.env_type = env_type
        self.policy_type = policy_type
        print(f'Количество действий: {self.nA}\nКоличество состояний: {self.nS}')

        if policy_type == PolicyType.Random:
            self.__policy_init_random()
        else:
            self.__policy_init_optimal()

        self.__direct_solution()

        self.V_fvmc = np.zeros(self.nS)
        self.V_evmc = np.zeros(self.nS)
        self.bias_fvmc = []
        self.bias_evmc = []
        self.var_fvmc = []
        self.var_evmc = []
        self.rewards = []
        self.obs = []
        self.returns_fvmc = []
        self.returns_evmc = []
        for s in range(self.nS):
            self.returns_fvmc.append([])
            self.returns_evmc.append([])

        self.play()
        self.__plot()

    def play(self):
        for e in range(self.episodes):
            print(f'episode: {e}')
            obs = self.env.reset()
            self.rewards = []
            self.obs = []
            done = False
            while not done:
                action = np.random.choice(np.arange(self.nA), 1, False, p=self.policy[obs])[0]
                self.obs.append(obs)
                obs, reward, done, _ = self.env.step(action)
                self.rewards.append(reward)
            self.MonteCarlo(solver_type=SolverType.FirstVisit)
            self.MonteCarlo(solver_type=SolverType.EveryVisit)

    def MonteCarlo(self, solver_type: SolverType):
        G = 0
        mask = np.ones(self.nS, dtype=np.bool_)
        for i in reversed(range(len(self.rewards))):
            G = self.rewards[i] + G * self.gamma
            state = self.obs[i]
            if solver_type == SolverType.FirstVisit:
                if mask[state]:
                    mask[state] = False
                    self.returns_fvmc[state].append(G)
                else:
                    continue
            else:
                self.returns_evmc[state].append(G)

        for s in range(self.nS):
            if solver_type == SolverType.FirstVisit:
                if len(self.returns_fvmc[s]) == 0:
                    self.V_fvmc[s] = 0
                else:
                    self.V_fvmc[s] = np.mean(self.returns_fvmc[s])
            else:
                if len(self.returns_evmc[s]) == 0:
                    self.V_evmc[s] = 0
                else:
                    self.V_evmc[s] = np.mean(self.returns_evmc[s])

        if solver_type == SolverType.FirstVisit:
            self.bias_fvmc.append(np.mean(self.V_direct - self.V_fvmc))
            self.var_fvmc.append(np.mean(self.V_fvmc ** 2) - np.mean(self.V_fvmc) ** 2)
        else:
            self.bias_evmc.append(np.mean(self.V_direct - self.V_evmc))
            self.var_evmc.append(np.mean(self.V_evmc ** 2) - np.mean(self.V_evmc) ** 2)

    def __policy_init_random(self):
        self.policy = np.full((self.nS, self.nA), 1. / self.nA)

    def __policy_init_optimal(self):
        if self.env_type == EnvType.Stochastic:
            optimal_actions = np.array([0, 3, 0, 3, 0, 0, 0, 0, 3, 1, 0, 0, 0, 2, 1, 0], dtype=np.int_)
        else:
            optimal_actions = np.array([1, 2, 1, 0, 1, 0, 1, 0, 2, 1, 1, 0, 0, 2, 2, 0], dtype=np.int_)
        self.policy = np.zeros((self.nS, self.nA))
        self.policy[np.arange(self.nS), optimal_actions] = 1.

    def __direct_solution(self):
        self.P = np.zeros((self.nS, self.nS))  # P[s', s]
        self.R = np.zeros(self.nS)
        for s in range(self.nS):
            for a in range(self.nA):
                for p in self.P_raw[s][a]:  # p, s', reward, terminal
                    self.R[s] += self.policy[s, a] * p[0] * p[2]
                    self.P[p[1], s] += self.policy[s, a] * p[0]
        self.V_direct = np.dot(self.R, np.linalg.inv(np.identity(self.nS) - self.gamma * self.P))

    def __plot(self):
        map_labels = ['S', 'F', 'F', 'F', 'F', 'H', 'F', 'H', 'F', 'F', 'F', 'H', 'H', 'F', 'F', 'G']

        fig, ax = plt.subplots(2, 2, figsize=(16, 9))
        fig.suptitle(f'First Visit and Every Visit Monte-Carlo ({self.env_type.name} env, {self.policy_type.name} policy)',
                     fontsize=16)

        ax[0][0].set_title('First Visit V function')
        V_labels = np.array([f'{np.around(v, 2)} ({l})' for v, l in zip(self.V_fvmc, map_labels)])
        sn.heatmap(self.V_fvmc.reshape(4, 4), annot=V_labels.reshape(4, 4), fmt='', cmap='Blues',
                   xticklabels=False, yticklabels=False, ax=ax[0][0], square=True)

        ax[0][1].set_title('Every Visit V function')
        V_labels = np.array([f'{np.around(v, 2)} ({l})' for v, l in zip(self.V_evmc, map_labels)])
        sn.heatmap(self.V_evmc.reshape(4, 4), annot=V_labels.reshape(4, 4), fmt='', cmap='Reds',
                   xticklabels=False, yticklabels=False, ax=ax[0][1], square=True)

        ax[1][0].set_title(f'Bias (last values:'
                           f' FV {np.around(self.bias_fvmc[-1], 4)},'
                           f' EV {np.around(self.bias_evmc[-1], 4)})')
        ax[1][0].plot(self.bias_fvmc, label='First Visit')
        ax[1][0].plot(self.bias_evmc, label='Every Visit')
        ax[1][0].set_xlabel('# of episode')
        ax[1][0].set_ylabel('bias value')
        ax[1][0].legend()

        ax[1][1].set_title(f'Variance (last:'
                           f' FV {np.around(self.var_fvmc[-1], 4)},'
                           f' EV {np.around(self.var_evmc[-1], 4)})')
        ax[1][1].plot(self.var_fvmc, label='First Visit')
        ax[1][1].plot(self.var_evmc, label='Every Visit')
        ax[1][1].set_xlabel('# of episode')
        ax[1][1].set_ylabel('variance value')
        ax[1][1].legend()

        fig.tight_layout(rect=(0, 0, 1., 0.95))
        plt.savefig(f'{self.env_type.name}_{self.policy_type.name}.png')
        plt.show()


solver = FrozenLakeSolver(env_type=EnvType.Stochastic,
                          policy_type=PolicyType.Optimal,
                          gamma=0.9,
                          episodes=10000)
