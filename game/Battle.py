from game.Mode import Mode
from game.Rules import Rules
from game.State import State
from game.Winner import Winner
from players.Bot import Bot


class Battle(object):
    def __init__(self, left_player: Bot, right_player: Bot, mode: Mode = Mode.DEFAULT):
        self.left_player = left_player
        self.right_player = right_player
        self.state = State(left_player.team, right_player.team)

        if mode not in {Mode.DEFAULT, Mode.HILLMAN}:
            print('Wrong mode, install default mode')
            mode = Mode.DEFAULT
        self.rules = Rules(self.state, mode)

    def play(self):
        while self.rules.check_win() == Winner.DRAW:
            result = self.rules.fight(self.left_player.move(self.state),
                                      self.right_player.move(self.state))

        if any(map(lambda glad: not glad.is_living, self.state.team_left)) \
                or any(map(lambda glad: not glad.is_living, self.state.team_right)):
            print(self.state)

        return self.rules.check_win()
