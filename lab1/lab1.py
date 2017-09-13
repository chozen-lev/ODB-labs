from product import Product
from products import Products
from order import Order
from orders import Orders
import pickle

products = None
orders = None

try:
    f = open('data.pkl', 'rb')
except IOError:
    print 'Cannot open file!!!'
    products = Products()
    orders = Orders()
else:
    print 'Data has been loaded'
    data = pickle.load(f)
    products = Products(data['products'], data['plast'])
    orders = Orders(data['orders'])
    f.close()
finally:
    data = {
        'products': products.get_products,
        'orders': orders.get_orders,
        'plast': products.get_last
    }


def select_menu():
    print "\n1. 'products'\n2. 'orders'\n3. Back"
    while True:
        key = raw_input("Select table: ")
        if key == '1':
            if not products.empty():
                print products
            else:
                print "'products' is empty"
            return
        elif key == '2':
            if not orders.empty():
                print orders
            else:
                print "'orders' is empty"
            return
        elif key == '3':
            return
        print "Wrong Answer. Please Try Again!"


def delete_menu():
    pass


def insert_menu():
    print "\n1. 'products'\n2. 'orders'\n3. Back"
    while True:
        key = raw_input("Select table: ")
        if key == '1':
            name = raw_input("Enter product name: ")
            while True:
                try:
                    price = int(raw_input("Enter product price: "))
                except ValueError:
                    print "Wrong Answer. Please Try Again!"
                else:
                    products.add(Product(products.get_last, name, price))
                    return
        elif key == '2':
            while True:
                try:
                    oid = int(raw_input("Enter oid: "))
                except ValueError:
                    print "Wrong Answer. Please Try Again!"
                else:
                    break
            while True:
                try:
                    pid = int(raw_input("Enter pid: "))
                except ValueError:
                    print "Wrong Answer. Please Try Again!"
                else:
                    if not products.exists(pid):
                        print "Product with pid={} doesn't exist!".format(pid)
                        return
                    orders.add(Order(oid, pid))
                    return
        elif key == '3':
            return
        print "Wrong Answer. Please Try Again!"


def update_menu():
    pass


def filter_menu():
    pass


actions = {'1': select_menu, '2': delete_menu, '3': insert_menu, '4': update_menu, '5': filter_menu}

while True:
    print "\n1. SELECT * FROM\n2. DELETE \'item\' FROM\n3. INSERT INTO\n4. UPDATE\n5. Filter\n6. Exit"
    key = raw_input("Select action: ")
    if key in actions:
        actions[key]()
    elif key == '6':
        with open('data.pkl', 'wb') as f:
            pickle.dump(data, f)
            f.close()
        print "Data has been saved"
        break
    else:
        print "Wrong Answer. Please Try Again!"
