from random import randint

from game.Mode import Mode
from stats.utils import gen_more_games


if __name__ == "__main__":
    right_powers = [1, 4]
    left_powers = [2, 3]
    gen_more_games(left_powers, right_powers, 500)
