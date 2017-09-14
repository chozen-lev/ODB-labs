class Products(object):
    def __init__(self, products=[], last=0):
        self._products = products
        self._last = last

    def __getitem__(self, pid):
        for product in self._products:
            if product.pid == pid:
                return product
        raise Exception("Product with pid={} doesn't exist!".format(pid))

    def __str__(self):
        return "\n".join(str(product) for product in self._products)

    def add(self, product):
        self._products.append(product)
        self._last += 1

    def delete(self, pid, orders):
        orders.delete_by_pid(pid)
        for index, product in enumerate(self._products):
            if product.pid == pid:
                self._products.pop(index)
                del product
                return
        raise Exception("Product with pid={} doesn't exist!".format(pid))

    def exists(self, pid):
        for product in self._products:
            if pid == product.pid:
                return True
        return False

    def empty(self):
        return not self._products

    @property
    def products(self):
        return self._products

    @property
    def last(self):
        return self._last
