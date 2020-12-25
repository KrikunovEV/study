import gym
import numpy as np
from enum import Enum
import seaborn as sn
import matplotlib.pyplot as plt
from TD import SARSA, QLearning, DQLearning


class EnvType(Enum):
    Deterministic = 1
    Stochastic = 2


class SolverType(Enum):
    SARSA = 1
    Qlearning = 2
    DQlearning = 3


class FrozenLakeSolver:
    def __init__(self, env_type: EnvType, solver_type: SolverType, gamma: float, episodes: int, alpha: float):
        self.env = gym.make('FrozenLake-v0', is_slippery=False if env_type == EnvType.Deterministic else True)
        self.gamma = gamma
        self.alpha = alpha
        self.episodes = episodes
        terminals = [False, False, False, False, False, True, False, True,
                     False, False, False, True, True, False, False, True]

        if solver_type == SolverType.SARSA:
            self.solver = SARSA(gamma, self.env, episodes, terminals, alpha, env_type == EnvType.Stochastic)
        elif solver_type == SolverType.Qlearning:
            self.solver = QLearning(gamma, self.env, episodes, terminals, alpha, env_type == EnvType.Stochastic)
        elif solver_type == SolverType.DQlearning:
            self.solver = DQLearning(gamma, self.env, episodes, terminals, alpha, env_type == EnvType.Stochastic)

    def play(self):
        return self.solver.solve()

    def policy_evaluation(self):
        policy = self.solver.get_policy()
        V = np.zeros(policy.shape[0])
        for episode in range(self.episodes):
            print(f'policy evaluation, episode: {episode}/{self.episodes}')
            obs = self.env.reset()
            done = False
            while not done:
                action = np.random.choice(np.arange(policy.shape[1]), 1, False, p=policy[obs])[0]
                next_obs, reward, done, _ = self.env.step(action)
                V[obs] += self.alpha * (reward + self.gamma * V[next_obs] - V[obs])
                obs = next_obs
        return V


def plot(V_det, V_stoc, steps_det, steps_stoc, alphas):
    map_labels = ['S', 'F', 'F', 'F', 'F', 'H', 'F', 'H', 'F', 'F', 'F', 'H', 'H', 'F', 'F', 'G']

    for id, solver_type in enumerate(SolverType):

        fig, ax = plt.subplots(1, 2, figsize=(16, 9))
        fig.suptitle(f'{solver_type.name} алгоритм', fontsize=16)

        ax[0].set_title('Policy evaluation для детерменированной среды')
        V_labels = np.array([f'{np.around(v, 2)} ({l})' for v, l in zip(V_det[id], map_labels)])
        sn.heatmap(V_det[id].reshape(4, 4), annot=V_labels.reshape(4, 4), fmt='', cmap='Blues',
                   xticklabels=False, yticklabels=False, ax=ax[0], square=True)

        ax[1].set_title('Policy evaluation для стохастической среды')
        V_labels = np.array([f'{np.around(v, 2)} ({l})' for v, l in zip(V_stoc[id], map_labels)])
        sn.heatmap(V_stoc[id].reshape(4, 4), annot=V_labels.reshape(4, 4), fmt='', cmap='Reds',
                   xticklabels=False, yticklabels=False, ax=ax[1], square=True)

        fig.tight_layout(rect=(0, 0, 1., 0.95))
        plt.savefig(f'{solver_type.name}.png')
        #plt.show()

    plt.clf()
    plt.title('Количество шагов, за которые была получена оптимальная политика для детерминированной среды'
              ' (перебор по alpha)')
    plt.xlabel('alpha')
    plt.ylabel('steps')
    for id, solver_type in enumerate(SolverType):
        plt.plot(alphas, steps_det[id], '-o', label=f'{solver_type.name}')
    plt.legend()
    plt.tight_layout()
    plt.savefig(f'steps_deterministic.png')

    plt.clf()
    plt.title('Количество шагов, за которые была получена оптимальная политика для стохастической среды'
              ' (перебор по alpha)')
    plt.xlabel('alpha')
    plt.ylabel('steps')
    for id, solver_type in enumerate(SolverType):
        plt.plot(alphas, steps_stoc[id], '-o', label=f'{solver_type.name}')
    plt.legend()
    plt.tight_layout()
    plt.savefig(f'steps_stochastic.png')


optimal_steps_deterministic = [[], [], []]
optimal_steps_stochastic = [[], [], []]
optimal_V_deterministic = [np.array([0]), np.array([0]), np.array([0])]
optimal_V_stochastic = [np.array([0]), np.array([0]), np.array([0])]
alphas = np.arange(1, 10) / 10.

for id, solver_type in enumerate(SolverType):

    for alpha in alphas:

        solver_deterministic = FrozenLakeSolver(env_type=EnvType.Deterministic,
                                                solver_type=solver_type,
                                                gamma=1.,
                                                episodes=3000,
                                                alpha=alpha)

        solver_stochastic = FrozenLakeSolver(env_type=EnvType.Stochastic,
                                             solver_type=solver_type,
                                             gamma=1.,
                                             episodes=3000,
                                             alpha=alpha)

        optimal_steps = solver_deterministic.play()
        optimal_steps_deterministic[id].append(optimal_steps)

        optimal_steps = solver_stochastic.play()
        optimal_steps_stochastic[id].append(optimal_steps)

        V_deterministic = solver_deterministic.policy_evaluation()
        if optimal_V_deterministic[id].sum() < V_deterministic.sum():
            optimal_V_deterministic[id] = V_deterministic

        V_stochastic = solver_stochastic.policy_evaluation()
        if optimal_V_stochastic[id].sum() < V_stochastic.sum():
            optimal_V_stochastic[id] = V_stochastic

plot(optimal_V_deterministic, optimal_V_stochastic, optimal_steps_deterministic, optimal_steps_stochastic, alphas)
