class Orders(object):
    def __init__(self, orders=[]):
        self.orders = orders

    def __len__(self):
        return len(self.orders)

    def __getitem__(self, oid):
        for order in self.orders:
            if order.pid == oid:
                return order
        raise Exception("Order with oid={} doesn't exist!".format(oid))

    def __str__(self):
        return "\n".join(str(order) for order in self.orders)

    def add(self, order):
        self.orders.append(order)

    def delete(self, oid):
        for index, order in enumerate(self.orders):
            if order.pid == oid:
                self.orders.pop(index)
                return
        raise Exception("Order with oid={} doesn't exist!".format(oid))

    def order(self, oid):
        return {order.pid for order in self.orders if order.oid == oid}

    def exists(self, oid):
        for order in self.orders:
            if oid == order.pid:
                return True
        return False

    def empty(self):
        return not self.orders

    @property
    def get_orders(self):
        return self.orders
