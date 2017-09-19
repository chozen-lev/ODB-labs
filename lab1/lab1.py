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
    data = {}
else:
    data = pickle.load(f)
    products = Products(data['products'], data['plast'])
    orders = Orders(data['orders'])
    f.close()
    if not products.empty():
        print 'Data has been loaded'
    else:
        print 'File has been found, but it has any data'


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
        print 'Wrong Answer. Please Try Again!'


def delete_menu():
    print "\n1. 'products'\n2. 'orders'\n3. Back"
    while True:
        key = raw_input('Select table: ')
        if key == '1':
            while True:
                try:
                    pid = int(raw_input('Enter pid: '))
                except ValueError:
                    print 'Wrong Answer. Please Try Again!'
                else:
                    if not products.exists(pid):
                        print "Product with pid={} doesn't exist!".format(pid)
                    else:
                        products.delete(pid, orders)
                    return
        elif key == '2':
            while True:
                try:
                    oid = int(raw_input('Enter oid: '))
                except ValueError:
                    print 'Wrong Answer. Please Try Again!'
                else:
                    if not orders.exists(oid):
                        print "Order with oid={} doesn't exist!".format(oid)
                    else:
                        orders.delete(oid)
                    return
            return
        elif key == '3':
            return
        print 'Wrong Answer. Please Try Again!'


def insert_menu():
    print "\n1. 'products'\n2. 'orders'\n3. Back"
    while True:
        key = raw_input("Select table: ")
        if key == '1':
            name = raw_input('Enter product name: ')
            while True:
                try:
                    price = int(raw_input('Enter product price: '))
                except ValueError:
                    print 'Wrong Answer. Please Try Again!'
                else:
                    products.add(Product(products.last, name, price))
                    return
        elif key == '2':
            while True:
                try:
                    oid = int(raw_input('Enter oid: '))
                except ValueError:
                    print 'Wrong Answer. Please Try Again!'
                else:
                    break
            while True:
                try:
                    pid = int(raw_input("Enter pid: "))
                except ValueError:
                    print 'Wrong Answer. Please Try Again!'
                else:
                    if not products.exists(pid):
                        print "Product with pid={} doesn't exist!".format(pid)
                        return
                    orders.add(Order(oid, pid))
                    return
        elif key == '3':
            return
        print 'Wrong Answer. Please Try Again!'


def update_menu():
    while True:
        try:
            pid = int(raw_input('Enter pid: '))
        except ValueError:
            print 'Wrong Answer. Please Try Again!'
        else:
            if not products.exists(pid):
                print "Product with pid={} doesn't exist!".format(pid)
            else:
                name = raw_input('Enter new name: ')
                products[pid].name = name
                while True:
                    try:
                        price = int(raw_input('Enter new price: '))
                    except ValueError:
                        print 'Wrong Answer. Please Try Again!'
                    else:
                        products[pid].price = price
                        break
            return


def filter_menu():
    prices = {}
    for order in orders.orders:
        if order.oid in prices:
            prices[order.oid] += products[order.pid].price
        else:
            prices[order.oid] = products[order.pid].price

    for oid in prices:
        if prices[oid] > 100:
            print "%d. %d" % (oid, prices[oid])


actions = {'1': select_menu, '2': delete_menu, '3': insert_menu, '4': update_menu, '5': filter_menu}

while True:
    print "\n1. SELECT * FROM\n2. DELETE \'item\' FROM\n3. INSERT INTO\n4. UPDATE 'products'\n5. Filter\n6. Save&Exit"
    try:
        key = raw_input('Select action: ')
    except KeyboardInterrupt:
        break
    else:
        if key in actions:
            actions[key]()
        elif key == '6':
            data = {
                'products': products.products,
                'orders': orders.orders,
                'plast': products.last
            }
            with open('data.pkl', 'wb') as f:
                pickle.dump(data, f)
                f.close()
            print 'Data has been saved'
            break
        else:
            print 'Wrong Answer. Please Try Again!'
