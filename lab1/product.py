class Product(object):
    def __init__(self, pid, name, price):
        self.pid = pid
        self._name = name
        self._price = price

    def __str__(self):
        return "%d. %s - %d UAH" % (self.pid, self._name, self.price)

    @property
    def price(self):
        return self._price

    @price.setter
    def price(self, value):
        self._price = value

    @property
    def name(self):
        return self._name

    @name.setter
    def name(self, name):
        self._name = name
