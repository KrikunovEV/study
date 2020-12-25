import numpy as np


def softmax(x):
    e_x = np.exp(x - np.max(x))
    return e_x / e_x.sum()


def is_policy_optimal(stochastic: bool, policy):
    if stochastic:
        optimal_policy = np.array([0, 3, 0, 3, 0, 0, 0, 0, 3, 1, 0, 0, 0, 2, 1, 0], dtype=np.int_)
    else:
        optimal_policy = np.array([1, 2, 1, 0, 1, 0, 1, 0, 2, 1, 1, 0, 0, 2, 2, 0], dtype=np.int_)

    if (optimal_policy == policy.argmax(-1)).all():
        return True
    else:
        return False


class SARSA:

    def __init__(self, gamma, env, episodes, terminals, alpha, stochastic_env: bool):
        self.gamma = gamma
        self.alpha = alpha
        self.env = env
        self.stochastic_env = stochastic_env
        self.episodes = episodes
        self.nA = self.env.action_space.n
        self.nS = self.env.observation_space.n
        self.Q = np.random.uniform(0.001, 1., (self.nS, self.nA))
        self.Q[terminals] = 0.

    def solve(self):
        steps = 0
        optimal_step = 0
        for episode in range(self.episodes):
            print(f'SARSA, episode: {episode}/{self.episodes}')
            obs = self.env.reset()
            done = False
            while not done:
                policy = softmax(self.Q[obs])
                action = np.random.choice(np.arange(self.nA), 1, False, p=policy)[0]
                next_obs, reward, done, _ = self.env.step(action)
                policy = softmax(self.Q[next_obs])
                next_action = np.random.choice(np.arange(self.nA), 1, False, p=policy)[0]
                self.Q[obs, action] += self.alpha * (reward + self.gamma * self.Q[next_obs, next_action] - self.Q[
                    obs, action])
                obs = next_obs

                if optimal_step == 0 and is_policy_optimal(self.stochastic_env, self.get_policy()):
                    optimal_step = steps
                steps += 1

        return optimal_step

    def get_policy(self):
        policy = self.Q.copy()
        for row, p in enumerate(policy):
            policy[row] = softmax(p)
        return policy


class QLearning:

    def __init__(self, gamma, env, episodes, terminals, alpha, stochastic_env: bool):
        self.gamma = gamma
        self.alpha = alpha
        self.env = env
        self.stochastic_env = stochastic_env
        self.episodes = episodes
        self.nA = self.env.action_space.n
        self.nS = self.env.observation_space.n
        self.Q = np.random.uniform(0.001, 1., (self.nS, self.nA))
        self.Q[terminals] = 0.

    def solve(self):
        steps = 0
        optimal_step = 0
        for episode in range(self.episodes):
            print(f'SARSA, episode: {episode}/{self.episodes}')
            obs = self.env.reset()
            done = False
            while not done:
                policy = softmax(self.Q[obs])
                action = np.random.choice(np.arange(self.nA), 1, False, p=policy)[0]
                next_obs, reward, done, _ = self.env.step(action)
                next_action = self.Q[next_obs].argmax()
                self.Q[obs, action] += self.alpha * (reward + self.gamma * self.Q[next_obs, next_action] - self.Q[
                    obs, action])
                obs = next_obs

                if optimal_step == 0 and is_policy_optimal(self.stochastic_env, self.get_policy()):
                    optimal_step = steps
                steps += 1

        return optimal_step

    def get_policy(self):
        policy = self.Q.copy()
        for row, p in enumerate(policy):
            policy[row] = softmax(p)
        return policy


class DQLearning:

    def __init__(self, gamma, env, episodes, terminals, alpha, stochastic_env: bool):
        self.gamma = gamma
        self.alpha = alpha
        self.env = env
        self.stochastic_env = stochastic_env
        self.episodes = episodes
        self.nA = self.env.action_space.n
        self.nS = self.env.observation_space.n
        self.Q1 = np.random.uniform(0.001, 1., (self.nS, self.nA))
        self.Q2 = np.random.uniform(0.001, 1., (self.nS, self.nA))
        self.Q1[terminals] = 0.
        self.Q2[terminals] = 0.

    def solve(self):
        steps = 0
        optimal_step = 0
        for episode in range(self.episodes):
            print(f'SARSA, episode: {episode}/{self.episodes}')
            obs = self.env.reset()
            done = False
            while not done:
                policy = softmax((self.Q1 + self.Q2)[obs])
                action = np.random.choice(np.arange(self.nA), 1, False, p=policy)[0]
                next_obs, reward, done, _ = self.env.step(action)

                # prob 0.5 to swap Qs
                if np.random.choice(['leave', 'swap'], 1, False, [0.5, 0.5]) == 'swap':
                    self.Q1, self.Q2 = self.Q2, self.Q1

                next_action = self.Q1[next_obs].argmax()
                self.Q1[obs, action] += self.alpha * (reward + self.gamma * self.Q2[next_obs, next_action] - self.Q1[
                    obs, action])
                obs = next_obs

                if optimal_step == 0 and is_policy_optimal(self.stochastic_env, self.get_policy()):
                    optimal_step = steps
                steps += 1

        return optimal_step

    def get_policy(self):
        policy = self.Q1.copy()
        for row, p in enumerate(policy):
            policy[row] = softmax(p)
        return policy
