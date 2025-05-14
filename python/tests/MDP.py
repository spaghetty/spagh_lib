import numpy as ql

R = ql.matrix([[0,0,0,0,1,0],
               [0,0,0,1,0,1],
               [0,0,100,1,0,0],
               [0,1,1,0,1,0],
               [1,0,0,1,0,0],
               [0,1,0,0,0,0]])
Q = ql.matrix(ql.zeros([6,6]))
gamma = 0.8

agent_s_state = 1

def possible_actions(state):
    current_state_row = R[state]
    possible_act = ql.where(current_state_row >0)[1]
    return possible_act

def ActionChoice(available_actions_range):
    next_action = int(ql.random.choice(PossibleAction, 1))
    return next_action

def reward(current_state, action, gamma):
    Max_State = ql.where(Q[action,] == ql.max(Q[action,]))[1]
    if Max_State.shape[0] > 1:
        Max_State = int(ql.random.choice(Max_State, size= 1))
    else:
        Max_State = int(Max_State)

    MaxValue = Q[action, Max_State]
    Q[current_state, action]= R[current_state, action] + gamma * MaxValue



for i in range(5000):
    current_state = ql.random.randint(0, int(Q.shape[0]))
    PossibleAction = possible_actions(current_state)
    action = ActionChoice(PossibleAction)
    reward(current_state,action, gamma)

print("Q: ")
print(Q)

print("Normed Q: ")
print(Q/ql.max(Q)*100)
