from game.Battle import Battle
from game.Party import Party
from game.Rules import Rules
from game.State import State
from game.Team import Team
from game.Winner import Winner
from game.Mode import Mode
from players.Bot import Bot
import matplotlib.pyplot as plt
import matplotlib.animation as animation


def get_stat(cur, total, left_wins, right_wins):
    return f"{(100 * cur) / total}% completed\n" \
        f"{(100 * left_wins) / cur}% left player wins\n" \
        f"{(100 * right_wins) / cur}% right player wins\n"


def gen_more_games(left_powers: list, right_powers: list, delta: int, mode: Mode = Mode.DEFAULT):
    def generate_example():
        nonlocal left_wins
        left_player = Bot(left_powers, Party.LEFT)
        right_player = Bot(right_powers, Party.RIGHT)
        battle = Battle(left_player, right_player, mode)
        winner = battle.play()
        if winner == Winner.LEFT:
            left_wins += 1

        count.append(len(count) + 1)
        mean.append(left_wins / count[-1])

    def gen_more(j: int):
        for _ in range(0, delta):
            generate_example()

        ax.clear()
        plt.ylim([0, 1])
        ax.plot(count, mean, label=f'wins of left team, last: {mean[-1]}')
        fig.legend()

    left_wins = 0
    count = []
    mean = []
    fig, ax = plt.subplots()
    ani = animation.FuncAnimation(fig, gen_more)
    plt.show()


def get_win_probability(left_powers: list, right_powers: list, mode: Mode):
    """
    This function gets probability
    of win, if both players should
    choose first alive gladiators
    """

    def get_left_win_prob(cur_state: State):
        if not cur_state.team_left.is_alive():
            return 0
        elif not cur_state.team_right.is_alive():
            return 1
        else:
            state_left_win = cur_state.__copy__()
            state_right_win = cur_state.__copy__()
            rules_left_win = Rules(state_left_win, mode)
            rules_right_win = Rules(state_right_win, mode)

            left_glad = cur_state.team_left[0]
            right_glad = cur_state.team_right[0]
            prob_glad_win = rules_left_win \
                .get_probability_glad_win(left_glad, right_glad)

            rules_left_win.fight(state_left_win.team_left[0],
                                 state_left_win.team_right[0],
                                 mode='test', winner=Party.LEFT)
            rules_right_win.fight(state_right_win.team_left[0],
                                  state_right_win.team_right[0],
                                  mode='test', winner=Party.RIGHT)

            return prob_glad_win[0] * get_left_win_prob(rules_left_win.state) \
                   + prob_glad_win[1] * get_left_win_prob(rules_right_win.state)

    left_team = Team(powers=left_powers)
    right_team = Team(powers=right_powers)

    prob_left_win = get_left_win_prob(State(left_team, right_team))
    return prob_left_win, 1 - prob_left_win
