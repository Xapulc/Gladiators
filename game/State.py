from game.Team import Team
from game.Party import Party


class State(object):
    def __init__(self, team_left: Team, team_right: Team):
        self.team_left = team_left
        self.team_right = team_right

    def get_team(self, party: Party):
        return self.team_left if party == Party.LEFT \
            else self.team_right

    def __str__(self):
        return '\n'.join((str(self.team_left), str(self.team_right)))

    def __eq__(self, other):
        return self.team_left == other.team_left \
               and self.team_right == other.team_right

    def __hash__(self):
        return hash(self.team_left) + hash(self.team_right)

    def __copy__(self):
        return State(self.team_left.__copy__(), self.team_right.__copy__())
