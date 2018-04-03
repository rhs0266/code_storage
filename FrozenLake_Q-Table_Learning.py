'''
Created on 2018. 4. 3.

@author: ryuhoseok
'''
import gym
import tensorflow as tf
import numpy as np
import random
from argparse import ArgumentParser

'''
OUT_DIR = 'FrozenLake_Q-Table_Learning-experiment' # default saving directory
MAX_SCORE_QUEUE_SIZE = 100  # number of episode scores to calculate average performance
'''
GAME = 'FrozenLake-v0'    # name of game


def get_options():
    parser = ArgumentParser()
    parser.add_argument('--MAX_EPISODE', type=int, default=2000,
                        help='max number of episodes iteration')
    parser.add_argument('--ACTION_DIM', type=int, default=1,
                        help='number of actions one can take')
    parser.add_argument('--OBSERVATION_DIM', type=int, default=1,
                        help='number of observations one can see')
    parser.add_argument('--GAMMA', type=float, default=0.97,
                        help='discount factor of Q learning')
    parser.add_argument('--INIT_EPS', type=float, default=1.0,
                        help='initial probability for randomly sampling action')
    parser.add_argument('--FINAL_EPS', type=float, default=1e-5,
                        help='finial probability for randomly sampling action')
    parser.add_argument('--EPS_DECAY', type=float, default=0.95,
                        help='epsilon decay rate')
    parser.add_argument('--EPS_ANNEAL_STEPS', type=int, default=10,
                        help='steps interval to decay epsilon')
    parser.add_argument('--LR', type=float, default=0.85,
                        help='learning rate')
    parser.add_argument('--MAX_EXPERIENCE', type=int, default=100,
                        help='size of experience replay memory')
    '''parser.add_argument('--BATCH_SIZE', type=int, default=256,
                        help='mini batch size'),
    parser.add_argument('--H1_SIZE', type=int, default=128,
                        help='size of hidden layer 1')
    parser.add_argument('--H2_SIZE', type=int, default=128,
                        help='size of hidden layer 2')
    parser.add_argument('--H3_SIZE', type=int, default=128,
                        help='size of hidden layer 3')'''
    options = parser.parse_args()
    return options

class QAgent:
    # A naive neural network with 0 hidden layers and relu as non-linear function.
    def __init__(self, options):
        self.Q = self.weight_variable([options.OBSERVATION_DIM, options.ACTION_DIM])
    
    def my_initializer(self, shape):
        return np.zeros(shape)   
    
    def xavier_initializer(self, shape):
        dim_sum = np.sum(shape)
        if len(shape) == 1: # for bias shape
            dim_sum += 1
        bound = np.sqrt(6.0 / dim_sum)
        return np.random.uniform(-bound, bound, shape)

    # Tool function to create weight variables
    def weight_variable(self, shape):
        # return self.xavier_initializer(shape)
        return self.my_initializer(shape) 

    # Sample action with random rate eps
    def sample_action(self, state, eps, options):
        
        '''
        act_values = self.Q[state, :]
        if random.random() <= eps:
            # action_index = env.action_space.sample()
            action_index = random.randrange(options.ACTION_DIM) # eps-greedy
        else:
            action_index = np.argmax(act_values) # action which returns maximum expected reward
            '''
        act_values = self.Q[state, :] + np.random.randn(1, options.ACTION_DIM)*eps
        action_index = np.argmax(act_values)
        return action_index



def train(env):
    # setting parameters of neural net
    options = get_options()
    options.OBSERVATION_DIM = env.observation_space.n
    options.ACTION_DIM = env.action_space.n
    
    # set agent
    agent = QAgent(options)
    
    print ("Initial Q-Table Values")
    print (np.round(agent.Q,3))
    
    # initialize some local vars
    score_cache = []
    eps = options.INIT_EPS
    global_step = 0
    
    state = env.reset()
    env.render()
    
    # start reinforcement learning
    for i_episode in xrange(options.MAX_EPISODE):
        state = env.reset()
        score = 0
        done = False
        
        for i_experience in xrange(options.MAX_EXPERIENCE):
            global_step += 1
            if global_step % options.EPS_ANNEAL_STEPS == 0 and eps > options.FINAL_EPS:
                eps = eps * options.EPS_DECAY
            #env.render()
            
            action = agent.sample_action(state, eps, options) #np.argmax(agent.Q[state,:] + np.random.randn(1,env.action_space.n)*(1./(i_episode+1)))
            next_state, reward, done, _ = env.step(action)
            max_value = np.max(agent.Q[next_state, :])
            agent.Q[state, action] = agent.Q[state,action] + options.LR * (reward + options.GAMMA * max_value - agent.Q[state, action])
            score += reward
            state = next_state
            if done == True:
                break
            
        if (options.MAX_EPISODE - i_episode <= 100):
            score_cache.append(score)
        
    print ("Score over time: " + str(sum(score_cache)/100))
    
    print ("Final Q-Table Values")
    for i in range(4):
        for j in range(4):
            print np.argmax(agent.Q[i*4+j,:]),
        print 
env = gym.make(GAME)
train(env)
