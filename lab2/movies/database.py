import _mysql

class Database:

    def __init__(self):
        self.con = None

    def connect(self):
        self.con = _mysql.connect('localhost', 'user', 'password', 'test')

    def select_something(self):
        self.con.query("SELECT VERSION()")
        result = self.con.use_result()

        print("MySQL version: %s" % result.fetch_row()[0])