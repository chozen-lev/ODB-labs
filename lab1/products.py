class Products(object):
    def __init__(self, products=[], last=0):
        self.products = products
        self.last = last

    def __getitem__(self, pid):
        for product in self.products:
            if product.pid == pid:
                return product
        raise Exception("Product with pid={} doesn't exist!".format(pid))

    def __len__(self):
        return len(self.products)

    def __str__(self):
        return "\n".join(str(product) for product in self.products)

    def add(self, product):
        self.products.append(product)
        self.last += 1

    def delete(self, pid, orders):
        for order in orders:
            if order.pid == pid:
                orders.delete(order.oid)
        for index, product in enumerate(self.products):
            if product.pid == pid:
                self.products.pop(index)
                del product
                return
        raise Exception("Product with pid={} doesn't exist!".format(pid))

    def exists(self, pid):
        for product in self.products:
            if pid == product.pid:
                return True
        return False

    def empty(self):
        return not self.products

    @property
    def get_products(self):
        return self.products

    @property
    def get_last(self):
        return self.last
