from numpy.random import choice

from game.Gladiator import Gladiator
from game.State import State
from game.Winner import Winner
from game.Party import Party
from game.Mode import Mode


class Rules(object):
    def __init__(self, state: State, mode: Mode):
        self.state = state
        self.mode = mode
        
    def get_probability_glad_win(self, left_glad: Gladiator, right_glad: Gladiator):
        probability_win_left = left_glad.strong / (left_glad.strong + right_glad.strong)
        return probability_win_left, 1 - probability_win_left

    def fight(self, left_glad: Gladiator, right_glad: Gladiator, **kwargs):
        def action_left_loose():
            self.state.team_left.dead(left_glad)
            if self.mode == Mode.HILLMAN:
                right_glad.strong += left_glad.strong

        def action_right_loose():
            self.state.team_right.dead(right_glad)
            if self.mode == Mode.HILLMAN:
                left_glad.strong += right_glad.strong

        if 'mode' in kwargs.keys():
            if kwargs['mode'] == 'test':
                if kwargs['winner'] == Party.LEFT:
                    action_right_loose()
                elif kwargs['winner'] == Party.RIGHT:
                    action_left_loose()
                else:
                    raise ValueError('Wrong type of winner')
        else:
            winner = choice((Party.LEFT, Party.RIGHT),
                            p=self.get_probability_glad_win(left_glad, right_glad))
            if winner == Party.LEFT:
                action_right_loose()
            else:
                action_left_loose()

            return winner

    def check_win(self):
        if not self.state.team_left.is_alive():
            return Winner.RIGHT
        elif not self.state.team_right.is_alive():
            return Winner.LEFT
        else:
            return Winner.DRAW
