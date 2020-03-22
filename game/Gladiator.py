from copy import deepcopy


class Gladiator(object):
    def __init__(self, strong=0):
        self.strong = strong
        self.is_living = True

    def dead(self):
        self.is_living = False
        return self

    def __copy__(self):
        new_glad = Gladiator(strong=self.strong)
        if not self.is_living:
            new_glad.dead()
        return new_glad

    def __eq__(self, other):
        return self.strong == other.strong \
               and self.is_living == other.is_living

    def __hash__(self):
        return hash(self.strong) + hash(self.is_living)

    def __str__(self):
        return f"Gladiator {'is alive' if self.is_living else 'dead'} " \
            f"with strong: {self.strong}"
