class Orders(object):
    def __init__(self, orders=[]):
        self._orders = orders

    def __getitem__(self, oid):
        return [order for order in self._orders if order.oid == oid]

    def __str__(self):
        return "\n".join(str(order) for order in self._orders)

    def add(self, order):
        self._orders.append(order)

    def delete(self, oid):
        i = -1
        index = 0
        while index < len(self._orders):
            if self._orders[index].oid == oid:
                del self._orders[index]
                i = index
                continue
            index += 1
        if i == -1:
            raise Exception("Order with oid={} doesn't exist!".format(oid))

    def delete_by_pid(self, pid):
        for index, order in enumerate(self._orders):
            if order.pid == pid:
                self._orders.pop(index)

    def exists(self, oid):
        for order in self._orders:
            if oid == order.pid:
                return True
        return False

    def empty(self):
        return not self._orders

    def pids(self):
        return {order.pid for order in self.orders}

    @property
    def orders(self):
        return self._orders
