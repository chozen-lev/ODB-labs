class Product(object):
    def __init__(self, pid, name, price):
        self.pid = pid
        self.name = name
        self.price = price

    def __str__(self):
        return "%d. %s - %d UAH" % (self.pid, self.name, self.price)
