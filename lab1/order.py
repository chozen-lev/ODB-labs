class Order(object):
    def __init__(self, oid, pid):
        self.oid = oid
        self.pid = pid

    def __str__(self):
        return "%d. %d" % (self.oid, self.pid)