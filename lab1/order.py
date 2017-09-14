class Order(object):
    def __init__(self, oid, pid):
        self.oid = oid
        self.pid = pid

    def __str__(self):
        return "%d. %d" % (self.oid, self.pid)

    def delete(self, pid, orders):
        for index, order in enumerate(orders):
            if order.oid == self.oid and order.pid == pid:
                orders.pop(index)

    def exist(self, pid, orders):
        for index, order in enumerate(orders):
            if order.oid == self.oid and order.pid == pid:
                return True
        return False
