from game.Gladiator import Gladiator


class Team(object):
    def __init__(self, **kwargs):
        if "members" in kwargs.keys():
            self.members: list = kwargs["members"]
        elif "powers" in kwargs.keys():
            self.members: list = [Gladiator(strong) for strong in kwargs["powers"]]
        else: 
            raise TypeError("Bad type in team")

    def __getitem__(self, item: int):
        return self.members[item]

    def dead(self, gladiator: Gladiator):
        n = len(self.members)
        assert gladiator.is_living, 'Bad gladiator'
        self.members.remove(gladiator)
        gladiator.dead()

        assert n - 1 == len(self.members), f"Wrong team's work\n" + \
                                           str(self) + \
                                           f" \nGLadiator:\n{gladiator}"

    def is_alive(self):
        return len(self.members) > 0

    def __len__(self):
        return len(self.members)

    def __add__(self, other):
        self.members = other.members + self.members

    def __str__(self):
        return f"Team: " + '\n'.join(str(glad) for glad in self.members)

    def __eq__(self, other):
        return all(map(lambda x, y: x == y, self.members, other.members))

    def __hash__(self):
        return sum(hash(glad) for glad in self.members)

    def __copy__(self):
        return Team(members=[glad.__copy__() for glad in self.members])
