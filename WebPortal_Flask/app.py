import json
import sqlite3

import requests as requests
from flask import Flask, jsonify, request

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
    cur.execute(
        '''INSERT INTO car(entry,details)VALUES ('command','MOV[1:1:3] MOV[4:1:3] MOV[3:1:3] MOV[1:1:3]')''')
    con.commit()
    print("Commands inserted")
    cur.execute('''INSERT INTO car(entry,details)VALUES ('stat','none')''')
    con.commit()
    cur.execute('''INSERT INTO car(entry,details)VALUES ('challenge','none')''')
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


@app.route('/ack<string:data>')
def carAcknowledgement(data):  # put application's code here
    con = sqlite3.connect('car_details')
    cur = con.cursor()
    cur.execute('''UPDATE car SET details = "acknowledged" WHERE entry = "command"''')
    con.commit()
    cur.execute('''UPDATE car SET details = "''' + data + '''" WHERE entry = "stat"''')
    con.commit()
    status = ""
    identify = ""
    cur.execute('''SELECT details FROM car WHERE entry = "stat"''')
    for row in cur.fetchone():
        status = row
    x = []
    for status in status.split("."):
        if status != '':
            r_loc = status.find("R")
            l_loc = status.find("L")
            e_loc = status.find("E")
            y = {"Right": str(status[r_loc + 1:l_loc]),
                 "Left": str(status[l_loc + 1:e_loc]),
                 "Emergency": str(status[e_loc + 1:])}
            x.append(y)
    cur.execute('''SELECT details FROM car WHERE entry = "challenge"''')
    for row in cur.fetchone():
        identify = row
    final = {"statistic": x, "challengeHistoryId": identify}
    response = requests.post('http://localhost:46290/Student/UpdateCarStatistic', json=final)
    return final


# This is the GET method
@app.route('/setCommand<string:command>/<string:identifier>')
def setCarInstructions(command, identifier):
    con = sqlite3.connect('car_details')
    cur = con.cursor()
    cur.execute('''UPDATE car SET details = "''' + command + '''" WHERE entry = "command"''')
    con.commit()
    cur.execute('''UPDATE car SET details = "''' + identifier + '''" WHERE entry = "challenge"''')
    con.commit()
    return "Updated: " + command


# This is the POST method
@app.route('/sendCarCommand', methods=['POST'])
def recvFromAsProject():
    content = request.json
    con = sqlite3.connect('car_details')
    cur = con.cursor()
    cur.execute('''UPDATE car SET details = "''' + str(content['carCommand']) + '''" WHERE entry = "command"''')
    cur.execute(
        '''UPDATE car SET details = "''' + str(content['challengeHistoryId']) + '''" WHERE entry = "challenge"''')
    con.commit()
    return content


@app.route('/getDistRead')
def fetchCarStatistics():
    con = sqlite3.connect('car_details')
    cur = con.cursor()
    status = ""
    identify = ""
    cur.execute('''SELECT details FROM car WHERE entry = "stat"''')
    for row in cur.fetchone():
        status = row
    x = []
    for status in status.split("."):
        if status != '':
            r_loc = status.find("R")
            l_loc = status.find("L")
            e_loc = status.find("E")
            y = {"Right": str(status[r_loc + 1:l_loc]),
                 "Left": str(status[l_loc + 1:e_loc]),
                 "Emergency": str(status[e_loc + 1:])}
            x.append(y)
    cur.execute('''SELECT details FROM car WHERE entry = "challenge"''')
    for row in cur.fetchone():
        identify = row
    final = {"statistic": x, "challengeHistoryId": identify}
    return jsonify(final)


if __name__ == '__main__':
    app.run(host='192.168.1.5', port=80, debug=True, threaded=True)
    # app.run(host='172.20.10.6', port=80, debug=True, threaded=True)
    # app.run(host='localhost', port=80, debug=True, threaded=True)
