import secrets
import sqlite3

from flask import Flask, jsonify, request, session

app = Flask(__name__)


# General idea just to bridge ASP & Flask
# ASP: http get - > req json
# link the stuff to the thingy.
# see if asp can interpret the json file
# 192.168.1.5/getJsonData
# FLASK: serve data to ASP
# sent -> data in JSON


def create_database():
    con = sqlite3.connect('car_details')
    cur = con.cursor()
    cur.execute('''DROP TABLE IF EXISTS car''')
    cur.execute('''CREATE TABLE IF NOT EXISTS car(entry TEXT PRIMARY KEY, details TEXT NOT NULL)''')
    con.commit()
    return 'Database Created'


def generate_database():
    con = sqlite3.connect('car_details')
    cur = con.cursor()
    cur.execute('''INSERT INTO car(entry,details)VALUES ('command','MOV[2:1:5] MOV[2:1:5] MOV[4:1:2] MOV[3:1:2]')''')
    con.commit()
    print("Commands inserted")
    cur.execute('''INSERT INTO car(entry,details)VALUES ('speed','1')''')
    cur.execute('''INSERT INTO car(entry,details)VALUES ('distance','1')''')
    con.commit()
    cur.execute('''SELECT * FROM car''')
    results = cur.fetchall()
    return results


@app.route('/setup')
def setup():
    return 'Database Setup: ' + create_database() + "\n" + "Database Contents: " + str(generate_database())


@app.route('/')
def fetchCarInstructions():  # put application's code here
    con = sqlite3.connect('car_details')
    cur = con.cursor()
    cur.execute('''SELECT details FROM car WHERE entry = "command"''')
    for row in cur.fetchone():
        return row


# This is the GET method
@app.route('/setCommand<string:command>')
def setCarInstructions(command):
    con = sqlite3.connect('car_details')
    cur = con.cursor()
    cur.execute('''UPDATE car SET details = "''' + command + '''" WHERE entry = "command"''')
    con.commit()
    return "Updated: " + command


# This is the POST method
@app.route('/setCarCommand', methods=['POST'])
def recvFromAsProject():
    content = request.json
    con = sqlite3.connect('car_details')
    cur = con.cursor()
    cur.execute('''UPDATE car SET details = "''' + str(content['carCommand']) + '''" WHERE entry = "command"''')
    con.commit()
    return content


@app.route('/setDist<int:dist>Speed<int:speed>')
def setCarStatistics(dist, speed):
    con = sqlite3.connect('car_details')
    cur = con.cursor()
    cur.execute('''UPDATE car SET details ="''' + str(speed) + '''" WHERE entry = "speed"''')
    cur.execute('''UPDATE car SET details ="''' + str(dist) + '''" WHERE entry = "distance"''')
    con.commit()
    return "Updated speed: " + str(speed) + " distance: " + str(dist)


@app.route('/getDistRead')
def fetchCarStatistics():
    con = sqlite3.connect('car_details')
    cur = con.cursor()
    status = []
    cur.execute('''SELECT details FROM car WHERE entry = "speed"''')
    for row in cur.fetchone():
        status.append(row)
    cur.execute('''SELECT details FROM car WHERE entry = "distance"''')
    for row in cur.fetchone():
        status.append(row)
    return jsonify({'speed': status[0], 'distance': status[1]})


if __name__ == '__main__':
    # app.run(host='192.168.1.5', port=80, debug=True, threaded=True)
    # app.run(host='172.20.10.6', port=80, debug=True, threaded=True)
    app.run(host='localhost', port=80, debug=True, threaded=True)
