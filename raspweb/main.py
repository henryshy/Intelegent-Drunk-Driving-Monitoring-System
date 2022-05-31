import sys
import socket
import threading
import time
from flask import Flask, render_template, jsonify
import serial


ser = serial.Serial('/dev/ttyUSB0', 9600)

app = Flask(__name__)

alcohol = 0
pulse = 1
temperature = 2

alert_str='8'
send_str='0'

@app.route('/index')
def data():
    global alcohol, pulse, temperature
    return jsonify(status=True, alcohol=str(alcohol), pulse1=str(pulse), temperature=str(temperature))

@app.route('/')
def index():
    return render_template("new.html")


@app.route('/one')
def one():
    doSomething1()


@app.route('/two')
def two():
    doSomething2()




# 数据来源进程
def work2():
    global alcohol, pulse  # 酒精、脉搏、电压
    while True:
        if (ser.readable()):
            data = ser.read(11)
            ser.flushInput()
        if data[0] != 97:
            ser.flushInput()
            continue
        print(data)
        count=0
        pulseread = 0
        alcoholread=0
        for i in range(6):
            ch=data[i]
            num=int(data[i] - 48)
            if ch==115:
                count=1
            if num <= 9 and num >= 0 and count==0:
                alcoholread = alcoholread * 10 + num
            if num <= 9 and num >= 0 and count == 1:
                pulseread = pulseread * 10 + num

        sensor_volt=alcoholread/1024*5.0
        RS_gas = ((5.0 * 200) / sensor_volt) - 200
        R0 = 31
        ratio = RS_gas / R0
        x = 0.4 * ratio
        alcohol = round(pow(x, -1.431)*1000-10,2)
        if alcohol<0:
            alcohol=0

        global alert_str

        if alcohol>20:
            alert_str='9'
        else:
            alert_str='8'

        pulse=pulseread
        time.sleep(0.1)

# 按钮对应的事件
def doSomething1():
    global send_str
    send_str='0'
    return True

# 按钮对应的事件
def doSomething2():
    global send_str
    send_str='1'
    return True

def socket_service():
    try:
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        s.bind(('192.168.1.115', 7777))
        s.listen(10)
    except socket.error as msg:
        print(msg)
        sys.exit(1)
    print('Waiting connection...')

    while 1:
        conn, addr = s.accept()
        t = threading.Thread(target=deal_data, args=(conn,addr))
        t.start()


def deal_data(conn,addr):

    global temperature
    global send_str
    global alert_str
    conn.send((send_str+alert_str).encode('utf-8'))
    send_str='3'
    data=conn.recv(4)
    temperature=round(int(data)*0.01,2)
    conn.close()

if __name__ == '__main__':
    thread=threading.Thread(target=work2)
    thread.start()
    thread1=threading.Thread(target=socket_service)
    thread1.start()
    app.run(host='0.0.0.0')







