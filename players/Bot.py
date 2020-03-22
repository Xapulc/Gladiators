from random import choice

from game.State import State
from game.Team import Team
from game.Party import Party


class Bot(object):
    def __init__(self, powers: list, party: Party):
        self.team = Team(powers=powers)
        self.party = party

    def move(self, state: State):
        return choice(state.get_team(self.party))
